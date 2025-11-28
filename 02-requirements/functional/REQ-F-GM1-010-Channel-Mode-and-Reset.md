# REQ-F-GM1-010: GM1 Channel Mode Messages and Reset Behavior (CC120–CC123, CC121)

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Need for predictable playback & recovery)  
- **Depends on**: REQ-F-GM1-009 (Controller base), REQ-F-GM1-006–008 (polyphony & lifecycle)  
- **Will be verified by**: TEST-GM1-MODE-001 (Reset Semantics), TEST-GM1-MODE-002 (All Notes Off Reliability)

## Description
Implement required Channel Mode messages ensuring reliable state recovery and predictable termination of sounding notes.
Mandatory subset for GM1 runtime:
- CC120 All Sound Off (optional use; immediate hard mute)
- CC121 Reset All Controllers (restore default controller values)
- CC122 Local Control On/Off (ignored or treated no-op in pure protocol library) 
- CC123 All Notes Off (transition active voices to RELEASE respecting sustain)

## Functional Requirements
1. FR1: CC121 MUST reset CC1, CC7, CC10, CC11, CC64 to documented defaults (see REQ-F-GM1-009) and clear any internal modulation/smoothing accumulators.  
2. FR2: CC123 MUST send all *non-percussion* active voices to RELEASE unless sustain pedal ON; if sustain ON, voices remain until sustain release.  
3. FR3: CC120 (if implemented) MUST immediately silence all voices (hard cut) and mark them FREE (distinct from CC123 graceful release).  
4. FR4: CC122 MUST NOT alter protocol state in hardware-agnostic library (documented as pass-through/no-op).  
5. FR5: Mode messages MUST NOT alter Program assignments or polyphony configuration.  
6. FR6: Concurrent receipt of CC121 followed by CC123 MUST produce a clean stable state (controllers default + voices released).  
7. FR7: Messages MUST be idempotent (repeated CC123 calls with no active notes produce no errors).  
8. FR8: Reset MUST clear sustain latch regardless of prior CC64 value (pedal forced OFF).  

## Controller Reset Routine
```c
void reset_all_controllers(uint8_t ch) {
    gm1_ctrl[ch].cc1_mod_depth  = 0;
    gm1_ctrl[ch].cc7_volume     = 100; // Implementation baseline
    gm1_ctrl[ch].cc10_pan       = 64;
    gm1_ctrl[ch].cc11_expression= 127;
    gm1_ctrl[ch].cc64_sustain   = 0;
    update_channel_volume(ch);
    update_channel_pan(ch);
    // Additional internal smoothing buffers cleared
}
```

## All Notes Off
```c
void all_notes_off(uint8_t ch) {
    for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
        gm1_voice_t* v = &pool.voices[i];
        if (v->channel == ch && v->state == VOICE_STATE_ACTIVE) {
            if (!is_voice_sustain_held(v)) {
                transition_to_release(v);
            }
        }
    }
}
```

## All Sound Off (Immediate Flush)
```c
void all_sound_off(uint8_t ch) {
    for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
        gm1_voice_t* v = &pool.voices[i];
        if (v->channel == ch && v->state != VOICE_STATE_FREE) {
            recycle_voice(v); // Hard cut – amplitude zero
        }
    }
}
```

## Idempotency Check
```c
bool verify_idempotent_reset(uint8_t ch) {
    gm1_channel_ctrl_t before = gm1_ctrl[ch];
    reset_all_controllers(ch);
    gm1_channel_ctrl_t after = gm1_ctrl[ch];
    // Compare fields; second invocation should produce identical outcome
    return memcmp(&before, &after, sizeof(gm1_channel_ctrl_t)) == 0 ? false : true; // first call changed, second would not
}
```

## Acceptance Criteria
1. AC1: CC121 sets controllers exactly to defined defaults (verified by state query).  
2. AC2: CC123 transitions active unsustained voices to RELEASE within ≤10 ms event dispatch latency.  
3. AC3: CC120 stops all sounding audio immediately (test harness shows amplitude drop <5 ms).  
4. AC4: Sustain-held voices remain sounding after CC123 until CC64 <64 arrives.  
5. AC5: Sequence CC121→CC123 always results in clean baseline state (no stuck voices).  
6. AC6: Repeated CC123 with no active voices yields no state mutation side effects.  
7. AC7: CC122 produces no change (documented no-op) in library-only environment.  
8. AC8: No program assignments altered after mode messages (pre/post comparison unchanged).  

## Test Scenarios
- **Graceful Release Test**: Trigger chord, send CC123, verify release envelopes active.
- **Hard Cut Test**: Trigger sustained notes, send CC120, confirm immediate silence.
- **Reset Ordering**: Interleave CC121 and CC123 in multiple permutations; assert final invariants.
- **Sustain Interaction**: Sustain ON, send CC123, ensure notes persist; sustain OFF flushes them.

## Risks / Considerations
- Hard cut (CC120) may produce click if not smoothed—optional quick fade (1–2 ms) recommended.
- Controller reset must not interfere with pending RPN state selection.

## References
- MIDI 1.0 Detailed Spec – Channel Mode Messages
- GM1 Predictability Requirements (Stable recovery and termination)
