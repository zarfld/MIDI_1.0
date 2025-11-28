# REQ-F-GM1-014: GM1 Mode Initialization and Reset Sequence

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Need for consistent baseline playback state)  
- **Depends on**: REQ-F-GM1-013 (GM ON trigger), REQ-F-GM1-001..012 (foundational features)  
- **Will be verified by**: TEST-GM1-INIT-001, TEST-GM1-INIT-002

## Description
Define the ordered atomic initialization sequence invoked when GM System ON is processed. Establishes deterministic baseline ensuring identical starting conditions across GM-compliant devices.

## Initialization Goals
1. Clear residual state from previous modes.
2. Standardize controller values.
3. Assign default programs to melodic channels, percussion mapping to Channel 10.
4. Reset tuning and pitch sensitivity.
5. Clear voice pool, sustain states, and RPN selection context.

## Required Steps (Order)
| Step | Action | Rationale |
|------|--------|-----------|
| 1 | Suspend real-time event dispatch | Prevent race conditions during reset |
| 2 | Clear voice pool (active→FREE) | Remove lingering sound artifacts |
| 3 | Reset controllers per channel (CC1=0, CC7=100, CC10=64, CC11=127, CC64=0) | Consistent expressive baseline |
| 4 | Set Pitch Bend Sensitivity RPN0 to ±2 semitones | Standard GM default |
| 5 | Clear fine/coarse tuning (RPN1/RPN2 neutral) | Tuning neutrality |
| 6 | Null RPN selection (101=127,100=127 effect) | Prevent unintended data entry |
| 7 | Assign Program 1 (Acoustic Grand Piano) to melodic channels (1–9,11–16) | Universal mapping baseline |
| 8 | Assign percussion mapping to Channel 10 (drums keys 35–81) | Standard percussion behavior |
| 9 | Reset channel pressure, modulation accumulators | Expressive neutrality |
|10 | Reinitialize pan equal-power calculations | Stereo consistency |
|11 | Resume event dispatch | System ready |

## Data Structure Hooks
```c
void gm_perform_initialization(void) {
    suspend_midi_event_dispatch();
    clear_all_voices();
    for (uint8_t ch = 0; ch < 16; ch++) {
        reset_all_controllers(ch);              // REQ-F-GM1-010 logic
        rpn_state[ch].bend_range_semitones = 2; 
        rpn_state[ch].bend_range_cents     = 0;
        rpn_state[ch].fine_tune_cents      = 0;
        rpn_state[ch].coarse_tune_semitones= 0;
        rpn_state[ch].current_rpn          = -1; // Null
        if (ch == 9) {
            configure_channel_percussion(ch);   // Channel 10
        } else {
            set_channel_program(ch, 1);         // Program 1 default
        }
        recompute_pan_cache(ch);
        clear_channel_pressure_state(ch);
    }
    resume_midi_event_dispatch();
}
```

## Atomicity Requirement
- Initialization MUST appear instantaneous to downstream consumers (no partial states).  
- Events received mid-sequence MUST be queued and processed only after completion.

## Acceptance Criteria
1. AC1: Post GM ON, all melodic channels report Program 1; Channel 10 percussion mapping active.  
2. AC2: All controllers reflect documented defaults per channel.  
3. AC3: Pitch Bend sensitivity = ±2; tuning offsets zeroed.  
4. AC4: No active voices remain (voice pool active_count=0) immediately after init.  
5. AC5: RPN selection = null; subsequent Data Entry ignored until new selection.  
6. AC6: No MIDI events processed during sequence (timestamp ordering preserved).  
7. AC7: Latency from GM ON completion to first accepted Note On <10 ms.  
8. AC8: Repeated GM ON does not accumulate duplicate initialization side effects (idempotent state).  

## Test Scenarios
- **Baseline Integrity**: Capture state pre/post GM ON; diff equals expected template.
- **Event Queueing**: Inject Note On during initialization; verify processed only after step 11.
- **Idempotency**: Invoke GM ON twice; confirm identical final state snapshot and no voice leaks.
- **Timing Measurement**: Profile sequence duration across target platforms.

## Risks / Considerations
- Large voice pool or sample unload operations may exceed latency target; optimize memory operations.
- External hardware sync (if any) must not cause partial audible artifacts.

## References
- GM1 Baseline Behavior (cross-device predictability)
- MIDI Controller and RPN definitions
