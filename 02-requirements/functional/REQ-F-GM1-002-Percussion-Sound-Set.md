# REQ-F-GM1-002: GM1 Percussion Sound Set (Channel 10, 47 Drum Sounds)

## Metadata
- **ID**: REQ-F-GM1-002
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Priority**: High (P1)
- **Status**: Draft
- **Specification**: GM1 (General MIDI System Level 1)
- **Created**: 2025-11-28
- **Related Issue**: #538

## Traceability
- **Traces to**: STR-GM1-Level-1 (RP-003: General MIDI System Level 1 Specification)
- **Depends on**: 
  - MIDI 1.0 Note On/Off (9n/8n)
  - #537 (REQ-F-GM1-001: GM1 melodic instruments)
- **Verified by**: TBD (TEST-GM1-PERCUSSION-*)
- **Related Issues**: #538

## Source Specification

**STR-GM1-Level-1** (RP-003: General MIDI System Level 1 Specification):
- 47 preset percussion sounds (MIDI Notes 35-81)
- Channel 10 exclusively percussion (MIDI channel 9, 0-indexed)
- Key-based percussion (not pitch-based instruments)
- Standard drum map for universal compatibility

## Description

Implement General MIDI Level 1 percussion sound set with 47 standardized drum sounds mapped to specific MIDI note numbers (35-81) on Channel 10. Support velocity-sensitive percussion playback with standard drum mapping. Ensure cross-device compatibility where Note 36 = Bass Drum 1, Note 38 = Acoustic Snare, etc. on all GM-compliant devices.

---

## GM1 Percussion Map (Channel 10, Keys 35-81)

### Bass Drums and Snares (Keys 35-40)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 35 | B0 | Acoustic Bass Drum | Deep acoustic kick drum |
| 36 | C1 | Bass Drum 1 | Standard bass drum (most common) |
| 37 | C#1 | Side Stick | Snare drum rim shot/cross-stick |
| 38 | D1 | Acoustic Snare | Acoustic snare drum (primary) |
| 39 | D#1 | Hand Clap | Hand clapping sound |
| 40 | E1 | Electric Snare | Electronic/gated snare drum |

### Toms (Keys 41, 43, 45, 47-48, 50)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 41 | F1 | Low Floor Tom | Large floor tom, lowest pitch |
| 43 | G1 | High Floor Tom | Smaller floor tom |
| 45 | A1 | Low Tom | Rack tom, low pitch |
| 47 | B1 | Low-Mid Tom | Rack tom, low-medium pitch |
| 48 | C2 | Hi Mid Tom | Rack tom, medium-high pitch |
| 50 | D2 | High Tom | Rack tom, highest pitch |

### Hi-Hats (Keys 42, 44, 46)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 42 | F#1 | Closed Hi Hat | Tight hi-hat cymbals |
| 44 | G#1 | Pedal Hi-Hat | Hi-hat foot splash |
| 46 | A#1 | Open Hi-Hat | Open hi-hat cymbals |

### Cymbals (Keys 49, 51-53, 55, 57, 59)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 49 | C#2 | Crash Cymbal 1 | Primary crash cymbal |
| 51 | D#2 | Ride Cymbal 1 | Primary ride cymbal |
| 52 | E2 | Chinese Cymbal | Chinese/splash cymbal |
| 53 | F2 | Ride Bell | Ride cymbal bell |
| 55 | G2 | Splash Cymbal | Small splash cymbal |
| 57 | A2 | Crash Cymbal 2 | Secondary crash cymbal |
| 59 | B2 | Ride Cymbal 2 | Secondary ride cymbal |

### Latin Percussion (Keys 54, 56, 58, 60-67)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 54 | F#2 | Tambourine | Tambourine with jingles |
| 56 | G#2 | Cowbell | Metal cowbell |
| 58 | A#2 | Vibraslap | Vibraslap percussion |
| 60 | C3 | Hi Bongo | High-pitched bongo drum |
| 61 | C#3 | Low Bongo | Low-pitched bongo drum |
| 62 | D3 | Mute Hi Conga | Muted high conga |
| 63 | D#3 | Open Hi Conga | Open high conga |
| 64 | E3 | Low Conga | Low conga drum |
| 65 | F3 | High Timbale | High timbale drum |
| 66 | F#3 | Low Timbale | Low timbale drum |
| 67 | G3 | High Agogo | High agogo bell |

### Additional Latin and World Percussion (Keys 68-81)

| MIDI Key | Note Name | Drum Sound | Description |
|----------|-----------|------------|-------------|
| 68 | G#3 | Low Agogo | Low agogo bell |
| 69 | A3 | Cabasa | Cabasa shaker |
| 70 | A#3 | Maracas | Maracas shaker |
| 71 | B3 | Short Whistle | Short whistle sound |
| 72 | C4 | Long Whistle | Long whistle sound |
| 73 | C#4 | Short Guiro | Short guiro scrape |
| 74 | D4 | Long Guiro | Long guiro scrape |
| 75 | D#4 | Claves | Wooden claves |
| 76 | E4 | Hi Wood Block | High woodblock |
| 77 | F4 | Low Wood Block | Low woodblock |
| 78 | F#4 | Mute Cuica | Muted cuica |
| 79 | G4 | Open Cuica | Open cuica |
| 80 | G#4 | Mute Triangle | Muted triangle |
| 81 | A4 | Open Triangle | Open triangle |

---

## Complete GM1 Drum Map Summary Table

| MIDI Key | Note | Drum Sound | Category |
|----------|------|------------|----------|
| 35 | B0 | Acoustic Bass Drum | Bass Drums |
| 36 | C1 | Bass Drum 1 | Bass Drums |
| 37 | C#1 | Side Stick | Snare Effects |
| 38 | D1 | Acoustic Snare | Snares |
| 39 | D#1 | Hand Clap | Percussive Effects |
| 40 | E1 | Electric Snare | Snares |
| 41 | F1 | Low Floor Tom | Toms |
| 42 | F#1 | Closed Hi Hat | Hi-Hats |
| 43 | G1 | High Floor Tom | Toms |
| 44 | G#1 | Pedal Hi-Hat | Hi-Hats |
| 45 | A1 | Low Tom | Toms |
| 46 | A#1 | Open Hi-Hat | Hi-Hats |
| 47 | B1 | Low-Mid Tom | Toms |
| 48 | C2 | Hi Mid Tom | Toms |
| 49 | C#2 | Crash Cymbal 1 | Cymbals |
| 50 | D2 | High Tom | Toms |
| 51 | D#2 | Ride Cymbal 1 | Cymbals |
| 52 | E2 | Chinese Cymbal | Cymbals |
| 53 | F2 | Ride Bell | Cymbals |
| 54 | F#2 | Tambourine | Latin Percussion |
| 55 | G2 | Splash Cymbal | Cymbals |
| 56 | G#2 | Cowbell | Latin Percussion |
| 57 | A2 | Crash Cymbal 2 | Cymbals |
| 58 | A#2 | Vibraslap | Latin Percussion |
| 59 | B2 | Ride Cymbal 2 | Cymbals |
| 60 | C3 | Hi Bongo | Latin Percussion |
| 61 | C#3 | Low Bongo | Latin Percussion |
| 62 | D3 | Mute Hi Conga | Latin Percussion |
| 63 | D#3 | Open Hi Conga | Latin Percussion |
| 64 | E3 | Low Conga | Latin Percussion |
| 65 | F3 | High Timbale | Latin Percussion |
| 66 | F#3 | Low Timbale | Latin Percussion |
| 67 | G3 | High Agogo | World Percussion |
| 68 | G#3 | Low Agogo | World Percussion |
| 69 | A3 | Cabasa | World Percussion |
| 70 | A#3 | Maracas | World Percussion |
| 71 | B3 | Short Whistle | Effects |
| 72 | C4 | Long Whistle | Effects |
| 73 | C#4 | Short Guiro | World Percussion |
| 74 | D4 | Long Guiro | World Percussion |
| 75 | D#4 | Claves | World Percussion |
| 76 | E4 | Hi Wood Block | World Percussion |
| 77 | F4 | Low Wood Block | World Percussion |
| 78 | F#4 | Mute Cuica | World Percussion |
| 79 | G4 | Open Cuica | World Percussion |
| 80 | G#4 | Mute Triangle | World Percussion |
| 81 | A4 | Open Triangle | World Percussion |

---

## Implementation Requirements

### Percussion Note Processing

```c
/**
 * Handle percussion Note On message on Channel 10
 * @param key MIDI note number (35-81 for GM1 percussion)
 * @param velocity Note velocity (0-127, 0 = Note Off)
 */
void handle_percussion_note_on(uint8_t key, uint8_t velocity) {
    // Validate percussion key range (35-81 for GM1)
    if (key < 35 || key > 81) {
        return;  // Outside GM1 percussion range
    }
    
    // Velocity 0 = Note Off for percussion
    if (velocity == 0) {
        handle_percussion_note_off(key);
        return;
    }
    
    // Trigger appropriate GM1 percussion sound
    trigger_gm1_percussion_sound(key, velocity);
}

/**
 * Trigger GM1 percussion sound with velocity sensitivity
 * @param key MIDI note number (35-81)
 * @param velocity Note velocity (1-127)
 */
void trigger_gm1_percussion_sound(uint8_t key, uint8_t velocity) {
    // Get GM1 percussion sound for key
    gm1_percussion_sound_t sound = get_gm1_percussion_sound(key);
    
    if (sound != GM1_PERC_INVALID) {
        // Allocate voice from percussion voice pool
        voice_t* voice = allocate_percussion_voice();
        if (voice) {
            // Configure voice for percussion (one-shot, no pitch bend)
            configure_percussion_voice(voice, sound, velocity);
            // Start playback
            start_percussion_voice(voice);
        }
    }
}
```

### GM1 Percussion Sound Enumeration

```c
/**
 * GM1 Percussion Sound Type Enumeration
 * Values correspond to MIDI note numbers (35-81)
 */
typedef enum {
    GM1_PERC_ACOUSTIC_BASS_DRUM = 35,
    GM1_PERC_BASS_DRUM_1 = 36,
    GM1_PERC_SIDE_STICK = 37,
    GM1_PERC_ACOUSTIC_SNARE = 38,
    GM1_PERC_HAND_CLAP = 39,
    GM1_PERC_ELECTRIC_SNARE = 40,
    GM1_PERC_LOW_FLOOR_TOM = 41,
    GM1_PERC_CLOSED_HI_HAT = 42,
    GM1_PERC_HIGH_FLOOR_TOM = 43,
    GM1_PERC_PEDAL_HI_HAT = 44,
    GM1_PERC_LOW_TOM = 45,
    GM1_PERC_OPEN_HI_HAT = 46,
    GM1_PERC_LOW_MID_TOM = 47,
    GM1_PERC_HI_MID_TOM = 48,
    GM1_PERC_CRASH_CYMBAL_1 = 49,
    GM1_PERC_HIGH_TOM = 50,
    GM1_PERC_RIDE_CYMBAL_1 = 51,
    GM1_PERC_CHINESE_CYMBAL = 52,
    GM1_PERC_RIDE_BELL = 53,
    GM1_PERC_TAMBOURINE = 54,
    GM1_PERC_SPLASH_CYMBAL = 55,
    GM1_PERC_COWBELL = 56,
    GM1_PERC_CRASH_CYMBAL_2 = 57,
    GM1_PERC_VIBRASLAP = 58,
    GM1_PERC_RIDE_CYMBAL_2 = 59,
    GM1_PERC_HI_BONGO = 60,
    GM1_PERC_LOW_BONGO = 61,
    GM1_PERC_MUTE_HI_CONGA = 62,
    GM1_PERC_OPEN_HI_CONGA = 63,
    GM1_PERC_LOW_CONGA = 64,
    GM1_PERC_HIGH_TIMBALE = 65,
    GM1_PERC_LOW_TIMBALE = 66,
    GM1_PERC_HIGH_AGOGO = 67,
    GM1_PERC_LOW_AGOGO = 68,
    GM1_PERC_CABASA = 69,
    GM1_PERC_MARACAS = 70,
    GM1_PERC_SHORT_WHISTLE = 71,
    GM1_PERC_LONG_WHISTLE = 72,
    GM1_PERC_SHORT_GUIRO = 73,
    GM1_PERC_LONG_GUIRO = 74,
    GM1_PERC_CLAVES = 75,
    GM1_PERC_HI_WOOD_BLOCK = 76,
    GM1_PERC_LOW_WOOD_BLOCK = 77,
    GM1_PERC_MUTE_CUICA = 78,
    GM1_PERC_OPEN_CUICA = 79,
    GM1_PERC_MUTE_TRIANGLE = 80,
    GM1_PERC_OPEN_TRIANGLE = 81,
    GM1_PERC_INVALID = 255
} gm1_percussion_sound_t;
```

### GM1 Percussion Name Lookup

```c
/**
 * Get human-readable name for GM1 percussion sound
 * @param key MIDI note number (35-81)
 * @return Percussion sound name or "Unknown"
 */
const char* get_gm1_percussion_name(uint8_t key) {
    static const char* gm1_perc_names[] = {
        [35] = "Acoustic Bass Drum",
        [36] = "Bass Drum 1",
        [37] = "Side Stick",
        [38] = "Acoustic Snare",
        [39] = "Hand Clap",
        [40] = "Electric Snare",
        [41] = "Low Floor Tom",
        [42] = "Closed Hi Hat",
        [43] = "High Floor Tom",
        [44] = "Pedal Hi-Hat",
        [45] = "Low Tom",
        [46] = "Open Hi-Hat",
        [47] = "Low-Mid Tom",
        [48] = "Hi Mid Tom",
        [49] = "Crash Cymbal 1",
        [50] = "High Tom",
        [51] = "Ride Cymbal 1",
        [52] = "Chinese Cymbal",
        [53] = "Ride Bell",
        [54] = "Tambourine",
        [55] = "Splash Cymbal",
        [56] = "Cowbell",
        [57] = "Crash Cymbal 2",
        [58] = "Vibraslap",
        [59] = "Ride Cymbal 2",
        [60] = "Hi Bongo",
        [61] = "Low Bongo",
        [62] = "Mute Hi Conga",
        [63] = "Open Hi Conga",
        [64] = "Low Conga",
        [65] = "High Timbale",
        [66] = "Low Timbale",
        [67] = "High Agogo",
        [68] = "Low Agogo",
        [69] = "Cabasa",
        [70] = "Maracas",
        [71] = "Short Whistle",
        [72] = "Long Whistle",
        [73] = "Short Guiro",
        [74] = "Long Guiro",
        [75] = "Claves",
        [76] = "Hi Wood Block",
        [77] = "Low Wood Block",
        [78] = "Mute Cuica",
        [79] = "Open Cuica",
        [80] = "Mute Triangle",
        [81] = "Open Triangle"
    };
    
    if (key >= 35 && key <= 81) {
        return gm1_perc_names[key];
    }
    return "Unknown";
}

/**
 * Validate if key is within GM1 percussion range
 * @param key MIDI note number
 * @return true if valid GM1 percussion key (35-81)
 */
bool is_valid_gm1_percussion_key(uint8_t key) {
    return (key >= 35 && key <= 81);
}
```

### Channel 10 Exclusive Processing

```c
/**
 * Handle Note On message with channel routing
 * @param channel MIDI channel (0-15, 0-indexed)
 * @param key MIDI note number (0-127)
 * @param velocity Note velocity (0-127)
 */
void handle_note_on(uint8_t channel, uint8_t key, uint8_t velocity) {
    if (channel == 9) {  // Channel 10 (0-indexed as 9)
        // Channel 10 = Percussion only
        handle_percussion_note_on(key, velocity);
    } else {
        // Channels 1-9, 11-16 = Melodic instruments
        handle_melodic_note_on(channel, key, velocity);
    }
}

/**
 * Handle Program Change message with channel routing
 * @param channel MIDI channel (0-15, 0-indexed)
 * @param program Program number (0-127)
 */
void handle_program_change(uint8_t channel, uint8_t program) {
    if (channel == 9) {
        // Channel 10: Ignore Program Change (always percussion)
        return;
    } else {
        // Other channels: Process Program Change normally
        set_channel_program(channel, program);
    }
}
```

---

## Percussion Characteristics

### Velocity Sensitivity

All 47 percussion sounds respond to velocity (0-127):
- **Velocity 0**: Note Off (stop sound if applicable)
- **Velocity 1-127**: Volume and/or timbre variation
- **Minimum velocity response**: 4 distinct volume levels
- **Recommended**: Continuous velocity response

### Sound Behavior

Percussion sounds are:
- **One-shot**: Not sustaining like melodic instruments
- **Key-based**: Specific MIDI key triggers specific drum sound
- **Non-pitched**: Do not follow chromatic scale
- **Re-triggerable**: New Note On can re-attack same sound

### Voice Allocation

Percussion voices allocated from shared 24-voice pool or dedicated percussion pool:
- **Option 1**: Dynamic allocation from 24 total voices
- **Option 2**: 8 dedicated percussion voices + 16 melodic voices
- **Re-attack behavior**: New percussion note gets fresh voice

---

## Acceptance Criteria

### AC1: Complete 47-Sound Support

**Given** Note On keys 35-81 on Channel 10  
**When** processing GM1 percussion  
**Then** all 47 standard percussion sounds respond

**Test Method**: Full percussion key test  
**Success Criteria**: All 47 sounds trigger

### AC2: Channel 10 Exclusivity

**Given** Note On on Channel 10  
**When** processing  
**Then** only percussion sounds (no melodic instruments)

**Test Method**: Channel isolation test  
**Success Criteria**: Channel 10 percussion-only

### AC3: Standard Drum Mapping

**Given** Key 36 on Channel 10  
**When** triggering percussion  
**Then** Bass Drum 1 sound (consistent across GM devices)

**Test Method**: Cross-device mapping test  
**Success Criteria**: Consistent mapping

### AC4: Velocity Sensitivity

**Given** percussion Note On with velocity 1-127  
**When** triggering percussion  
**Then** volume/timbre varies with velocity

**Test Method**: Velocity response test  
**Success Criteria**: 4+ velocity levels

### AC5: Program Change Immunity

**Given** Program Change on Channel 10  
**When** processing  
**Then** Program Change ignored (percussion unchanged)

**Test Method**: Program Change test  
**Success Criteria**: No effect on Channel 10

### AC6: Key Range Validation

**Given** Note On outside keys 35-81 on Channel 10  
**When** processing  
**Then** no percussion sound or graceful handling

**Test Method**: Range validation test  
**Success Criteria**: Proper range enforcement

### AC7: Re-attack Capability

**Given** repeated Note On same percussion key  
**When** retriggering  
**Then** sound re-attacks with fresh voice

**Test Method**: Re-trigger test  
**Success Criteria**: Multiple attacks work

### AC8: Percussion Name Resolution

**Given** GM1 percussion key 35-81  
**When** querying percussion name  
**Then** return standard GM1 drum name

**Test Method**: Name lookup test  
**Success Criteria**: Correct percussion names

---

## Dependencies

- MIDI 1.0 Note On/Off messages (9n/8n)
- #537 (REQ-F-GM1-001: GM1 melodic instruments - for channel allocation)
- Percussion sample library/drum sounds
- Voice allocation system

---

## Labels

`type:requirement:functional`, `phase:02-requirements`, `priority:P1`, `component:gm1`, `midi-spec:gm1`, `midi-spec:channel-voice`, `midi-spec:percussion`

---

## References

- **STR-GM1-Level-1**: RP-003 General MIDI System Level 1 Specification
- **MMA RP-003**: Complete percussion map and implementation guidelines
- **GM Logo Program**: Percussion compatibility requirements
