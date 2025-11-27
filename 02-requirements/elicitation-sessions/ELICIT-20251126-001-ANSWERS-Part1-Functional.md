# Stakeholder Responses - Part 1: Functional Behavior

**Session**: ELICIT-20251126-001-MIDI-1.0-Core-Protocol  
**Section**: 1. Functional Behavior 🔧  
**Date**: 2025-11-26

---

## 1.1 Message Parsing and Validation

### Q1.1.1: Exact byte sequences for 28 MIDI message types

**ANSWER**:

#### Channel Voice Messages (8 types):
1. **Note Off**: `0x8n + note(0-127) + velocity(0-127)` where n=channel(0-15)
2. **Note On**: `0x9n + note(0-127) + velocity(0-127)` 
3. **Polyphonic Key Pressure**: `0xAn + note(0-127) + pressure(0-127)`
4. **Control Change**: `0xBn + controller(0-127) + value(0-127)`
5. **Program Change**: `0xCn + program(0-127)` (1 data byte)
6. **Channel Pressure**: `0xDn + pressure(0-127)` (1 data byte)
7. **Pitch Bend**: `0xEn + LSB(0-127) + MSB(0-127)` (14-bit value, center=8192)

#### Channel Mode Messages (4 types, subset of Control Change 0xBn):
8. **Local Control**: `0xBn 0x7A value(0=Off, 127=On)`
9. **All Notes Off**: `0xBn 0x7B 0x00`
10. **Omni Mode Off**: `0xBn 0x7C 0x00`
11. **Omni Mode On**: `0xBn 0x7D 0x00`
12. **Mono Mode On**: `0xBn 0x7E channels(0-16, 0=all)`
13. **Poly Mode On**: `0xBn 0x7F 0x00`

#### System Common Messages (6 types):
14. **System Exclusive**: `0xF0 ... 0xF7` (variable length)
15. **MTC Quarter Frame**: `0xF1 + data(0-127)` (1 data byte)
16. **Song Position Pointer**: `0xF2 + LSB(0-127) + MSB(0-127)` (14-bit value)
17. **Song Select**: `0xF3 + song(0-127)` (1 data byte)
18. **Tune Request**: `0xF6` (no data bytes)
19. **End of Exclusive (EOX)**: `0xF7` (terminates SysEx)

#### System Real-Time Messages (6 types):
20. **Timing Clock**: `0xF8` (no data bytes)
21. **Start**: `0xFA` (no data bytes)
22. **Continue**: `0xFB` (no data bytes)
23. **Stop**: `0xFC` (no data bytes)
24. **Active Sensing**: `0xFE` (no data bytes)
25. **System Reset**: `0xFF` (no data bytes)

#### Reserved/Undefined (2 status bytes):
26. **Undefined 0xF4**: Reserved for future use
27. **Undefined 0xF5**: Reserved for future use
28. **Undefined 0xF9**: Reserved for future use
29. **Undefined 0xFD**: Reserved for future use

**Constraint**: All status bytes have bit 7 = 1 (0x80-0xFF), all data bytes have bit 7 = 0 (0x00-0x7F).

---

### Q1.1.2: Running Status optimization handling

**ANSWER**: 

**SHALL implement Running Status per MIDI 1.0 specification**:

1. **Running Status Buffer**: Parser maintains last valid Channel Voice/Mode status byte (0x80-0xEF)
2. **Omit status on repetition**: When consecutive messages have same status, omit status byte after first
3. **Clear conditions**: Running Status buffer cleared by:
   - System Common messages (0xF0-0xF7) **except** Real-Time (0xF8-0xFF)
   - New Channel Voice/Mode status byte received
   - Parser reset/initialization

**Example sequences**:
- `90 3C 64 3D 60` = Note On Ch1 Note60 Vel100, then Note61 Vel96 (Running Status)
- `90 3C 64 F8 3D 60` = Note On, Timing Clock interrupts, then Note61 (Running Status preserved)
- `90 3C 64 F3 05 90 3D 60` = Note On, Song Select (clears Running Status), new status 0x90 required

**Implementation requirement**: Parser MUST maintain separate Running Status state per MIDI input port.

---

### Q1.1.3: Real-Time message interruption handling

**ANSWER**:

**SHALL implement immediate Real-Time processing**:

1. **Real-Time messages (0xF8-0xFF)** can occur at ANY byte position, including mid-SysEx
2. **Parser behavior**:
   - Detect Real-Time status byte immediately
   - **Pause** current message parsing (save parser state)
   - **Process** Real-Time message (invoke callback, update clock, etc.)
   - **Resume** previous message parsing exactly where interrupted
   - Real-Time messages **DO NOT** affect Running Status buffer

3. **SysEx interruption example**:
   ```
   F0 7E 00 F8 09 01 F7
   ^^^^^^^^^^  ^^  ^^^^^^
   SysEx start  |  SysEx continues
              Timing Clock (process immediately, then resume)
   ```

4. **Implementation**:
   - Parser state machine must support "suspend/resume" for non-Real-Time messages
   - Real-Time callback invoked synchronously (or queued for immediate processing)
   - No buffering delay for Real-Time messages (latency-critical)

**Constraint**: Real-Time processing MUST be deterministic and <1μs overhead.

---

### Q1.1.4: Data byte validation

**ANSWER**:

**SHALL enforce strict 7-bit data byte validation**:

1. **Validation rule**: All data bytes MUST have bit 7 = 0 (value range 0x00-0x7F)
2. **Invalid data byte detection**: If bit 7 = 1 (0x80-0xFF) in data byte position:
   - **Log error**: `MIDI_ERROR_INVALID_DATA_BYTE` with byte value and offset
   - **Recovery action**: Treat as new status byte (resynchronize parser)
   - **Discard partial message**: Drop incomplete message being parsed

3. **Example error scenario**:
   ```
   90 3C FF <- Invalid velocity 0xFF (bit 7 = 1)
   Parser detects 0xFF as new status (System Reset), discards incomplete Note On
   ```

4. **Masking NOT permitted**: Parser SHALL NOT silently mask invalid data bytes to 7 bits
   - Rationale: Masking hides protocol violations and complicates debugging

**Implementation**: Validation MUST occur before data byte processing, not after.

---

### Q1.1.5: Note On velocity=0 vs. Note Off

**ANSWER**:

**SHALL preserve original message type, provide helper for semantic equivalence**:

1. **Parser behavior**: 
   - Note On (0x9n) with velocity=0 parsed as **Note On message** (preserve original)
   - Note Off (0x8n) parsed as **Note Off message**
   - Parser does NOT automatically convert Note On vel=0 to Note Off

2. **Rationale**:
   - MIDI specification allows both interpretations
   - Some devices use Note On vel=0 for Running Status optimization
   - Application may need original message type for protocol analysis/logging

3. **API helper function**:
   ```cpp
   bool is_note_off_event(const MIDIMessage& msg) {
       return (msg.status() == NOTE_OFF) || 
              (msg.status() == NOTE_ON && msg.velocity() == 0);
   }
   ```

4. **Documentation**: API docs SHALL clearly state semantic equivalence and recommend helper usage.

---

## 1.2 Message Generation and Formatting

### Q1.2.1: Automatic Running Status optimization

**ANSWER**:

**SHALL provide opt-in Running Status with per-transport configuration**:

1. **Default behavior**: Running Status **DISABLED** by default (explicit status bytes)
   - Rationale: Maximize compatibility, simplify debugging, clearer protocol traces

2. **Opt-in API**:
   ```cpp
   midi_generator.enable_running_status(true);  // Per-generator instance
   ```

3. **Transport-specific override**:
   - 5-pin DIN: Running Status recommended (bandwidth limited at 31.25 kbaud)
   - USB MIDI: Running Status optional (4-byte packet framing already efficient)
   - BLE-MIDI: Running Status optional (depends on MTU and latency requirements)

4. **State management**: Generator tracks last status byte sent, clears on System Common messages.

**Implementation note**: Running Status MUST be stateful per output port (multi-port devices).

---

### Q1.2.2: System Exclusive (SysEx) construction API

**ANSWER**:

**SHALL provide both low-level and high-level SysEx APIs**:

1. **Low-level byte stream API** (for custom SysEx):
   ```cpp
   midi_generator.send_byte(0xF0);           // Start SysEx
   midi_generator.send_byte(manufacturer_id);
   midi_generator.send_data(data, length);   // Multiple data bytes
   midi_generator.send_byte(0xF7);           // End SysEx
   ```

2. **High-level SysEx builder API** (for common patterns):
   ```cpp
   SysExMessage sysex;
   sysex.set_manufacturer_id(0x7E);  // Universal Non-Real-Time
   sysex.set_device_id(0x7F);        // All devices
   sysex.set_sub_id(0x09, 0x01);     // General MIDI System On
   midi_generator.send_sysex(sysex); // Automatically frames with F0/F7
   ```

3. **Fragmentation support** (for large SysEx):
   ```cpp
   // Send SysEx in chunks (e.g., firmware upload)
   midi_generator.send_sysex_fragment(data, length, is_last_fragment);
   ```

4. **Validation**: High-level API SHALL validate:
   - Manufacturer ID (1-byte: 0x01-0x7D, or 3-byte: 0x00 + 2 bytes)
   - Data bytes are 7-bit (0x00-0x7F)
   - Total length doesn't exceed configured maximum

**Constraint**: Low-level API provided for flexibility, high-level API for safety/convenience.

---

### Q1.2.3: 14-bit Control Change generation

**ANSWER**:

**SHALL provide helper function for 14-bit CC with correct MSB/LSB ordering**:

1. **Helper API**:
   ```cpp
   // Automatically sends two CC messages in correct order
   midi_generator.send_14bit_cc(channel, controller_msb, value_14bit);
   
   // Example: Bank Select (CC 0 MSB, CC 32 LSB)
   midi_generator.send_14bit_cc(0, 0, 0x0210);  // Value 528 (0x210)
   // Generates: B0 00 02 (CC0 MSB=2), B0 20 10 (CC32 LSB=16)
   ```

2. **MSB/LSB pairing** (MIDI 1.0 specification):
   - CC 0-31: MSB (coarse)
   - CC 32-63: LSB (fine), corresponding to CC 0-31
   - Example: CC 1 (Modulation MSB) pairs with CC 33 (Modulation LSB)

3. **Value range**: 14-bit value = 0-16383
   - MSB = (value >> 7) & 0x7F (bits 7-13)
   - LSB = value & 0x7F (bits 0-6)

4. **Low-level API** also available for manual control:
   ```cpp
   midi_generator.send_cc(channel, cc_msb, msb_value);
   midi_generator.send_cc(channel, cc_lsb, lsb_value);
   ```

**Documentation**: API docs SHALL include table of 14-bit CC pairs (CC 0-31 with CC 32-63).

---

### Q1.2.4: RPN/NRPN message sequence order

**ANSWER**:

**SHALL provide RPN/NRPN helper functions enforcing correct sequence**:

1. **RPN helper API**:
   ```cpp
   // Automatically generates 5-message sequence
   midi_generator.send_rpn(channel, rpn_msb, rpn_lsb, data_msb, data_lsb);
   
   // Example: Pitch Bend Range = 12 semitones
   midi_generator.send_rpn(0, 0x00, 0x00, 0x0C, 0x00);
   // Generates:
   // B0 65 00  (RPN MSB = 0)
   // B0 64 00  (RPN LSB = 0)
   // B0 06 0C  (Data Entry MSB = 12)
   // B0 26 00  (Data Entry LSB = 0, optional if 0)
   // B0 65 7F  (RPN MSB = 127, null)
   // B0 64 7F  (RPN LSB = 127, null)
   ```

2. **NRPN helper** (similar pattern):
   ```cpp
   midi_generator.send_nrpn(channel, nrpn_msb, nrpn_lsb, data_msb, data_lsb);
   // Uses CC 99 (NRPN MSB), CC 98 (NRPN LSB), CC 6/38 (Data Entry)
   ```

3. **Correct sequence** (per MIDI 1.0 specification):
   - Step 1: Send RPN/NRPN MSB (CC 101/99)
   - Step 2: Send RPN/NRPN LSB (CC 100/98)
   - Step 3: Send Data Entry MSB (CC 6)
   - Step 4: Send Data Entry LSB (CC 38) if needed
   - Step 5: Send RPN/NRPN Null (MSB=127, LSB=127) to deselect

4. **Null termination**: Helper SHALL automatically send null sequence to prevent accidental parameter changes from subsequent Data Inc/Dec messages.

**Low-level API**: Manual CC sending still available for advanced users.

---

## 1.3 Channel Mode Messages

### Q1.3.1: All Notes Off vs. All Sound Off behavior

**ANSWER**:

**Parser SHALL expose both messages, synthesizer implementation determines behavior**:

1. **All Notes Off (CC 123, value 0)**:
   - **Semantic**: Release all notes currently sounding on channel
   - **Behavior**: Synthesizer SHOULD trigger note Release phase (respecting envelope)
   - **Parser action**: Deliver message to application, does NOT track note state

2. **All Sound Off (CC 120, value 0)**:
   - **Semantic**: Immediately mute all sound on channel (emergency stop)
   - **Behavior**: Synthesizer SHOULD silence audio instantly (bypass envelope)
   - **Parser action**: Deliver message to application

3. **Parser does NOT implement synthesizer behavior**: Core library only parses/generates messages
   - Rationale: Hardware-agnostic library, synthesizer logic is application-specific

4. **Optional helper module** (separate from core):
   - `MIDINoteTracker` module can track Note On/Off state per channel
   - Application can query active notes when All Notes Off received

**API design**: Messages delivered via callback, application implements response.

---

### Q1.3.2: Omni Mode channel routing

**ANSWER**:

**Parser SHALL provide Omni Mode state, application implements channel filtering**:

1. **Parser behavior**:
   - Detects Omni Mode On (CC 125) and Omni Mode Off (CC 124)
   - Maintains Omni Mode state per device (or per Function Block in MIDI 2.0 context)
   - Exposes state via API: `bool is_omni_mode_on(channel)`

2. **Application responsibility**: Channel filtering based on Omni Mode
   ```cpp
   if (device.is_omni_mode_on() || message.channel() == device.base_channel()) {
       process_message(message);  // Accept message
   }
   ```

3. **Omni Mode semantics**:
   - **Omni On**: Device responds to messages on ALL channels
   - **Omni Off**: Device responds only to assigned channel (typically channel 1 or user-configured)

4. **Multi-channel devices**: Omni Mode may apply per-port or per-Function Block
   - Parser tracks Omni state per MIDI input context

**Rationale**: Parser provides state management, application implements routing policy.

---

### Q1.3.3: Mono Mode vs. Poly Mode state changes

**ANSWER**:

**Parser SHALL detect mode changes, synthesizer implements voice allocation**:

1. **Mono Mode On (CC 126, value = number of channels)**:
   - **Value 0**: Mono Mode on all channels (channel assignment TBD by device)
   - **Value 1-16**: Mono Mode on specified number of channels starting from base channel
   - **Effect**: One note at a time per channel (monophonic synthesizer behavior)
   - **Parser action**: Update mode state, signal mode change event

2. **Poly Mode On (CC 127, value 0)**:
   - **Effect**: Multiple simultaneous notes allowed (polyphonic synthesizer behavior)
   - **Parser action**: Update mode state, signal mode change event

3. **Mode change side effects** (per MIDI specification):
   - Receiving Mono/Poly Mode message SHALL turn off All Notes (implicit All Notes Off)
   - Parser SHOULD signal this side effect via callback

4. **Voice allocation**: Application/synthesizer responsibility
   - **Mono Mode**: Implement note priority (last-note, high-note, low-note priority)
   - **Poly Mode**: Implement voice stealing algorithm if polyphony limit exceeded

**API design**:
```cpp
enum class ChannelMode { POLY, MONO };
callback->on_channel_mode_change(channel, mode, num_channels);
```

**Parser scope**: Detects and signals mode changes, does NOT implement synthesizer voice allocation.

---

**End of Part 1: Functional Behavior Answers**

**Next parts to follow**:
- Part 2: Boundary Values & Ranges
- Part 3: Error Handling & Validation
- Part 4: Performance & Scalability
- Part 5: Security, Integration, UX, Compliance
