# P1 (High Priority) Requirements - Comprehensive MIDI 1.0 Coverage

Based on loaded MIDI specifications: Core MIDI 1.0, MIDI Time Code, General MIDI Level 1

## Channel Voice Messages (Advanced)

### REQ-F-CV-003: Parse MIDI Polyphonic Key Pressure (Aftertouch)

**Priority**: P1 (High)  
**Traces to**: #2 (STR-PROT-001)  
**Depends on**: #17 (REQ-F-CORE-001)

**Requirement**: The system SHALL parse and validate MIDI Polyphonic Key Pressure (Aftertouch) messages (0xAn) with note number (0-127) and pressure value (0-127).

**Specification**: MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"

**Acceptance Criteria**:
```gherkin
Scenario: Parse Polyphonic Key Pressure message
  Given a MIDI byte stream [0xA0, 0x3C, 0x50]
  When the parser processes the sequence
  Then the system identifies it as Poly Aftertouch
  And the channel is 0
  And the note number is 60 (Middle C)
  And the pressure value is 80
```

---

### REQ-F-CV-005: Parse MIDI Channel Pressure (Aftertouch)

**Priority**: P1 (High)  
**Traces to**: #2 (STR-PROT-001)  
**Depends on**: #17 (REQ-F-CORE-001)

**Requirement**: The system SHALL parse and validate MIDI Channel Pressure (Aftertouch) messages (0xDn) with pressure value (0-127) according to MIDI 1.0 Detailed Specification.

**Specification**: MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"

**Acceptance Criteria**:
```gherkin
Scenario: Parse Channel Pressure message
  Given a MIDI byte stream [0xD0, 0x50]
  When the parser processes the sequence
  Then the system identifies it as Channel Pressure
  And the channel is 0
  And the pressure value is 80
```

---

## MIDI Time Code (MTC)

### REQ-F-MTC-001: Parse MIDI Time Code Quarter Frame Messages

**Priority**: P1 (High)  
**Traces to**: #4 (STR-SYNC-001: Time synchronization)  
**Depends on**: #23 (REQ-F-CORE-002: System Messages)

**Requirement**: The system SHALL parse MIDI Time Code Quarter Frame messages (0xF1) containing message type (0-7) and 4-bit data nibbles for SMPTE time encoding.

**Specification**: RP-004-008 MIDI Time Code Specification - Quarter Frame Messages

**Details**:
- Message format: F1 <0nnn dddd>
- 8 messages encode complete SMPTE time (hours, minutes, seconds, frames)
- Update rate: 120 Quarter Frame messages/second at 30fps (8.3ms latency)
- Message types: 0=Frame LS, 1=Frame MS, 2=Sec LS, 3=Sec MS, 4=Min LS, 5=Min MS, 6=Hour LS, 7=Hour MS + Type

**Acceptance Criteria**:
```gherkin
Scenario: Parse Quarter Frame message sequence
  Given 8 Quarter Frame messages for time 01:37:52:16 (30fps non-drop)
  When the parser processes F1 00, F1 11, F1 24, F1 33, F1 45, F1 52, F1 61, F1 76
  Then the system assembles SMPTE time 01:37:52:16
  And the SMPTE type is identified as 30 fps non-drop frame
```

---

### REQ-F-MTC-002: Parse MIDI Time Code Full Messages

**Priority**: P1 (High)  
**Traces to**: #4 (STR-SYNC-001)  
**Depends on**: #27 (REQ-F-SYSEX-001: SysEx parsing)

**Requirement**: The system SHALL parse MIDI Time Code Full Messages (F0 7F 7F 01 01 hr mn sc fr F7) for fast-forward/rewind/locate operations.

**Specification**: RP-004-008 MIDI Time Code Specification - Full Message

**Details**:
- 10-byte SysEx message with complete SMPTE time
- Used during shuttle, fast-forward, rewind modes
- Contains hours (with type), minutes, seconds, frames
- SMPTE types: 00=24fps, 01=25fps, 10=30fps drop, 11=30fps non-drop

**Acceptance Criteria**:
```gherkin
Scenario: Parse MTC Full Message for locate
  Given SysEx message F0 7F 7F 01 01 61 25 34 10 F7
  When the parser processes the message
  Then the system extracts SMPTE time with hours=01, minutes=37, seconds=52, frames=16
  And the SMPTE type is 30fps non-drop
```

---

## Registered Parameter Numbers (RPN)

### REQ-F-RPN-001: Process Registered Parameter Numbers (RPN)

**Priority**: P1 (High)  
**Traces to**: #5 (STR-FUNC-002: Advanced MIDI features)  
**Depends on**: #24 (REQ-F-CV-002: Control Change parsing)

**Requirement**: The system SHALL process Registered Parameter Number (RPN) messages using CC#101 (RPN MSB), CC#100 (RPN LSB), CC#6 (Data Entry MSB), CC#38 (Data Entry LSB) according to MIDI 1.0 Detailed Specification.

**Specification**: MIDI 1.0 Detailed Specification - Registered Parameters

**Standard RPNs**:
- RPN 00 00: Pitch Bend Sensitivity
- RPN 00 01: Fine Tuning
- RPN 00 02: Coarse Tuning
- RPN 7F 7F: Null (reset RPN selection)

**Acceptance Criteria**:
```gherkin
Scenario: Process Pitch Bend Sensitivity RPN
  Given Control Change sequence CC#101=00, CC#100=00, CC#6=02, CC#38=00
  When the parser processes the RPN sequence
  Then the system sets Pitch Bend Sensitivity to 2 semitones
  And the RPN state machine resets after data entry
```

---

## Non-Registered Parameter Numbers (NRPN)

### REQ-F-NRPN-001: Process Non-Registered Parameter Numbers (NRPN)

**Priority**: P2 (Medium) → P1 for completeness  
**Traces to**: #5 (STR-FUNC-002)  
**Depends on**: #24 (REQ-F-CV-002), #REQ-F-RPN-001

**Requirement**: The system SHALL process Non-Registered Parameter Number (NRPN) messages using CC#99 (NRPN MSB), CC#98 (NRPN LSB), CC#6 (Data Entry MSB), CC#38 (Data Entry LSB) for manufacturer-specific parameters.

**Specification**: MIDI 1.0 Detailed Specification - Non-Registered Parameters

**Acceptance Criteria**:
```gherkin
Scenario: Process NRPN message
  Given Control Change sequence CC#99=01, CC#98=20, CC#6=64
  When the parser processes the NRPN sequence
  Then the system identifies NRPN 0120h with data value 64
  And the NRPN state machine allows manufacturer-specific handling
```

---

## General MIDI Level 1

### REQ-F-GM1-001: Support General MIDI Level 1 Sound Set

**Priority**: P1 (High)  
**Traces to**: #11 (STR-COMP-003: General MIDI compliance)  
**Depends on**: #22 (REQ-F-CV-004: Program Change)

**Requirement**: The system SHALL recognize and validate General MIDI Level 1 Program Change numbers (1-128) mapped to standard instrument groups per RP-003 specification.

**Specification**: RP-003 General MIDI System Level 1 Specification - Table 2

**GM Sound Set Groups**:
- Prog 1-8: Piano
- Prog 9-16: Chromatic Percussion
- Prog 17-24: Organ
- Prog 25-32: Guitar
- Prog 33-40: Bass
- Prog 41-48: Strings
- Prog 49-56: Ensemble
- Prog 57-64: Brass
- Prog 65-72: Reed
- Prog 73-80: Pipe
- Prog 81-88: Synth Lead
- Prog 89-96: Synth Pad
- Prog 97-104: Synth Effects
- Prog 105-112: Ethnic
- Prog 113-120: Percussive
- Prog 121-128: Sound Effects

**Acceptance Criteria**:
```gherkin
Scenario: Validate GM Level 1 Program Change
  Given a Program Change message on any channel except 10
  When the program number is 1 (Acoustic Grand Piano)
  Then the system validates it as GM Level 1 Piano group
  And maps it to the appropriate instrument category
```

---

### REQ-F-GM1-002: Support General MIDI Percussion Map (Channel 10)

**Priority**: P1 (High)  
**Traces to**: #11 (STR-COMP-003)  
**Depends on**: #20 (REQ-F-CV-001: Note On/Off parsing)

**Requirement**: The system SHALL recognize General MIDI percussion note mapping on MIDI channel 10 with 47 standard drum sounds (MIDI keys 35-81) per RP-003 specification.

**Specification**: RP-003 General MIDI System Level 1 Specification - Table 3

**Standard GM Percussion Notes**:
- Key 35: Acoustic Bass Drum
- Key 36: Bass Drum 1
- Key 38: Acoustic Snare
- Key 42: Closed Hi-Hat
- Key 46: Open Hi-Hat
- Key 49: Crash Cymbal 1
- Key 51: Ride Cymbal 1
- (Total: 47 percussion sounds, keys 35-81)

**Acceptance Criteria**:
```gherkin
Scenario: Validate GM percussion note on channel 10
  Given a Note On message on channel 10 (0x99)
  When the note number is 38 (Acoustic Snare)
  Then the system validates it as GM Level 1 percussion
  And maps it to the standard Acoustic Snare sound
```

---

## Running Status Optimization

### REQ-F-RS-001: Support MIDI Running Status

**Priority**: P1 (High)  
**Traces to**: #2 (STR-PROT-001), #8 (STR-PERF-002: Bandwidth optimization)  
**Depends on**: #17 (REQ-F-CORE-001)

**Requirement**: The system SHALL support MIDI Running Status by omitting repeated status bytes for consecutive messages of the same type on the same channel, reducing bandwidth usage per MIDI 1.0 Detailed Specification.

**Specification**: MIDI 1.0 Detailed Specification - Running Status

**Details**:
- Omit status byte when same as previous message
- Applies to Channel Voice messages only (0x80-0xEF)
- Reset Running Status on System Common/Real-Time messages
- Reset Running Status on status byte reception

**Acceptance Criteria**:
```gherkin
Scenario: Parse messages with Running Status
  Given MIDI byte sequence [0x90, 0x3C, 0x64, 0x40, 0x60, 0x44, 0x58]
  When the parser processes with Running Status enabled
  Then the system interprets as three Note On messages:
    | Channel | Note | Velocity |
    | 0       | 60   | 100      |
    | 0       | 64   | 96       |
    | 0       | 68   | 88       |
```

---

## Channel Mode Messages

### REQ-F-CM-001: Process Channel Mode Messages

**Priority**: P1 (High)  
**Traces to**: #2 (STR-PROT-001)  
**Depends on**: #24 (REQ-F-CV-002: Control Change parsing)

**Requirement**: The system SHALL process MIDI Channel Mode messages (CC#120-127) including All Sound Off, Reset All Controllers, Local Control, All Notes Off, Omni Mode On/Off, Mono Mode On, Poly Mode On per MIDI 1.0 Detailed Specification.

**Specification**: MIDI 1.0 Detailed Specification - Channel Mode Messages

**Channel Mode CCs**:
- CC#120 (78h): All Sound Off
- CC#121 (79h): Reset All Controllers
- CC#122 (7Ah): Local Control On/Off
- CC#123 (7Bh): All Notes Off
- CC#124 (7Ch): Omni Mode Off (+ All Notes Off)
- CC#125 (7Dh): Omni Mode On (+ All Notes Off)
- CC#126 (7Eh): Mono Mode On (Poly Off) (+ All Notes Off)
- CC#127 (7Fh): Poly Mode On (Mono Off) (+ All Notes Off)

**Acceptance Criteria**:
```gherkin
Scenario: Process All Notes Off channel mode message
  Given active notes playing on channel 0
  When Control Change CC#123 (All Notes Off) is received on channel 0
  Then the system releases all notes on channel 0
  And honors sustain pedal state per specification
```

---

## Additional P1 Requirements Summary

**Created**: 10 P1 requirements covering:
- Advanced Channel Voice Messages (2): Polyphonic/Channel Pressure
- MIDI Time Code (2): Quarter Frame, Full Messages
- Parameter Numbers (2): RPN, NRPN
- General MIDI Level 1 (2): Sound Set, Percussion Map
- Protocol Optimizations (1): Running Status
- Channel Modes (1): Mode Messages

**Total Requirements So Far**:
- P0 (Critical): 19 requirements (Issues #16-#34)
- P1 (High): 10 requirements (this batch)
- **Grand Total**: 29 requirements

---

## Next Steps

1. Create these P1 requirements as GitHub Issues
2. Create P2 (Medium) requirements for optional/extended features:
   - MIDI Show Control (MSC)
   - MIDI Machine Control (MMC)
   - MPE (MIDI Polyphonic Expression)
   - MIDI Tuning Standard
   - Sample Dump Standard
   - Extended addenda features
