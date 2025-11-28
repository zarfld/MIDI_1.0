# REQ-F-GM1-015: GM1 Mode State Management and Transition Safeguards

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: Medium
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Robust multi-mode operation)  
- **Depends on**: REQ-F-GM1-013 (SysEx ON/OFF), REQ-F-GM1-014 (Initialization), prior sound/controller requirements  
- **Will be verified by**: TEST-GM1-MODESTATE-001, TEST-GM1-MODESTATE-002

## Description
Provide a reliable state management layer for GM1 mode activation, persistence, inspection, and safe transition out of GM1. Prevent inconsistent hybrid states and ensure other protocol subsystems can query GM activation quickly.

## Functional Requirements
1. FR1: Maintain boolean `gm_mode_active` plus a monotonically increasing `gm_mode_epoch` capturing each activation instance (for debugging/logging).  
2. FR2: Expose query API `gm_is_active()` returning current mode without side effects.  
3. FR3: Prevent simultaneous activation of mutually exclusive higher modes (e.g., GM2 placeholder) — if such mode active, GM ON ignored or queued (design decision: ignore with warning).  
4. FR4: Transition out (GM OFF) MUST complete graceful voice shutdown before changing `gm_mode_active`.  
5. FR5: Events that depend on GM semantics (Program mapping, controller defaults) MUST branch on `gm_mode_active` immediately (no delayed shadow state).  
6. FR6: Provide structured log/event callback on mode changes (`GM_MODE_ENTER`, `GM_MODE_EXIT`).  
7. FR7: If initialization fails mid-sequence (error return), system MUST roll back to previous stable state (remain inactive) and emit failure event.  
8. FR8: A diagnostic snapshot API MUST return consolidated GM related state (programs, controller baselines, bend range, percussion mapping flag).  
9. FR9: GM OFF MUST NOT clear user-altered non-GM configurations outside GM baseline (segregated state boundaries).  
10. FR10: Concurrency: Mode transitions MUST acquire a global mode mutex/spinlock to avoid race with arriving SysEx.  

## Data Structures
```c
typedef struct {
    bool     active;
    uint32_t epoch;             // Incremented each successful activation
    uint64_t last_transition_us; // Timestamp of last mode change
    uint32_t failed_inits;      // Count of failed attempts
} gm_mode_state_t;

gm_mode_state_t gm_mode_state;
```

## APIs
```c
bool gm_is_active(void) { return gm_mode_state.active; }
uint32_t gm_mode_current_epoch(void) { return gm_mode_state.epoch; }

// Snapshot structure
typedef struct {
    bool     active;
    uint32_t epoch;
    uint8_t  channel_programs[16];
    uint8_t  percussion_channel; // Typically 10 (index 9)
    uint8_t  bend_range_semitones[16];
} gm_snapshot_t;

void gm_get_snapshot(gm_snapshot_t* out) {
    out->active = gm_mode_state.active;
    out->epoch  = gm_mode_state.epoch;
    for (int ch=0; ch<16; ch++) {
        out->channel_programs[ch] = get_channel_program(ch);
        out->bend_range_semitones[ch] = rpn_state[ch].bend_range_semitones;
    }
    out->percussion_channel = 10; // Fixed for GM1
}
```

## Transition Workflow (Simplified)
```c
bool gm_enter_mode(void) {
    lock_mode_mutex();
    if (gm_mode_state.active) { unlock_mode_mutex(); return true; } // Idempotent
    if (gm2_mode_active()) { unlock_mode_mutex(); return false; }   // Conflict
    if (!gm_perform_initialization()) { gm_mode_state.failed_inits++; unlock_mode_mutex(); return false; }
    gm_mode_state.active = true;
    gm_mode_state.epoch++;
    gm_mode_state.last_transition_us = system_time_us();
    emit_event(GM_MODE_ENTER, gm_mode_state.epoch);
    unlock_mode_mutex();
    return true;
}

bool gm_exit_mode(void) {
    lock_mode_mutex();
    if (!gm_mode_state.active) { unlock_mode_mutex(); return true; } // Idempotent
    gm_graceful_shutdown();
    gm_mode_state.active = false;
    gm_mode_state.last_transition_us = system_time_us();
    emit_event(GM_MODE_EXIT, gm_mode_state.epoch);
    unlock_mode_mutex();
    return true;
}
```

## Acceptance Criteria
1. AC1: After valid GM ON, `gm_is_active()` returns true and epoch increments exactly once.  
2. AC2: GM OFF sets `gm_is_active()` false only after all voices silent (active_count=0).  
3. AC3: Concurrent GM ON sysEx flood processed serially (no overlapping partial states; epoch increments once).  
4. AC4: Failed initialization leaves `gm_is_active()` false and increments failure counter.  
5. AC5: Snapshot API returns consistent state (program 1 mapping etc.) immediately after activation.  
6. AC6: Mode transition logs emit exactly one ENTER and one EXIT event per successful cycle.  
7. AC7: GM ON while hypothetical GM2 active is ignored (no partial activation, epoch unchanged).  
8. AC8: High-frequency query calls to `gm_is_active()` incur negligible overhead (O(1)).

## Test Scenarios
- **Normal Cycle**: GM ON → GM OFF; verify epoch increments and voices flush.
- **Failure Rollback**: Inject simulated initialization failure; ensure mode inactive.
- **Concurrency Flood**: Send multiple GM ON in rapid succession; ensure single activation.
- **Snapshot Consistency**: Immediately capture snapshot post-init and validate expected defaults.

## Risks / Considerations
- Race conditions without locking can cause partial initialization exposure.
- Future GM2 or XG coexistence requires more elaborate mode arbitration—placeholder logic documented.

## References
- GM1 Predictable Mode Operation Guidelines
- MIDI Universal SysEx Mode Control
