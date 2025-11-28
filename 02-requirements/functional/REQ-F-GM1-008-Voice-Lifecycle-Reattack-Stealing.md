# REQ-F-GM1-008: GM1 Voice Lifecycle, Note Re-Attack, and Stealing Strategy

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1 (General MIDI System Level 1)
- **Status**: Draft
- **Priority**: Medium
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (General MIDI Level 1 Stakeholder Requirement)
- **Depends on**: #537, #538, #539, REQ-F-GM1-006, REQ-F-GM1-007
- **Related**: #540 (Sound Quality), #541 (Loading), polyphony & fairness model
- **Will be verified by**: TEST-GM1-VOICE-LIFE-001, TEST-GM1-VOICE-LIFE-002

## Description
Define the lifecycle of a GM1 voice (from allocation to release), handling of repeated notes (“re-attack”), sustain pedal interaction, and deterministic voice stealing policy under polyphony limits. Ensure musical continuity while respecting minimum polyphony and fairness rules.

## Voice Lifecycle States
1. **FREE**: Unallocated; available for new Note On.
2. **ACTIVE (ATTACK/SUSTAIN)**: Playing primary envelope segments.
3. **RELEASE**: Note Off received (or damped); envelope tail decaying.
4. **WAITING_UNLOAD**: (Optional) Marked for resource recycling once inaudible threshold reached.

## Required Behaviors
- RB1: Each Note On MUST acquire a *distinct voice* even if the same key/channel is already sounding (polyphonic layering).
- RB2: Note Off MUST transition voice to RELEASE unless sustain pedal (CC#64 >= 64) holds it in SUSTAIN.
- RB3: Sustain pedal release (transition from ≥64 to <64) MUST flush all held voices to RELEASE.
- RB4: Percussion voices (Channel 10) MUST ignore sustain pedal semantics (one-shot playback).
- RB5: Re-attack (same key played again before previous voice released) MUST **NOT** terminate previous voice prematurely; new voice allocated independently.
- RB6: Optional optimization: If velocity is extremely low (< threshold), may reuse existing voice if still in early ATTACK (configurable) — only if this does not violate RB1 (default disabled).
- RB7: Voices in RELEASE longer than configured tail timeout (e.g., 2000 ms or amplitude < -50 dB) SHALL auto-return to FREE.
- RB8: Voice stealing MUST prefer RELEASE voices then quiet ACTIVE voices as per policies defined below.

## Re-Attack Handling
```c
bool handle_note_on(uint8_t channel, uint8_t key, uint8_t velocity) {
    // Always allocate a fresh voice (baseline compliance)
    int slot = allocate_voice(channel, key, velocity); // Uses REQ-F-GM1-006 logic
    if (slot < 0) return false; // Allocation failure (should be rare)
    init_voice_envelope_attack(&pool.voices[slot], velocity);
    return true;
}
```

## Sustain Pedal Interaction
```c
void handle_sustain_pedal(uint8_t channel, uint8_t value) {
    bool sustain_on = (value >= 64);
    if (!sustain_on) {
        // Flush all voices in sustain state for this channel to RELEASE
        for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
            gm1_voice_t* v = &pool.voices[i];
            if (v->channel == channel && v->state == VOICE_STATE_ACTIVE && v->envelope_hold) {
                v->envelope_hold = false;
                transition_to_release(v);
            }
        }
    } else {
        // Mark voices as held (prevent release transition)
        for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
            gm1_voice_t* v = &pool.voices[i];
            if (v->channel == channel && v->state == VOICE_STATE_ACTIVE) {
                v->envelope_hold = true;
            }
        }
    }
}
```

## Voice Stealing Hierarchy
Order of preference for selection:
1. RELEASE voices beyond minimum tail time.
2. RELEASE voices (any age) with amplitude below quiet threshold.
3. ACTIVE voices with amplitude below quiet threshold.
4. Oldest ACTIVE voices not held by sustain pedal.
5. Least recently activated percussion voice (if absolutely necessary, avoid fresh transient).

### Selection Algorithm (Pseudo-Code)
```c
int select_steal_candidate(void) {
    int candidate = -1;
    float best_rank = -1.0f; // Higher rank = better victim
    uint32_t now = get_time_ms();
    
    for (int i = 0; i < GM1_MIN_POLYPHONY; i++) {
        gm1_voice_t* v = &pool.voices[i];
        if (v->state == VOICE_STATE_FREE) continue;
        float age_ms = (float)(now - v->start_timestamp);
        float amp = v->current_level; // 0.0 - 1.0
        bool release_stage = (v->state == VOICE_STATE_RELEASE);
        bool percussion = v->is_percussion;
        bool sustain_held = v->envelope_hold;
        
        float rank = 0.0f;
        if (release_stage) rank += 2.0f; // Strong preference
        if (amp < 0.1f) rank += 1.0f;    // Quiet
        if (age_ms > 1500.0f) rank += 0.5f;
        if (percussion && age_ms < 50.0f) rank -= 2.0f; // Protect fresh hits
        if (sustain_held) rank -= 1.5f;  // Avoid held notes
        
        if (rank > best_rank) {
            best_rank = rank;
            candidate = i;
        }
    }
    return candidate;
}
```

## Release and Recycling
```c
void update_voice_release(gm1_voice_t* v) {
    if (v->state != VOICE_STATE_RELEASE) return;
    if (v->current_level < 0.01f || voice_age_ms(v) > 2000) {
        recycle_voice(v); // Return to FREE
    }
}
```

## Diagnostics
Expose instrumentation:
- active_voices
- voices_in_release
- average_release_duration_ms
- reattack_events (same key/channel sequential allocations)
- stealing_operations_count
- sustain_held_count

## Acceptance Criteria
- AC1: Two rapid Note On events (same key/channel) produce two distinct voices (verified by active count increment).
- AC2: Sustain pedal press prevents voices from entering RELEASE until pedal lifted.
- AC3: Voice stealing preferentially targets RELEASE voices in ≥80% of steal events (stress test).
- AC4: Fresh percussion attacks (<50 ms) are not stolen in polyphony exhaustion scenarios.
- AC5: Voices in RELEASE auto-recycle within specified tail timeout when amplitude decays below threshold.
- AC6: Re-attack test (arpeggiated chord with overlapping same-key strikes) maintains proper layering without premature truncation.
- AC7: Diagnostic counters reflect correct counts after scripted lifecycle simulation.
- AC8: No memory leaks or dangling states observed after repeated allocation/release cycles (valgrind or equivalent check where applicable).

## Test Scenarios
1. **Re-Attack Layering**: Rapid repeated Note On on middle C; verify multiple active voices until each release concludes.
2. **Sustain Flush**: Hold pedal through chord, release pedal; confirm simultaneous transition to RELEASE.
3. **Steal Preference**: Force >24 active voices; log victim states; confirm release-first behavior.
4. **Percussion Protection**: Dense drum fill + sustained pads; ensure drum attacks preserved.
5. **Release Recycling**: Long decay instruments (strings/pads) eventually freed under inactivity.

## Dependencies
- REQ-F-GM1-006 polyphony pool definition
- REQ-F-GM1-007 fairness weighting (optional integration for steal decisions)
- Amplitude metering subsystem
- Timing source (ms resolution)

## Risks / Considerations
- Sustained pedal-heavy passages may reduce available steal candidates; must balance musicality vs resource limits.
- Overly aggressive stealing can cause audible artifacts; ranking weights require tuning.
- Low-latency systems may need tighter release recycling thresholds to maintain pool availability.

## References
- STR-GM1-Level-1 (multi-timbral and performance expectations)
- MIDI 1.0 (Note On/Off, CC#64 semantics)
