# Stakeholder Responses - Part 2: Boundaries & Error Handling

**Session**: ELICIT-20251126-001-MIDI-1.0-Core-Protocol  
**Sections**: 2. Boundary Values & Ranges, 3. Error Handling & Validation  
**Date**: 2025-11-26

---

## 2. Boundary Values & Ranges 📏

### 2.1 Data Range Limits

#### Q2.1.1: Exact numerical ranges for MIDI data types

**ANSWER**:

| Data Type | Internal Range | User-Facing Range | Notes |
|-----------|---------------|-------------------|-------|
| **Channel Number** | 0-15 (4 bits) | 1-16 (human-readable) | API uses 0-based, docs show 1-based |
| **Note Number** | 0-127 (7 bits) | MIDI Note 0 = C-1, MIDI Note 60 = C4 (Middle C), MIDI Note 127 = G9 | Per General MIDI convention |
| **Velocity** | 0-127 (7 bits) | 0 = Note Off (in Note On), 1-127 = attack velocity | Value 64 = mezzo-forte (mf) |
| **Control Change Value** | 0-127 (7 bits) | 0 = minimum, 64 = center (for bipolar), 127 = maximum | CC-specific interpretation |
| **Program Change** | 0-127 (7 bits) | Program 0-127 | GM: 0-127 instruments, 128 = drumkit (channel 10) |
| **Pitch Bend** | 0-16383 (14 bits) | 0 = max down, 8192 = center (no bend), 16383 = max up | LSB in first data byte, MSB in second |
| **Channel Pressure** | 0-127 (7 bits) | 0 = no pressure, 127 = maximum pressure | Aftertouch |
| **Poly Key Pressure** | 0-127 per note | 0 = no pressure, 127 = maximum | Per-note aftertouch |

**Channel numbering convention**:
- **Internal/API**: 0-based (channel 0-15) for consistency with bit masking (status byte & 0x0F)
- **User-facing/docs**: 1-based (channel 1-16) per MIDI convention
- **API provides converters**: `channel_to_display(internal)`, `channel_from_display(user)`

---

#### Q2.1.2: Timing-related parameter ranges

**ANSWER**:

| Parameter | Range | Units | Notes |
|-----------|-------|-------|-------|
| **MIDI Timing Clock** | 24 PPQN | Pulses Per Quarter Note | Fixed by MIDI 1.0 specification |
| **Song Position Pointer** | 0-16383 (14 bits) | MIDI Beats | 1 MIDI Beat = 6 Timing Clocks, Position in 16th notes |
| **Tempo** | Derived | BPM (Beats Per Minute) | Calculated from interval between Timing Clock messages |
| **Clock Interval** | Variable | Microseconds | At 120 BPM: 20.833ms per clock (48 clocks/second) |
| **Active Sensing Timeout** | 300ms typical | Milliseconds | If Active Sensing enabled, timeout if no MIDI message in 300ms |
| **Byte Transmission Time** | 320μs nominal | Microseconds | At 31.25 kbaud: 10 bits/byte * 32μs/bit = 320μs |

**Tempo calculation** (from Timing Clock interval):
```
BPM = 2500000 / (interval_us)
where interval_us = microseconds between consecutive Timing Clock messages
Example: 20833μs interval = 120 BPM
```

**Active Sensing**:
- **Enabled**: If device receives at least one Active Sensing message (0xFE)
- **Timeout**: Device SHOULD assume connection lost if >300ms without ANY MIDI message
- **Recovery**: Device SHOULD turn off all notes, reset to known state

---

#### Q2.1.3: SysEx message length limits

**ANSWER**:

| Limit | Value | Rationale |
|-------|-------|-----------|
| **Minimum SysEx length** | 2 bytes (0xF0 0xF7) | Empty SysEx (Start + End) |
| **Maximum SysEx length** | Configurable, default 64KB | Balance memory vs. use cases |
| **Recommended buffer size** | 256 bytes | Covers 99% of common SysEx messages |
| **Large SysEx support** | Up to 1MB | For firmware uploads, sample dumps (compile-time option) |

**Configuration**:
```cpp
// Compile-time configuration
#define MIDI_SYSEX_BUFFER_SIZE 256      // Default
#define MIDI_SYSEX_MAX_SIZE 65536        // Maximum allowed (64KB)

// Runtime query
size_t max_sysex = midi_parser.get_max_sysex_size();
```

**Fragmentation handling**:
1. **Small SysEx (<= buffer size)**: Buffered completely, delivered when EOX received
2. **Large SysEx (> buffer size)**: 
   - **Option A**: Reject with error `MIDI_ERROR_SYSEX_TOO_LARGE`
   - **Option B**: Deliver in fragments via callback (`on_sysex_fragment(data, length, is_complete)`)
   - **Configuration**: Application chooses fragmentation mode

**Use case examples**:
- **256 bytes**: General MIDI System On/Off, Device Inquiry, Tuning messages
- **4KB**: DLS sound bank headers, sample metadata
- **64KB**: Small sample dumps, firmware updates
- **1MB**: Large sample libraries (requires streaming architecture)

---

### 2.2 Edge Cases

#### Q2.2.1: Incomplete message handling

**ANSWER**:

**SHALL detect and recover from incomplete messages**:

1. **Detection criteria**:
   - **Timeout**: No data bytes received within configurable timeout (default 100ms)
   - **New status byte**: Status byte received before all expected data bytes
   - **Buffer full**: Receive buffer full before message complete

2. **Recovery actions**:
   ```cpp
   // Error signal
   callback->on_parse_error(MIDI_ERROR_INCOMPLETE_MESSAGE, 
                            partial_status, bytes_received, bytes_expected);
   
   // Parser state reset
   parser.reset_message_buffer();
   parser.clear_running_status();  // Conservative: clear Running Status
   ```

3. **Examples**:
   ```
   Case 1: 90 3C [timeout] -> Note On incomplete (missing velocity)
   Action: Discard, log error, wait for next status

   Case 2: 90 3C [new status 80 ...] -> Note On incomplete, Note Off starts
   Action: Discard Note On, process Note Off normally
   
   Case 3: F0 7E 00 [1MB of data...] -> SysEx exceeds buffer
   Action: Error MIDI_ERROR_SYSEX_TOO_LARGE or fragment delivery
   ```

4. **Timeout configuration**:
   - **Default**: 100ms (sufficient for 31.25 kbaud worst-case jitter)
   - **Real-time systems**: 10ms (tighter timeout for low-latency)
   - **Bulk transfers**: 1000ms (accommodate slow SysEx dumps)

**Implementation**: Parser maintains "expected byte count" state for current message type.

---

#### Q2.2.2: Rapid message burst handling

**ANSWER**:

**SHALL use ring buffer with overflow detection**:

1. **Ring buffer design**:
   ```cpp
   // Fixed-size ring buffer (static allocation)
   #define MIDI_RX_BUFFER_SIZE 1024  // Configurable (512, 1024, 2048)
   
   uint8_t rx_buffer[MIDI_RX_BUFFER_SIZE];
   size_t write_ptr;
   size_t read_ptr;
   size_t bytes_available;  // Atomic counter
   ```

2. **Overflow behavior**:
   - **Detection**: `bytes_available == MIDI_RX_BUFFER_SIZE` (buffer full)
   - **Action**: Drop **oldest** messages (FIFO), log overflow event
   - **Counter**: Track `overflow_count` for diagnostics
   - **Callback**: `on_buffer_overflow(dropped_bytes)`

3. **Burst capacity**:
   - **1024-byte buffer**: ~340 Note On messages (3 bytes each)
   - **At 31.25 kbaud**: ~300ms worth of continuous MIDI data
   - **Sufficient for**: Typical burst scenarios (e.g., All Notes Off, chord release)

4. **Backpressure** (optional, for high-reliability):
   ```cpp
   // If application can't keep up, signal backpressure
   if (midi_parser.get_buffer_fullness() > 0.9) {
       callback->on_buffer_warning(WARN_BUFFER_90_PERCENT_FULL);
   }
   ```

**Performance target**: Process ring buffer at >=3125 bytes/second to match MIDI bandwidth.

---

#### Q2.2.3: Running Status broken by System Common

**ANSWER**:

**SHALL detect Running Status violations and recover**:

1. **System Common messages that clear Running Status**:
   - **0xF0** (SysEx Start): Clears Running Status
   - **0xF1** (MTC Quarter Frame): Clears Running Status
   - **0xF2** (Song Position): Clears Running Status
   - **0xF3** (Song Select): Clears Running Status
   - **0xF6** (Tune Request): Clears Running Status
   - **0xF7** (EOX): Clears Running Status
   - **System Real-Time (0xF8-0xFF)**: Does NOT clear Running Status

2. **Violation detection**:
   ```
   Sequence: 90 3C 64 F3 05 3D 60
             ^^^^^^^^^  ^^^^^ ^^^^^^
             Note On    Song  Invalid (no status, Running Status cleared)
                        Select
   ```

3. **Parser behavior**:
   ```cpp
   // After System Common (F3 05)
   running_status_buffer = 0x00;  // Cleared
   
   // Next bytes: 3D 60 (both have bit 7 = 0, invalid as status)
   if (byte < 0x80 && running_status_buffer == 0x00) {
       callback->on_parse_error(MIDI_ERROR_MISSING_STATUS_BYTE, byte);
       // Discard bytes until next valid status byte
   }
   ```

4. **Recovery**: Resynchronize at next status byte (bit 7 = 1)

**Specification compliance**: MIDI 1.0 explicitly states System Common clears Running Status.

---

#### Q2.2.4: Reserved status byte handling

**ANSWER**:

**SHALL log warnings but allow reserved bytes for future compatibility**:

1. **Reserved status bytes**: 0xF4, 0xF5, 0xF9, 0xFD

2. **Parser behavior**:
   ```cpp
   if (status == 0xF4 || status == 0xF5 || 
       status == 0xF9 || status == 0xFD) {
       
       // Log warning (non-fatal)
       log_warning("Reserved status byte 0x%02X received", status);
       
       // Deliver to application callback (for future extensions)
       callback->on_reserved_status(status);
       
       // Assume single-byte message (no data bytes)
       // Clear Running Status (conservative)
       running_status_buffer = 0x00;
   }
   ```

3. **Rationale**:
   - **Forward compatibility**: Future MIDI standards may define these bytes
   - **Non-blocking**: Don't halt parser on reserved bytes
   - **Visibility**: Application can log/analyze reserved byte usage

4. **Best practice**: Application SHOULD ignore reserved bytes unless implementing experimental extensions.

---

## 3. Error Handling & Validation ⚠️

### 3.1 Parse-Time Errors

#### Q3.1.1: Error conditions and detection

**ANSWER**:

**SHALL detect following error conditions**:

| Error Code | Condition | Detection Method |
|------------|-----------|------------------|
| `MIDI_ERROR_INVALID_STATUS` | Bit 7 = 0 when expecting status | `if (byte < 0x80 && expect_status)` |
| `MIDI_ERROR_INVALID_DATA` | Bit 7 = 1 when expecting data | `if (byte >= 0x80 && expect_data)` |
| `MIDI_ERROR_INCOMPLETE_MESSAGE` | Timeout or new status before complete | Timer expiry or status interruption |
| `MIDI_ERROR_RESERVED_STATUS` | 0xF4, 0xF5, 0xF9, 0xFD received | `if (status == 0xF4...)` |
| `MIDI_ERROR_SYSEX_NOT_TERMINATED` | EOX missing before timeout/status | SysEx start without EOX within timeout |
| `MIDI_ERROR_BUFFER_OVERFLOW` | RX buffer full | `if (bytes_available == buffer_size)` |
| `MIDI_ERROR_SYSEX_TOO_LARGE` | SysEx exceeds max size | `if (sysex_length > max_sysex_size)` |
| `MIDI_ERROR_MISSING_STATUS` | Data byte without Running Status | `if (byte < 0x80 && running_status == 0)` |

**Error context structure**:
```cpp
struct MIDIParseError {
    MIDIErrorCode error_code;
    uint8_t byte_value;           // Offending byte
    size_t byte_offset;           // Position in stream
    uint8_t partial_status;       // Status of incomplete message
    uint8_t bytes_received;       // Count in incomplete message
    uint64_t timestamp_us;        // When error occurred
};
```

---

#### Q3.1.2: Error communication methods

**ANSWER**:

**SHALL provide callback-based error handling (real-time safe)**:

1. **Primary method**: Error callback
   ```cpp
   class MIDIParserCallback {
   public:
       virtual void on_parse_error(const MIDIParseError& error) = 0;
       virtual void on_buffer_warning(MIDIWarningCode warning) = 0;
   };
   
   // Application implements callback
   class MyMIDIHandler : public MIDIParserCallback {
       void on_parse_error(const MIDIParseError& error) override {
           log_error("MIDI parse error: code=%d, byte=0x%02X", 
                     error.error_code, error.byte_value);
       }
   };
   ```

2. **Return codes** (for synchronous API):
   ```cpp
   MIDIErrorCode result = midi_parser.parse_byte(byte);
   if (result != MIDI_SUCCESS) {
       // Handle error
   }
   ```

3. **Error statistics** (diagnostics):
   ```cpp
   struct MIDIErrorStats {
       uint32_t invalid_status_count;
       uint32_t invalid_data_count;
       uint32_t incomplete_message_count;
       uint32_t buffer_overflow_count;
       uint32_t sysex_error_count;
   };
   
   const MIDIErrorStats& stats = midi_parser.get_error_stats();
   ```

4. **NO exceptions**: Exceptions not permitted (not real-time safe)

5. **Logging** (optional, compile-time):
   ```cpp
   #ifdef MIDI_ENABLE_LOGGING
   #define MIDI_LOG_ERROR(...) log_error(__VA_ARGS__)
   #else
   #define MIDI_LOG_ERROR(...) 
   #endif
   ```

**Design principle**: Errors reported via callback, parser continues (resilient to corruption).

---

#### Q3.1.3: Recovery actions per error type

**ANSWER**:

| Error Type | Recovery Action | Parser State |
|------------|-----------------|--------------|
| **Invalid Status Byte** | Discard byte, wait for valid status | Reset message buffer, clear Running Status |
| **Invalid Data Byte** | Treat as new status (resync) | Process as status byte, discard partial message |
| **Incomplete Message** | Discard partial message | Reset message buffer, wait for next status |
| **Reserved Status** | Deliver to callback, assume 1-byte | Clear Running Status, continue parsing |
| **SysEx Not Terminated** | Discard SysEx, log error | Reset SysEx buffer, wait for next status |
| **Buffer Overflow** | Drop oldest messages (FIFO) | Continue parsing, increment overflow counter |
| **SysEx Too Large** | Error or fragment delivery | Depends on fragmentation mode |
| **Missing Status** | Discard data bytes until status | Wait for byte with bit 7 = 1 |

**Resynchronization strategy**:
1. **Conservative**: Discard all state, wait for unambiguous status byte (0x80-0xEF, 0xF0-0xFF)
2. **Aggressive**: Attempt to infer status from byte patterns (NOT recommended, ambiguous)

**Implementation**:
```cpp
void MIDIParser::recover_from_error(MIDIErrorCode error) {
    switch (error) {
        case MIDI_ERROR_INVALID_STATUS:
        case MIDI_ERROR_INCOMPLETE_MESSAGE:
            reset_message_buffer();
            running_status = 0x00;  // Clear Running Status
            break;
        
        case MIDI_ERROR_BUFFER_OVERFLOW:
            drop_oldest_message();  // FIFO drop
            overflow_count++;
            break;
        
        // ... other cases
    }
}
```

**Goal**: Parser remains operational despite errors (graceful degradation).

---

### 3.2 Validation Rules

#### Q3.2.1: Semantic validation of syntactically correct messages

**ANSWER**:

**SHALL provide optional semantic validation (opt-in)**:

1. **Note On velocity=0** (valid syntax, semantic ambiguity):
   ```cpp
   if (msg.status() == NOTE_ON && msg.velocity() == 0) {
       callback->on_semantic_warning(WARN_NOTE_ON_VELOCITY_ZERO, msg);
       // Continue processing (valid per spec)
   }
   ```

2. **Channel Mode in normal CC range** (CC 120-127):
   ```cpp
   if (msg.is_control_change() && msg.controller() >= 120) {
       // This is Channel Mode message, not general CC
       callback->on_semantic_warning(WARN_CHANNEL_MODE_CC, msg);
   }
   ```

3. **Program Change > 127** (impossible with 7-bit data):
   - **Not detectable**: Parser already validates data bytes are 7-bit
   - **Redundant check**: Would never occur if parse validation correct

4. **Configuration**:
   ```cpp
   midi_parser.enable_semantic_validation(true);  // Default: false
   // When enabled, generates warnings but allows messages through
   ```

**Rationale**:
- **Syntax**: Always validated (hard errors)
- **Semantics**: Optionally warned (soft warnings)
- **Trust caller**: By default, assume application sends correct messages

---

#### Q3.2.2: SysEx Manufacturer ID validation

**ANSWER**:

**SHALL validate Manufacturer ID format, optionally check registry**:

1. **Format validation** (always on):
   ```cpp
   // 1-byte Manufacturer ID: 0x01-0x7D (0x7E/0x7F reserved for Universal)
   // 3-byte Manufacturer ID: 0x00 + 2 bytes (0x00-0x7F each)
   
   if (sysex[0] == 0x00) {
       // 3-byte ID
       if (sysex_length < 3) {
           return MIDI_ERROR_INVALID_MANUFACTURER_ID;
       }
       manufacturer_id = (sysex[1] << 7) | sysex[2];
   } else if (sysex[0] >= 0x01 && sysex[0] <= 0x7F) {
       // 1-byte ID
       manufacturer_id = sysex[0];
   } else {
       return MIDI_ERROR_INVALID_MANUFACTURER_ID;  // 0x80-0xFF invalid
   }
   ```

2. **Registry validation** (optional):
   ```cpp
   // Compile-time option to include MMA manufacturer registry
   #ifdef MIDI_VALIDATE_MANUFACTURER_REGISTRY
   if (!is_registered_manufacturer_id(manufacturer_id)) {
       callback->on_semantic_warning(WARN_UNREGISTERED_MANUFACTURER_ID, 
                                     manufacturer_id);
   }
   #endif
   ```

3. **Special Manufacturer IDs**:
   - **0x7D**: Educational use (non-commercial), ALWAYS valid
   - **0x7E**: Universal Non-Real-Time SysEx
   - **0x7F**: Universal Real-Time SysEx

4. **Registry database** (optional):
   - Include MMA manufacturer ID list as static table
   - Periodically updated with new assignments
   - Size: ~500 entries (1-byte IDs) + ~200 entries (3-byte IDs)

**Recommendation**: Format validation ON, registry validation OFF (reduces binary size).

---

#### Q3.2.3: Universal SysEx validation

**ANSWER**:

**SHALL parse Universal SysEx structure, validate sub-IDs**:

1. **Universal SysEx structure**:
   ```
   F0 7E/7F <device_id> <sub_id_1> <sub_id_2> [data...] F7
   
   7E = Universal Non-Real-Time
   7F = Universal Real-Time
   device_id = 0x00-0x7F (0x7F = all devices)
   sub_id_1 = Category (0x00-0x7F)
   sub_id_2 = Sub-category (0x00-0x7F)
   ```

2. **Validation rules**:
   ```cpp
   if (sysex[0] == 0x7E || sysex[0] == 0x7F) {
       // Universal SysEx
       if (sysex_length < 4) {
           return MIDI_ERROR_INVALID_UNIVERSAL_SYSEX;  // Too short
       }
       
       uint8_t device_id = sysex[1];
       uint8_t sub_id_1 = sysex[2];
       uint8_t sub_id_2 = sysex[3];
       
       if (device_id > 0x7F || sub_id_1 > 0x7F || sub_id_2 > 0x7F) {
           return MIDI_ERROR_INVALID_UNIVERSAL_SYSEX;
       }
       
       // Optionally validate known sub-ID combinations
       callback->on_universal_sysex(device_id, sub_id_1, sub_id_2, 
                                    &sysex[4], sysex_length - 4);
   }
   ```

3. **Known Universal SysEx sub-IDs** (examples):
   - **7E 7F 09 01**: General MIDI System On
   - **7E 7F 09 02**: General MIDI System Off
   - **7E 00 06 01**: Identity Request (device inquiry)
   - **7F xx 04 01**: Master Volume

4. **Sub-ID registry** (optional):
   - Validate sub_id_1/sub_id_2 against known combinations
   - Warn on unknown sub-IDs (forward compatibility)

**API design**:
```cpp
class UniversalSysExParser {
    virtual void on_general_midi_system_on() = 0;
    virtual void on_master_volume(uint8_t msb, uint8_t lsb) = 0;
    virtual void on_unknown_universal_sysex(uint8_t sub1, uint8_t sub2, 
                                            const uint8_t* data, size_t len) = 0;
};
```

**Recommendation**: Parse structure, validate format, deliver sub-IDs to application.

---

**End of Part 2: Boundary Values & Error Handling Answers**

**Parts completed**: 1-2 of 5  
**Next parts to follow**:
- Part 3: Performance & Scalability
- Part 4: Security & Privacy
- Part 5: Integration, UX, Compliance
