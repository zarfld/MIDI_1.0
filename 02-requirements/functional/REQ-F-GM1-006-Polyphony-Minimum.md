# REQ-F-GM1-006: GM1 Minimum Polyphony and Baseline Allocation Models

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1 (General MIDI System Level 1)
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (General MIDI Level 1 Stakeholder Requirement)
- **Depends on**: #537, #538, #539 (GM1 instrument, percussion and program change foundations)
- **Related**: #540 (Sound Quality), #541 (Loading & Sample Management)
- **Will be verified by**: TEST-GM1-POLY-001, TEST-GM1-POLY-002

## Description
The implementation SHALL support a minimum simultaneous polyphony of **24 voices** while in GM1 mode. Two compliance allocation baselines are acceptable:
1. Fully dynamic pool of 24 voices shared across all melodic and percussion parts.
2. Partitioned model: 16 melodic voices + 8 percussion voices (Channel 10) when resource constraints demand deterministic percussion reservation.

If multiple polyphony strategies are implemented (e.g., user selectable), both MUST satisfy minimum voice guarantees and voice stealing rules defined herein.

## Functional Requirements
1. FR1: System SHALL always allow at least 24 Note On events to sound concurrently before any voice stealing occurs.
2. FR2: Partitioned model (if used) SHALL reserve 8 percussion voice slots strictly for Channel 10 events.
3. FR3: Dynamic model SHALL not permanently assign fixed quotas per channel; voices are allocated on demand.
4. FR4: Voice limit enforcement SHALL trigger a stealing algorithm only after exceeding 24 active voices.
5. FR5: Sustained notes (including with CC#64) count toward active voice total until release phase completes.
6. FR6: Percussion voices (Channel 10) are mono event voices (no pitch tracking) but count toward total polyphony.
7. FR7: Implementation SHALL expose current polyphony usage metrics for diagnostic/testing.
8. FR8: Initialization SHALL start with zero active voices and empty allocation tables.

## Voice Data Model (Conceptual)
```c
#define GM1_MIN_POLYPHONY 24

typedef enum {
    VOICE_STATE_FREE = 0,
    VOICE_STATE_ACTIVE,
    VOICE_STATE_RELEASE,
    VOICE_STATE_WAITING_UNLOAD
} voice_state_t;

typedef struct {
    uint16_t        id;              // Unique voice id
    uint8_t         channel;         // MIDI channel (0-15)
    uint8_t         key;             // MIDI key (0-127) or percussion key
    uint8_t         velocity;        // Note On velocity
    uint8_t         program;         // Program at allocation (melodic only)
    bool            is_percussion;   // Channel 10 flag
    voice_state_t   state;           // Lifecycle state
    uint32_t        start_timestamp; // Allocation time (for stealing heuristics)
    float           current_level;   // Amplitude estimate (for quiet-voice stealing)
} gm1_voice_t;

typedef struct {
    gm1_voice_t voices[GM1_MIN_POLYPHONY];
    uint8_t     active_count;
    uint8_t     melodic_active;
    uint8_t     percussion_active;
} gm1_voice_pool_t;
```

## Allocation Workflow (Dynamic Model)
```c
bool allocate_voice(uint8_t channel, uint8_t key, uint8_t velocity) {
    // 1. Try free slot
    int idx = find_free_voice_slot();
    if (idx >= 0) {
        init_voice(&pool.voices[idx], channel, key, velocity);
        return true;
    }
    // 2. If pool full → perform voice stealing
    int steal_idx = select_voice_to_steal();
    if (steal_idx >= 0) {
        recycle_voice(&pool.voices[steal_idx], channel, key, velocity);
        return true;
    }
    return false; // Should not happen if stealing works
}
```

## Allocation Workflow (Partitioned Model)
```c
#define GM1_MELODIC_PARTITION 16
#define GM1_PERC_PARTITION     8

bool allocate_partitioned(uint8_t channel, uint8_t key, uint8_t velocity) {
    if (channel == 9) { // Channel 10 (0-indexed)
        return allocate_in_partition(PERCUSSION);
    } else {
        return allocate_in_partition(MELODIC);
    }
}
```

## Voice Stealing Policy (Minimum Rules)
1. Prefer stealing voices in RELEASE state first.
2. If none in RELEASE: steal the quietest ACTIVE voice (lowest current_level) not on Channel 10 when possible.
3. Never steal percussion voice still within its transient attack (configurable attack window, e.g. <20 ms).
4. Sustain pedal (CC#64) held voices MAY be stolen if polyphony exhaustion threatens real-time timing (last resort).
5. Provide deterministic fallback tie-breaker (oldest start_timestamp).

## Metrics and Instrumentation
Implementation SHALL publish (internal or test interface):
- current_active_voices
- melodic_vs_percussion_distribution
- steals_per_minute
- average_voice_lifetime_ms

## Acceptance Criteria
- AC1: System plays 24 simultaneous distinct Note On events without premature truncation.
- AC2: When 25th Note On arrives, only one existing voice is justifiably reallocated per stealing policy.
- AC3: Partition model (if enabled) never allows percussion allocation to consume melodic reserved slots.
- AC4: Release-phase voices are preferentially stolen before active sustained voices (observed in test logs).
- AC5: Percussion transient (first 20 ms) is never audibly interrupted by stealing.
- AC6: Diagnostic metrics report accurate counts after stress test (±1 tolerance).
- AC7: Startup test shows zero active voices and no allocation residue.
- AC8: No starvation: rapid repetition on a single channel does not permanently block other channels from acquiring voices.

## Test Scenarios (Outline)
1. **Polyphony Flood**: Trigger 30 Note On events with staggered timing, verify first 24 remain, next 6 cause controlled stealing.
2. **Percussion Reservation** (partition model): Fill melodic voices; confirm Channel 10 still allocates all 8 percussion voices.
3. **Release Preference**: Sustain long tones, release some; new notes steal only released ones first.
4. **Quiet Steal**: Mix loud and soft notes; ensure soft (low amplitude) voices are stolen before loud sustained ones.
5. **Transient Protection**: Rapid drum pattern during polyphony stress does not truncate attacks.

## Dependencies
- Voice lifecycle management (see planned REQ-F-GM1-008)
- Sound generation back-end amplitude metering
- Timing source for timestamps

## Risks / Considerations
- Limited amplitude measurement precision may affect quietest selection.
- Aggressive stealing can degrade musical quality; tune thresholds carefully.
- Partition model reduces flexibility but improves rhythmic reliability.

## References
- STR-GM1-Level-1 (Polyphony baseline 24 voices)
- MIDI 1.0 Spec (Note On/Off semantics, CC#64 sustain)
- Industry practice (1990s GM sound modules: 24–32 voice common limits)
