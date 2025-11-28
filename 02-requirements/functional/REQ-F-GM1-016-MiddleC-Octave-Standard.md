# REQ-F-GM1-016: GM1 Pitch Reference and Octave Numbering Standard (Middle C = MIDI Key 60)

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Cross-device musical compatibility)
- **Depends on**: REQ-F-GM1-001..015 (sound set, controllers, mode)
- **Verified by (planned)**: TEST-GM1-PITCH-REF-001, TEST-GM1-PITCH-REF-002

## Description
Establish the canonical pitch reference mapping and octave numbering for GM1 so MIDI files yield consistent musical register across compliant devices.

## Requirements
1. FR1: Middle C MUST be represented by MIDI note number 60 (C4 scientific pitch notation).  
2. FR2: A4 (pitch reference) MUST be MIDI note 69 at 440.0 Hz before tuning offsets.  
3. FR3: Octave numbering MUST increment every 12 semitones such that: C4=60, C5=72, C3=48.  
4. FR4: Derived note frequencies MUST follow equal temperament formula f(n) = 440 * 2^((n - 69)/12) prior to tuning adjustments.  
5. FR5: Tuning offsets (fine/coarse, RPN1/RPN2) MUST apply additive semitone+cent deltas after base frequency calculation.  
6. FR6: Implementation MUST expose an API to retrieve base frequency and adjusted frequency for any MIDI key & channel.  
7. FR7: Negative MIDI note numbers (underflows) and >127 MUST be rejected or clamped; no undefined behavior.  
8. FR8: Percussion channel (10) MUST ignore pitched frequency calculations (percussion treated as unpitched; optional mapping retained only for diagnostic).  
9. FR9: Documentation MUST state the adopted octave convention to avoid conflicts (some legacy systems use C3=60; GM1 standard uses C4=60).  
10. FR10: Frequency computation MUST achieve accuracy within ±0.5 cent (≈0.03%) for all representable notes when using double precision; ±1 cent allowed with single precision.

## Frequency Calculation
```c
// Returns base (pre-tuning) frequency in Hz for MIDI key (0-127)
static inline double gm1_base_frequency(uint8_t key) {
    // key 69 = A4 = 440 Hz
    int diff = (int)key - 69;
    return 440.0 * pow(2.0, diff / 12.0);
}

// Apply channel tuning offsets (coarse: semitones, fine: cents)
double gm1_adjusted_frequency(uint8_t channel, uint8_t key) {
    double base = gm1_base_frequency(key);
    const gm1_rpn_state_t* st = &rpn_state[channel];
    double semitone_offset = (double)st->coarse_tune_semitones + (double)st->fine_tune_cents / 100.0;
    // Pitch bend influence
    int16_t bend14 = current_channel_bend_14bit[channel]; // -8192..+8191
    double bend_range = st->bend_range_semitones + st->bend_range_cents / 100.0;
    double bend_semitones = (double)bend14 / 8192.0 * bend_range;
    double total_semitones = semitone_offset + bend_semitones;
    return base * pow(2.0, total_semitones / 12.0);
}
```

## Octave Mapping Table (Reference)
| MIDI Key | Note | Octave | Base Freq (Hz) |
|----------|------|--------|----------------|
| 60 | C | 4 | 261.6256 |
| 61 | C# | 4 | 277.1826 |
| 62 | D | 4 | 293.6648 |
| 63 | D# | 4 | 311.1270 |
| 64 | E | 4 | 329.6276 |
| 65 | F | 4 | 349.2282 |
| 66 | F# | 4 | 369.9944 |
| 67 | G | 4 | 391.9954 |
| 68 | G# | 4 | 415.3047 |
| 69 | A | 4 | 440.0000 |
| 70 | A# | 4 | 466.1638 |
| 71 | B | 4 | 493.8833 |

(Values shown for validation; not reproduced from spec but derived via formula.)

## Acceptance Criteria
1. AC1: Querying key 60 returns ≈261.63 Hz (±0.5 cent).  
2. AC2: Key 69 returns exactly 440.0 Hz before tuning (±0.1 Hz tolerance).  
3. AC3: Applying +100 cent fine tune raises A4 to ≈466.16 Hz (one semitone).  
4. AC4: Bend range ±2 semitones on A4 yields ≈415.30 Hz and ≈466.16 Hz extremes (±1 cent).  
5. AC5: Percussion channel requests do not alter pitched frequency path (returns base or sentinel).  
6. AC6: Octave mapping test confirms C5=72 and C3=48 using declared convention.  
7. AC7: Out-of-range key (e.g., 192) rejected with error code or clamped to 127 without crash.  
8. AC8: Over 0–127 sweep, max frequency error never exceeds ±1 cent.

## Test Scenarios
- Frequency sweep comparing computed vs theoretical formula.  
- Tuning offset application sequence (coarse + fine + bend).  
- Octave boundary validation (C transitions).  
- Percussion isolation test.  
- Precision benchmark with randomized keys under tuning.

## Risks / Considerations
- Floating point precision differences across platforms; prefer double for reference, float for inner loops.  
- Legacy octave numbering confusion; explicit documentation prevents interpretation errors.  
- Extreme bend + tuning combos risk sample playback artifacts (out-of-range or aliasing).

## References
- Equal Temperament Frequency Formula
- GM1 Cross-Device Pitch Consistency Objective
