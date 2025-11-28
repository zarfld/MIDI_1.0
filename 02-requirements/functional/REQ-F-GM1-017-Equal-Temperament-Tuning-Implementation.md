# REQ-F-GM1-017: GM1 Equal Temperament Tuning and Precision Guarantees (A4 = 440 Hz)

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Musical interoperability & pitch standardization)  
- **Depends on**: REQ-F-GM1-011 (RPN tuning), REQ-F-GM1-016 (pitch reference)  
- **Verified by (planned)**: TEST-GM1-TEMP-001, TEST-GM1-TEMP-002

## Description
Specify the use of 12-tone equal temperament as the baseline tuning system for GM1 implementations and define precision, performance, and validation metrics for frequency derivation.

## Requirements
1. FR1: All pitched notes MUST use 12-tone equal temperament ratio: multiplier per semitone = 2^(1/12).  
2. FR2: A4 MUST be fixed at exactly 440.0 Hz prior to tuning offsets; drift >0.1 Hz over session is disallowed.  
3. FR3: Frequency derivation MUST avoid cumulative rounding leading to >±1 cent deviation after 10,000 conversions.  
4. FR4: Implementation MUST provide a *reference* high-precision path (double) and MAY use cached tables for performance (float) if error bounds maintained.  
5. FR5: Optional frequency cache/table MUST cover all 128 keys; cache build time <5 ms.  
6. FR6: Tuning offsets (fine, coarse, pitch bend) MUST be applied as additive semitone fractions (not multiplicative distortions).  
7. FR7: Provide API to return both raw (equal temperament) frequency and adjusted (post-tuning) frequency.
8. FR8: Changing RPN tuning or bend sensitivity MUST immediately invalidate cached adjusted results if caching is used.  
9. FR9: Provide cent error reporting for diagnostic mode (difference between computed and theoretical).  
10. FR10: Performance: bulk frequency retrieval for 128 keys <1 ms on reference desktop target (vectorized or fast path acceptable).

## Reference Table Generation (Optional)
```c
static double freq_table_high_precision[128];
static float  freq_table_fast[128];

void build_frequency_tables(void) {
    for (int key=0; key<128; key++) {
        double base = 440.0 * pow(2.0, (key - 69) / 12.0);
        freq_table_high_precision[key] = base;
        freq_table_fast[key] = (float)base; // Cast; error tracked
    }
}
```

## Adjusted Frequency Retrieval
```c
double gm1_get_frequency(uint8_t channel, uint8_t key, bool adjusted) {
    if (key > 127) key = 127; // Clamp
    double base = freq_table_high_precision[key];
    if (!adjusted) return base;
    const gm1_rpn_state_t* st = &rpn_state[channel];
    double semitone_offset = st->coarse_tune_semitones + st->fine_tune_cents / 100.0;
    int16_t bend14 = current_channel_bend_14bit[channel];
    double bend_range = st->bend_range_semitones + st->bend_range_cents / 100.0;
    double bend_semitones = (double)bend14 / 8192.0 * bend_range;
    double total = semitone_offset + bend_semitones;
    return base * pow(2.0, total / 12.0);
}
```

## Cent Error Utility
```c
double compute_cent_error(double theoretical, double measured) {
    // cents = 1200 * log2(measured/theoretical)
    return 1200.0 * (log(measured/theoretical) / log(2.0));
}
```

## Acceptance Criteria
1. AC1: Equal temperament frequencies match theoretical formula within ±0.5 cent for all keys.  
2. AC2: After 10,000 random frequency queries, accumulated max error ≤ ±1 cent (no drift escalation).  
3. AC3: Cache build time measured <5 ms (profile log).  
4. AC4: Adjusted frequency correctly reflects +3 semitone coarse +25 cent fine +1 semitone bend combined (aggregate).  
5. AC5: Changing bend sensitivity forces immediate recompute (no stale values).  
6. AC6: Bulk retrieval (128 adjusted frequencies) completes <1 ms on reference desktop environment.  
7. AC7: Cent error API returns near zero (<0.1 cent) when measured == theoretical.  
8. AC8: A4 remains 440.0 Hz (not drifting) after sequence of tuning changes reverting to neutral.

## Test Scenarios
- Full key sweep vs theoretical (log ratio comparison).  
- Stress random tuning + bend variations measuring error bounds.  
- Cache invalidation test after RPN changes.  
- Performance benchmark for bulk retrieval.

## Risks / Considerations
- Single precision tables may edge beyond cent tolerance for high keys; fallback to double if needed.  
- Excessive recomputation on frequent bend changes can impact performance; consider incremental optimization.  
- Extreme transpositions may demand anti-aliasing measures in playback engine (outside this requirement scope).

## References
- Equal Temperament Mathematical Definition
- GM1 Pitch Standardization Objectives
