# REQ-F-GM1-007: GM1 Dynamic Multi-Channel Voice Allocation and Fairness

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1 (General MIDI System Level 1)
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (General MIDI Level 1 Stakeholder Requirement)
- **Depends on**: #537, #538, #539, #541 (instrument, percussion, program change, loading)
- **Related**: #540 (Sound Quality), REQ-F-GM1-006 (Polyphony minimum)
- **Will be verified by**: TEST-GM1-POLY-FAIR-001, TEST-GM1-POLY-FAIR-002

## Description
Implement a dynamic, demand-driven voice allocation mechanism across GM1 channels that ensures:
- Fair distribution of available polyphony.
- No permanent monopolization by any single busy channel.
- Priority behavior for short-lived percussion voices when musically justified.
- Prevention of starvation for low-activity channels.

## Allocation Principles
1. **Demand-Based**: Voices assigned on each Note On; no static per-channel quotas.
2. **Fairness Window**: Track recent allocations per channel over a rolling time window (e.g., 500 ms) to avoid domination.
3. **Starvation Prevention**: If a channel has received zero allocations in the last fairness window while requesting notes, its next request is boosted.
4. **Priority Hints**: Percussion (Channel 10) MAY receive slight priority for transient integrity (configurable weight).
5. **Adaptive Weighting**: Instrument category (e.g., sustained pads vs staccato drums) can influence retention vs stealing.

## Data Structures
```c
typedef struct {
    uint8_t  channel;            // MIDI channel (0-15)
    uint16_t allocations_last_window; // Count in rolling window
    uint32_t last_allocation_ts; // Timestamp (ms)
    uint16_t pending_requests;   // Note On attempts while starved
    float    priority_weight;    // Dynamic weight (percussion > melodic sustain?)
} channel_allocation_stats_t;

channel_allocation_stats_t channel_stats[16];

#define FAIRNESS_WINDOW_MS 500
#define STARVATION_THRESHOLD 0  // No allocations in window qualifies
#define BOOST_WEIGHT 1.25f
```

## Fairness Update Routine
```c
void update_channel_allocation(uint8_t ch) {
    channel_allocation_stats_t* s = &channel_stats[ch];
    uint32_t now = get_time_ms();
    
    // Decay allocations outside fairness window
    if (now - s->last_allocation_ts > FAIRNESS_WINDOW_MS) {
        s->allocations_last_window = 0;
    }
    s->allocations_last_window++;
    s->last_allocation_ts = now;
    s->pending_requests = 0; // Reset on successful allocation
}
```

## Allocation Decision (Pseudo-Code)
```c
int select_voice_for_channel(uint8_t ch, uint8_t key, uint8_t velocity) {
    // 1. If free voice available → allocate immediately
    int free_idx = find_free_voice_slot();
    if (free_idx >= 0) {
        return free_idx;
    }
    
    // 2. Apply fairness boost if channel starved
    channel_allocation_stats_t* s = &channel_stats[ch];
    bool starved = (s->allocations_last_window == STARVATION_THRESHOLD && s->pending_requests > 0);
    float channel_weight = s->priority_weight * (starved ? BOOST_WEIGHT : 1.0f);
    
    // 3. Select candidate voice to steal
    int victim = select_voice_to_steal_fair(channel_weight); // Weighted selection
    return victim; // -1 if none (should be rare)
}
```

## Weighted Stealing Strategy
```c
int select_voice_to_steal_fair(float requesting_weight) {
    int victim = -1;
    float best_score = -1.0f; // Higher score → better steal candidate
    
    for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
        gm1_voice_t* v = &pool.voices[i];
        if (v->state == VOICE_STATE_ACTIVE) {
            float age = (get_time_ms() - v->start_timestamp) / 1000.0f; // seconds
            float quiet_factor = 1.0f - clamp01(v->current_level); // Quiet voices preferred
            float percussion_penalty = v->is_percussion ? 0.3f : 1.0f; // Avoid stealing fresh drums
            float score = (age * 0.4f + quiet_factor * 0.6f) * percussion_penalty;
            
            if (score > best_score) {
                best_score = score;
                victim = i;
            }
        }
    }
    return victim;
}
```

## Starvation Tracking
```c
void note_on_request(uint8_t ch) {
    channel_allocation_stats_t* s = &channel_stats[ch];
    uint32_t now = get_time_ms();
    if (now - s->last_allocation_ts > FAIRNESS_WINDOW_MS) {
        s->pending_requests++; // Counts toward starvation boost
    }
}
```

## Fairness Metrics
Implementation SHALL expose:
- allocations_last_window per channel
- pending_requests per channel
- steals_per_channel count
- starvation_events count

## Acceptance Criteria
- AC1: No single busy channel secures >70% of allocations over any 1s interval when ≥3 channels active.
- AC2: A channel attempting notes during a fairness window with zero allocations receives boosted allocation within next 3 Note On attempts.
- AC3: Percussion heavy patterns do not excessively starve sustained melodic channels (≤50% starvation events in stress test).
- AC4: Weighted stealing selects quieter/older voices ≥80% of time under polyphony stress.
- AC5: Fairness metrics accurately reflect distribution after a synthetic multi-channel test.
- AC6: Starvation prevention reduces pending_requests counter as soon as allocation succeeds.
- AC7: No indefinite starvation observed in 60s randomized allocation simulation.
- AC8: CPU overhead of fairness algorithm <5% of total synthesis budget (measured).

## Test Scenarios (Outline)
1. **Busy Channel Domination**: Flood Channel 1 vs moderate usage on Channels 2–5 → verify distribution.
2. **Starvation Recovery**: Force Channel 3 to request after silence → ensure boosted success.
3. **Percussion Burst**: Rapid drums + sustained pads → measure fairness metrics.
4. **Voice Steal Quality**: Log victim selection attributes, verify quiet/old preference.
5. **Long Simulation**: 60s randomized multi-channel playback → ensure no channel exclusion.

## Dependencies
- REQ-F-GM1-006 (Polyphony baseline)
- Amplitude metering; timestamp source
- Voice steal policy integration (REQ-F-GM1-008 forthcoming)

## Risks / Considerations
- Over-aggressive fairness could degrade deliberate musical emphasis.
- Priority weights must be tunable to avoid over-favoring percussion.
- Instrument category awareness optional but enhances musical retention logic.

## References
- STR-GM1-Level-1 (GM1 multi-timbral expectations)
- MIDI 1.0 (Channel independence semantics)
