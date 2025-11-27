# Stakeholder Responses - Part 4: Integration & UX

**Session**: ELICIT-20251126-001-MIDI-1.0-Core-Protocol  
**Sections**: 6. Integration & Dependencies, 7. User Experience & Accessibility  
**Date**: 2025-11-26

---

## 6. Integration & Dependencies 🔗

### 6.1 Transport Abstraction (Q6.1.1, Q6.1.2 Already Answered)

✅ **See earlier answers**:
- Q6.1.1: HAL SHALL provide timing functions for Active Sensing
- Q6.1.2: Reference implementation examples provided (P2 priority)

---

#### Q6.1.3: MIDI routing and merging functionality

**ANSWER**:

**SHALL provide separate optional module (not in core Standards layer)**

1. **Architecture separation**:
   ```
   lib/Standards/MMA/MIDI/1_0/v1996/    <- Core protocol (no routing)
   lib/Service/MIDI/Routing/             <- Optional routing module
   ```

2. **Routing module API** (Service layer):
   ```cpp
   namespace Service::MIDI::Routing {
   
   class MIDIRouter {
   public:
       // Route messages by channel
       void add_channel_route(uint8_t input_port, uint8_t channel, 
                             uint8_t output_port);
       
       // Route all messages from input to output
       void add_port_route(uint8_t input_port, uint8_t output_port);
       
       // Process and route message
       void route_message(uint8_t input_port, const MIDIMessage& msg);
   };
   
   class MIDIMerger {
   public:
       // Merge multiple MIDI inputs to single output
       void add_input(uint8_t input_port);
       void set_output(uint8_t output_port);
       
       // Handle collisions (e.g., same note on different inputs)
       void process_merged_message(uint8_t input_port, const MIDIMessage& msg);
   };
   
   }  // namespace Service::MIDI::Routing
   ```

3. **Use cases**:
   - **Router**: Channel-based routing (channels 1-4 to output A, 5-8 to output B)
   - **Merger**: Combine multiple MIDI keyboards to single synthesizer input
   - **Filter**: Routing with message filtering (e.g., route only Note On/Off)

4. **Module priority**: P1 (high priority for Service layer after core Standards P0 complete)

**Rationale**: Routing is application-level functionality, not protocol definition.

---

### 6.2 External System Integration

#### Q6.2.1: MIDI Time Code (MTC) integration

**ANSWER**:

**Core parser exposes raw MTC messages, optional MTC decoder module provided**

1. **Core parser behavior** (Standards layer):
   ```cpp
   // Delivers raw MTC Quarter Frame messages
   callback->on_mtc_quarter_frame(uint8_t data);  // 0xF1 + data byte
   
   // Delivers raw MTC Full Frame SysEx
   callback->on_universal_sysex(0x7F, 0x01, 0x01, sysex_data, length);
   ```

2. **Optional MTC decoder module** (Service layer):
   ```cpp
   namespace Service::MIDI::MTC {
   
   struct MTCTime {
       uint8_t hours;        // 0-23
       uint8_t minutes;      // 0-59
       uint8_t seconds;      // 0-59
       uint8_t frames;       // 0-29 (depends on frame rate)
       enum FrameRate { FPS_24, FPS_25, FPS_29_97_DROP, FPS_30 } frame_rate;
   };
   
   class MTCDecoder {
   public:
       // Feed quarter frame messages
       void process_quarter_frame(uint8_t data);
       
       // Get current MTC time (after 8 quarter frames)
       bool get_current_time(MTCTime& time) const;
       
       // Check if time is locked
       bool is_time_locked() const;  // True after 2 complete MTC cycles
   };
   
   }  // namespace Service::MIDI::MTC
   ```

3. **MTC Quarter Frame decoding** (8 messages to complete time):
   ```
   Message piece:  0   1   2   3   4   5   6   7
   Data:          F0  F1  F2  F3  F4  F5  F6  F7
   Content:       frames (LSB), frames (MSB), seconds (LSB), seconds (MSB),
                  minutes (LSB), minutes (MSB), hours (LSB), hours (MSB+rate)
   ```

4. **Integration with IEEE 1588 PTP** (per StR #340):
   - MTC time synchronized with PTP clock (<10μs jitter target)
   - Shared Kernel `IClock` interface for time correlation

**Module priority**: P2 (MTC decoder after core protocol P0/P1 complete)

---

#### Q6.2.2: General MIDI (GM) integration

**ANSWER**:

**Core parser delivers Program Change messages, optional GM module provides instrument names**

1. **Core parser behavior** (Standards layer):
   ```cpp
   // Delivers raw Program Change messages
   callback->on_program_change(uint8_t channel, uint8_t program);  // 0xCn + program
   ```

2. **Optional GM module** (Service layer):
   ```cpp
   namespace Service::MIDI::GeneralMIDI {
   
   enum class GMInstrument {
       ACOUSTIC_GRAND_PIANO = 0,
       BRIGHT_ACOUSTIC_PIANO = 1,
       // ... 128 instruments
       GUNSHOT = 127
   };
   
   class GMSoundSet {
   public:
       // Get instrument name from program number
       const char* get_instrument_name(uint8_t program) const;
       
       // Get GM drum kit note name (channel 10)
       const char* get_drum_note_name(uint8_t note) const;
       
       // Validate GM compliance
       bool is_gm_compliant_program(uint8_t program) const;
   };
   
   }  // namespace Service::MIDI::GeneralMIDI
   ```

3. **GM Level 1/2/Lite databases**:
   ```cpp
   static const char* gm1_instruments[128] = {
       "Acoustic Grand Piano",  // 0
       "Bright Acoustic Piano", // 1
       // ... full GM1 sound set
   };
   
   static const char* gm_drum_notes[128] = {
       // MIDI note 35 = Acoustic Bass Drum
       // MIDI note 36 = Bass Drum 1
       // ... GM drum kit mapping
   };
   ```

4. **GM System On/Off detection**:
   - Parser delivers Universal SysEx (7E 7F 09 01 = GM System On)
   - Application can respond by loading GM sound set

**Module priority**: P2 (GM module after core protocol complete)

---

#### Q6.2.3: Data exchange formats beyond real-time MIDI

**ANSWER**:

**Standard MIDI File (SMF) and JSON MIDI provided as separate modules**

1. **Standard MIDI File (SMF) module** (separate from core):
   ```cpp
   namespace Service::MIDI::SMF {
   
   class SMFReader {
   public:
       // Read .mid file
       bool open(const char* filename);
       
       // Get file format (0, 1, 2)
       SMFFormat get_format() const;
       
       // Get timing (PPQN or SMPTE)
       uint16_t get_division() const;
       
       // Read track events
       bool read_track_event(SMFTrackEvent& event);
   };
   
   class SMFWriter {
   public:
       // Create .mid file
       bool create(const char* filename, SMFFormat format, uint16_t division);
       
       // Write track events
       bool write_track_event(uint32_t delta_time, const MIDIMessage& msg);
       
       // Write meta events (tempo, time signature, etc.)
       bool write_meta_event(MetaEventType type, const uint8_t* data, size_t length);
   };
   
   }  // namespace Service::MIDI::SMF
   ```

2. **JSON MIDI serialization** (for web APIs):
   ```cpp
   namespace Service::MIDI::JSON {
   
   class JSONMIDISerializer {
   public:
       // Serialize MIDI message to JSON
       std::string to_json(const MIDIMessage& msg);
       // Output: {"type":"note_on","channel":1,"note":60,"velocity":100,"time":12345}
       
       // Deserialize JSON to MIDI message
       bool from_json(const std::string& json, MIDIMessage& msg);
   };
   
   }  // namespace Service::MIDI::JSON
   ```

3. **Module priorities**:
   - **SMF Reader/Writer**: P2 (useful for DAW integration, MIDI file import/export)
   - **JSON serialization**: P2 (useful for web MIDI APIs, logging, debugging)

4. **Out of scope for core library**: XMF (eXtensible Music Format), RIFF MIDI, other proprietary formats

**Rationale**: Core library focuses on real-time MIDI 1.0 protocol, file formats are optional add-ons.

---

## 7. User Experience & Accessibility ♿

### 7.1 API Design and Usability

#### Q7.1.1: API style

**ANSWER**:

**SHALL provide all three API styles (low-level, high-level, callback-based)**

1. **Low-level byte-oriented API** (for protocol analysis):
   ```cpp
   class MIDIParser {
   public:
       // Parse single byte
       MIDIErrorCode parse_byte(uint8_t byte);
       
       // Get parser state
       ParserState get_state() const;
       
       // Get partial message (for debugging)
       const PartialMessage& get_partial_message() const;
   };
   ```

2. **High-level message-oriented API** (for applications):
   ```cpp
   class MIDIGenerator {
   public:
       // Send complete messages
       void send_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
       void send_note_off(uint8_t channel, uint8_t note, uint8_t velocity);
       void send_control_change(uint8_t channel, uint8_t controller, uint8_t value);
       void send_program_change(uint8_t channel, uint8_t program);
       
       // High-level helpers
       void send_14bit_cc(uint8_t channel, uint8_t controller, uint16_t value);
       void send_rpn(uint8_t channel, uint16_t rpn, uint16_t value);
   };
   ```

3. **Callback-based API** (for event-driven architectures):
   ```cpp
   class MIDIMessageCallback {
   public:
       virtual void on_note_on(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
       virtual void on_note_off(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
       virtual void on_control_change(uint8_t channel, uint8_t controller, uint8_t value) = 0;
       // ... other message types
   };
   
   // Application implements callback
   class MySynthesizer : public MIDIMessageCallback {
       void on_note_on(uint8_t channel, uint8_t note, uint8_t velocity) override {
           trigger_note(note, velocity);
       }
   };
   ```

4. **API style selection**:
   - **Low-level**: MIDI protocol analyzers, debuggers, test tools
   - **High-level**: Synthesizers, DAWs, MIDI controllers
   - **Callback**: Real-time audio applications, plugin architectures

**All three styles coexist**: Users choose appropriate style for their use case.

---

#### Q7.1.2: Channel numbering convention

**ANSWER**:

**Internal 0-based, user-facing 1-based, explicit API converters**

1. **Internal representation** (0-based):
   ```cpp
   struct MIDIMessage {
       uint8_t channel;  // 0-15 (matches status byte masking: status & 0x0F)
       // ...
   };
   ```

2. **User-facing API** (1-based helpers):
   ```cpp
   class MIDIChannelHelper {
   public:
       // Convert internal channel (0-15) to display (1-16)
       static uint8_t to_display_channel(uint8_t internal_channel) {
           return internal_channel + 1;
       }
       
       // Convert display channel (1-16) to internal (0-15)
       static uint8_t from_display_channel(uint8_t display_channel) {
           assert(display_channel >= 1 && display_channel <= 16);
           return display_channel - 1;
       }
   };
   ```

3. **API documentation convention**:
   ```cpp
   /**
    * @brief Send Note On message
    * 
    * @param channel MIDI channel (0-15 internal, 1-16 user-facing)
    *                Use MIDIChannelHelper::from_display_channel(1-16) to convert
    * @param note MIDI note number (0-127, Middle C = 60)
    * @param velocity Attack velocity (1-127, 0 = Note Off)
    * 
    * @example
    * // Send Note On to channel 1 (display) = channel 0 (internal)
    * midi.send_note_on(0, 60, 100);  // Internal API (0-based)
    * 
    * // Or use helper
    * uint8_t ch = MIDIChannelHelper::from_display_channel(1);  // Returns 0
    * midi.send_note_on(ch, 60, 100);
    */
   void send_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
   ```

4. **Logging and display**:
   ```cpp
   // Always display 1-based to user
   printf("Note On: channel %d, note %d\n", 
          msg.channel() + 1,  // Convert to 1-based for display
          msg.note());
   ```

**Design principle**: Internal efficiency (0-based), user familiarity (1-based), explicit conversions.

---

#### Q7.1.3: Documentation and examples

**ANSWER**:

**SHALL provide comprehensive documentation and examples (P1 priority)**

1. **API Reference** (Doxygen):
   ```
   docs/api/
   ├── html/                    # Generated HTML documentation
   ├── latex/                   # Generated LaTeX/PDF documentation
   └── Doxyfile                 # Doxygen configuration
   
   /**
    * @brief Parse MIDI 1.0 byte stream
    * 
    * This class implements the MIDI 1.0 Detailed Specification v4.2.1 (1996)
    * message parser. It handles all 28 message types including Channel Voice,
    * Channel Mode, System Common, and System Real-Time messages.
    * 
    * @section example_usage Example Usage
    * @code{.cpp}
    * MIDIParser parser;
    * parser.set_callback(&my_callback);
    * 
    * for (uint8_t byte : midi_stream) {
    *     parser.parse_byte(byte);
    * }
    * @endcode
    * 
    * @see MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"
    * @see MIDIMessageCallback for handling parsed messages
    */
   class MIDIParser { /* ... */ };
   ```

2. **Tutorial examples** (priority order):
   ```
   examples/
   ├── 01-hello-midi/           # P0: Basic Note On/Off parsing
   ├── 02-message-generator/    # P0: Generate MIDI messages
   ├── 03-running-status/       # P1: Running Status optimization
   ├── 04-sysex-handling/       # P1: SysEx message processing
   ├── 05-rpn-nrpn/            # P1: RPN/NRPN sequences
   ├── 06-mtc-decoder/         # P2: MIDI Time Code decoding
   ├── 07-multi-port/          # P2: Multiple MIDI ports
   └── 08-transport-adapter/    # P2: Custom transport implementation
   ```

3. **Cookbook recipes** (common patterns):
   ```
   docs/cookbook/
   ├── sending-program-change.md       # How to send Program Change
   ├── handling-14bit-cc.md           # 14-bit Control Change
   ├── implementing-transport.md       # Custom transport adapter
   ├── thread-safe-parsing.md         # Multi-threaded scenarios
   └── error-recovery.md              # Handling malformed MIDI
   ```

4. **Quick start guide**:
   ```markdown
   # Quick Start: MIDI 1.0 Parser
   
   ## Step 1: Include headers
   ```cpp
   #include <MMA/MIDI/1_0/v1996/core/midi_parser.hpp>
   using namespace MMA::MIDI::_1_0::v1996::core;
   ```
   
   ## Step 2: Implement callback
   ```cpp
   class MyCallback : public MIDIMessageCallback {
       void on_note_on(uint8_t ch, uint8_t note, uint8_t vel) override {
           printf("Note On: ch=%d note=%d vel=%d\n", ch+1, note, vel);
       }
   };
   ```
   
   ## Step 3: Parse MIDI bytes
   ```cpp
   MIDIParser parser;
   MyCallback callback;
   parser.set_callback(&callback);
   
   parser.parse_byte(0x90);  // Note On channel 1
   parser.parse_byte(0x3C);  // Note 60 (Middle C)
   parser.parse_byte(0x64);  // Velocity 100
   ```
   ```

**Documentation priority**: P1 (API reference and basic tutorials before first release).

---

### 7.2 Developer Experience

#### Q7.2.1: Debugging and diagnostics

**ANSWER**:

**SHALL provide opt-in diagnostics (compile-time configuration)**

1. **MIDI message logging**:
   ```cpp
   #ifdef MIDI_ENABLE_MESSAGE_LOGGING
   class MIDIMessageLogger {
   public:
       void log_message(const MIDIMessage& msg) {
           printf("[%010llu] %s: ch=%d data=[0x%02X, 0x%02X]\n",
                  msg.timestamp_us(),
                  message_type_name(msg.status()),
                  msg.channel() + 1,
                  msg.data1(), msg.data2());
       }
       
       void log_hex_dump(const uint8_t* data, size_t length) {
           for (size_t i = 0; i < length; i++) {
               printf("%02X ", data[i]);
               if ((i + 1) % 16 == 0) printf("\n");
           }
       }
   };
   #endif
   ```

2. **Parser state inspection**:
   ```cpp
   struct MIDIParserState {
       uint8_t running_status;           // Current Running Status buffer
       uint8_t partial_status;           // Incomplete message status
       uint8_t partial_data[2];          // Partial message data bytes
       uint8_t bytes_received;           // Count in partial message
       ParserState state_enum;           // Parser FSM state
       uint64_t last_byte_time_us;       // Timestamp of last byte
   };
   
   // Query parser state (for debugging)
   const MIDIParserState& state = parser.get_state();
   printf("Running Status: 0x%02X\n", state.running_status);
   printf("Partial message: status=0x%02X bytes=%d\n", 
          state.partial_status, state.bytes_received);
   ```

3. **Performance profiling** (built-in):
   ```cpp
   struct MIDIParserMetrics {
       uint64_t messages_parsed;
       uint64_t total_parse_time_us;
       uint64_t min_parse_time_us;
       uint64_t max_parse_time_us;
       double avg_parse_time_us;
       
       // Histogram bins for latency distribution
       uint32_t latency_histogram[10];  // 0-1μs, 1-2μs, ..., >10μs
   };
   
   const MIDIParserMetrics& metrics = parser.get_metrics();
   printf("Average parse time: %.2f μs\n", metrics.avg_parse_time_us);
   ```

4. **Compile-time configuration**:
   ```cpp
   // Enable/disable diagnostics
   #define MIDI_ENABLE_MESSAGE_LOGGING 1    // Detailed message logs
   #define MIDI_ENABLE_STATE_INSPECTION 1   // Parser state queries
   #define MIDI_ENABLE_PROFILING 1          // Performance metrics
   #define MIDI_ENABLE_ASSERTIONS 1         // Runtime assertions (debug builds)
   ```

**Design principle**: Zero overhead when diagnostics disabled (production builds).

---

#### Q7.2.2: Error message formatting

**ANSWER**:

**SHALL provide detailed error context with human-readable descriptions**

1. **Error message format**:
   ```cpp
   void format_parse_error(const MIDIParseError& error, char* buffer, size_t size) {
       snprintf(buffer, size,
                "MIDI Parse Error [%s] at offset %zu (timestamp %llu μs):\n"
                "  Offending byte: 0x%02X\n"
                "  Partial message: status=0x%02X, %d bytes received\n"
                "  Expected: %s\n"
                "  Context: %s",
                error_code_name(error.error_code),
                error.byte_offset,
                error.timestamp_us,
                error.byte_value,
                error.partial_status,
                error.bytes_received,
                expected_description(error),
                context_description(error));
   }
   ```

2. **Example error messages**:
   ```
   MIDI Parse Error [INVALID_DATA_BYTE] at offset 42 (timestamp 1234567 μs):
     Offending byte: 0x90
     Partial message: status=0x90, 2 bytes received
     Expected: Data byte (0x00-0x7F)
     Context: Parsing Note On message, expected velocity byte
   
   MIDI Parse Error [INCOMPLETE_MESSAGE] at offset 67 (timestamp 2345678 μs):
     Offending byte: 0xB0
     Partial message: status=0x90, 2 bytes received
     Expected: Velocity data byte for Note On
     Context: New Control Change status interrupted incomplete Note On
   ```

3. **Error code descriptions**:
   ```cpp
   const char* error_code_description(MIDIErrorCode code) {
       switch (code) {
           case MIDI_ERROR_INVALID_STATUS:
               return "Bit 7 = 0 when expecting status byte (bit 7 must = 1)";
           case MIDI_ERROR_INVALID_DATA:
               return "Bit 7 = 1 when expecting data byte (bit 7 must = 0)";
           case MIDI_ERROR_INCOMPLETE_MESSAGE:
               return "Message incomplete (timeout or interrupted by new status)";
           // ... other error codes
           default:
               return "Unknown error";
       }
   }
   ```

4. **Contextual help**:
   ```cpp
   void print_error_help(MIDIErrorCode code) {
       switch (code) {
           case MIDI_ERROR_INVALID_DATA:
               printf("HELP: All MIDI data bytes must have bit 7 = 0 (value 0x00-0x7F).\n");
               printf("      Status bytes have bit 7 = 1 (value 0x80-0xFF).\n");
               printf("      Check if transmitting device is MIDI 1.0 compliant.\n");
               break;
           // ... other help messages
       }
   }
   ```

**User experience**: Clear, actionable error messages with context and expected values.

---

**End of Part 4: Integration & UX Answers**

**Parts completed**: 1-4 of 5  
**Final part to follow**: Part 5 (Compliance & Regulations)
