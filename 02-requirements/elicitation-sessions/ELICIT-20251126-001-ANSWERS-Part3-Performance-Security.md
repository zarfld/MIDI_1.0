# Stakeholder Responses - Part 3: Performance & Security

**Session**: ELICIT-20251126-001-MIDI-1.0-Core-Protocol  
**Sections**: 4. Performance & Scalability, 5. Security & Privacy  
**Date**: 2025-11-26

---

## 4. Performance & Scalability ⚡

### 4.1 Real-Time Performance

#### Q4.1.1: Maximum latency for message parsing

**ANSWER**:

**Target: <10μs per message (P95), <50μs worst-case**

1. **Latency budget breakdown**:
   ```
   MIDI byte arrival (320μs @ 31.25 kbaud)
   ├─ Status byte decode: <1μs
   ├─ Data byte validation: <0.5μs per byte
   ├─ Message assembly: <2μs
   ├─ Callback invocation: <5μs
   └─ Total: <10μs (95th percentile)
   ```

2. **Rationale for <10μs target**:
   - **Audio frame time @ 44.1kHz**: 22.6μs per sample
   - **MIDI should not exceed 50% of one sample period**
   - **Allows for**: Multiple MIDI messages per audio callback

3. **Measurement method**:
   ```cpp
   uint64_t start = get_time_us();
   midi_parser.parse_byte(byte);
   uint64_t end = get_time_us();
   uint64_t latency = end - start;
   
   // Track histogram for P50, P95, P99, max
   latency_histogram.record(latency);
   ```

4. **Performance validation**:
   - **Unit tests**: Measure parse latency for all 28 message types
   - **Stress tests**: Sustained 3125 bytes/second (max MIDI bandwidth)
   - **Profiling**: Identify hotspots, optimize critical paths

**Acceptance criteria**: 95% of messages parsed in <10μs on ARM Cortex-M7 @ 480MHz.

---

#### Q4.1.2: Throughput requirements

**ANSWER**:

**SHALL sustain 3125 bytes/second (31.25 kbaud) without message loss**

1. **MIDI 1.0 bandwidth limits**:
   ```
   Bit rate: 31.25 kbaud = 31,250 bits/second
   Byte rate: 31,250 / 10 bits/byte = 3,125 bytes/second
   Message rate (3-byte Note On): ~1,041 messages/second
   Message rate (1-byte Real-Time): 3,125 messages/second
   ```

2. **Parser throughput targets**:
   - **Sustained**: 3,125 bytes/second continuous (no drops)
   - **Burst**: 10,000 bytes/second for 100ms (accommodate USB latency)
   - **Multi-port**: 3,125 bytes/second **per port** (16 ports = 50KB/s total)

3. **Throughput validation tests**:
   ```cpp
   // Test 1: Sustained load (1 minute)
   for (int i = 0; i < 187500; i++) {  // 3125 bytes/sec * 60 sec
       midi_parser.parse_byte(test_data[i % test_data_size]);
       usleep(320);  // Simulate 31.25 kbaud timing
   }
   ASSERT_EQ(midi_parser.get_error_stats().dropped_message_count, 0);
   
   // Test 2: Burst (10KB in 100ms)
   for (int i = 0; i < 10000; i++) {
       midi_parser.parse_byte(test_data[i]);
   }
   ```

4. **Performance optimization**:
   - **Minimize allocations**: Static buffers only
   - **Optimize hot path**: Inline functions, branch prediction hints
   - **Lock-free data structures**: For multi-threaded scenarios

**Success metric**: Zero message drops during 1-hour continuous MIDI stream at full bandwidth.

---

#### Q4.1.3: Memory allocation constraints (Already Answered)

**ANSWER**: ✅ **Covered in Q4.1.3 update**

**Remaining question answered**: Default buffer sizes:
- **MIDI RX buffer**: 1024 bytes (default), configurable to 512/2048/4096
- **SysEx buffer**: 256 bytes (default), configurable to 64/512/1024/4096/65536
- **Message queue**: 64 messages (default), configurable to 32/128/256

**Rationale**:
- 1KB RX buffer = ~300ms of MIDI data at full bandwidth (adequate for jitter absorption)
- 256-byte SysEx = covers 99% of common SysEx messages (GM System On, Device Inquiry, etc.)
- 64-message queue = ~192 bytes for 3-byte messages (low memory footprint)

---

#### Q4.1.4: CPU overhead budget

**ANSWER**:

**Target: <1% CPU on ARM Cortex-M7 @ 480MHz, <0.1% on x86-64 @ 3GHz**

1. **CPU budget allocation** (embedded system):
   ```
   Total CPU: 480 MHz (ARM Cortex-M7)
   Audio DSP: ~60% (288 MHz for audio processing)
   MIDI processing: <1% (4.8 MHz)
   Other tasks: 39%
   ```

2. **MIDI CPU overhead calculation**:
   ```
   Parse time: 10μs per message
   Message rate: 1,000 messages/second (typical)
   CPU time: 10μs * 1,000 = 10ms/second = 1% CPU
   ```

3. **Measurement method**:
   ```cpp
   // Profile MIDI parser CPU usage
   uint64_t total_cycles = 0;
   uint64_t message_count = 0;
   
   for each message {
       uint64_t start_cycles = read_cycle_counter();
       midi_parser.parse_message(message);
       uint64_t end_cycles = read_cycle_counter();
       total_cycles += (end_cycles - start_cycles);
       message_count++;
   }
   
   double cpu_percent = (total_cycles / (cpu_freq * measurement_duration)) * 100;
   double avg_cycles_per_message = total_cycles / message_count;
   ```

4. **Optimization targets**:
   - **Fast path**: Status byte decode in <100 cycles
   - **Data byte processing**: <50 cycles per byte
   - **Callback overhead**: <500 cycles (application-dependent)

**Validation**: Continuous profiling in CI/CD pipeline, regression detection.

---

### 4.2 Concurrency and Thread Safety

#### Q4.2.1: Thread safety for concurrent access

**ANSWER**:

**SHALL provide lock-free single-producer/single-consumer (SPSC) design**

1. **Architecture**:
   ```
   MIDI RX Thread (Producer)          Audio Thread (Consumer)
   ├─ Receive MIDI bytes              ├─ Read parsed messages
   ├─ Parse into messages             ├─ Process messages
   ├─ Write to lock-free queue ─────> ├─ Generate audio
   └─ No locks (real-time safe)       └─ No locks (real-time safe)
   ```

2. **Lock-free ring buffer**:
   ```cpp
   template<typename T, size_t SIZE>
   class LockFreeRingBuffer {
       std::array<T, SIZE> buffer;
       std::atomic<size_t> write_index{0};
       std::atomic<size_t> read_index{0};
       
   public:
       bool push(const T& item) {  // Called by MIDI thread
           size_t current_write = write_index.load(std::memory_order_relaxed);
           size_t next_write = (current_write + 1) % SIZE;
           if (next_write == read_index.load(std::memory_order_acquire))
               return false;  // Full
           buffer[current_write] = item;
           write_index.store(next_write, std::memory_order_release);
           return true;
       }
       
       bool pop(T& item) {  // Called by audio thread
           size_t current_read = read_index.load(std::memory_order_relaxed);
           if (current_read == write_index.load(std::memory_order_acquire))
               return false;  // Empty
           item = buffer[current_read];
           read_index.store((current_read + 1) % SIZE, std::memory_order_release);
           return true;
       }
   };
   ```

3. **Thread safety guarantees**:
   - **Single producer (MIDI RX thread)**: Writes parsed messages to queue
   - **Single consumer (audio thread)**: Reads messages from queue
   - **No locks required**: Atomic operations only (real-time safe)
   - **Wait-free**: Operations complete in bounded time

4. **Multi-producer scenario** (multiple MIDI ports):
   - **Option A**: One parser per port, each with its own SPSC queue
   - **Option B**: Use MPSC (multi-producer/single-consumer) queue with atomic operations

**Design principle**: Minimize contention, avoid blocking operations in real-time threads.

---

#### Q4.2.2: Multi-port MIDI handling

**ANSWER**:

**SHALL provide independent parser instances per port**

1. **Architecture**:
   ```cpp
   class MIDIDevice {
       std::array<MIDIParser, 16> port_parsers;  // 16 virtual ports (USB MIDI)
       std::array<LockFreeRingBuffer<MIDIMessage, 64>, 16> port_queues;
       
   public:
       void parse_byte_on_port(uint8_t port, uint8_t byte) {
           port_parsers[port].parse_byte(byte);
       }
       
       bool read_message_from_port(uint8_t port, MIDIMessage& msg) {
           return port_queues[port].pop(msg);
       }
   };
   ```

2. **Per-port state** (independent):
   - Running Status buffer (separate per port)
   - Parse state machine (separate per port)
   - Error statistics (separate per port)
   - Timing state (separate per port, for Active Sensing)

3. **Port tagging**:
   ```cpp
   struct MIDIMessage {
       uint8_t port;      // Port number (0-15)
       uint8_t status;    // MIDI status byte
       uint8_t data1;     // First data byte
       uint8_t data2;     // Second data byte (if applicable)
       uint64_t timestamp_us;  // JR timestamp
   };
   ```

4. **Memory footprint**:
   ```
   Per-port overhead:
   ├─ MIDIParser: ~128 bytes (state machine, buffers)
   ├─ Ring buffer: ~256 bytes (64 messages * 4 bytes)
   └─ Total: ~384 bytes per port
   
   16 ports: 6KB total (acceptable for embedded systems)
   ```

**Rationale**: Independent parsers avoid port crosstalk, simplify state management.

---

## 5. Security & Privacy 🔒

### 5.1 Input Validation and Sanitization

#### Q5.1.1: Protection against malformed MIDI streams

**ANSWER**:

**SHALL enforce limits and detect anomalies**

1. **Maximum message length limits**:
   ```cpp
   #define MIDI_MAX_SYSEX_SIZE 65536       // 64KB (configurable)
   #define MIDI_MAX_MESSAGE_RATE 10000     // 10K messages/second per port
   #define MIDI_RX_BUFFER_SIZE 1024        // 1KB RX buffer
   ```

2. **Attack vectors and mitigations**:

   **Attack 1: Excessive SysEx length (memory exhaustion)**
   ```cpp
   // Mitigation: Enforce maximum SysEx size
   if (sysex_length > MIDI_MAX_SYSEX_SIZE) {
       callback->on_security_event(SECURITY_SYSEX_SIZE_EXCEEDED, sysex_length);
       discard_sysex();
       return MIDI_ERROR_SYSEX_TOO_LARGE;
   }
   ```

   **Attack 2: Rapid status byte spam (CPU exhaustion)**
   ```cpp
   // Mitigation: Rate limiting
   uint32_t messages_per_second = get_message_rate();
   if (messages_per_second > MIDI_MAX_MESSAGE_RATE) {
       callback->on_security_event(SECURITY_RATE_LIMIT_EXCEEDED, messages_per_second);
       throttle_input();  // Drop messages above rate limit
   }
   ```

   **Attack 3: Invalid byte sequences (buffer overflow)**
   ```cpp
   // Mitigation: Strict validation, bounds checking
   if (buffer_index >= MIDI_RX_BUFFER_SIZE) {
       callback->on_security_event(SECURITY_BUFFER_OVERFLOW_ATTEMPT, buffer_index);
       reset_parser();
       return MIDI_ERROR_BUFFER_OVERFLOW;
   }
   ```

3. **Anomaly detection**:
   ```cpp
   // Detect unusual patterns
   if (consecutive_sysex_count > 100) {
       callback->on_security_event(SECURITY_SYSEX_FLOOD_DETECTED, consecutive_sysex_count);
   }
   
   if (error_rate > 0.1) {  // >10% error rate
       callback->on_security_event(SECURITY_HIGH_ERROR_RATE, error_rate);
   }
   ```

4. **Fail-safe design**:
   - **Bounded buffers**: All buffers have fixed maximum sizes
   - **Timeout protection**: Incomplete messages discarded after timeout
   - **Graceful degradation**: Parser continues after errors (resilient)

**Security principle**: Defense in depth, fail-safe defaults, rate limiting.

---

#### Q5.1.2: SysEx content filtering

**ANSWER**:

**SHALL provide hooks for application-level content filtering**

1. **SysEx callback API**:
   ```cpp
   class MIDISysExFilter {
   public:
       // Called before SysEx delivered to application
       virtual bool should_accept_sysex(uint8_t manufacturer_id,
                                        const uint8_t* data,
                                        size_t length) = 0;
   };
   
   // Application implements filtering policy
   class SafeSysExFilter : public MIDISysExFilter {
       bool should_accept_sysex(uint8_t mfr_id, const uint8_t* data, size_t len) override {
           // Block firmware upload SysEx (example)
           if (mfr_id == 0x43 && data[0] == 0x7E) {  // Yamaha firmware upload
               log_security_event("Blocked firmware upload SysEx");
               return false;  // Reject
           }
           return true;  // Accept
       }
   };
   ```

2. **Known exploit patterns** (examples to filter):
   - **Firmware upload**: Block unless explicitly enabled
   - **Factory reset**: Block unless authorized
   - **Bulk dumps**: Rate-limit to prevent DoS

3. **Whitelisting approach**:
   ```cpp
   // Accept only known-safe SysEx types
   std::set<uint8_t> allowed_manufacturer_ids = {
       0x7E,  // Universal Non-Real-Time (GM System On/Off)
       0x7F,  // Universal Real-Time (Master Volume)
       0x7D   // Educational use
   };
   
   if (allowed_manufacturer_ids.find(mfr_id) == allowed_manufacturer_ids.end()) {
       return false;  // Reject unknown manufacturers
   }
   ```

4. **Configuration**:
   - **Default**: No filtering (all SysEx accepted)
   - **Opt-in**: Application registers SysExFilter implementation
   - **Logging**: All blocked SysEx logged for security audit

**Rationale**: Library provides hooks, application defines security policy (context-dependent).

---

### 5.2 Access Control

#### Q5.2.1: Channel-level access control

**ANSWER**:

**SHALL provide channel filtering API, application enforces access policy**

1. **Channel filter API**:
   ```cpp
   class MIDIChannelFilter {
   public:
       // Configure allowed channels per user/context
       void set_allowed_channels(const std::set<uint8_t>& channels);
       
       // Check if message should be processed
       bool is_channel_allowed(uint8_t channel) const;
   };
   
   // Application usage
   MIDIChannelFilter user_a_filter;
   user_a_filter.set_allowed_channels({0, 1, 2, 3});  // Channels 1-4
   
   MIDIChannelFilter user_b_filter;
   user_b_filter.set_allowed_channels({4, 5, 6, 7});  // Channels 5-8
   ```

2. **Filter enforcement**:
   ```cpp
   void on_midi_message(const MIDIMessage& msg) {
       if (!channel_filter.is_channel_allowed(msg.channel())) {
           log_security_event("Blocked message on unauthorized channel %d", 
                             msg.channel() + 1);
           return;  // Discard message
       }
       process_message(msg);  // Authorized, process normally
   }
   ```

3. **Use cases**:
   - **Multi-user DAW**: User A controls channels 1-4, User B controls 5-8
   - **Live performance**: Prevent accidental channel crosstalk between performers
   - **Educational setting**: Limit student access to specific channels

4. **Auditing**:
   ```cpp
   struct MIDIAccessLog {
       uint64_t timestamp_us;
       uint8_t user_id;
       uint8_t channel;
       uint8_t status;
       bool was_blocked;
   };
   ```

**Design principle**: Library provides filtering mechanism, application defines policy.

---

#### Q5.2.2: Dangerous message blocking

**ANSWER**:

**SHALL provide configurable message blocking**

1. **Dangerous message types** (configurable blocklist):
   ```cpp
   enum class MIDIDangerousMessage {
       ALL_SOUND_OFF     = 0x78,  // CC 120 (emergency mute)
       RESET_ALL_CC      = 0x79,  // CC 121 (reset controllers)
       ALL_NOTES_OFF     = 0x7B,  // CC 123 (release all notes)
       OMNI_MODE_OFF     = 0x7C,  // CC 124 (change Omni mode)
       OMNI_MODE_ON      = 0x7D,  // CC 125
       MONO_MODE_ON      = 0x7E,  // CC 126 (change Poly/Mono)
       POLY_MODE_ON      = 0x7F,  // CC 127
       SYSTEM_RESET      = 0xFF   // System Reset (catastrophic)
   };
   ```

2. **Blocklist configuration**:
   ```cpp
   class MIDIMessageBlocklist {
   public:
       void block_message_type(MIDIDangerousMessage msg_type);
       void unblock_message_type(MIDIDangerousMessage msg_type);
       bool is_message_blocked(const MIDIMessage& msg) const;
   };
   
   // Example: Block All Sound Off during live performance
   MIDIMessageBlocklist blocklist;
   blocklist.block_message_type(MIDIDangerousMessage::ALL_SOUND_OFF);
   blocklist.block_message_type(MIDIDangerousMessage::SYSTEM_RESET);
   ```

3. **Enforcement**:
   ```cpp
   void on_midi_message(const MIDIMessage& msg) {
       if (message_blocklist.is_message_blocked(msg)) {
           log_security_event("Blocked dangerous message: status=0x%02X", 
                             msg.status());
           return;  // Drop message
       }
       process_message(msg);
   }
   ```

4. **Context-dependent blocking**:
   - **Live performance mode**: Block All Sound Off, System Reset
   - **Normal operation mode**: Allow all messages
   - **Safe mode**: Block all Channel Mode messages, System Reset

**Rationale**: Prevent malicious or accidental disruption in critical scenarios.

---

**End of Part 3: Performance & Security Answers**

**Parts completed**: 1-3 of 5  
**Final parts to follow**:
- Part 4: Integration & Dependencies
- Part 5: User Experience & Compliance
