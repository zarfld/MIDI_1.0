# REQ-F-GM1-011: GM1 Registered Parameter Numbers (RPN 0, 1, 2) – Pitch Bend Sensitivity & Tuning

## Metadata
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: High
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Tuning & musical compatibility needs)  
- **Depends on**: REQ-F-GM1-009 (Controllers), REQ-F-GM1-010 (Reset semantics)  
- **Will be verified by**: TEST-GM1-RPN-001 (Pitch Bend Range), TEST-GM1-RPN-002 (Fine/Coarse Tuning Accuracy)

## Description
Implement the MIDI Registered Parameter Number selection and data entry process for the GM1-required parameters:
- **RPN 0**: Pitch Bend Sensitivity (MSB = semitones, LSB = cents) – default ±2 semitones (2,0)
- **RPN 1**: Fine Tuning (LSB resolution in cents, ±100 cent range) – center default
- **RPN 2**: Coarse Tuning (MSB semitone offset, ±64 semitones) – center default

## RPN Selection Mechanism
MIDI uses Controllers 101 (RPN MSB) and 100 (RPN LSB) to select parameter, followed by Controllers 6 (Data Entry MSB) and optionally 38 (Data Entry LSB) for value.

## Functional Requirements
1. FR1: RPN selection MUST persist until a new selection or a Null RPN (101=127, 100=127) resets selection context.  
2. FR2: Data Entry (CC6/CC38) MUST apply to currently selected RPN only.  
3. FR3: RPN 0 MUST interpret CC6 as semitones (0–24 typical range; allow full 0–127) and CC38 as cents (0–99).  
4. FR4: RPN 1 MUST interpret combined data as ±100 cents offset; center = 0 shift.  
5. FR5: RPN 2 MUST allow semitone transposition within ±64 (center = 64) using CC6; CC38 ignored or used for extended resolution (implementation choice).  
6. FR6: Pitch Bend computation MUST reference current Pitch Bend Sensitivity (RPN 0) for scaling.  
7. FR7: Reset All Controllers (CC121) MUST restore RPN 0 to default ±2 semitone range and tuning offsets to neutral.  
8. FR8: Null RPN selection (101=127,100=127) MUST prevent further Data Entry from mutating prior RPN targets.  
9. FR9: Out-of-range entries MUST be clamped safely (no wrap-around).  
10. FR10: Channel isolation: RPN configuration is per-channel; changes do NOT affect other channels.

## State Structure
```c
typedef struct {
    int8_t  coarse_tune_semitones;   // RPN 2 (±64)
    int16_t fine_tune_cents;         // RPN 1 (±100)
    uint8_t bend_range_semitones;    // RPN 0 (semitones)
    uint8_t bend_range_cents;        // RPN 0 (cents)
    int16_t current_rpn;             // -1 = null, else (MSB<<7)|LSB
} gm1_rpn_state_t;

gm1_rpn_state_t rpn_state[16];
```

## Selection Processing
```c
void process_rpn_select(uint8_t ch, uint8_t msb, uint8_t lsb) {
    if (msb == 127 && lsb == 127) {
        rpn_state[ch].current_rpn = -1; // Null selection
        return;
    }
    rpn_state[ch].current_rpn = (msb << 7) | lsb; // Combine
}
```

## Data Entry Handling
```c
void process_data_entry(uint8_t ch, uint8_t msb, int lsb_valid, uint8_t lsb) {
    int16_t sel = rpn_state[ch].current_rpn;
    if (sel < 0) return; // Null – ignore
    uint8_t rpn_msb = (sel >> 7) & 0x7F;
    uint8_t rpn_lsb = sel & 0x7F;
    
    if (rpn_msb == 0 && rpn_lsb == 0) { // Pitch Bend Sensitivity
        rpn_state[ch].bend_range_semitones = msb; // Typical 2
        if (lsb_valid) rpn_state[ch].bend_range_cents = lsb; // 0 default
    } else if (rpn_msb == 0 && rpn_lsb == 1) { // Fine Tuning
        // Map MSB/LSB to -100..+100 cents (example: center 64)
        int16_t combined = (int16_t)((msb << 7) | (lsb_valid ? lsb : 0));
        // Implementation-specific mapping; simplest: MSB-64 yields semitone fraction
        rpn_state[ch].fine_tune_cents = (int16_t)( ( (int)msb - 64 ) ); // Simplified placeholder
    } else if (rpn_msb == 0 && rpn_lsb == 2) { // Coarse Tuning
        rpn_state[ch].coarse_tune_semitones = (int8_t)(msb - 64); // Center 64 = 0
    }
}
```

## Pitch Bend Scaling (Interaction)
```c
float compute_pitch_bend_multiplier(uint8_t ch, int16_t bend_value_14bit /* -8192..+8191 */) {
    // Convert bend_value into semitone offset based on range
    float range = (float)rpn_state[ch].bend_range_semitones + (float)rpn_state[ch].bend_range_cents / 100.0f;
    float normalized = (float)bend_value_14bit / 8192.0f; // -1..+1
    float semitone_offset = normalized * range; // Apply sensitivity
    // Add tuning offsets
    semitone_offset += rpn_state[ch].coarse_tune_semitones;
    semitone_offset += (float)rpn_state[ch].fine_tune_cents / 100.0f;
    return semitone_offset;
}
```

## Acceptance Criteria
1. AC1: Default pitch bend sensitivity after reset = ±2 semitones (exact mapping).  
2. AC2: Setting RPN 0 to 12 semitones permits one-octave bend range (bend extremes map to ±12).  
3. AC3: Coarse tuning RPN 2 value 65 produces +1 semitone transposition (center 64 baseline).  
4. AC4: Fine tuning RPN 1 adjustment modifies pitch by expected cent offsets (test within ±5 cent accuracy).  
5. AC5: Null RPN selection prevents subsequent Data Entry from modifying previously set sensitivity.  
6. AC6: Per-channel isolation confirmed: changing Channel 1 RPN does not affect Channel 2 pitch bend scaling.  
7. AC7: Reset All Controllers (CC121) reverts bend range to ±2; coarse/fine tuning neutralized.  
8. AC8: Out-of-range semitone or cent values clamped without wrap (e.g., semitone >24 capped; negative fine tuning limited to -100 cents).

## Test Scenarios
- **Bend Range Expansion**: RPN0 set to 12; apply full bend; measure frequency shift ±12 semitones.
- **Fine Tuning Accuracy**: Sweep fine tuning in small steps; measure pitch shift (Hz → cents). 
- **Isolation Test**: Configure Channel 1 range to 12, Channel 2 stays at 2; verify independent scaling.
- **Null Selection Safety**: Select RPN0 then null; send data entry; confirm no change.

## Risks / Considerations
- Fine tuning interpretation across devices historically inconsistent; document mapping explicitly.  
- Large coarse transpositions may reveal sample aliasing or range limitations.  
- Bend scaling must avoid floating-point accumulation drift (consider caching).

## References
- MIDI 1.0 Detailed Spec – Registered Parameter Numbers (RPN)  
- GM1 Tuning Consistency Requirements
