# P1 Requirements - MTC and GM1 Specifications (Ready for GitHub Issues)

**Status**: READY FOR GITHUB ISSUE CREATION  
**Source Specifications**: 
- RP-004-008: MIDI Time Code Specification (MTC)
- RP-003: General MIDI System Level 1 Specification (GM1)  
**Date**: 2025-11-24  
**Phase**: 02-requirements

## Summary

This document contains 10 P1 (High Priority) requirements derived from MIDI Time Code and General MIDI Level 1 specifications. These requirements are READY TO BE CREATED AS GITHUB ISSUES to complete the specification loop before loading the next MIDI specification.

**CRITICAL**: These requirements MUST be created as GitHub Issues before proceeding to load the next specification (e.g., GM Level 2, MPE, MSC, MMC, etc.) to prevent length errors and ensure specification content retention.

---

## REQ-F-CV-003: Parse Polyphonic Key Pressure Messages

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: Channel Voice Messages

### Description
The MIDI parser shall correctly parse and validate MIDI 1.0 Polyphonic Key Pressure (Aftertouch) messages (status byte 0xAn where n = channel 0-15).

### Message Format
- **Byte 1**: Status byte 0xAn (where n = MIDI channel 0-15)
- **Byte 2**: Note number (0-127) - which key
- **Byte 3**: Pressure value (0-127) - pressure amount

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #2 (STR-FUNC-002), #8 (STR-PERF-002)
- **Related**: #18 (REQ-F-CV-001), #19 (REQ-F-CV-002)

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:channel-voice`

---

## REQ-F-CV-005: Parse Channel Pressure Messages

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: Channel Voice Messages

### Description
The MIDI parser shall correctly parse and validate MIDI 1.0 Channel Pressure (Channel Aftertouch) messages (status byte 0xDn where n = channel 0-15).

### Message Format
- **Byte 1**: Status byte 0xDn (where n = MIDI channel 0-15)
- **Byte 2**: Pressure value (0-127) - greatest pressure across all keys

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #2 (STR-FUNC-002), #8 (STR-PERF-002)
- **Related**: #18 (REQ-F-CV-001), REQ-F-CV-003

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:channel-voice`

---

## REQ-F-MTC-001: Parse MIDI Time Code Quarter Frame Messages

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: MIDI Time Code (RP-004-008)

### Description
The MIDI parser shall correctly parse and reconstruct MIDI Time Code (MTC) from Quarter Frame messages to enable SMPTE timecode synchronization.

### Message Format
- **Status**: 0xF1 (System Common - MTC Quarter Frame)
- **Data Byte**: 0nnn dddd (nnn = piece number 0-7, dddd = 4-bit data value)

### Timing Requirements
- Update rate: 120 Quarter Frames/second at 30fps
- Update rate: 96 Quarter Frames/second at 24fps
- Latency: 8.3ms between updates at 30fps

### SMPTE Types
- 00 = 24 frames/second
- 01 = 25 frames/second
- 10 = 30 frames/second (drop frame)
- 11 = 30 frames/second (non-drop frame)

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #6 (STR-INT-001), #8 (STR-PERF-002)
- **Related**: REQ-F-MTC-002, #24 (REQ-F-CORE-004)

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:mtc`, `midi-spec:system-common`

---

## REQ-F-MTC-002: Parse MIDI Time Code Full Messages

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: MIDI Time Code (RP-004-008)

### Description
The MIDI parser shall correctly parse MIDI Time Code Full Messages (System Exclusive) used for locate, shuttle, and immediate timecode positioning.

### Message Format (10 bytes)
```
F0 7F <device> 01 01 <hr> <mn> <sc> <fr> F7
```

### Use Cases
- Locate: Jump to specific timecode position
- Shuttle: Fast forward/rewind positioning
- Initialize: Set initial timecode on startup

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #6 (STR-INT-001), #8 (STR-PERF-002)
- **Related**: REQ-F-MTC-001, #25 (REQ-F-CORE-005)

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:mtc`, `midi-spec:sysex`

---

## REQ-F-RPN-001: Process Registered Parameter Numbers

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: Control Change, GM1 (RP-003)

### Description
The MIDI parser shall correctly process Registered Parameter Number (RPN) sequences using Control Change messages.

### RPN Message Sequence
1. CC#101 (0x65): RPN Parameter Number MSB
2. CC#100 (0x64): RPN Parameter Number LSB
3. CC#6 (0x06): Data Entry MSB
4. CC#38 (0x26): Data Entry LSB (optional)

### Standard RPNs
- **00 00**: Pitch Bend Sensitivity (semitones + cents)
- **00 01**: Fine Tuning (-8192 to +8191 cents, 14-bit)
- **00 02**: Coarse Tuning (-64 to +63 semitones)
- **7F 7F**: NULL RPN (deselect)

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #6 (STR-INT-001), #7 (STR-FUNC-005)
- **Related**: #19 (REQ-F-CV-002), REQ-F-NRPN-001, REQ-F-GM1-001

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:control-change`, `midi-spec:gm1`

---

## REQ-F-NRPN-001: Process Non-Registered Parameter Numbers

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: Control Change

### Description
The MIDI parser shall correctly process Non-Registered Parameter Number (NRPN) sequences using Control Change messages for manufacturer-specific parameter control.

### NRPN Message Sequence
1. CC#99 (0x63): NRPN Parameter Number MSB
2. CC#98 (0x62): NRPN Parameter Number LSB
3. CC#6 (0x06): Data Entry MSB
4. CC#38 (0x26): Data Entry LSB (optional)

### Key Characteristics
- Manufacturer-specific (16,384 parameters)
- 14-bit parameter number (MSB+LSB)
- 14-bit values (CC#6 MSB + CC#38 LSB)
- NULL NRPN: 7F 7F to deselect

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #6 (STR-INT-001)
- **Related**: #19 (REQ-F-CV-002), REQ-F-RPN-001

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:control-change`

---

## REQ-F-GM1-001: Support General MIDI Level 1 Sound Set

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: GM1  
**MIDI Spec**: General MIDI Level 1 (RP-003)

### Description
The MIDI implementation shall recognize and validate General MIDI Level 1 Program Change messages from the 128-instrument standard sound set.

### Sound Set (128 Instruments in 16 Groups)
1. **Piano** (1-8): Acoustic Grand Piano → Clavi
2. **Chromatic Percussion** (9-16): Celesta → Dulcimer
3. **Organ** (17-24): Drawbar Organ → Tango Accordion
4. **Guitar** (25-32): Acoustic Guitar (nylon) → Guitar harmonics
5. **Bass** (33-40): Acoustic Bass → Synth Bass 2
6. **Strings** (41-48): Violin → Timpani
7. **Ensemble** (49-56): String Ensemble 1 → Orchestra Hit
8. **Brass** (57-64): Trumpet → SynthBrass 2
9. **Reed** (65-72): Soprano Sax → Clarinet
10. **Pipe** (73-80): Piccolo → Ocarina
11. **Synth Lead** (81-88): Lead 1 (square) → Lead 8 (bass + lead)
12. **Synth Pad** (89-96): Pad 1 (new age) → Pad 8 (sweep)
13. **Synth Effects** (97-104): FX 1 (rain) → FX 8 (sci-fi)
14. **Ethnic** (105-112): Sitar → Shanai
15. **Percussive** (113-120): Tinkle Bell → Reverse Cymbal
16. **Sound Effects** (121-128): Guitar Fret Noise → Gunshot

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #7 (STR-FUNC-005), #6 (STR-INT-001)
- **Related**: #20 (REQ-F-CV-004), REQ-F-GM1-002, REQ-F-RPN-001

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:gm1`, `midi-spec:gm1`, `midi-spec:program-change`

---

## REQ-F-GM1-002: Support General MIDI Level 1 Percussion Map

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: GM1  
**MIDI Spec**: General MIDI Level 1 (RP-003)

### Description
The MIDI implementation shall recognize and validate General MIDI Level 1 percussion notes on Channel 10 (MIDI channel 9, 0-indexed) with the standard 47-drum map.

### Percussion Map (Channel 10, Keys 35-81)
- **Key 35**: Acoustic Bass Drum
- **Key 36**: Bass Drum 1
- **Key 38**: Acoustic Snare
- **Key 42**: Closed Hi Hat
- **Key 46**: Open Hi-Hat
- **Key 49**: Crash Cymbal 1
- **Key 51**: Ride Cymbal 1
- ... (47 total drum sounds from keys 35-81)

### Key Requirements
- Channel 10 exclusive (MIDI channel 9, 0-indexed)
- 47 defined sounds (keys 35-81)
- Velocity-sensitive (0-127)

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #7 (STR-FUNC-005), #6 (STR-INT-001)
- **Related**: #18 (REQ-F-CV-001), REQ-F-GM1-001

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:gm1`, `midi-spec:gm1`, `midi-spec:channel-voice`

---

## REQ-F-RS-001: Support MIDI Running Status Optimization

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: MIDI 1.0 Running Status

### Description
The MIDI parser shall support Running Status optimization where repeated status bytes are omitted to reduce bandwidth usage for consecutive messages on the same channel.

### Running Status Rules
1. Applies to: Channel Voice Messages (0x80-0xEF only)
2. Does NOT apply to: System Messages (0xF0-0xFF)
3. Status byte can be omitted if same as previous message
4. System Common/Real-Time messages reset Running Status
5. Efficiency: Saves 33% bandwidth for consecutive Note On

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #8 (STR-PERF-002), #13 (STR-PERF-005)
- **Related**: #18 (REQ-F-CV-001), #19 (REQ-F-CV-002)

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:running-status`, `midi-spec:optimization`

---

## REQ-F-CM-001: Process MIDI Channel Mode Messages

**Priority**: P1 (High)  
**Type**: Functional  
**Component**: Parser  
**MIDI Spec**: Channel Mode Messages

### Description
The MIDI parser shall correctly parse and process Channel Mode Messages (Control Change 120-127) that control channel-wide behavior.

### Channel Mode Messages (CC#120-127)
- **CC#120**: All Sound Off (immediate silence)
- **CC#121**: Reset All Controllers (restore defaults)
- **CC#122**: Local Control (0=Off, 127=On)
- **CC#123**: All Notes Off (natural release)
- **CC#124**: Omni Mode Off (+ All Notes Off)
- **CC#125**: Omni Mode On (+ All Notes Off)
- **CC#126**: Mono Mode On / Poly Off (+ All Notes Off)
- **CC#127**: Poly Mode On / Mono Off (+ All Notes Off)

### Operating Modes
1. **Mode 1**: Omni On, Poly
2. **Mode 2**: Omni On, Mono
3. **Mode 3**: Omni Off, Poly (most common)
4. **Mode 4**: Omni Off, Mono (MIDI guitar)

### Traceability
- **Traces to**: #1 (STR-FUNC-001), #6 (STR-INT-001)
- **Related**: #19 (REQ-F-CV-002), #18 (REQ-F-CV-001)

### Labels
`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:parser`, `midi-spec:channel-mode`, `midi-spec:control-change`

---

## Completion Status

**MTC and GM1 Specification Loop Status**: REQUIREMENTS DEFINED ✅

**Next Steps to Complete Loop**:
1. ✅ Load MTC specification (RP-004-008) - DONE
2. ✅ Load GM1 specification (RP-003) - DONE
3. ✅ Define requirements from MTC and GM1 - DONE (10 requirements documented above)
4. ⏳ **CREATE GITHUB ISSUES** for these 10 P1 requirements - PENDING
5. ⏳ Verify all requirements created successfully
6. ⏳ Mark MTC and GM1 loop COMPLETE
7. ⏳ THEN proceed to load next specification

**CRITICAL**: Do NOT load the next specification until these 10 requirements are created as GitHub Issues!

---

## Implementation Notes

- **GitHub Issue Creation Tool**: `mcp_github_github_issue_write` has parameter format issues
- **Alternative Approach**: May need to use GitHub CLI or Python GitHub API
- **Manual Creation**: If automated tools fail, these requirements can be created manually via GitHub web UI
- **Issue Numbers**: Will be assigned sequentially starting from #35 (last created issue was #34)

**Priority**: Complete this loop BEFORE loading General MIDI Level 2, MPE, MSC, MMC, or any other specification!
