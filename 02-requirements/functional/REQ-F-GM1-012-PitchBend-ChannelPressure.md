# REQ-F-GM1-012: GM1 Pitch Bend and Channel Pressure Expressive Control

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: Medium
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Expressive performance requirement)  
- **Depends on**: REQ-F-GM1-009 (Controllers), REQ-F-GM1-011 (Pitch bend sensitivity RPN)  
- **Will be verified by**: TEST-GM1-EXP-001 (Pitch Bend Scaling), TEST-GM1-EXP-002 (Aftertouch Mapping)

## Description
Define behavior for Pitch Bend (14-bit) and Channel Pressure (Aftertouch) in GM1 mode:
- Pitch Bend range default ±2 semitones (modifiable via RPN 0).
- High-resolution center at 8192 (0x2000). Values map linearly to semitone offset scaled by bend range.
- Channel Pressure single value (0–127) provides channel-wide expressive modulation (timbre/volume/brightness). No polyphonic key pressure required by GM1 core.

## Functional Requirements
1. FR1: Pitch Bend MUST recalculate real-time pitch offset using current bend sensitivity and tuning offsets (coarse/fine) per channel.  
2. FR2: Center value (8192) MUST produce zero pitch offset (±1 cent tolerance).  
3. FR3: Channel Pressure MUST apply a musically relevant modulation (e.g., brightness filter gain or mild volume/timbre change) consistent across programs.  
4. FR4: After Pitch Bend range change (RPN 0), subsequent bends MUST reflect new scaling immediately (no stale cache).  
5. FR5: Pitch Bend MUST NOT affect percussion (Channel 10) voices.  
6. FR6: Channel Pressure MUST NOT modify percussion one-shot playback envelope shape (optional negligible effect).  
7. FR7: Implementation MUST avoid zipper artifacts; apply optional smoothing (≤5 ms) for rapid Channel Pressure changes.  
8. FR8: Data path MUST preserve 14-bit precision (LSB + MSB) without truncation to 7-bit.  
9. FR9: Bend calculation MUST remain stable under rapid successive bend messages (no oscillation/cumulative drift).  
10. FR10: Reset All Controllers restores bend range but DOES NOT force bend position to center; separate explicit bend-centering required (optional). 

## Pitch Bend Conversion
```c
// midi_msb = data[1], midi_lsb = data[0]
uint16_t bend_14bit = (midi_msb << 7) | midi_lsb; // 0..16383
int16_t bend_signed = (int16_t)bend_14bit - 8192; // -8192..+8191

float compute_bend_semitones(uint8_t ch, int16_t bend_signed) {
    gm1_rpn_state_t* st = &rpn_state[ch];
    float range = (float)st->bend_range_semitones + st->bend_range_cents / 100.0f; // e.g. 2.00
    float norm = (float)bend_signed / 8192.0f; // -1..+1 scale
    float semitone_offset = norm * range;
    // Apply tuning offsets
    semitone_offset += st->coarse_tune_semitones;
    semitone_offset += (float)st->fine_tune_cents / 100.0f;
    return semitone_offset;
}
```

## Channel Pressure Mapping
```c
void apply_channel_pressure(uint8_t ch, uint8_t pressure_value) {
    float norm = (float)pressure_value / 127.0f; // 0..1
    // Example mapping: blend brightness + subtle volume
    set_filter_brightness(ch, norm);         // 0=dark, 1=bright
    set_pressure_gain_scale(ch, 1.0f + norm * 0.05f); // up to +5% gain
}
```

## Smoothing (Optional Implementation)
```c
#define PRESSURE_SMOOTH_MS 5

void update_pressure_smoothed(uint8_t ch, uint8_t raw_value) {
    pressure_target[ch] = raw_value;
    // Interpolate in audio callback or periodic control update
}
```

## Acceptance Criteria
1. AC1: Full negative bend (0) reaches -Range semitones, full positive (16383) reaches +Range semitones within ±10 cent accuracy.  
2. AC2: Center bend (8192) produces offset < ±1 cent after processing path.  
3. AC3: Changing bend sensitivity mid-note updates subsequent bend messages without artifacts.  
4. AC4: Channel Pressure modulation audibly increases brightness/energy from 0→127 across typical programs (strings, brass, synth lead).  
5. AC5: Percussion ignores bend effect (frequency invariance test).  
6. AC6: Rapid bend sweeps (>=100 messages/sec) show stable, artifact-free pitch transitions.  
7. AC7: Rapid Channel Pressure changes (≥50 messages/sec) produce smooth output (no stepping >3 dB).  
8. AC8: Reset All Controllers leaves current bend position unchanged, only range resets to default.  

## Test Scenarios
- **Range Accuracy**: Program bend range to 12 semitones; measure pitch at extremes vs theoretical.  
- **Mid-Performance Sensitivity Change**: Start with ±2; shift to ±7; verify new scaling instantly.  
- **Percussion Immunity**: Apply extreme bends during drum pattern; verify pitch stability.  
- **Aftertouch Expressiveness**: Sweep pressure from 0→127; log filter cutoff or brightness metric.  
- **High-Frequency Bend Flood**: Stress test with fast wheel movements; ensure no numerical drift.

## Risks / Considerations
- Floating-point precision may introduce cent-level drift; consider caching derived multipliers.  
- Excessive Channel Pressure effects can mask velocity nuances; calibrate mapping per instrument category.  
- Large bend ranges amplify tuning offsets; ensure combined offset stays musically sensible.

## References
- MIDI 1.0 Detailed Spec – Pitch Bend & Channel Pressure
- GM1 Expressive Performance Expectations
