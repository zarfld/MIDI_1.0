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
├── MMA/                       # MIDI Manufacturers Association
│   ├── MIDI/                  # MIDI standards family
│   │   └── 1_0/               # MIDI 1.0 version
│   │       ├── v1996/         # MIDI 1.0 Detailed Specification (1996)
│   │       │   ├── core/      # Core MIDI protocol
│   │       │   ├── channel_voice/    # Channel Voice Messages
│   │       │   ├── channel_mode/     # Channel Mode Messages
│   │       │   ├── system_common/    # System Common Messages
│   │       │   ├── system_realtime/  # System Real-Time Messages
│   │       │   ├── sysex/     # System Exclusive Messages
│   │       │   └── conformity/ # MIDI conformance testing
│   │       └── extensions/    # MIDI 1.0 Extensions
│   │           ├── mtc/       # MIDI Time Code
│   │           ├── msc/       # MIDI Show Control
│   │           ├── mmc/       # MIDI Machine Control
│   │           ├── mpe/       # MIDI Polyphonic Expression
│   │           ├── tuning/    # MIDI Tuning Specification
│   │           └── controllers/ # Extended Controller Definitions
│   ├── General_MIDI/          # General MIDI specifications
│   │   ├── Level_1/           # General MIDI Level 1
│   │   ├── Level_2/           # General MIDI Level 2
│   │   └── Lite/              # General MIDI Lite
│   └── Addenda/               # MIDI 1.0 Addenda and Recommended Practices
│       ├── rp015/             # Response to Reset All Controllers
│       ├── rp018/             # Response to Data Inc/Dec Controllers
│       ├── rp021/             # Sound Controller Defaults
│       ├── rp023/             # Renaming of CC91 and CC93
│       ├── ca018/             # File Reference SysEx
│       └── ca031/             # High Resolution Velocity Prefix
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

**NAMESPACE TRANSLATION RULE**: `<Organization>::<Standard>::<Version>::<Subpart>`

**Examples of Translation:**
- Folder: `MMA/MIDI/1_0/v1996/` → Namespace: `MMA::MIDI::_1_0::v1996`
- Folder: `MMA/MIDI/1_0/extensions/mtc/` → Namespace: `MMA::MIDI::_1_0::extensions::mtc`
- Folder: `MMA/General_MIDI/Level_1/` → Namespace: `MMA::General_MIDI::Level_1`

```cpp
// ✅ CORRECT namespace hierarchy following generic pattern
namespace MMA {
    namespace MIDI {             // Standard: MIDI
        namespace _1_0 {         // Version: 1_0 (underscores for dots)
            namespace v1996 {    // Specification revision: v1996
                // MIDI 1.0 v1996 core implementation
                namespace core {
                    class MIDIParser;
                    class MessageValidator;
                }
                namespace channel_voice {
                    struct NoteOnMessage;
                    struct NoteOffMessage;
                    struct ControlChangeMessage;
                }
                namespace channel_mode {
                    class ChannelModeHandler;
                }
                namespace system_common {
                    class SysExParser;
                    struct MTCQuarterFrame;
                }
                namespace system_realtime {
                    class ClockHandler;
                    struct TimingClock;
                }
                namespace conformity {
                    class ConformanceTestFramework;
                }
            }
            namespace extensions {    // Extensions to MIDI 1.0
                namespace mtc {       // MIDI Time Code
                    class MTCHandler;
                }
                namespace msc {       // MIDI Show Control
                    class MSCCommandParser;
                }
                namespace mmc {       // MIDI Machine Control
                    class MMCTransportControl;
                }
                namespace mpe {       // MIDI Polyphonic Expression
                    class MPEMessageRouter;
                }
            }
        } // namespace _1_0
    } // namespace MIDI
    
    namespace General_MIDI {     // General MIDI specifications
        namespace Level_1 {      // GM Level 1
            class SoundSet;
            class DrumKit;
        }
        namespace Level_2 {      // GM Level 2
            class ExtendedSoundSet;
            class ExtendedControllers;
        }
        namespace Lite {         // GM Lite
            class MinimalSoundSet;
        }
    } // namespace General_MIDI
    
    namespace Addenda {          // MIDI 1.0 Addenda
        namespace rp015 {        // Reset All Controllers
            class ResetAllControllersHandler;
        }
        namespace rp018 {        // Data Inc/Dec Controllers
            class DataIncDecHandler;
        }
    } // namespace Addenda
} // namespace MMA

namespace Common {               // Cross-organization utilities
    namespace interfaces {
        // Hardware abstraction interfaces - ONLY in Common namespace
        class MIDIInterface;
        class TimerInterface;
        class ClockInterface;
    }
    namespace utils {
        // Cross-standard utilities
        class ByteParser;
        class ChecksumCalculator;
    }
    namespace testing {
        // Common testing frameworks
        class TestFrameworkBase;
    }
} // namespace Common
```

### File Naming Conventions Following Generic Pattern

**PATTERN**: `<Organization>/<Standard>/<Version>/<Subpart>/<functional_area>/<file_name>.<ext>`

```cpp
// ✅ CORRECT file naming following generic pattern

// MIDI Examples:
lib/Standards/MMA/MIDI/1_0/v1996/core/midi_parser.hpp                  // MMA::MIDI::_1_0::v1996::core
lib/Standards/MMA/MIDI/1_0/v1996/core/midi_parser.cpp
lib/Standards/MMA/MIDI/1_0/v1996/core/message_validator.hpp            // MMA::MIDI::_1_0::v1996::core
lib/Standards/MMA/MIDI/1_0/v1996/core/message_validator.cpp

lib/Standards/MMA/MIDI/1_0/v1996/channel_voice/note_on_message.hpp    // MMA::MIDI::_1_0::v1996::channel_voice
lib/Standards/MMA/MIDI/1_0/v1996/channel_voice/note_on_message.cpp
lib/Standards/MMA/MIDI/1_0/v1996/channel_voice/control_change.hpp     // MMA::MIDI::_1_0::v1996::channel_voice
lib/Standards/MMA/MIDI/1_0/v1996/channel_voice/control_change.cpp

lib/Standards/MMA/MIDI/1_0/v1996/system_common/sysex_parser.hpp        // MMA::MIDI::_1_0::v1996::system_common
lib/Standards/MMA/MIDI/1_0/v1996/system_common/sysex_parser.cpp
lib/Standards/MMA/MIDI/1_0/v1996/system_realtime/clock_handler.hpp     // MMA::MIDI::_1_0::v1996::system_realtime

lib/Standards/MMA/MIDI/1_0/v1996/conformity/conformance_tests.hpp      // MMA::MIDI::_1_0::v1996::conformity
lib/Standards/MMA/MIDI/1_0/v1996/conformity/conformance_tests.cpp

// MIDI Extensions:
lib/Standards/MMA/MIDI/1_0/extensions/mtc/mtc_handler.hpp              // MMA::MIDI::_1_0::extensions::mtc
lib/Standards/MMA/MIDI/1_0/extensions/mtc/mtc_handler.cpp
lib/Standards/MMA/MIDI/1_0/extensions/msc/msc_command_parser.hpp       // MMA::MIDI::_1_0::extensions::msc
lib/Standards/MMA/MIDI/1_0/extensions/mpe/mpe_message_router.hpp       // MMA::MIDI::_1_0::extensions::mpe

// General MIDI:
lib/Standards/MMA/General_MIDI/Level_1/sound_set.hpp                   // MMA::General_MIDI::Level_1
lib/Standards/MMA/General_MIDI/Level_1/sound_set.cpp
lib/Standards/MMA/General_MIDI/Level_2/extended_sound_set.hpp          // MMA::General_MIDI::Level_2
lib/Standards/MMA/General_MIDI/Lite/minimal_sound_set.hpp              // MMA::General_MIDI::Lite

// MIDI Addenda:
lib/Standards/MMA/Addenda/rp015/reset_all_controllers.hpp              // MMA::Addenda::rp015
lib/Standards/MMA/Addenda/rp018/data_inc_dec_handler.hpp               // MMA::Addenda::rp018

// Common utilities (organization-agnostic):
lib/Standards/Common/interfaces/midi_interface.hpp                      // Common::interfaces
lib/Standards/Common/utils/byte_parser.hpp                             // Common::utils
lib/Standards/Common/testing/test_framework_base.hpp                    // Common::testing
```

### Header Guard and Include Conventions Following Actual Pattern
```cpp
// ✅ CORRECT header guards following MMA namespace structure
#ifndef MMA_MIDI_1_0_V1996_CORE_MIDI_PARSER_H
#define MMA_MIDI_1_0_V1996_CORE_MIDI_PARSER_H

// ✅ CORRECT include structure - relative paths from MMA namespace
#include "../channel_voice/note_on_message.h"
#include "../../../../Common/interfaces/midi_interface.h"

namespace MMA {
namespace MIDI {
namespace _1_0 {
namespace v1996 {
namespace core {

class MIDIParser {
    // MIDI 1.0 v1996 compliant implementation
};

} // namespace core
} // namespace v1996
} // namespace _1_0
} // namespace MIDI
} // namespace MMA

#endif // MMA_MIDI_1_0_V1996_CORE_MIDI_PARSER_H
```

### Correct Include Patterns for MIDI Dependencies
```cpp
// ✅ CORRECT - MIDI components can reference each other within same version
#include "../channel_voice/note_on_message.h"        // Core using channel voice
#include "../system_common/sysex_parser.h"           // Core using sysex

// ✅ CORRECT - Extensions can use core MIDI
#include "../../v1996/core/midi_parser.h"            // MTC using core parser

// ✅ CORRECT - Common utilities accessible to all standards
#include "../../../Common/interfaces/midi_interface.h"
#include "../../../Common/utils/byte_parser.h"

// ❌ WRONG - No hardware-specific includes in MMA namespace
// #include "../../../../../roland_hal/include/midi_hal.h"  // NO!
// #include "../../../../../common/hal/midi_device_hal.h"   // NO!
```

### MIDI Internal Reuse and Dependencies

**MANDATORY RULE**: MIDI 1.0 is a self-contained standard. Reuse components within the MIDI namespace rather than duplicating code.

#### Examples of MIDI Internal Reuse:

**MIDI Extensions Reusing Core:**
```cpp
namespace MMA {
namespace MIDI {
namespace _1_0 {
namespace extensions {
namespace mtc {

// ✅ CORRECT - MTC extension reuses core MIDI parser
#include "../../v1996/core/midi_parser.h"
using MMA::MIDI::_1_0::v1996::core::MIDIParser;

class MTCHandler {
    MIDIParser parser;  // Reuse core parser, don't reimplement
    
    // MTC-specific Quarter Frame parsing
    void parse_quarter_frame(uint8_t data) {
        // MTC-specific logic only
        // Use core parser for basic validation
    }
};

} // namespace mtc
} // namespace extensions
} // namespace _1_0
} // namespace MIDI
} // namespace MMA
```

**General MIDI Reusing Core Messages:**
```cpp
namespace MMA {
namespace General_MIDI {
namespace Level_1 {

// ✅ CORRECT - GM reuses MIDI 1.0 core messages
#include "../../MIDI/1_0/v1996/channel_voice/control_change.h"
#include "../../MIDI/1_0/v1996/channel_voice/program_change.h"

class SoundSet {
    // GM builds on MIDI 1.0, reuse message structures
    void set_sound(uint8_t channel, uint8_t program) {
        // Use MIDI 1.0 Program Change message
        MMA::MIDI::_1_0::v1996::channel_voice::ProgramChange msg;
        msg.channel = channel;
        msg.program = program;
        // ... send via interface
    }
};

} // namespace Level_1
} // namespace General_MIDI
} // namespace MMA
```

#### Forbidden Redundant Implementations:
```cpp
// ❌ WRONG - Reimplementing existing MIDI core
namespace MMA {
namespace MIDI {
namespace _1_0 {
namespace extensions {
namespace mtc {

// DON'T DO THIS - Parser already exists in core
class MTCMIDIParser {  // NO - use MMA::MIDI::_1_0::v1996::core::MIDIParser
    // ... redundant parser implementation
};

} // namespace mtc
} // namespace extensions
} // namespace _1_0
} // namespace MIDI
} // namespace MMA
```

#### MIDI Dependency Rules:
1. **MIDI Layering**:
   - **Core Layer**: MIDI 1.0 v1996 core protocol (message parsing, validation)
   - **Extension Layer**: MTC, MSC, MMC, MPE (build on core)
   - **Profile Layer**: General MIDI (uses core messages with specific constraints)

2. **Dependency Direction**: 
   - ✅ **Extensions CAN depend on MIDI 1.0 core**
   - ✅ **General MIDI CAN depend on MIDI 1.0 core**
   - ✅ **Addenda CAN reference and extend core**
   - ❌ **Core CANNOT depend on extensions or profiles**

3. **Component Reuse**:
   - ✅ **All MIDI components share Common utilities** (byte parsing, buffer management)
   - ✅ **Extensions reuse core message structures**
   - ✅ **Multiple MIDI versions CAN coexist** (for legacy support)

#### CMake Dependencies for MIDI Reuse:
```cmake
# ✅ CORRECT - MIDI extensions depend on core
target_link_libraries(mma_midi_1_0_mtc
    mma_midi_1_0_core           # MTC depends on core protocol
    standards_common            # All can use Common utilities
)

target_link_libraries(mma_midi_1_0_msc
    mma_midi_1_0_core           # MSC depends on core protocol
    standards_common
)

target_link_libraries(mma_general_midi_level_1
    mma_midi_1_0_core           # GM uses core messages
    standards_common
)

# ❌ WRONG - Core cannot depend on extensions
# target_link_libraries(mma_midi_1_0_core
#     mma_midi_1_0_mtc          # NO - Core cannot depend on MTC
# )
```

This approach ensures:
- **No code duplication** across MIDI components
- **Proper layering** following MIDI specification structure
- **Consistent behavior** when extensions use core functionality
- **Maintainable codebase** with single source of truth for protocol features

### Forbidden Namespace Violations
```cpp
// ❌ WRONG - mixing MIDI standards with hardware
namespace MMA {
namespace roland {             // NO - hardware vendor in MMA namespace
    class RolandMIDIInterface;
}
}

// ❌ WRONG - OS-specific namespaces in MIDI standards
namespace MMA {
namespace windows {            // NO - OS specific in MMA namespace
    class WinMMInterface;
}
}

// ❌ WRONG - implementation details in MMA namespace  
namespace MMA {
namespace MIDI {
namespace _1_0 {
    class WindowsSerialImpl;  // NO - implementation detail, not protocol
}
}
}

// ✅ CORRECT - MMA standards are pure protocol implementations
namespace MMA {
namespace MIDI {
namespace _1_0 {
namespace v1996 {
namespace core {
    class MIDIParser;         // YES - pure MIDI 1.0 protocol
}
}
}
}
}
```

### CMake Integration with Correct Structure
```cmake
# ✅ CORRECT CMake structure following MMA MIDI hierarchy
add_library(mma_midi_1_0_core STATIC
    MMA/MIDI/1_0/v1996/core/midi_parser.cpp
    MMA/MIDI/1_0/v1996/core/message_validator.cpp
    MMA/MIDI/1_0/v1996/channel_voice/note_on_message.cpp
    MMA/MIDI/1_0/v1996/channel_voice/control_change.cpp
    MMA/MIDI/1_0/v1996/system_common/sysex_parser.cpp
)

add_library(mma_midi_1_0_mtc STATIC
    MMA/MIDI/1_0/extensions/mtc/mtc_handler.cpp
)

add_library(mma_midi_1_0_msc STATIC
    MMA/MIDI/1_0/extensions/msc/msc_command_parser.cpp
)

add_library(mma_general_midi_level_1 STATIC
    MMA/General_MIDI/Level_1/sound_set.cpp
    MMA/General_MIDI/Level_1/drum_kit.cpp
)

add_library(standards_common STATIC
    Common/interfaces/midi_interface.cpp
    Common/utils/byte_parser.cpp
    Common/testing/test_framework_base.cpp
)

# MIDI extensions depend on core
target_link_libraries(mma_midi_1_0_mtc
    mma_midi_1_0_core           # MTC depends on core protocol
    standards_common            # All can use Common utilities
)
```

### Documentation Structure Requirements - Corrected
```cpp
/**
 * @file conformity_test_framework.h
 * @brief MIDI 1.0 Conformity Testing Framework
 * @namespace MMA::MIDI::_1_0::v1996::conformity
 * 
 * Implements conformity testing according to MIDI 1.0 Detailed Specification.
 * This namespace contains all testing functionality for validating MIDI 1.0
 * compliance including message parsing, protocol validation, and interoperability.
 * 
 * @see MIDI 1.0 Detailed Specification, Table 1 "Channel Voice Messages"
 * @see MIDI 1.0 Detailed Specification, Appendix A "Status Byte Summary"
 */
```

### Enforcement Rules - Corrected with Copyright Compliance
1. **MMA namespaces are top-level** - organized by standard organization
2. **Namespace must match folder structure** exactly (`MMA::MIDI::_1_0::v1996` = `MMA/MIDI/1_0/v1996/`)
3. **Version numbers use underscores** in namespaces (`_1_0`, `_2_0`) to avoid conflicts
4. **Dots become underscores** in namespaces (`1.0` becomes `_1_0`)
5. **No hardware vendors** in MMA namespace hierarchy
6. **No OS-specific namespaces** in MIDI standards
7. **MIDI layering respected** - extensions/profiles can depend on core, but not vice versa
8. **Cross-standard utilities** only in Common namespace
9. **Conformance testing** isolated in conformity sub-namespaces
10. **COPYRIGHT COMPLIANCE MANDATORY**:
    - **NEVER reproduce copyrighted specification content** in source code or documentation
    - **Reference specifications by section number only** (e.g., "MIDI 1.0 Detailed Specification, Table 1")
    - **Implement based on understanding**, not by copying specification text
    - **Use MCP-Server for compliance verification only**, not content reproduction
    - **Respect all copyright holders**: MMA (MIDI Manufacturers Association)
    - **Include copyright disclaimer** in implementations referencing MIDI standards
    - **Document original implementation** that achieves compliance through understanding

### Repository Copyright Policy
```cpp
// ✅ REQUIRED copyright notice for standards-based implementations
/*
 * This file implements protocol functionality based on understanding of:
 * - MIDI 1.0 Detailed Specification (1996 revision) - Copyright MMA
 * - MIDI Time Code Specification - Copyright MMA
 * - MIDI Show Control Specification - Copyright MMA
 * - General MIDI Level 1 Specification - Copyright MMA
 * 
 * No copyrighted content from these specifications is reproduced.
 * Implementation is original work achieving compliance through
 * understanding of specification requirements.
 * 
 * For authoritative requirements, refer to original specifications
 * available from MIDI Manufacturers Association.
 */
```

This structure ensures clear separation of MIDI standards versions, prevents architectural violations, and maintains the hardware-agnostic principle while following standards-compliant development practices.

This architecture ensures the Standards layer remains pure, testable, reusable across different hardware platforms, and maintains strict MIDI 1.0 compliance while respecting MMA copyright.

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