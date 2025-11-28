# REQ-F-GM1-018: GM1 Pitch Bend Range Consistency and Interaction Rules

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: Medium
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Expressive control consistency)  
- **Depends on**: REQ-F-GM1-011 (RPN 0 sensitivity), REQ-F-GM1-012 (Bend behavior), REQ-F-GM1-016..017 (base tuning)
- **Verified by (planned)**: TEST-GM1-BEND-RANGE-001, TEST-GM1-BEND-RANGE-002

## Description
Ensure pitch bend scaling remains stable, deterministic, and correctly composed with tuning offsets and sensitivity changes. Define interactions and ordering rules for multiple simultaneous influences on pitch.

## Requirements
1. FR1: Effective semitone offset = Coarse + Fine + BendContribution; each term additive (no multiplicative distortions).  
2. FR2: BendContribution = (bend14 / 8192.0) * (range_semitones + range_cents/100) using current RPN 0 at evaluation time.  
3. FR3: Changing RPN 0 MUST affect subsequent bend messages immediately; existing sustained notes recompute pitch on next synthesis block.  
4. FR4: Fine and coarse tuning changes MUST NOT retroactively alter historical bend center (8192 always neutral).  
5. FR5: Extreme bend ranges (>24 semitones) MUST be clamped (implementation safety; GM1 typical ≤12).  
6. FR6: Effective pitch offset MUST cap total deviation at ±48 semitones to avoid numerical instability.  
7. FR7: Provide API returning structured pitch components for diagnostics: {base_freq, coarse, fine, bend, total}.  
8. FR8: No cumulative drift: returning bend to center resets contribution to ~0 (±1 cent).  
9. FR9: Simultaneous rapid bend and tuning changes processed in arrival order; no frame reordering.  
10. FR10: Percussion channel MUST always report bend contribution = 0.

## Diagnostic API
```c
typedef struct {
    double base_frequency;
    double coarse_semitones;
    double fine_cents;
    double bend_semitones;
    double total_semitones;
} gm1_pitch_components_t;

gm1_pitch_components_t gm1_get_pitch_components(uint8_t ch, uint8_t key) {
    gm1_pitch_components_t pc = {0};
    pc.base_frequency = gm1_base_frequency(key);
    const gm1_rpn_state_t* st = &rpn_state[ch];
    pc.coarse_semitones = st->coarse_tune_semitones;
    pc.fine_cents       = st->fine_tune_cents;
    int16_t bend14      = current_channel_bend_14bit[ch];
    double bend_range   = st->bend_range_semitones + st->bend_range_cents/100.0;
    pc.bend_semitones   = (double)bend14 / 8192.0 * bend_range;
    double bend_total   = pc.coarse_semitones + pc.fine_cents/100.0 + pc.bend_semitones;
    // Clamp safety
    if (bend_total > 48.0) bend_total = 48.0;
    if (bend_total < -48.0) bend_total = -48.0;
    pc.total_semitones  = bend_total;
    return pc;
}
```

## Acceptance Criteria
1. AC1: Bend + tuning composition yields correct aggregate semitone offset for test vectors (within ±1 cent).  
2. AC2: Returning bend to center resets bend_semitones to ~0 (±1 cent) despite prior tuning changes.  
3. AC3: Increasing sensitivity from ±2 to ±12 updates bend mapping immediately (pitch extremes scale accordingly).  
4. AC4: Out-of-range requested sensitivity (e.g., 36 semitones) clamps to implementation maximum (logged).  
5. AC5: Percussion channel always reports bend_semitones = 0 while other channels respond.  
6. AC6: Rapid interleaving (bend, fine tune, coarse tune) preserves ordering (no stale component in diagnostics).  
7. AC7: Extreme combined offset attempts (>±48) safely clamped without overflow.  
8. AC8: Diagnostic API values match internal pitch path (frequency derived equals base * 2^(total/12)).

## Test Scenarios
- Sensitivity escalation tests (2→7→12) with full negative/positive bends.  
- Combined offset scenario: coarse +5, fine +37 cents, bend mid-positive; verify aggregate.  
- Center reset drift test: apply many bend sweeps, verifying near-zero center accuracy.  
- Percussion immunity test.  
- Clamp test for oversized sensitivity request.

## Risks / Considerations
- Large ranges can make small cent errors perceptually larger; maintain precision.  
- Frequent RPN changes could cause performance spikes if not optimized.  
- Clamping must be documented to avoid user confusion.

## References
- GM1 Tuning and Bend Behavior Goals
- MIDI Pitch Bend & RPN Sensitivity Specification
