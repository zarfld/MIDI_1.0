# REQ-F-GM1-003: GM1 Program Change Processing and Channel Assignment

## Metadata
- **ID**: REQ-F-GM1-003
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Priority**: High
- **Status**: Draft
- **Specification**: GM1 (General MIDI System Level 1)
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (pending GitHub issue creation)
- **Depends on**: MIDI 1.0 Program Change (Cn), #537 (GM1 melodic instruments), #538 (GM1 percussion)
- **Verified by**: TBD (TEST-GM1-PROGRAM-CHANGE-*)

## Source Specification
**STR-GM1-Level-1** (RP-003: General MIDI System Level 1 Specification):
- Program Change messages (Cn pp) set melodic channel instruments
- 15 melodic channels: 1-9, 11-16 (Channel 10 reserved for percussion)
- Each channel can use different instrument from 128 GM programs
- Dynamic channel assignment (polyphony not fixed per channel)

## Description
Process MIDI Program Change messages to assign GM1 instruments to melodic channels. Support independent program selection on 15 channels (1-9, 11-16) while preserving Channel 10 for percussion. Ensure proper program validation, default initialization, and channel-specific instrument state management.

## Channel Configuration

### Melodic Channels (15 total)
```
Channel 1 (MIDI 0): Full GM1 access (Programs 1-128)
Channel 2 (MIDI 1): Full GM1 access (Programs 1-128)
Channel 3 (MIDI 2): Full GM1 access (Programs 1-128)
Channel 4 (MIDI 3): Full GM1 access (Programs 1-128)
Channel 5 (MIDI 4): Full GM1 access (Programs 1-128)
Channel 6 (MIDI 5): Full GM1 access (Programs 1-128)
Channel 7 (MIDI 6): Full GM1 access (Programs 1-128)
Channel 8 (MIDI 7): Full GM1 access (Programs 1-128)
Channel 9 (MIDI 8): Full GM1 access (Programs 1-128)
Channel 10 (MIDI 9): PERCUSSION ONLY (ignores Program Change)
Channel 11 (MIDI 10): Full GM1 access (Programs 1-128)
Channel 12 (MIDI 11): Full GM1 access (Programs 1-128)
Channel 13 (MIDI 12): Full GM1 access (Programs 1-128)
Channel 14 (MIDI 13): Full GM1 access (Programs 1-128)
Channel 15 (MIDI 14): Full GM1 access (Programs 1-128)
Channel 16 (MIDI 15): Full GM1 access (Programs 1-128)
```

### Default Program Assignment
```
GM Reset/Power-On state:
- All melodic channels (1-9, 11-16): Program 1 (Acoustic Grand Piano)
- Channel 10: GM1 Percussion Set (fixed, no Program Change)
- Total melodic channels: 15
- Total percussion channels: 1
```

## Program Change Processing

### Program Change Message Format
```
MIDI Program Change: Cn pp
- C: Program Change status (0xC0-0xCF)
- n: MIDI channel (0-15)
- pp: Program number (0-127, representing GM programs 1-128)

Example:
C0 00  → Channel 1, Program 1 (Acoustic Grand Piano)
C1 18  → Channel 2, Program 25 (Acoustic Guitar nylon)
C9 40  → Channel 10, Program 65 (IGNORED - percussion channel)
CA 38  → Channel 11, Program 57 (Trumpet)
```

### Implementation Logic
```c
typedef struct {
    uint8_t current_program;        // Current GM program (1-128)
    bool    is_percussion_channel;  // True for Channel 10
    uint8_t voice_count;           // Current active voices
} gm1_channel_state_t;

// Global channel state (16 channels)
gm1_channel_state_t gm1_channels[16];

void handle_program_change(uint8_t channel, uint8_t program) {
    // Validate channel (0-15)
    if (channel > 15) {
        return;  // Invalid channel
    }
    
    // Channel 10 (index 9) ignores Program Change
    if (channel == 9) {
        // Channel 10 is percussion-only, ignore Program Change
        return;
    }
    
    // Validate program number (0-127 → GM programs 1-128)
    if (program > 127) {
        return;  // Invalid program
    }
    
    // Convert MIDI program (0-127) to GM program (1-128)
    uint8_t gm_program = program + 1;
    
    // Set channel program
    gm1_channels[channel].current_program = gm_program;
    
    // Load instrument samples/patches for new program
    load_gm1_instrument(channel, gm_program);
    
    // All active notes on channel should switch to new instrument
    update_channel_voices(channel, gm_program);
}
```

### Channel State Management
```c
void initialize_gm1_channels(void) {
    for (int i = 0; i < 16; i++) {
        if (i == 9) {  // Channel 10 (0-indexed as 9)
            gm1_channels[i].current_program = 0;  // No program (percussion)
            gm1_channels[i].is_percussion_channel = true;
        } else {
            gm1_channels[i].current_program = 1;  // Default: Acoustic Grand Piano
            gm1_channels[i].is_percussion_channel = false;
        }
        gm1_channels[i].voice_count = 0;
    }
}

uint8_t get_channel_program(uint8_t channel) {
    if (channel > 15) return 0;  // Invalid
    return gm1_channels[channel].current_program;
}

bool is_percussion_channel(uint8_t channel) {
    if (channel > 15) return false;
    return gm1_channels[channel].is_percussion_channel;
}
```

## Multi-Timbral Operation

### Independent Channel Programs
```
GM1 supports multi-timbral operation:
- Each melodic channel has independent program assignment
- Channels can play different instruments simultaneously
- Example setup:
  Channel 1: Program 1 (Acoustic Grand Piano)
  Channel 2: Program 25 (Acoustic Guitar nylon)
  Channel 3: Program 57 (Trumpet)
  Channel 4: Program 33 (Acoustic Bass)
  Channel 10: GM1 Percussion Set
  
- All channels can play simultaneously with different timbres
```

### Voice Sharing Across Channels
```c
void allocate_voice_for_channel(uint8_t channel, uint8_t note, uint8_t velocity) {
    // Get current program for channel
    uint8_t program = get_channel_program(channel);
    
    if (is_percussion_channel(channel)) {
        // Channel 10: Allocate percussion voice
        allocate_percussion_voice(note, velocity);
    } else {
        // Melodic channels: Allocate voice with channel's current program
        voice_t* voice = allocate_melodic_voice();
        if (voice) {
            configure_voice(voice, channel, program, note, velocity);
            gm1_channels[channel].voice_count++;
        }
    }
}
```

## Program Change Examples

### Example 1: Basic Program Assignment
```
C0 00    ; Channel 1 → Program 1 (Acoustic Grand Piano)
C1 07    ; Channel 2 → Program 8 (Clavi)
C2 18    ; Channel 3 → Program 25 (Acoustic Guitar nylon)
C3 20    ; Channel 4 → Program 33 (Acoustic Bass)

Result:
- Channel 1: Piano sounds
- Channel 2: Clavinet sounds  
- Channel 3: Classical guitar sounds
- Channel 4: Acoustic bass sounds
- Channel 10: Still percussion (unchanged)
```

### Example 2: Program Change During Playback
```
C0 00    ; Channel 1 → Program 1 (Acoustic Grand Piano)
90 3C 64 ; Channel 1 Note On C4 (plays piano)
C0 38    ; Channel 1 → Program 57 (Trumpet) - switches instrument
90 40 64 ; Channel 1 Note On E4 (plays trumpet, not piano)
```

### Example 3: Multi-Channel Arrangement
```
C0 00    ; Channel 1 → Piano
C1 18    ; Channel 2 → Guitar
C2 20    ; Channel 3 → Bass  
C3 38    ; Channel 4 → Trumpet
C4 30    ; Channel 5 → Strings
CA 40    ; Channel 11 → Brass section

All channels can now play simultaneously with different instruments
```

### Example 4: Channel 10 Immunity
```
C9 00    ; Channel 10 → Program 1 (IGNORED)
C9 7F    ; Channel 10 → Program 128 (IGNORED)
99 24 64 ; Channel 10 Note On (still percussion - Bass Drum 1)

Channel 10 always percussion regardless of Program Change messages
```

## Error Handling

### Invalid Program Numbers
```c
void handle_program_change_with_validation(uint8_t channel, uint8_t program) {
    // Option 1: Clamp to valid range
    if (program > 127) {
        program = 127;  // Clamp to Program 128 (Gunshot)
    }
    
    // Option 2: Ignore invalid programs
    if (program > 127) {
        return;  // Ignore, keep current program
    }
    
    // Process valid program
    handle_program_change(channel, program);
}
```

### Channel Validation
```c
bool is_valid_midi_channel(uint8_t channel) {
    return (channel <= 15);  // MIDI channels 1-16 (0-15 indexed)
}

bool is_melodic_channel(uint8_t channel) {
    return (channel <= 15 && channel != 9);  // All except Channel 10
}
```

## Program Memory and Persistence

### Program State Persistence
```c
typedef struct {
    uint8_t melodic_programs[15];   // Programs for channels 1-9, 11-16
    bool    gm_mode_active;         // GM Mode on/off
} gm1_program_state_t;

void save_gm1_program_state(gm1_program_state_t* state) {
    int melodic_index = 0;
    for (int channel = 0; channel < 16; channel++) {
        if (channel != 9) {  // Skip Channel 10 (percussion)
            state->melodic_programs[melodic_index++] = gm1_channels[channel].current_program;
        }
    }
    state->gm_mode_active = true;
}

void restore_gm1_program_state(const gm1_program_state_t* state) {
    int melodic_index = 0;
    for (int channel = 0; channel < 16; channel++) {
        if (channel != 9) {  // Skip Channel 10 (percussion)
            uint8_t program = state->melodic_programs[melodic_index++];
            handle_program_change(channel, program - 1);  // Convert GM to MIDI program
        }
    }
}
```

## Acceptance Criteria

### AC1: Melodic Channel Program Assignment
**Given** Program Change 0-127 on melodic channels (1-9, 11-16)  
**When** processing Program Change  
**Then** channel assigned correct GM1 instrument (1-128)

**Test Method**: Program assignment test  
**Success Criteria**: Correct instrument per channel

### AC2: Channel 10 Program Change Immunity
**Given** Program Change on Channel 10  
**When** processing Program Change  
**Then** Channel 10 remains percussion (Program Change ignored)

**Test Method**: Channel 10 immunity test  
**Success Criteria**: No program change effect

### AC3: Independent Channel Operation
**Given** different programs on multiple channels  
**When** playing notes simultaneously  
**Then** each channel uses its assigned instrument

**Test Method**: Multi-channel test  
**Success Criteria**: Independent timbres

### AC4: Default Program Assignment
**Given** GM reset/initialization  
**When** no Program Change sent  
**Then** all melodic channels default to Program 1 (Piano)

**Test Method**: Default state test  
**Success Criteria**: All melodic channels Piano

### AC5: Program Range Validation
**Given** Program Change >127  
**When** processing invalid program  
**Then** ignore or clamp to valid range (no crash)

**Test Method**: Range validation test  
**Success Criteria**: Graceful handling

### AC6: Real-Time Program Switching
**Given** Program Change during active notes  
**When** switching programs  
**Then** new notes use new program, existing notes continue

**Test Method**: Real-time switching test  
**Success Criteria**: Smooth program transitions

### AC7: Multi-Timbral Capability
**Given** 15 melodic channels with different programs  
**When** playing simultaneously  
**Then** all programs accessible at same time

**Test Method**: Multi-timbral test  
**Success Criteria**: 15 simultaneous timbres

### AC8: Channel State Persistence
**Given** program assignments per channel  
**When** querying channel state  
**Then** return current program for each channel

**Test Method**: State query test  
**Success Criteria**: Correct state tracking

## Dependencies
- MIDI 1.0 Program Change messages (Cn)
- #537 (GM1 melodic instruments - 128 programs)
- #538 (GM1 percussion - Channel 10 exclusivity)
- Voice allocation and instrument loading system

## References
- **STR-GM1-Level-1**: RP-003 General MIDI System Level 1 Specification
- **MMA RP-003**: Multi-timbral operation and channel assignment
- **MIDI 1.0**: Program Change message specification
