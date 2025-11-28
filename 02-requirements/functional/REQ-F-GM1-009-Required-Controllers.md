# REQ-F-GM1-009: GM1 Required MIDI Channel Controllers (CC1, CC7, CC10, CC11, CC64)

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1 (General MIDI Level 1)
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Stakeholder requirement for expressive control)  
- **Depends on**: #537–#541 (sound set, quality, loading), REQ-F-GM1-006–008 (polyphony/voice lifecycle)  
- **Will be verified by**: TEST-GM1-CTRL-001 (Controller Semantics), TEST-GM1-CTRL-002 (Velocity + CC Interplay)

## Description
Implement all *mandatory* GM1 channel controllers with correct scaling, side effects, and musical semantics:
- CC1 Modulation Wheel – Vibrato / modulation depth (instrument-category adaptive)
- CC7 Channel Volume – Logarithmic amplitude control (0 = silent, 127 = nominal max)
- CC10 Pan – Stereo placement (0 hard left, 64 center, 127 hard right)
- CC11 Expression – Fine volume sub-scaling (acts as multiplicative factor relative to CC7)
- CC64 Sustain Pedal – Sustain latch for melodic voices (≥64 = ON, <64 = OFF) – percussion ignored

## Functional Requirements
1. FR1: CC1 MUST modify vibrato LFO depth (or appropriate modulation) for applicable instrument families (strings, brass, leads).  
2. FR2: CC7 MUST apply a logarithmic volume curve; linear raw value mapping is NOT acceptable.
3. FR3: CC10 MUST pan voices using equal-power panning law (avoid volume dip at center).  
4. FR4: CC11 MUST multiply (not replace) CC7 result: EffectiveVolume = Volume(CC7) * Expression(CC11).  
5. FR5: CC64 (≥64) MUST hold active voices (excluding percussion) in sustain until release (<64) triggers envelope release.  
6. FR6: Controller changes MUST be sample-accurate or at least event-order consistent (no reordering across voices).  
7. FR7: Out-of-range values (>127) MUST be clamped gracefully (defensive parsing).  
8. FR8: Channel state MUST persist across Program Change events (controller values retained).  
9. FR9: Reset All Controllers (CC121) MUST restore CC1/7/10/11 to default neutral values; sustain pedal released.  
10. FR10: Volume and expression changes MUST avoid zipper noise (apply smoothing optional—5–10 ms ramp).

## Controller Defaults
| Controller | Default | Notes |
|------------|---------|-------|
| CC1 Mod    | 0       | No modulation  |
| CC7 Volume | 100–127 | Recommended nominal start (implementation choice; often 100) |
| CC10 Pan   | 64      | Center |
| CC11 Expr  | 127     | Full expression (unity) |
| CC64 Sustain | 0     | Off |

## Data Structures (Example)
```c
typedef struct {
    uint8_t cc1_mod_depth;     // 0-127
    uint8_t cc7_volume;        // 0-127
    uint8_t cc10_pan;          // 0-127
    uint8_t cc11_expression;   // 0-127
    uint8_t cc64_sustain;      // 0-127
    float   cached_gain_linear; // Computed from CC7+CC11
    float   cached_pan_left;    // Pan gains
    float   cached_pan_right;
} gm1_channel_ctrl_t;

gm1_channel_ctrl_t gm1_ctrl[16];
```

## Volume Computation
```c
static float midi_cc_to_gain(uint8_t v) {
    if (v == 0) return 0.0f;
    // Approximate logarithmic (power) curve
    float norm = (float)v / 127.0f;
    return powf(norm, 1.8f); // Exponent tunes perceived loudness
}

void update_channel_volume(uint8_t ch) {
    float vol = midi_cc_to_gain(gm1_ctrl[ch].cc7_volume);
    float expr = midi_cc_to_gain(gm1_ctrl[ch].cc11_expression);
    gm1_ctrl[ch].cached_gain_linear = vol * expr; // Multiplicative model
}
```

## Pan Equal-Power Law
```c
static void update_channel_pan(uint8_t ch) {
    float norm = (float)gm1_ctrl[ch].cc10_pan / 127.0f; // 0..1
    // Equal power: angle mapping
    float angle = norm * (M_PI / 2.0f); // 0=left, pi/4=center, pi/2=right
    gm1_ctrl[ch].cached_pan_left  = cosf(angle);
    gm1_ctrl[ch].cached_pan_right = sinf(angle);
}
```

## Modulation (CC1)
```c
void apply_modulation(uint8_t ch, float base_signal) {
    float depth = (float)gm1_ctrl[ch].cc1_mod_depth / 127.0f; // 0..1
    // Category-adaptive (simplified): strings/brass use vibrato; synth leads use LFO pitch/amp
    // Implementation selects modulation target; here vibrato depth example
    float lfo = get_vibrato_lfo_value(ch); // -1..1
    float modulated = base_signal * (1.0f + depth * lfo * 0.03f); // up to ±3% pitch or amplitude variation
}
```

## Sustain Handling (Interacts with REQ-F-GM1-008)
```c
bool is_voice_sustain_held(const gm1_voice_t* v) {
    if (v->is_percussion) return false; // Ignore percussion
    return gm1_ctrl[v->channel].cc64_sustain >= 64 && v->state == VOICE_STATE_ACTIVE;
}
```

## Controller Event Processing
```c
void process_cc(uint8_t channel, uint8_t controller, uint8_t value) {
    switch (controller) {
        case 1:  gm1_ctrl[channel].cc1_mod_depth = value; break;
        case 7:  gm1_ctrl[channel].cc7_volume = value; update_channel_volume(channel); break;
        case 10: gm1_ctrl[channel].cc10_pan = value; update_channel_pan(channel); break;
        case 11: gm1_ctrl[channel].cc11_expression = value; update_channel_volume(channel); break;
        case 64: gm1_ctrl[channel].cc64_sustain = value; handle_sustain_pedal(channel, value); break;
        case 121: reset_all_controllers(channel); break; // See REQ-F-GM1-010
        default: /* ignore here */ break;
    }
}
```

## Acceptance Criteria
1. AC1: Changing CC7 from 0→127 produces smooth audible gain ramp (no zipper artifacts >3 dB steps).  
2. AC2: CC11 at 64 halves (±10%) effective loudness relative to CC11 at 127 for same CC7 value.  
3. AC3: CC10 at 0 yields left gain ≈1.0, right gain ≈0.0; at 127 reversed; at 64 both ≈0.707 (±5%).  
4. AC4: Sustain pedal ON retains voices after Note Off until pedal release; OFF flushes all held voices to RELEASE within 10 ms.  
5. AC5: CC1 modulation depth audibly increases vibrato on designated instrument families (test harness verifies pitch deviation).  
6. AC6: Program Change does NOT reset controller states (values persist).  
7. AC7: Reset All Controllers (CC121) restores default values and releases sustain-held voices.  
8. AC8: Running Status or interleaved messages preserve event ordering (no reordering trace observed).  

## Test Scenarios
- Volume/Expression Interaction Test – Sweep CC7 and CC11 verifying multiplicative combination.
- Pan Consistency Test – Measure channel output RMS in stereo for CC10 values (0,32,64,96,127).
- Sustain Pedal Chord Retention – Hold pedal through chord progression; verify voice counts.
- Modulation Depth Test – CC1 increments cause measurable vibrato depth delta.
- Controller Persistence – Apply controllers, send Program Change, confirm unchanged states.

## Risks / Considerations
- Excessive vibrato depth scaling can cause pitch instability; cap at musically sensible range.
- Pan law misimplementation may cause center dip or loudness imbalance.
- Inadequate smoothing may produce audible stepping on CC sweeps.

## References
- MIDI 1.0 Detailed Spec (Controller Numbers Table)
- GM1 Expressive Control Requirements (Stakeholder expectation for consistent playback)
- Audio Engineering Practices (Equal-power panning)
