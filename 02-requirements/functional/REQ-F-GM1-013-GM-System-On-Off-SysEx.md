# REQ-F-GM1-013: GM1 System On / System Off Universal SysEx Handling

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Need for predictable cross-device mode activation)
- **Depends on**: REQ-F-GM1-001..012 (sound set, controllers, tuning)  
- **Will be verified by**: TEST-GM1-SYSEX-ONOFF-001, TEST-GM1-SYSEX-ONOFF-002

## Description
Implement recognition and processing of the *Universal Non-Realtime* SysEx messages that toggle GM1 mode:
- **GM System ON**: `F0 7E 7F 09 01 F7`
- **GM System OFF**: `F0 7E 7F 09 02 F7`

These messages MUST NOT contain manufacturer-specific IDs and MUST be treated as authoritative commands to enter or exit GM1 operating mode. Transition effects include setting default controller states, loading baseline program assignments, and establishing percussion mapping.

## Functional Requirements
1. FR1: Parser MUST correctly identify GM ON/OFF by exact byte sequence (no wildcard matching).  
2. FR2: On GM ON, implementation SHALL set internal `gm_mode_active = true`.  
3. FR3: On GM OFF, implementation SHALL set `gm_mode_active = false` while preserving non-GM runtime safely (sound set remains accessible but GM compliance layer disabled).  
4. FR4: GM ON MUST trigger baseline initialization (see REQ-F-GM1-014) atomically before any subsequent MIDI events are processed.  
5. FR5: Duplicate GM ON messages while already active MUST be idempotent (no re-initialization storms).  
6. FR6: GM OFF MUST gracefully release/flush active voices (option: treat as CC123 + controller neutralization without program reassignment).  
7. FR7: SysEx parsing MUST be streaming-safe: partial reception buffers until end byte (F7) before processing.  
8. FR8: GM ON/OFF detection MUST NOT misclassify unrelated Universal SysEx (e.g., General MIDI Level 2 or MMC).  
9. FR9: Mode flag MUST be immediately queryable for downstream logic (voice allocation, controller interpretation).  
10. FR10: Latency from reception of final F7 byte to completed GM ON init MUST be <10 ms (target).  

## SysEx Recognition Logic (Example)
```c
#define GM_ON_LEN 6
static const uint8_t GM_ON_MSG[GM_ON_LEN]  = {0xF0,0x7E,0x7F,0x09,0x01,0xF7};
static const uint8_t GM_OFF_MSG[GM_ON_LEN] = {0xF0,0x7E,0x7F,0x09,0x02,0xF7};

typedef enum { GM_SYSEX_NONE=0, GM_SYSEX_ON, GM_SYSEX_OFF } gm_sysex_type_t;

gm_sysex_type_t classify_gm_sysex(const uint8_t* data, size_t len) {
    if (len == GM_ON_LEN && memcmp(data, GM_ON_MSG, GM_ON_LEN) == 0)  return GM_SYSEX_ON;
    if (len == GM_ON_LEN && memcmp(data, GM_OFF_MSG, GM_ON_LEN) == 0) return GM_SYSEX_OFF;
    return GM_SYSEX_NONE;
}
```

## Processing Flow
```c
void process_sysex(const uint8_t* data, size_t len) {
    gm_sysex_type_t t = classify_gm_sysex(data, len);
    switch (t) {
        case GM_SYSEX_ON:
            if (!gm_mode_active) {
                gm_mode_active = true;
                gm_perform_initialization(); // REQ-F-GM1-014 sequence
            }
            break;
        case GM_SYSEX_OFF:
            if (gm_mode_active) {
                gm_mode_active = false;
                gm_graceful_shutdown(); // Flush voices, keep safe state
            }
            break;
        default:
            // Ignore or forward to other handlers
            break;
    }
}
```

## Error Handling
- Invalid length or premature end-of-stream MUST NOT trigger mode changes.
- Partial SysEx sequences buffered until F7 or timeout (e.g., >2 s) then discarded.
- If GM OFF received while already inactive → no-op.

## Acceptance Criteria
1. AC1: Exact GM ON message transitions system to GM active state and performs full initialization.  
2. AC2: GM OFF message transitions system out of GM state, releasing active voices cleanly.  
3. AC3: Repeated GM ON while active causes zero additional initialization cycles (metric count).  
4. AC4: Partial SysEx (missing F7) does not change mode even if prefix matches.  
5. AC5: Non-GM Universal SysEx (e.g., MMC) never changes `gm_mode_active`.  
6. AC6: Latency measurement shows <10 ms from F7 receipt to post-init ready state (profiling log).  
7. AC7: GM OFF preserves system stability: no orphaned voice entries remain (active_count=0).  
8. AC8: Mode flag accessible to allocation and controller logic immediately after transition.

## Test Scenarios
- **Valid GM ON**: Feed exact sequence; verify state, controllers, program defaults.
- **Valid GM OFF**: Play notes, send OFF; confirm release and state flag false.
- **Duplicate ON**: Send GM ON twice; check initialization count=1.
- **Truncated SysEx**: Send first 5 bytes only; ensure no mode switch.
- **Mixed Universal Messages**: Interleave GM ON with other Universal SysEx; ensure isolation.

## Risks / Considerations
- Streaming fragmentation may delay recognition; buffer management must be robust.
- Large initialization in low-power targets could exceed latency target—optimize sequence.

## References
- MIDI Universal Non-Realtime SysEx definitions
- GM1 Mode Activation Requirements (predictable baseline state)
