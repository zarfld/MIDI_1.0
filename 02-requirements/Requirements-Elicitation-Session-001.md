# Requirements Elicitation Session: MIDI 1.0 Protocol Implementation

**Date**: 2025-11-24
**Session ID**: ELICIT-20251124-001
**Phase**: 02 - Requirements Analysis & Specification
**Standard**: ISO/IEC/IEEE 29148:2018 (System Requirements)

## Session Overview

This elicitation session transforms the 15 stakeholder requirements (GitHub Issues #1-#15) into detailed system requirements (functional and non-functional) that will be tracked as GitHub Issues.

**Parent Stakeholder Requirements**:
- #1: STR-BUS-001 - Hardware-Agnostic MIDI 1.0 Reference Implementation
- #2: STR-FUNC-001 - Complete MIDI 1.0 Protocol Compliance
- #3: STR-PERF-001 - Real-Time Performance Guarantees
- #4: STR-USER-001 - Developer Experience and API Usability
- #5: STR-COMP-004 - Modular Architecture with Optional Extensions
- #6: STR-SEC-001 - Security and Input Validation
- #7: STR-USER-002 - Educational Use Cases and Learning Materials
- #8: STR-BUS-002 - Open Source Community Building
- #9: STR-COMP-003 - Testing Strategy and Quality Assurance
- #10: STR-COMP-005 - Build System and CI/CD Pipeline
- #11: STR-FUNC-002 - Control Change Controllers and 14-bit Resolution
- #12: STR-FUNC-003 - System Exclusive (SysEx) Message Handling
- #13: STR-FUNC-004 - MIDI Time Code (MTC) Extension
- #14: STR-FUNC-005 - General MIDI Compliance
- #15: STR-COMP-006 - C++14 Standard Compliance

**MIDI Specification References** (from MIDI1.0.md):
- MIDI 1.0 Detailed Specification (1996 revision) - Core protocol
- MIDI Time Code Specification - Synchronization
- MIDI Show Control Specification - Theatrical/multimedia control
- MIDI Machine Control Specification - Recording/production systems
- MPE: MIDI Polyphonic Expression - Polyphonic pitch/timbre control
- General MIDI Level 1, 2, and Lite - Sound set specifications
- MIDI 1.0 Addenda (28+ extensions and recommended practices)

## Elicitation Strategy

### Approach
1. **Group by functional area** - Organize requirements by MIDI protocol subsystems
2. **Apply 8-dimension analysis** - Functional, Boundary, Error, Performance, Security, Integration, UX, Compliance
3. **Reference MIDI specs** - Link each requirement to specific MIDI specification sections
4. **Define acceptance criteria** - Use Gherkin format for testability
5. **Establish traceability** - Link to parent StR issues and MIDI specifications

### Functional Areas (from MIDI 1.0 Detailed Specification)
1. **Channel Voice Messages** - Note On/Off, Polyphonic Key Pressure, Control Change, Program Change, Channel Pressure, Pitch Bend
2. **Channel Mode Messages** - Local Control, All Notes Off, Omni Mode, Mono/Poly Mode
3. **System Common Messages** - SysEx, MTC Quarter Frame, Song Position Pointer, Song Select, Tune Request, EOX
4. **System Real-Time Messages** - Timing Clock, Start, Continue, Stop, Active Sensing, System Reset
5. **Protocol Features** - Running Status, message validation, byte timing, status/data byte distinction
6. **Extensions** - MTC, MSC, MMC, MPE, GM Level 1/2, Tuning, Controllers

## Requirements Elicitation by Stakeholder Requirement

### STR #1: Hardware-Agnostic MIDI 1.0 Reference Implementation

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **Primary purpose**: Provide pure MIDI protocol implementation independent of hardware
2. **Inputs**: MIDI byte streams, abstract hardware interface callbacks
3. **Outputs**: Parsed MIDI messages, protocol state, validation results
4. **Business rules**: 
   - NO direct hardware calls in protocol layer
   - Hardware abstraction via dependency injection
   - Compile without vendor drivers or OS-specific headers
5. **Workflows**: Initialize parser → Register hardware callbacks → Parse byte stream → Emit structured messages

#### 2. Boundary Values & Ranges 📏
1. **MIDI byte ranges**: Status bytes (0x80-0xFF), Data bytes (0x00-0x7F)
2. **Channel range**: 0-15 (or 1-16 user-facing)
3. **Note number range**: 0-127
4. **Velocity range**: 0-127 (0 = Note Off)
5. **Controller range**: 0-127
6. **SysEx length**: Variable (3 bytes to theoretically unlimited, practical limit ~64KB)
7. **Edge cases**: Running status, interleaved Real-Time messages, Active Sensing timeout (300ms)

#### 3. Error Handling & Validation ⚠️
1. **Error conditions**: Invalid status byte, data byte > 0x7F, truncated messages, malformed SysEx
2. **User messages**: Return error codes with diagnostic context (not exceptions in real-time path)
3. **Validation rules**: Status byte bit 7 = 1, data byte bit 7 = 0, valid message lengths
4. **Recovery actions**: Resynchronize on next status byte, discard malformed message

#### 4. Performance & Scalability ⚡
1. **Response time**: <10μs per byte parsing (non-blocking)
2. **Throughput**: 31.25 kbaud (3125 bytes/sec) sustained
3. **Concurrency**: Single-threaded parser (caller manages threads)
4. **Real-time safe**: No dynamic allocation, no blocking calls, deterministic timing

#### 5. Security & Privacy 🔒
1. **Access**: No authentication (protocol layer)
2. **Data protection**: Validate all input bytes, prevent buffer overflows
3. **Audit logging**: Optional callback for protocol events
4. **Compliance**: N/A (protocol implementation)

#### 6. Integration & Dependencies 🔗
1. **External systems**: None (hardware-agnostic)
2. **APIs**: Abstract hardware interface (send_byte, receive_byte, get_time)
3. **Dependencies**: C++14 standard library only
4. **Data formats**: MIDI 1.0 binary protocol

#### 7. User Experience & Accessibility ♿
1. **User roles**: C++ developers integrating MIDI support
2. **Accessibility**: N/A (library)
3. **Platforms**: Windows, Linux, RTOS, bare-metal ARM Cortex-M
4. **Internationalization**: N/A (binary protocol)

#### 8. Compliance & Regulations 📋
1. **Standards**: MIDI 1.0 Detailed Specification (1996)
2. **Best practices**: ISO C++14, real-time programming guidelines
3. **Data retention**: N/A
4. **Audit trails**: Optional protocol event logging

**Derived System Requirements** (to be created as GitHub Issues):

**REQ-F-CORE-001**: Parse MIDI Channel Voice Messages (Note On/Off, CC, etc.)
- Traces to: #1, #2
- MIDI Spec: Table 1 "Channel Voice Messages"
- Priority: P0 (Critical)

**REQ-F-CORE-002**: Parse MIDI System Messages (Real-Time, Common)
- Traces to: #1, #2
- MIDI Spec: Tables 2-3 "System Messages"
- Priority: P0 (Critical)

**REQ-F-CORE-003**: Implement Running Status Optimization
- Traces to: #1, #2
- MIDI Spec: "Running Status" section
- Priority: P1 (High)

**REQ-F-CORE-004**: Validate Status and Data Byte Ranges
- Traces to: #1, #2, #6
- MIDI Spec: "Status Byte" and "Data Byte" definitions
- Priority: P0 (Critical)

**REQ-F-HAL-001**: Define Hardware Abstraction Interface
- Traces to: #1
- Copilot Instructions: Hardware abstraction via dependency injection
- Priority: P0 (Critical)

**REQ-NF-PERF-001**: Parse MIDI Bytes with <10μs Latency
- Traces to: #1, #3
- Performance: Sub-10μs per byte, no blocking
- Priority: P0 (Critical)

**REQ-NF-COMP-001**: Compile Without Vendor-Specific Headers
- Traces to: #1, #15
- Constraint: No hardware drivers, OS headers, or vendor code
- Priority: P0 (Critical)

---

### STR #2: Complete MIDI 1.0 Protocol Compliance

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **Primary purpose**: Implement all MIDI 1.0 messages and features per specification
2. **Inputs**: All MIDI message types (Channel Voice, Channel Mode, System)
3. **Outputs**: Complete message parsing, generation, and validation
4. **Key business rules**: 100% MIDI 1.0 conformance, no proprietary extensions in core
5. **Workflows**: Support all 16 MIDI channels, all message types, proper timing

#### 2. Boundary Values & Ranges 📏
1. **All MIDI message types**: 
   - Channel Voice: Note On (0x9n), Note Off (0x8n), Polyphonic Key Pressure (0xAn), Control Change (0xBn), Program Change (0xCn), Channel Pressure (0xDn), Pitch Bend (0xEn)
   - System Common: SysEx (0xF0), MTC Quarter Frame (0xF1), Song Position (0xF2), Song Select (0xF3), Tune Request (0xF6), EOX (0xF7)
   - System Real-Time: Timing Clock (0xF8), Start (0xFA), Continue (0xFB), Stop (0xFC), Active Sensing (0xFE), System Reset (0xFF)
2. **Message length validation**: 1-3 bytes (excluding SysEx)
3. **Pitch bend range**: -8192 to +8191 (14-bit signed)

#### 3. Error Handling & Validation ⚠️
1. **Detect**: Incomplete messages, invalid channel (>15), malformed multi-byte values
2. **Messages**: Clear error codes (INCOMPLETE_MESSAGE, INVALID_CHANNEL, etc.)
3. **Validation**: Per-message-type length and range checks
4. **Recovery**: Discard invalid message, continue parsing

#### 4. Performance & Scalability ⚡
1. **Throughput**: Handle 31.25 kbaud continuous stream
2. **Latency**: Message parsing <50μs total
3. **Real-time**: No jitter introduction, deterministic timing

**Derived System Requirements**:

**REQ-F-CV-001**: Implement Note On/Off Message Parsing
- Traces to: #2
- MIDI Spec: Note On (0x9n), Note Off (0x8n) or Note On with velocity 0
- Priority: P0 (Critical)

**REQ-F-CV-002**: Implement Control Change Message Parsing
- Traces to: #2, #11
- MIDI Spec: Control Change (0xBn), 120 controllers
- Priority: P0 (Critical)

**REQ-F-CV-003**: Implement Polyphonic Key Pressure Parsing
- Traces to: #2
- MIDI Spec: Polyphonic Key Pressure (0xAn)
- Priority: P1 (High)

**REQ-F-CV-004**: Implement Program Change Parsing
- Traces to: #2
- MIDI Spec: Program Change (0xCn)
- Priority: P0 (Critical)

**REQ-F-CV-005**: Implement Channel Pressure Parsing
- Traces to: #2
- MIDI Spec: Channel Pressure (0xDn)
- Priority: P1 (High)

**REQ-F-CV-006**: Implement Pitch Bend Parsing (14-bit)
- Traces to: #2
- MIDI Spec: Pitch Bend (0xEn), 14-bit resolution
- Priority: P0 (Critical)

**REQ-F-CM-001**: Implement Channel Mode Messages
- Traces to: #2
- MIDI Spec: Local Control, All Notes Off, Omni Mode, Mono/Poly Mode
- Priority: P1 (High)

**REQ-F-SRT-001**: Implement System Real-Time Messages
- Traces to: #2
- MIDI Spec: Clock (0xF8), Start (0xFA), Continue (0xFB), Stop (0xFC), Active Sensing (0xFE), Reset (0xFF)
- Priority: P0 (Critical)

**REQ-F-SC-001**: Implement System Common Messages
- Traces to: #2
- MIDI Spec: MTC Quarter Frame, Song Position, Song Select, Tune Request
- Priority: P1 (High)

---

### STR #3: Real-Time Performance Guarantees

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **Purpose**: Ensure deterministic, low-latency MIDI processing suitable for real-time audio
2. **Key requirements**: No blocking, no dynamic allocation in critical path, bounded execution time
3. **Workflows**: Non-blocking parse, callback-based event delivery

#### 4. Performance & Scalability ⚡
1. **Latency targets**:
   - Byte parsing: <10μs (mean), <20μs (worst-case)
   - Message dispatch: <50μs total
   - No memory allocation in critical path
2. **Jitter**: <5μs variance in timing
3. **Real-time OS**: Compatible with RTOS schedulers

**Derived System Requirements**:

**REQ-NF-RT-001**: Guarantee Deterministic Execution Time
- Traces to: #3
- Metric: Worst-case execution time (WCET) <20μs per byte
- Priority: P0 (Critical)

**REQ-NF-RT-002**: Prohibit Dynamic Memory Allocation
- Traces to: #3
- Constraint: No malloc/new in message parsing critical path
- Priority: P0 (Critical)

**REQ-NF-RT-003**: Minimize Timing Jitter
- Traces to: #3
- Metric: Jitter <5μs standard deviation
- Priority: P0 (Critical)

**REQ-NF-RT-004**: Support Real-Time Operating Systems
- Traces to: #3
- Platform: FreeRTOS, ThreadX, bare-metal ARM Cortex-M7
- Priority: P1 (High)

---

### STR #6: Security and Input Validation

**Questions Across 8 Dimensions**:

#### 3. Error Handling & Validation ⚠️
1. **Validate all MIDI input**: Prevent buffer overflows, invalid state transitions
2. **SysEx bounds checking**: Prevent unbounded SysEx messages
3. **Malformed message handling**: Graceful rejection without crash

#### 5. Security & Privacy 🔒
1. **Input validation**: Every byte validated before processing
2. **Buffer safety**: No buffer overruns on malformed SysEx
3. **Fuzzing resilience**: Withstand malicious MIDI input

**Derived System Requirements**:

**REQ-NF-SEC-001**: Validate All MIDI Input Bytes
- Traces to: #6
- Validation: Status byte bit 7, data byte bit 7, message length
- Priority: P0 (Critical)

**REQ-NF-SEC-002**: Prevent SysEx Buffer Overflows
- Traces to: #6, #12
- Protection: Configurable max SysEx length (default 64KB)
- Priority: P0 (Critical)

**REQ-NF-SEC-003**: Pass Fuzz Testing Suite
- Traces to: #6, #9
- Testing: AFL, libFuzzer with 1M+ mutations
- Priority: P1 (High)

---

### STR #9: Testing Strategy and Quality Assurance

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **Testing levels**: Unit tests (TDD), conformance tests, hardware-in-loop
2. **Coverage targets**: >80% code coverage, 100% MIDI message type coverage
3. **Conformance**: Validate against MIDI 1.0 specification test vectors

**Derived System Requirements**:

**REQ-NF-TEST-001**: Achieve 80% Code Coverage
- Traces to: #9
- Metric: Line coverage >80%, branch coverage >75%
- Priority: P1 (High)

**REQ-F-TEST-001**: Implement MIDI Conformance Test Suite
- Traces to: #9, #2
- Coverage: All MIDI message types, edge cases, error conditions
- Priority: P0 (Critical)

**REQ-F-TEST-002**: Support Hardware-in-Loop Testing
- Traces to: #9
- Interface: Mock hardware interface for automated testing
- Priority: P1 (High)

---

### STR #11: Control Change Controllers and 14-bit Resolution

**Questions Across 8 Dimensions**:

#### 2. Boundary Values & Ranges 📏
1. **Controller numbers**: 0-127 (120 defined, some reserved)
2. **14-bit resolution**: MSB (CC 0-31) + LSB (CC 32-63)
3. **RPN/NRPN**: 4-message sequence for extended parameters

**Derived System Requirements**:

**REQ-F-CC-001**: Parse 7-bit Control Change Messages
- Traces to: #11, #2
- MIDI Spec: CC 0-127, single-byte value
- Priority: P0 (Critical)

**REQ-F-CC-002**: Support 14-bit Controller Resolution
- Traces to: #11
- MIDI Spec: MSB (CC 0-31) + LSB (CC 32-63) → 14-bit value
- Priority: P1 (High)

**REQ-F-CC-003**: Implement RPN (Registered Parameter Number)
- Traces to: #11
- MIDI Spec: RPN MSB/LSB (CC 101/100) + Data Entry
- Priority: P1 (High)

**REQ-F-CC-004**: Implement NRPN (Non-Registered Parameter Number)
- Traces to: #11
- MIDI Spec: NRPN MSB/LSB (CC 99/98) + Data Entry
- Priority: P2 (Medium)

---

### STR #12: System Exclusive (SysEx) Message Handling

**Questions Across 8 Dimensions**:

#### 2. Boundary Values & Ranges 📏
1. **SysEx structure**: 0xF0 [manufacturer ID] [data...] 0xF7
2. **Manufacturer ID**: 1-byte (00-7F) or 3-byte (00 ID1 ID2)
3. **Universal SysEx**: 0xF0 0x7E (Non-Real-Time) or 0x7F (Real-Time)
4. **Data length**: Variable (practical limit ~64KB)

**Derived System Requirements**:

**REQ-F-SYSEX-001**: Parse System Exclusive Messages
- Traces to: #12, #2
- MIDI Spec: 0xF0 ... 0xF7 with manufacturer ID
- Priority: P0 (Critical)

**REQ-F-SYSEX-002**: Validate Manufacturer IDs
- Traces to: #12
- MIDI Spec: 1-byte or 3-byte manufacturer ID format
- Priority: P1 (High)

**REQ-F-SYSEX-003**: Support Universal SysEx Messages
- Traces to: #12
- MIDI Spec: Universal Non-Real-Time (0x7E) and Real-Time (0x7F)
- Priority: P1 (High)

**REQ-NF-SYSEX-001**: Configure Maximum SysEx Length
- Traces to: #12, #6
- Constraint: Configurable limit (default 64KB), prevent DoS
- Priority: P0 (Critical)

---

### STR #13: MIDI Time Code (MTC) Extension

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **MTC types**: Quarter Frame (0xF1), Full Frame (SysEx)
2. **Frame rates**: 24, 25, 29.97 drop/non-drop, 30 fps
3. **Synchronization**: Reconstruct timecode from 8 Quarter Frames

**Derived System Requirements**:

**REQ-F-MTC-001**: Parse MTC Quarter Frame Messages
- Traces to: #13
- MIDI Spec: MTC Quarter Frame (0xF1), 8-message sequence
- Priority: P2 (Medium) - Optional extension

**REQ-F-MTC-002**: Support All MTC Frame Rates
- Traces to: #13
- MIDI Spec: 24, 25, 29.97 (drop/non-drop), 30 fps
- Priority: P2 (Medium)

**REQ-F-MTC-003**: Parse MTC Full Frame SysEx
- Traces to: #13
- MIDI Spec: Universal SysEx MTC Full Message
- Priority: P2 (Medium)

---

### STR #14: General MIDI Compliance

**Questions Across 8 Dimensions**:

#### 1. Functional Behavior 🔧
1. **GM Level 1**: 128 instruments + drum kits on channel 10
2. **GM Level 2**: Extended sound sets, additional controllers
3. **Mandatory controllers**: Volume (CC7), Pan (CC10), Expression (CC11)

**Derived System Requirements**:

**REQ-F-GM-001**: Support GM Level 1 Sound Set Definitions
- Traces to: #14
- GM Spec: 128 instrument definitions, drum kits
- Priority: P2 (Medium) - Optional profile

**REQ-F-GM-002**: Enforce GM Channel 10 Drum Behavior
- Traces to: #14
- GM Spec: Channel 10 reserved for percussion
- Priority: P2 (Medium)

**REQ-F-GM-003**: Implement GM Mandatory Controllers
- Traces to: #14
- GM Spec: Volume, Pan, Expression, Sustain, etc.
- Priority: P2 (Medium)

---

## Summary: System Requirements to Create

### Total Requirements Count: ~35-40 GitHub Issues

#### Core Protocol (Critical - P0)
1. REQ-F-CORE-001: Parse Channel Voice Messages
2. REQ-F-CORE-002: Parse System Messages
3. REQ-F-CORE-004: Validate Status/Data Byte Ranges
4. REQ-F-HAL-001: Hardware Abstraction Interface
5. REQ-F-CV-001: Note On/Off Parsing
6. REQ-F-CV-002: Control Change Parsing
7. REQ-F-CV-004: Program Change Parsing
8. REQ-F-CV-006: Pitch Bend Parsing (14-bit)
9. REQ-F-SRT-001: System Real-Time Messages
10. REQ-F-SYSEX-001: SysEx Message Parsing
11. REQ-F-TEST-001: MIDI Conformance Test Suite

#### Performance & Real-Time (Critical - P0)
12. REQ-NF-PERF-001: <10μs Byte Parsing Latency
13. REQ-NF-RT-001: Deterministic Execution Time
14. REQ-NF-RT-002: No Dynamic Allocation in Critical Path
15. REQ-NF-RT-003: Minimize Timing Jitter

#### Security (Critical - P0)
16. REQ-NF-SEC-001: Validate All Input Bytes
17. REQ-NF-SEC-002: Prevent SysEx Buffer Overflows
18. REQ-NF-COMP-001: Compile Without Vendor Headers
19. REQ-NF-SYSEX-001: Configure Max SysEx Length

#### High Priority (P1)
20. REQ-F-CORE-003: Running Status Optimization
21. REQ-F-CV-003: Polyphonic Key Pressure
22. REQ-F-CV-005: Channel Pressure
23. REQ-F-CM-001: Channel Mode Messages
24. REQ-F-SC-001: System Common Messages
25. REQ-F-CC-002: 14-bit Controller Resolution
26. REQ-F-CC-003: RPN Support
27. REQ-F-SYSEX-002: Validate Manufacturer IDs
28. REQ-F-SYSEX-003: Universal SysEx Support
29. REQ-NF-RT-004: RTOS Compatibility
30. REQ-NF-SEC-003: Fuzz Testing
31. REQ-NF-TEST-001: 80% Code Coverage
32. REQ-F-TEST-002: Hardware-in-Loop Testing

#### Medium Priority (P2) - Optional Extensions
33. REQ-F-CC-004: NRPN Support
34. REQ-F-MTC-001: MTC Quarter Frame
35. REQ-F-MTC-002: MTC Frame Rates
36. REQ-F-MTC-003: MTC Full Frame SysEx
37. REQ-F-GM-001: GM Level 1 Sound Sets
38. REQ-F-GM-002: GM Channel 10 Drums
39. REQ-F-GM-003: GM Mandatory Controllers

## Next Steps

1. **Create GitHub Issues** for each system requirement using templates:
   - Functional requirements → REQ-F template
   - Non-functional requirements → REQ-NF template

2. **Establish traceability** to parent stakeholder requirements (#1-#15)

3. **Reference MIDI specifications** in each requirement description

4. **Define acceptance criteria** using Gherkin scenarios

5. **Prioritize** based on criticality and dependencies

6. **Validate** with stakeholders before implementation

---

**Session Status**: Ready to create GitHub Issues for system requirements
**Next Action**: Begin creating REQ-F and REQ-NF issues via GitHub MCP
