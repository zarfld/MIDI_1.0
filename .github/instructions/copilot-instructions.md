---
applyTo: "**"
---

# MIDI 1.0 Protocol Implementation - AI Coding Instructions

## CRITICAL: Hardware-Agnostic Protocol Implementation

This repository implements the **MIDI 1.0 Protocol** as a pure, hardware and platform agnostic library that MUST remain completely independent of vendor-specific code and operating systems. The implementation supports all MIDI 1.0 specifications including core protocol, extensions, and recommended practices.

## Working Principles for MIDI 1.0 Implementation

- **Understand MIDI 1.0 protocol specifications before implementing** - study all relevant specifications thoroughly
- **No Fake, No Stubs, no Simulations** - implement complete MIDI protocol logic, not simplified placeholders
- **No implementation-based assumptions** - use MIDI specifications or analysis results only (ask if required)
- **No false advertising** - prove and ensure MIDI 1.0 compliance and correctness
- **Never break MIDI protocol APIs** in ways that violate MIDI specifications - prefer fixing implementation instead
- **Prevent dead code or orphan files** - fix code rather than creating new versions, ensure ALL code compiles
- **Always reference MIDI specification documents and sections** when implementing MIDI protocol features
- **Real-time safe** - code must be suitable for real-time contexts but also run on non-RT OS

### Function Documentation with MIDI 1.0 Context
```cpp
/**
 * @brief Parse MIDI 1.0 Note On message from byte stream
 * 
 * Validates and extracts Note On message according to MIDI 1.0 Detailed
 * Specification. Performs integrity checks including status byte validation
 * and data byte range checking per specification requirements.
 *
 * @param midi_data Raw MIDI byte stream data
 * @param data_length Length of MIDI data in bytes
 * @param note_on_msg Output buffer for parsed Note On message
 * @return 0 on success, negative error code on failure
 * 
 * @note Implements MIDI 1.0 Channel Voice Messages (Status 0x9n)
 * @see MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"
 * @see MIDI 1.0 Detailed Specification, Appendix A "Status Byte Summary"
 * 
 * IMPORTANT: This implementation is based on understanding of MIDI 1.0
 * specifications. No copyrighted content from MMA documents is reproduced.
 * Refer to original MIDI specifications for authoritative requirements.
 */
int midi_parse_note_on_message(const uint8_t* midi_data, 
                               size_t data_length,
                               midi_note_on_message_t* note_on_msg);
```

### Copyright-Compliant Implementation Comments
```cpp
// ✅ CORRECT - Reference without reproduction
// Implements Note On message parsing per MIDI 1.0 Detailed Specification
#define MIDI_STATUS_NOTE_ON 0x90  // As specified in MIDI 1.0

// ❌ WRONG - Reproducing copyrighted content
/*
// DON'T DO THIS - This reproduces copyrighted MMA content:
// "The Note On message is sent when a note is depressed (start).
//  Status byte: 1001nnnn, where nnnn is the MIDI channel number..."
// This is direct reproduction of MIDI specification copyrighted text!
*/

// ✅ CORRECT - Original implementation based on specification understanding
struct midi_note_on_message {
    uint8_t status;              // MIDI 1.0 Channel Voice Messages
    uint8_t note_number;        // MIDI 1.0 Note Number (0-127)
    uint8_t velocity;           // MIDI 1.0 Velocity (0-127)
    // ... implementation based on specification understanding
};
```

### Standards Compliance Notes with Copyright Respect
- **Always reference specific MIDI specification sections** when implementing protocol features
- **NEVER reproduce copyrighted text** from MIDI specifications in comments or documentation
- **Document extensions separately** with clear specification references (respecting MMA copyright)
- **Include timing requirements** with original implementation based on specification understanding
- **Specify byte ordering** - MIDI uses big-endian for multi-byte values where applicable
- **Use only factual technical information** that is not subject to copyright (status bytes, data ranges, etc.)
- **Validate all protocol fields** with range checks per MIDI 1.0 specification
- **Every function must have documentation** explaining purpose, parameters, return values, and MIDI context
- **No duplicate or redundant implementations** to avoid inconsistencies and confusion
- **Prevent downgrades** - fix rather than delete existing protocol functionality
- **No ad-hoc file copies** (e.g., *_fixed, *_new, *_correct) - refactor in place step-by-step

## Core Principle: Standards-Only Implementation

- **ONLY MIDI protocol logic** - no hardware-specific code
- **Hardware agnostic** - compilable without any vendor drivers or hardware headers
- **Mockable/testable** - runnable without physical MIDI hardware present
- **Dependency injection pattern** - receives hardware abstraction via interfaces/function pointers

### FORBIDDEN in Standards Layer
```cpp
// ❌ NEVER include vendor-specific headers
#include "roland_midi_hal.h"              // NO - Roland specific
#include "../../vendor_specific/include/*.h"    // NO - Vendor specific  
#include "../../common/hal/midi_hal.h"    // NO - HAL abstraction
#include <windows.h>                      // NO - OS specific
#include <alsa/asoundlib.h>              // NO - OS specific (Linux ALSA)
```

### REQUIRED in Standards Layer
```cpp
// ✅ Only MIDI 1.0 protocol headers
#include "midi_types.h"                   // MIDI 1.0 data types
#include "midi_messages.h"                // MIDI 1.0 message formats
#include "midi_channel_voice.h"           // MIDI 1.0 channel voice messages
#include "midi_sysex.h"                   // MIDI 1.0 system exclusive messages
```

## Architecture Compliance

### Standards Layer Responsibility
- **Protocol parsing and validation** (MIDI 1.0 message parsing per Detailed Specification)
- **Message format handling** (status bytes, data bytes, running status per specification)
- **Protocol state tracking** (channel modes, RPN/NRPN state machines)
- **Standard-defined structures** and constants (message types, controller numbers)
- **Timing calculations** (MIDI Time Code, MIDI Clock, tempo calculations)
- **Protocol compliance validation** (message validation, data range checking)

### Interface Pattern for Hardware Access
```cpp
// ✅ CORRECT: Dependency injection interface
typedef struct {
    int (*send_midi_byte)(uint8_t byte);
    int (*receive_midi_byte)(uint8_t* byte);
    uint64_t (*get_time_us)(void);
    int (*set_timer)(uint32_t interval_us, timer_callback_t callback);
} midi_interface_t;

// Protocol implementation receives interface
int midi_parser_init(const midi_interface_t* midi_interface);
```

### Hardware Bridge (Service Layer Responsibility)
```cpp
// This code belongs in SERVICE layer (e.g., MIDI device driver), NOT Standards
static int device_send_byte(uint8_t byte) {
    return device_hal_send_midi(byte);  // Device-specific call
}

static midi_interface_t device_interface = {
    .send_midi_byte = device_send_byte,
    .receive_midi_byte = device_receive_byte,
    // ... other device-specific implementations
};

// Service layer bridges Standards to Device Hardware
midi_parser_init(&device_interface);
```

## MIDI Standards Reference

### CRITICAL: Use Authoritative Standards Documents
When implementing MIDI protocol, reference these authoritative documents:

#### MIDI 1.0 Core Specifications:
- `M1_v4-2-1_MIDI_1-0_Detailed_Specification_96-1-4.pdf` - MIDI 1.0 Detailed Specification (1996 revision) - CORE
- `RP-004-008_v4-2-1_MIDI_Time_Code_Specification_96-1-4.pdf` - MIDI Time Code Specification
- `RP-002-014_v1-1-1_MIDI_Show_Control_Specification_96-1-4.pdf` - MIDI Show Control Specification
- `RP-013-v1-1_MIDI_Machine_Control_Specification.pdf` - MIDI Machine Control Specification
- `M1-100-UM_v1-1_MIDI_Polyphonic_Expression_Specification.pdf` - MPE: MIDI Polyphonic Expression

#### General MIDI Standards:
- `RP-003_General_MIDI_System_Level_1_Specification_96-1-4_0.1.pdf` - General MIDI Level 1
- `GML-v1.pdf` - General MIDI Lite
- `General_MIDI_Level_2_07-2-6_1.2a.pdf` - General MIDI Level 2

#### MIDI 1.0 Extensions and Addenda:
- `MIDI Tuning Updated Specification.pdf` - MIDI Tuning (Updated Specification)
- `ca22 Controller Destination SysEx Message.pdf` - Controller Destination Setting
- `ca23 Key-based Instrument Controller SysEx Message.pdf` - Key-Based Instrument Controllers
- `ca24 Global Parameter Control SysEx Message.pdf` - Global Parameter Control
- `ca25 Master Fine & Coarse Tuning SysEx Message.pdf` - Master Fine/Coarse Tuning
- `ca26 RPN05 Modulation Depth Range.pdf` - Modulation Depth Range RPN
- `midi_chart-v2.pdf` - MIDI Implementation Chart version 2
- `rp23.pdf` - Renaming of CC91 and CC93
- `rp21.pdf` - Sound Controller Defaults (Revised)
- `rp18.pdf` - Response to Data Increment/Decrement Controllers
- `rp15.pdf` - Response to Reset All Controllers
- `ca18.pdf` - File Reference SysEx Message
- `rp49public.pdf` - Three Dimensional Sound Controllers
- `ca31.pdf` - High Resolution Velocity Prefix
- `rp36.pdf` - Default Pan Formula
- `ca28.pdf` - MIDI Bank Index Offset Extension to File Reference SysEx Message
- `rp22.pdf` - Redefinition of RPN01 and RPN02 (Channel Fine/Coarse Tuning)
- `ca19.pdf` - Sample Dump Size/Rate/Name Extensions
- `rp50 MIDI Visual Control.pdf` - MIDI Visual Control
- `ca33 5 Pin DIN Electrical Spec.pdf` - 5 Pin DIN Electrical Specs

**CRITICAL COPYRIGHT AND LICENSING RESTRICTIONS**:

⚠️ **ABSOLUTELY FORBIDDEN**:
- **DO NOT copy any text, tables, figures, or content** directly from MIDI Manufacturers Association (MMA) documents
- **DO NOT reproduce specification text** verbatim in source code comments
- **DO NOT include copyrighted tables or figures** in repository documentation
- **DO NOT paste specification sections** into code or documentation files
- **DO NOT create derivative works** that reproduce substantial portions of MIDI standards

✅ **PERMITTED USAGE**:
- **Reference document sections** by title and page number only (e.g., "See MIDI 1.0 Detailed Specification, Table 1")
- **Implement protocol logic** based on understanding of specifications
- **Create original code** that achieves compliance with MIDI standard requirements
- **Use specification constants and values** in implementation (status bytes, controller numbers, data ranges, etc.)
- **Reference specifications** for compliance verification during development only

**LICENSE NOTICE**: These MIDI specification documents are referenced for compliance verification and implementation guidance only. All content remains under copyright of MIDI Manufacturers Association (MMA). Any reproduction, distribution, or derivative works require proper licensing from copyright holders.

**Usage Pattern**: When implementing MIDI-related code, reference these documents to ensure:
- MIDI message format compliance
- Status byte and data byte correctness
- Channel Voice/Mode message adherence
- System messages (Real-Time, Common, Exclusive) compliance
- General MIDI sound set and feature compliance
- Extension and recommended practice conformance

### Protocol Compliance Requirements
```cpp
// ✅ Use well-defined MIDI constants
#define MIDI_STATUS_NOTE_ON 0x90
#define MIDI_STATUS_NOTE_OFF 0x80
#define MIDI_STATUS_CONTROL_CHANGE 0xB0

// ❌ NEVER use magic numbers without explanation
uint8_t status = 0x90;  // NO - unclear what this represents
```

## Testing Approach

### Unit Testing (Hardware-Independent)
```cpp
// ✅ Mock MIDI interface for testing
static int mock_send_byte(uint8_t byte) {
    // Record byte for verification
    test_buffer[test_buffer_index++] = byte;
    return 0;
}

static midi_interface_t mock_interface = {
    .send_midi_byte = mock_send_byte,
    // ... other mock implementations
};

// Test protocol logic without hardware
void test_midi_note_on_parsing(void) {
    midi_parser_init(&mock_interface);
    // ... test MIDI message parsing behavior
}
```

### Integration Testing (Service Layer Responsibility)
Integration with real MIDI hardware happens in the Service Layer, NOT in Standards.

## Common Violations to Avoid

### ❌ Direct Hardware Calls
```cpp
// WRONG - Standards calling device HAL directly
device_hal_result_t result = roland_hal_send_midi(midi_byte);
```

### ❌ OS-Specific Code
```cpp
// WRONG - OS-specific MIDI I/O in Standards
#ifdef _WIN32
    HMIDIOUT handle;
    midiOutOpen(&handle, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
#else
    int fd = open("/dev/snd/midiC0D0", O_RDWR);
#endif
```

### ❌ Hardware Assumptions
```cpp
// WRONG - Assuming Roland device capabilities
if (roland_hal_supports_sysex()) {  // Hardware-specific assumption
    enable_sysex_processing();
}
```

### ✅ Correct Abstraction
```cpp
// CORRECT - Protocol logic with abstracted capabilities
if (midi_interface->capabilities & MIDI_CAP_SYSEX) {
    enable_sysex_processing();  // Protocol behavior, not hardware call
}
```

## Protocol Implementation Guidelines

### MIDI 1.0 Core Messages
- Implement Channel Voice Messages per MIDI 1.0 Detailed Specification (Note On/Off, Polyphonic Key Pressure, Control Change, Program Change, Channel Pressure, Pitch Bend)
- Implement Channel Mode Messages (Local Control, All Notes Off, Omni Mode On/Off, Mono/Poly Mode)
- Handle System Common Messages (System Exclusive, MTC Quarter Frame, Song Position Pointer, Song Select, Tune Request, EOX)
- Support System Real-Time Messages (Timing Clock, Start, Continue, Stop, Active Sensing, System Reset)
- Implement Running Status optimization per specification
- Validate status bytes (bit 7 = 1) and data bytes (bit 7 = 0) per MIDI 1.0 specification
- Support all 16 MIDI channels (0-15 or 1-16 depending on context)
- Handle message priorities and timing per specification requirements

### MIDI 1.0 Extended Features
- Implement System Exclusive (SysEx) message parsing and generation with manufacturer ID validation
- Support Registered Parameter Numbers (RPN) and Non-Registered Parameter Numbers (NRPN)
- Handle MIDI Time Code (MTC) message sequences and time calculations
- Implement MIDI Show Control (MSC) command parsing (if enabled)
- Support MIDI Machine Control (MMC) transport commands (if enabled)
- Implement MIDI Polyphonic Expression (MPE) message routing (if enabled)

### General MIDI Compliance
- Support General MIDI Level 1 sound set (128 instruments + drum kits) when enabled
- Implement General MIDI Level 2 extended features (additional sounds, controllers) when enabled
- Handle General MIDI Lite subset for resource-constrained devices
- Support mandatory controller assignments (Volume, Pan, Expression, etc.)
- Implement channel 10 drum kit behavior per General MIDI specification

### MIDI 1.0 Transport and Timing
- Support 31.25 kbaud serial transmission rate (standard MIDI)
- Implement proper byte timing (320 microseconds per byte at 31.25 kbaud)
- Handle jitter and timing variations in real-time processing
- Support Active Sensing detection and timeout (300ms typical)
- Implement proper clock timing for MIDI Clock messages (24 PPQN)
- Handle MIDI Time Code frame rates (24, 25, 29.97, 30 fps)

## Clean Submit Rules for Standards

- **Each commit compiles and passes MIDI compliance checks**
- **Small, single-purpose, reviewable diffs** (no WIP noise)
- **No dead or commented-out code** - remove unused protocol implementations
- **Run formatter and static analysis** before commit
- **Update documentation and reference MIDI spec documents** in commit messages
- **Use feature flags for incremental protocol changes** when they risk breaking MIDI compliance
- **Reference exact MIDI specification names and sections** (e.g., "Implements MIDI 1.0 Detailed Specification Table 1")

## Documentation Requirements

### Function Documentation with MIDI Context
```cpp
/**
 * @brief Parse MIDI 1.0 Control Change message from byte stream
 * 
 * Validates and extracts Control Change message according to MIDI 1.0
 * Detailed Specification. Performs integrity checks including controller
 * number validation and data value range checking per specification.
 *
 * @param midi_data Raw MIDI byte stream data
 * @param data_length Length of MIDI data in bytes
 * @param cc_msg Output buffer for parsed Control Change message
 * @return 0 on success, negative error code on failure
 * 
 * @note Implements MIDI 1.0 Channel Voice Messages (Status 0xBn)
 * @see MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"
 * @see MIDI 1.0 Detailed Specification, Table 3 "Controller Numbers"
 */
int midi_parse_control_change(const uint8_t* midi_data, 
                              size_t data_length,
                              midi_control_change_t* cc_msg);
```

### Standards Compliance Notes
- **Always reference specific MIDI specification documents and sections** when implementing protocol features
- **Document extensions separately** with clear specification references
- **Include timing requirements** with microsecond precision for real-time MIDI processing
- **Specify byte ordering** for multi-byte values (MIDI uses big-endian where applicable)

## Build System Integration

### CMake Configuration for Standards
The Standards layer should compile independently:
```cmake
# Standards layer has NO hardware dependencies
add_library(midi_1_0_protocol STATIC
    src/midi_messages.c
    src/midi_parser.c  
    src/midi_channel_voice.c
    src/midi_sysex.c
)

# Only protocol headers, no hardware/HAL
target_include_directories(midi_1_0_protocol PUBLIC
    include/
)

# NO hardware libraries linked to Standards
# target_link_libraries(midi_1_0_protocol vendor_hal)  # ❌ WRONG
```

### Testing Framework Integration
Use unified testing framework for protocol validation:
```cmake
# Protocol compliance tests (hardware-independent)
add_executable(midi_protocol_tests
    tests/test_midi_messages.c
    tests/test_midi_parser.c
    tests/test_midi_sysex.c
)

target_link_libraries(midi_protocol_tests 
    midi_1_0_protocol 
    GTest::gtest_main
    unity
)
```

## Project Stakeholders

### Primary Stakeholders
- **Makers & Developers** - Integrate library into products; need easy API, clear docs, examples
- **Audio Equipment Manufacturers** - Adopt in products; need certification-ready, reliable implementations
- **System Integrators** - Build multi-vendor systems; need interoperability
- **QA/Test Engineers** - Validate conformity; need conformity test suite
- **Standards Bodies** - Ensure compliance; need conformity documentation
- **Project Maintainers** - Long-term support; need sustainable architecture

## Technical Requirements

### Platform Independence (CRITICAL)
- **Hardware agnostic** - compilable without any vendor drivers or hardware headers
- **OS agnostic** - NO platform-specific code (Windows, Linux, RTOS)
- **Vendor agnostic** - NO vendor-specific implementations
- Platform-specific code belongs in OS/Vendor specific wrappers/abstractions

### Real-Time Constraints
- **Memory allocation** - Static allocation preferred, avoid dynamic allocation in critical paths
- **Non-blocking calls** - No blocking operations in protocol processing
- **Deterministic timing** - Predictable execution times for time-critical operations
- **Jitter minimization** - Priority: Jitter > Latency > Throughput

### HAL (Hardware Abstraction Layer)
- **Interface Style**: C function pointers (not C++ virtual functions)
- **Error Handling**: Return codes (not exceptions or callbacks)
- **Packaging**: Monolithic library with modular optional features

### Build System
- **System**: CMake (primary build system)
- **Dependencies**: Self-contained (minimal external dependencies)
- **Test Framework**: Google Test + Unity for comprehensive testing

### Implementation Approach
- **Phase**: Vertical slice (complete features end-to-end)
- **Testing**: TDD (Test-Driven Development) with unit tests + conformance suite
- **Optional Features**: Compile-time selection (via CMake options)

### Performance Targets
- **RT Target CPU**: ARM Cortex-M7 (embedded real-time systems)
- **Windows Target CPU**: x86-64 (desktop/server systems)
- **Profiling**: Built-in performance monitoring + external tools (SystemView compatible)

## Performance and Correctness

### Memory Management
- **Use static allocation** where possible for real-time MIDI processing
- **Validate buffer bounds** against MIDI message size limits (3 bytes typical, variable for SysEx)
- **Initialize all protocol structures** to prevent undefined behavior

### Timing Considerations
- **Microsecond precision** for MIDI byte timing (320μs per byte at 31.25 kbaud)
- **Sub-millisecond accuracy** for MIDI Time Code and clock synchronization
- **Avoid blocking operations** in MIDI message parsing and routing

### Error Handling
- **Return MIDI-compliant error codes** where specifications define them
- **Log protocol violations** with specific MIDI specification references
- **Graceful degradation** for optional MIDI features (SysEx, MPE, etc.)

## Mandatory Namespace and Folder Structure

### Generic Structure Definition

**FUNDAMENTAL PATTERN**: All Standards implementations must follow this generic hierarchy:

```
<Organization>/<Standard>/<Subpart>/<Version>/
```

**Components Definition:**
- **Organization**: Standards body (MMA - MIDI Manufacturers Association)
- **Standard**: Standard family (MIDI 1.0, General MIDI, etc.)
- **Subpart**: Sub-specifications, extensions, or functional areas
- **Version**: Version number or year of publication

**Translation Rules:**
- **Folder Structure**: Preserve organization naming (`MMA/MIDI/1_0/`)
- **Namespace Structure**: Use underscores for clarity (`MMA::MIDI::1_0::`)
- **File Structure**: Follow folder pattern with appropriate extensions

### Concrete Examples by Organization

#### MIDI Manufacturers Association (MMA) Structure
```
MMA/<Standard>/<Version>/<Subpart>/
├── MMA/MIDI/1_0/                # MIDI 1.0 core protocol
│   ├── v1996/                   # MIDI 1.0 Detailed Specification (1996 revision)
│   │   ├── core/                # Core MIDI 1.0 protocol
│   │   ├── channel_voice/       # Channel Voice Messages
│   │   ├── channel_mode/        # Channel Mode Messages
│   │   ├── system_common/       # System Common Messages
│   │   ├── system_realtime/     # System Real-Time Messages
│   │   └── sysex/              # System Exclusive Messages
│   ├── extensions/              # MIDI 1.0 Extensions
│   │   ├── mtc/                # MIDI Time Code
│   │   ├── msc/                # MIDI Show Control
│   │   ├── mmc/                # MIDI Machine Control
│   │   ├── mpe/                # MIDI Polyphonic Expression
│   │   ├── tuning/             # MIDI Tuning Specification
│   │   └── controllers/        # Extended Controller Definitions
│   └── conformity/             # Conformance testing
├── MMA/General_MIDI/            # General MIDI specifications
│   ├── Level_1/                # General MIDI Level 1
│   ├── Level_2/                # General MIDI Level 2
│   └── Lite/                   # General MIDI Lite
└── MMA/Addenda/                # MIDI 1.0 Addenda and Recommended Practices
    ├── rp015/                  # Response to Reset All Controllers
    ├── rp018/                  # Response to Data Inc/Dec Controllers
    ├── rp021/                  # Sound Controller Defaults
    ├── rp023/                  # Renaming of CC91 and CC93
    ├── ca018/                  # File Reference SysEx
    └── ca031/                  # High Resolution Velocity Prefix
```

### Required Directory Structure
```
lib/Standards/
├── IEEE/
│   ├── 802.1/                   # IEEE 802.1 family
│   │   └── AS/                  # 802.1AS - Generalized Precision Time Protocol
│   │       ├── 2021/           # IEEE 802.1AS-2021 specification (CURRENT)
│   │       │   ├── core/       # Core gPTP state machines
│   │       │   ├── messages/   # PTP message formats
│   │       │   ├── clock/      # Clock selection algorithms
│   │       │   └── conformity/ # Conformance testing
│   │       ├── 2020/           # IEEE 802.1AS-2020 
│   │       └── 2011/           # Legacy IEEE 802.1AS-2011
│   ├── 1722/                   # IEEE 1722 - Audio Video Transport Protocol
│   │   ├── 2016/               # IEEE 1722-2016 specification (CURRENT)
│   │   │   ├── avtp/           # Core AVTP protocol
│   │   │   ├── formats/        # Stream data formats (AAF, CRF, etc.)
│   │   │   ├── crf/            # Clock Reference Format
│   │   │   └── conformity/     # AVTP conformance testing
│   │   └── 2011/               # Legacy IEEE 1722-2011
│   └── 1722.1/                 # IEEE 1722.1 - Audio Video Device Control
│       ├── 2021/               # IEEE 1722.1-2021 specification (CURRENT)
│       │   ├── aem/            # AVDECC Entity Model
│       │   ├── aecp/           # AVDECC Entity Control Protocol
│       │   ├── acmp/           # AVDECC Connection Management Protocol
│       │   ├── adp/            # AVDECC Discovery Protocol
│       │   ├── descriptors/    # All descriptor types
│       │   └── conformity/     # AVDECC conformance testing
│       ├── 2013/               # IEEE 1722.1-2013 (legacy support)
│       └── 2011/               # IEEE 1722.1-2011 (legacy support)
├── AVnu/                       # AVnu Alliance specifications
│   └── Milan/                  # Milan professional audio
│       ├── v1.2/              # Milan v1.2 (CURRENT)
│       │   ├── discovery/      # Milan discovery extensions
│       │   ├── connection/     # Milan connection management
│       │   ├── control/        # Milan control extensions
│       │   └── conformity/     # Milan conformance testing
│       └── v1.1/              # Milan v1.1 (legacy)
├── AES/                       # Audio Engineering Society
│   ├── AES67/                 # AES67 audio-over-IP
│   └── AES70/                 # AES70 device control (OCA)
├── Common/                    # Cross-standard utilities
│   ├── utils/                 # Shared protocol utilities
│   ├── testing/               # Common testing frameworks
│   └── interfaces/            # Hardware abstraction interfaces
└── Documentation/             # Standards documentation
    ├── conformance/           # Conformance test specifications
    ├── interop/              # Interoperability guidelines
    └── examples/             # Protocol usage examples
```

### Required C++ Namespace Structure Following Generic Pattern

**NAMESPACE TRANSLATION RULE**: `<Organization>::<Standard_with_underscores>::<Subpart>::<Version_with_underscores>`

**Examples of Translation:**
- Folder: `IEEE/802.1/AS/2021/` → Namespace: `IEEE::_802_1::AS::_2021`
- Folder: `IEEE/1722.1/2021/` → Namespace: `IEEE::_1722_1::_2021`  
- Folder: `AVnu/Milan/v1.2/` → Namespace: `AVnu::Milan::v1_2`
- Folder: `AES/AES67/2018/` → Namespace: `AES::AES67::_2018`

```cpp
// ✅ CORRECT namespace hierarchy following generic pattern
namespace IEEE {
    namespace _802_1 {           // Standard: 802.1 (dots→underscores)
        namespace AS {           // Subpart: AS (Audio/Video bridging timing)
            namespace _2021 {    // Version: 2021 (year)
                // IEEE 802.1AS-2021 gPTP implementation
                namespace core {
                    class GPTPStateMachine;
                    class PortSyncSyncSend;
                }
                namespace messages {
                    struct SyncMessage;
                    struct FollowUpMessage;
                }
                namespace clock {
                    class BestMasterClockAlgorithm;
                }
                namespace Testing {
                    class ConformityTestFramework;
                }
            }
            namespace _2020 {    // Version: 2020 (previous year)
                // IEEE 802.1AS-2020 implementation
            }
            namespace _2011 {    // Version: 2011 (legacy year)
                // IEEE 802.1AS-2011 legacy implementation
            }
        }
        namespace Q {            // Subpart: Q (VLAN and QoS)
            namespace _2018 {    // Version: 2018
                // IEEE 802.1Q-2018 implementation
            }
        }
    }
    
    namespace _1722 {            // Standard: 1722 (AVTP)
        namespace _2016 {        // Version: 2016 (year)
            // IEEE 1722-2016 AVTP implementation
            namespace avtp {
                class AVTPPacket;
                class StreamDataHeader;
            }
            namespace formats {
                namespace aaf {
                    class AudioFormat;
                }
                namespace crf {
                    class ClockReferenceFormat;
                }
            }
        }
        namespace _2011 {        // Version: 2011 (legacy year)
            // IEEE 1722-2011 legacy implementation
        }
    }
    
    namespace _1722_1 {          // Standard: 1722.1 (dots→underscores)
        namespace _2021 {        // Version: 2021 (year)
            // IEEE 1722.1-2021 AVDECC implementation
            namespace aem {
                class EntityModel;
                class ConfigurationDescriptor;
            }
            namespace aecp {
                class EntityControlProtocol;
                class AEMCommand;
            }
            namespace acmp {
                class ConnectionManagementProtocol;
                class ConnectTXCommand;
            }
            namespace adp {
                class DiscoveryProtocol;
                class EntityAvailable;
            }
            namespace descriptors {
                class EntityDescriptor;
                class StreamInputDescriptor;
                class StreamOutputDescriptor;
            }
        }
        namespace _2013 {        // Version: 2013 (legacy year)
            // IEEE 1722.1-2013 legacy implementation
        }
        namespace _2011 {        // Version: 2011 (legacy year)
            // IEEE 1722.1-2011 legacy implementation
        }
    }
    
    namespace _1588 {            // Standard: 1588 (PTPv2)
        namespace _2019 {        // Version: 2019 (year)
            // IEEE 1588-2019 PTPv2 implementation
        }
    }
} // namespace IEEE

namespace AVnu {                 // Organization: AVnu Alliance
    namespace Milan {            // Standard: Milan professional audio
        namespace v1_2 {         // Version: v1.2 (version number)
            // Milan v1.2 extensions
            namespace discovery {
                class MilanDiscoveryExtensions;
            }
            namespace connection {
                class RedundantStreamPairs;
            }
        }
        namespace v1_1 {         // Version: v1.1 (legacy version)
            // Milan v1.1 legacy implementation
        }
    }
    namespace Profiles {         // Standard: Profiles
        namespace AVB {          // Subpart: AVB
            namespace _2_0 {     // Version: 2.0 (dots→underscores)
                // AVB Profile 2.0 implementation
            }
        }
    }
} // namespace AVnu

namespace AES {                  // Organization: Audio Engineering Society
    namespace AES67 {            // Standard: AES67 (audio-over-IP)
        namespace _2018 {        // Version: 2018 (year)
            class AudioOverIP;
        }
    }
    namespace AES70 {            // Standard: AES70 (device control)
        namespace _2021 {        // Version: 2021 (year)
            class DeviceControl;
        }
    }
} // namespace AES

namespace ITU {                  // Organization: ITU-T (if needed)
    namespace G {                // Standard: G-series recommendations
        namespace G8275_1 {      // Subpart: G.8275.1 (dots→underscores)
            namespace _2016 {    // Version: 2016 (year)
                // ITU-T G.8275.1-2016 telecom PTP
            }
        }
    }
} // namespace ITU

namespace Common {               // Cross-organization utilities
    namespace interfaces {
        // Hardware abstraction interfaces - ONLY in Common namespace
        class NetworkInterface;
        class TimerInterface;
        class ClockInterface;
    }
    namespace utils {
        // Cross-standard utilities
        class PacketParser;
        class CRCCalculator;
    }
    namespace testing {
        // Common testing frameworks
        class TestFrameworkBase;
    }
} // namespace Common
```

### File Naming Conventions Following Generic Pattern

**PATTERN**: `<Organization>/<Standard>/<Subpart>/<Version>/<functional_area>/<file_name>.<ext>`

```cpp
// ✅ CORRECT file naming following generic pattern

// IEEE Examples:
lib/Standards/IEEE/1722.1/2021/aem/entity_model.hpp                    // IEEE::_1722_1::_2021::aem
lib/Standards/IEEE/1722.1/2021/aem/entity_model.cpp
lib/Standards/IEEE/1722.1/2021/aecp/aem_command.hpp                    // IEEE::_1722_1::_2021::aecp
lib/Standards/IEEE/1722.1/2021/aecp/aem_command.cpp
lib/Standards/IEEE/1722.1/2021/descriptors/entity_descriptor.hpp       // IEEE::_1722_1::_2021::descriptors

lib/Standards/IEEE/802.1/AS/2021/core/conformity_test_framework.h       // IEEE::_802_1::AS::_2021::core
lib/Standards/IEEE/802.1/AS/2021/core/gptp_state_machine.hpp           // IEEE::_802_1::AS::_2021::core
lib/Standards/IEEE/802.1/AS/2021/messages/sync_message.hpp             // IEEE::_802_1::AS::_2021::messages
lib/Standards/IEEE/802.1/AS/2021/messages/sync_message.cpp
lib/Standards/IEEE/802.1/AS/2021/clock/best_master_clock.hpp           // IEEE::_802_1::AS::_2021::clock

lib/Standards/IEEE/1722/2016/avtp/avtp_packet.hpp                      // IEEE::_1722::_2016::avtp
lib/Standards/IEEE/1722/2016/avtp/avtp_packet.cpp
lib/Standards/IEEE/1722/2016/formats/aaf/audio_format.hpp              // IEEE::_1722::_2016::formats::aaf
lib/Standards/IEEE/1722/2016/formats/crf/clock_reference.hpp           // IEEE::_1722::_2016::formats::crf

lib/Standards/IEEE/1588/2019/core/ptp_state_machine.hpp                // IEEE::_1588::_2019::core
lib/Standards/IEEE/802.1/Q/2018/vlan/vlan_management.hpp               // IEEE::_802_1::Q::_2018::vlan

// AVnu Examples:
lib/Standards/AVnu/Milan/v1.2/discovery/milan_discovery.hpp            // AVnu::Milan::v1_2::discovery
lib/Standards/AVnu/Milan/v1.2/discovery/milan_discovery.cpp
lib/Standards/AVnu/Milan/v1.2/connection/redundant_streams.hpp         // AVnu::Milan::v1_2::connection
lib/Standards/AVnu/Profiles/AVB/2.0/interop/profile_validation.hpp     // AVnu::Profiles::AVB::_2_0::interop

// AES Examples:
lib/Standards/AES/AES67/2018/audio/audio_over_ip.hpp                   // AES::AES67::_2018::audio
lib/Standards/AES/AES67/2018/audio/audio_over_ip.cpp
lib/Standards/AES/AES70/2021/control/device_control.hpp                // AES::AES70::_2021::control

// ITU Examples (if needed):
lib/Standards/ITU/G/G.8275.1/2016/telecom/telecom_ptp.hpp              // ITU::G::G8275_1::_2016::telecom

// Common utilities (organization-agnostic):
lib/Standards/Common/interfaces/network_interface.hpp                   // Common::interfaces
lib/Standards/Common/utils/packet_parser.hpp                           // Common::utils
lib/Standards/Common/testing/test_framework_base.hpp                    // Common::testing
```

### Header Guard and Include Conventions Following Actual Pattern
```cpp
// ✅ CORRECT header guards following IEEE namespace structure
#ifndef IEEE_1722_1_2021_AEM_ENTITY_MODEL_H
#define IEEE_1722_1_2021_AEM_ENTITY_MODEL_H

// ✅ CORRECT include structure - relative paths from IEEE namespace
#include "../descriptors/entity_descriptor.h"
#include "../../../../Common/interfaces/network_interface.h"

namespace IEEE {
namespace _1722_1 {
namespace _2021 {
namespace aem {

class EntityModel {
    // IEEE 1722.1-2021 compliant implementation
};

} // namespace aem
} // namespace _2021
} // namespace _1722_1
} // namespace IEEE

#endif // IEEE_1722_1_2021_AEM_ENTITY_MODEL_H
```

### Correct Include Patterns for Cross-Standard Dependencies
```cpp
// ✅ CORRECT - IEEE standards can reference each other
#include "../../1722/2016/avtp/avtp_packet.h"        // AVDECC using AVTP
#include "../../802.1/AS/2021/core/time_sync.h"      // AVDECC using gPTP

// ✅ CORRECT - Common utilities accessible to all standards
#include "../../../Common/interfaces/network_interface.h"
#include "../../../Common/utils/packet_parser.h"

// ❌ WRONG - No hardware-specific includes in IEEE namespace
// #include "../../../../../intel_avb/include/intel_hal.h"  // NO!
// #include "../../../../../common/hal/network_hal.h"       // NO!
```

### Cross-Standard Reuse and Dependencies

**MANDATORY RULE**: When an IEEE standard references or builds upon another IEEE standard, **ALWAYS reuse the existing implementation** rather than creating redundant code.

#### Examples of Required Cross-Standard Reuse:

**IEEE 1722.1 (AVDECC) Dependencies:**
```cpp
namespace IEEE {
namespace _1722_1 {
namespace _2021 {
namespace aecp {

// ✅ CORRECT - Reuse IEEE 1722 AVTP implementation
#include "../../../1722/2016/avtp/avtp_packet.h"
using IEEE::_1722::_2016::avtp::AVTPPacket;

// ✅ CORRECT - Reuse IEEE 802.1AS time synchronization
#include "../../../802.1/AS/2021/core/time_sync.h"
using IEEE::_802_1::AS::_2021::core::TimeSynchronization;

class AEMCommand {
    // AVDECC commands are transported over AVTP
    IEEE::_1722::_2016::avtp::AVTPPacket create_avtp_packet() {
        // Reuse AVTP implementation, don't reimplement
        return IEEE::_1722::_2016::avtp::AVTPPacket::create_aecp_packet();
    }
    
    // AVDECC requires synchronized time from gPTP
    uint64_t get_synchronized_time() {
        // Reuse gPTP time, don't reimplement time sync
        return IEEE::_802_1::AS::_2021::core::TimeSynchronization::get_current_time();
    }
};

} // namespace aecp
} // namespace _2021
} // namespace _1722_1
} // namespace IEEE
```

**IEEE 1722 (AVTP) Dependencies:**
```cpp
namespace IEEE {
namespace _1722 {
namespace _2016 {
namespace avtp {

// ✅ CORRECT - Reuse IEEE 802.1AS timing for presentation time
#include "../../../802.1/AS/2021/core/time_sync.h"

class StreamDataHeader {
    uint64_t calculate_presentation_time(uint32_t delay_ns) {
        // Reuse gPTP synchronized time, don't reimplement
        auto current_time = IEEE::_802_1::AS::_2021::core::TimeSynchronization::get_current_time();
        return current_time + delay_ns;
    }
};

} // namespace avtp
} // namespace _2016
} // namespace _1722
} // namespace IEEE
```

**Milan Extensions Dependencies:**
```cpp
namespace AVnu {
namespace Milan {
namespace v1_2 {
namespace discovery {

// ✅ CORRECT - Milan builds on IEEE 1722.1, reuse implementation
#include "../../../../IEEE/1722.1/2021/adp/discovery_protocol.h"
#include "../../../../IEEE/1722.1/2021/aem/entity_model.h"

class MilanDiscoveryExtensions : public IEEE::_1722_1::_2021::adp::DiscoveryProtocol {
    // Milan extends IEEE 1722.1 AVDECC, inherit don't reimplement
public:
    // Milan-specific discovery features
    void discover_milan_devices() {
        // Use base IEEE 1722.1 discovery, add Milan extensions
        DiscoveryProtocol::discover_devices();
        apply_milan_filtering();
    }
    
private:
    void apply_milan_filtering() {
        // Milan-specific logic only
    }
};

} // namespace discovery
} // namespace v1_2
} // namespace Milan
} // namespace AVnu
```

#### Forbidden Redundant Implementations:
```cpp
// ❌ WRONG - Reimplementing existing IEEE standards
namespace IEEE {
namespace _1722_1 {
namespace _2021 {

// DON'T DO THIS - AVTP already exists in IEEE::_1722
class AVDECCTransportPacket {  // NO - use IEEE::_1722::_2016::avtp::AVTPPacket
    // ... redundant AVTP implementation
};

// DON'T DO THIS - gPTP already exists in IEEE::_802_1::AS
class AVDECCTimeSync {  // NO - use IEEE::_802_1::AS::_2021::core::TimeSynchronization
    // ... redundant time sync implementation
};

} // namespace _2021
} // namespace _1722_1
} // namespace IEEE
```

#### Cross-Standard Dependency Rules:
1. **IEEE Layering Hierarchy** (higher layers depend on lower layers):
   - **Application Layer**: IEEE 1722.1 (AVDECC)
   - **Transport Layer**: IEEE 1722 (AVTP) 
   - **Timing Layer**: IEEE 802.1AS (gPTP)
   - **Network Layer**: IEEE 802.1Q (VLAN/QoS)

2. **Dependency Direction**: 
   - ✅ **IEEE 1722.1 CAN depend on IEEE 1722 and IEEE 802.1AS**
   - ✅ **IEEE 1722 CAN depend on IEEE 802.1AS**
   - ❌ **IEEE 802.1AS CANNOT depend on IEEE 1722 or IEEE 1722.1**

3. **Extension Standards**:
   - ✅ **Milan CAN depend on any IEEE standard it extends**
   - ✅ **AES67 CAN depend on IEEE standards it references**
   - ✅ **Multiple standard versions CAN coexist** (2021, 2016, 2013)

#### CMake Dependencies for Cross-Standard Reuse:
```cmake
# ✅ CORRECT - Respect IEEE layering in CMake dependencies
target_link_libraries(ieee_1722_1_2021
    ieee_1722_2016               # AVDECC depends on AVTP
    ieee_802_1_as_2021          # AVDECC depends on gPTP
    standards_common            # All can use Common utilities
)

target_link_libraries(ieee_1722_2016
    ieee_802_1_as_2021          # AVTP depends on gPTP for timing
    standards_common
)

target_link_libraries(avnu_milan_v12
    ieee_1722_1_2021            # Milan extends IEEE 1722.1
    ieee_1722_2016              # Milan may use AVTP directly
    ieee_802_1_as_2021          # Milan requires precise timing
    standards_common
)

# ❌ WRONG - Violates layering hierarchy
# target_link_libraries(ieee_802_1_as_2021
#     ieee_1722_2016            # NO - gPTP cannot depend on AVTP
# )
```

This approach ensures:
- **No code duplication** across IEEE standards
- **Proper architectural layering** following IEEE specifications  
- **Consistent behavior** when standards reference each other
- **Maintainable codebase** with single source of truth for each protocol feature

### Forbidden Namespace Violations - Corrected
```cpp
// ❌ WRONG - mixing standards with hardware (corrected understanding)
namespace IEEE {
namespace intel {              // NO - hardware vendor in IEEE namespace
    class IntelAVBInterface;
}
}

// ❌ WRONG - OS-specific namespaces in IEEE standards
namespace IEEE {
namespace windows {            // NO - OS specific in IEEE namespace
    class WinSockInterface;
}
}

// ❌ WRONG - implementation details in IEEE namespace  
namespace IEEE {
namespace _1722_1 {
namespace _2021 {
    class WindowsSocketImpl;  // NO - implementation detail, not protocol
}
}
}

// ✅ CORRECT - IEEE standards are pure protocol implementations
namespace IEEE {
namespace _1722_1 {
namespace _2021 {
namespace aem {
    class EntityModel;        // YES - pure IEEE 1722.1-2021 protocol
}
}
}
}
```

### CMake Integration with Correct Structure
```cmake
# ✅ CORRECT CMake structure following actual IEEE hierarchy
add_library(ieee_802_1_as_2021 STATIC
    IEEE/802.1/AS/2021/core/gptp_state_machine.cpp
    IEEE/802.1/AS/2021/messages/sync_message.cpp
    IEEE/802.1/AS/2021/clock/best_master_clock.cpp
    IEEE/802.1/AS/2021/core/conformity_test_framework.cpp
)

add_library(ieee_1722_1_2021 STATIC
    IEEE/1722.1/2021/aem/entity_model.cpp
    IEEE/1722.1/2021/aecp/aem_command.cpp
    IEEE/1722.1/2021/descriptors/entity_descriptor.cpp
)

add_library(ieee_1722_2016 STATIC
    IEEE/1722/2016/avtp/avtp_packet.cpp
    IEEE/1722/2016/formats/aaf/audio_format.cpp
)

add_library(avnu_milan_v12 STATIC
    AVnu/Milan/v1.2/discovery/milan_discovery.cpp
    AVnu/Milan/v1.2/connection/redundant_streams.cpp
)

add_library(standards_common STATIC
    Common/interfaces/network_interface.cpp
    Common/utils/packet_parser.cpp
    Common/testing/test_framework_base.cpp
)

# IEEE standards libraries can depend on each other following IEEE layering
target_link_libraries(ieee_1722_1_2021
    ieee_1722_2016               # AVDECC depends on AVTP
    ieee_802_1_as_2021          # AVDECC depends on gPTP
    standards_common            # All can use Common utilities
)
```

### Documentation Structure Requirements - Corrected
```cpp
/**
 * @file conformity_test_framework.h
 * @brief IEEE 802.1AS-2021 Conformity Testing Framework
 * @namespace IEEE::_802_1::AS::_2021::Testing
 * 
 * Implements conformity testing according to IEEE 802.1AS-2021 specification.
 * This namespace contains all testing functionality for validating IEEE 802.1AS-2021
 * compliance including state machine behavior, timing requirements, and interoperability.
 * 
 * @see IEEE 802.1AS-2021, Clauses 11.2-11.5 "Conformance requirements"
 * @see IEEE 802.1AS-2021, Annex A "Implementation conformance statement (ICS)"
 */
```

### Enforcement Rules - Corrected with Copyright Compliance
1. **IEEE namespaces are top-level** - not wrapped in `openavnu::standards`
2. **Namespace must match folder structure** exactly (`IEEE::_802_1::AS::_2021` = `IEEE/802.1/AS/2021/`)
3. **Version numbers use underscores** in namespaces (`_2021`, `_2016`) to avoid conflicts
4. **Dots become underscores** in namespaces (`802.1` becomes `_802_1`, `1722.1` becomes `_1722_1`)
5. **No hardware vendors** in IEEE namespace hierarchy
6. **No OS-specific namespaces** in IEEE standards
7. **IEEE layering respected** - higher layer standards can depend on lower layers
8. **Cross-standard utilities** only in Common namespace
9. **Conformance testing** isolated in Testing sub-namespaces
10. **COPYRIGHT COMPLIANCE MANDATORY**:
    - **NEVER reproduce copyrighted specification content** in source code or documentation
    - **Reference specifications by section number only** (e.g., "IEEE 1722.1-2021, Section 7.2.1")
    - **Implement based on understanding**, not by copying specification text
    - **Use MCP-Server for compliance verification only**, not content reproduction
    - **Respect all copyright holders**: IEEE, AES, AVnu Alliance, ITU, etc.
    - **Include copyright disclaimer** in implementations referencing multiple standards
    - **Document original implementation** that achieves compliance through understanding

### Repository Copyright Policy
```cpp
// ✅ REQUIRED copyright notice for standards-based implementations
/*
 * This file implements protocol functionality based on understanding of:
 * - IEEE 1722.1-2021 (AVDECC) - Copyright IEEE
 * - IEEE 1722-2016 (AVTP) - Copyright IEEE  
 * - IEEE 802.1AS-2021 (gPTP) - Copyright IEEE
 * - Milan v1.2 - Copyright AVnu Alliance
 * 
 * No copyrighted content from these specifications is reproduced.
 * Implementation is original work achieving compliance through
 * understanding of specification requirements.
 * 
 * For authoritative requirements, refer to original specifications
 * available from respective standards organizations.
 */
```

This structure ensures clear separation of IEEE standards versions, prevents architectural violations, and maintains the hardware-agnostic principle while following the actual implementation pattern used in the codebase.

This architecture ensures the Standards layer remains pure, testable, reusable across different hardware platforms, and maintains strict IEEE compliance while following OpenAvnu's core development principles.

## ⚠️ MANDATORY: YAML Front Matter Schema Compliance

**CRITICAL ENFORCEMENT**: All specification files MUST use EXACT YAML front matter format per authoritative schemas.

**Authoritative Schema Sources** (SINGLE SOURCE OF TRUTH):
- Requirements: `spec-kit-templates/schemas/requirements-spec.schema.json`  
- Architecture: `spec-kit-templates/schemas/architecture-spec.schema.json`
- Design: `spec-kit-templates/schemas/ieee-design-spec.schema.json`
- Phase Gates: `spec-kit-templates/schemas/phase-gate-validation.schema.json`

**ZERO TOLERANCE POLICY**: 
- ❌ DO NOT modify schemas to fit incorrect front matter
- ❌ DO NOT use alternative YAML formats  
- ❌ DO NOT use full standard names where schemas expect short patterns
- ✅ ALWAYS reference authoritative schema files for format
- ✅ ALWAYS validate against schemas before submitting
- ✅ ALWAYS use phase-specific copilot-instructions for examples

**CI ENFORCEMENT**: Validation failures will block CI pipeline and prevent merges. There are no exceptions to schema compliance.