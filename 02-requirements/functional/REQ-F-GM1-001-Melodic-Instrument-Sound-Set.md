# REQ-F-GM1-001: GM1 Melodic Instrument Sound Set (128 Programs)

## Metadata

| Field | Value |
|-------|-------|
| **ID** | REQ-F-GM1-001 |
| **Type** | Functional Requirement |
| **Phase** | 02-requirements |
| **Priority** | High (P1) |
| **Status** | Approved |
| **Specification** | GM1 (General MIDI System Level 1) |
| **Standard** | RP-003: General MIDI System Level 1 Specification |
| **Created** | 2025-11-28 |
| **Version** | 1.0 |

## Traceability

| Relationship | Reference |
|-------------|-----------|
| **Traces to** | STR-GM1-Level-1 (General MIDI System Level 1 Stakeholder Requirement) |
| **Depends on** | MIDI 1.0 Program Change messages (Cn pp) |
| **Related to** | REQ-F-GM1-002 (GM1 Percussion Map) |
| **Verified by** | TEST-GM1-SOUND-SET-* (Test cases pending) |

---

## Description

The system **SHALL** implement General MIDI Level 1 melodic instrument sound set with 128 standardized programs organized in 16 instrument groups. The system **SHALL** support Program Change messages (Cn pp) on all melodic channels (1-9, 11-16) with consistent timbre mapping. The system **SHALL** ensure cross-device compatibility where Program 1 always represents Acoustic Grand Piano, Program 25 represents Acoustic Guitar (nylon), and so forth per the RP-003 specification.

## Rationale

General MIDI Level 1 provides a standardized sound set that ensures universal compatibility across all GM-compliant devices. When a MIDI file is created using GM sounds, it will produce predictable results when played on any GM-compatible synthesizer, sound module, or software instrument. This standardization is essential for:

1. **Interoperability**: MIDI files created on one device sound recognizably similar on another
2. **Predictability**: Composers and arrangers know exactly what timbre category each program represents
3. **Compatibility**: Legacy MIDI files remain playable on modern GM implementations
4. **Simplicity**: No complex bank/patch setup required for basic music production

---

## GM1 Melodic Instrument Groups

### Group 1: Piano (Programs 1-8)

| Program | Name | Description |
|---------|------|-------------|
| 1 | Acoustic Grand Piano | Full 88-key range, velocity-sensitive dynamics, sustain pedal response |
| 2 | Bright Acoustic Piano | Brighter timbre variation of acoustic grand, enhanced high-frequency content |
| 3 | Electric Grand Piano | Electric piano timbre (Rhodes/Wurlitzer hybrid style) |
| 4 | Honky-tonk Piano | Detuned/chorused acoustic piano effect, barroom/ragtime character |
| 5 | Electric Piano 1 | Classic electric piano sound (Rhodes-style tine-based) |
| 6 | Electric Piano 2 | Alternative electric piano sound (Wurlitzer-style reed-based) |
| 7 | Harpsichord | Baroque keyboard instrument, plucked strings, characteristic attack |
| 8 | Clavi | Clavinet sound (percussive, funky attack, string-striking mechanism) |

### Group 2: Chromatic Percussion (Programs 9-16)

| Program | Name | Description |
|---------|------|-------------|
| 9 | Celesta | Metallic bell-like keyboard instrument, ethereal tone |
| 10 | Glockenspiel | Bright metallic mallet percussion, high register |
| 11 | Music Box | Toy-like bell sound with characteristic mechanical timbre |
| 12 | Vibraphone | Mellow mallet percussion with motor-driven vibrato |
| 13 | Marimba | Wooden mallet percussion, warm resonant tone |
| 14 | Xylophone | Bright wooden mallet percussion, short sustain |
| 15 | Tubular Bells | Large metallic bells (church/orchestral bells), long decay |
| 16 | Dulcimer | Hammered dulcimer or similar string percussion |

### Group 3: Organ (Programs 17-24)

| Program | Name | Description |
|---------|------|-------------|
| 17 | Drawbar Organ | Hammond-style tonewheel organ with adjustable harmonic content |
| 18 | Percussive Organ | Organ with percussive attack envelope, key click |
| 19 | Rock Organ | Distorted/overdriven organ sound, aggressive character |
| 20 | Church Organ | Pipe organ sound (cathedral/church style), full registration |
| 21 | Reed Organ | Harmonium/pump organ sound, free-reed aerophone |
| 22 | Accordion | Acoustic accordion with characteristic tremolo/vibrato |
| 23 | Harmonica | Mouth harp/blues harp sound, breath-controlled |
| 24 | Tango Accordion | Bandoneón/tango accordion style, distinctive bellows character |

### Group 4: Guitar (Programs 25-32)

| Program | Name | Description |
|---------|------|-------------|
| 25 | Acoustic Guitar (nylon) | Classical guitar, fingerpicked, warm nylon string tone |
| 26 | Acoustic Guitar (steel) | Folk/country steel-string guitar, brighter attack |
| 27 | Electric Guitar (jazz) | Clean electric guitar, warm/mellow jazz tone |
| 28 | Electric Guitar (clean) | Clean electric guitar, bright/clear tone |
| 29 | Electric Guitar (muted) | Muted electric guitar (palm muting), percussive |
| 30 | Overdriven Guitar | Moderate overdrive/distortion, tube amplifier character |
| 31 | Distortion Guitar | Heavy distortion (rock/metal style), high-gain |
| 32 | Guitar Harmonics | Natural/artificial harmonics effects, bell-like |

### Group 5: Bass (Programs 33-40)

| Program | Name | Description |
|---------|------|-------------|
| 33 | Acoustic Bass | Upright/double bass, pizzicato or bowed articulation |
| 34 | Electric Bass (finger) | Fingerstyle electric bass, smooth round tone |
| 35 | Electric Bass (pick) | Pick-style electric bass, brighter attack, defined transient |
| 36 | Fretless Bass | Fretless electric bass, smooth gliding pitch capability |
| 37 | Slap Bass 1 | Slap/pop technique, percussive attack, funky character |
| 38 | Slap Bass 2 | Alternative slap bass sound variant |
| 39 | Synth Bass 1 | Synthesized bass sound (analog-style), electronic character |
| 40 | Synth Bass 2 | Alternative synthesized bass sound variant |

### Group 6: Strings (Programs 41-48)

| Program | Name | Description |
|---------|------|-------------|
| 41 | Violin | Solo violin with bowing articulation, expressive |
| 42 | Viola | Solo viola, slightly darker than violin, alto register |
| 43 | Cello | Solo cello with characteristic warm tone, tenor/bass register |
| 44 | Contrabass | Double bass/string bass (bowed), lowest string register |
| 45 | Tremolo Strings | String section with rapid bow tremolo effect |
| 46 | Pizzicato Strings | String section, pizzicato plucked articulation |
| 47 | Orchestral Harp | Concert harp with glissando capability, ethereal |
| 48 | Timpani | Orchestral timpani/kettle drums, pitched percussion |

### Group 7: Ensemble (Programs 49-56)

| Program | Name | Description |
|---------|------|-------------|
| 49 | String Ensemble 1 | Orchestral string section (warm), lush ensemble |
| 50 | String Ensemble 2 | Alternative string section sound, brighter variant |
| 51 | SynthStrings 1 | Synthesized string pad (analog-style), electronic ensemble |
| 52 | SynthStrings 2 | Alternative synthesized string sound variant |
| 53 | Choir Aahs | Vocal choir "Ah" vowel sound, mixed voices |
| 54 | Voice Oohs | Vocal choir "Ooh" vowel sound, softer character |
| 55 | Synth Voice | Synthesized vocal/choir pad, electronic character |
| 56 | Orchestra Hit | Staccato orchestral accent (brass + strings combined) |

### Group 8: Brass (Programs 57-64)

| Program | Name | Description |
|---------|------|-------------|
| 57 | Trumpet | Solo trumpet with characteristic brass attack |
| 58 | Trombone | Solo trombone with slide/glissando capability |
| 59 | Tuba | Solo tuba, low brass instrument, foundation |
| 60 | Muted Trumpet | Trumpet with mute (Harmon, cup, straight, etc.) |
| 61 | French Horn | Solo French horn with warm, mellow brass tone |
| 62 | Brass Section | Ensemble brass section (trumpets + trombones combined) |
| 63 | SynthBrass 1 | Synthesized brass sound (analog-style), electronic brass |
| 64 | SynthBrass 2 | Alternative synthesized brass sound variant |

### Group 9: Reed (Programs 65-72)

| Program | Name | Description |
|---------|------|-------------|
| 65 | Soprano Sax | Soprano saxophone, highest saxophone register |
| 66 | Alto Sax | Alto saxophone with characteristic jazz tone |
| 67 | Tenor Sax | Tenor saxophone, warmer than alto, common jazz voice |
| 68 | Baritone Sax | Baritone saxophone, low register, powerful |
| 69 | Oboe | Double reed woodwind with nasal/penetrating timbre |
| 70 | English Horn | Alto oboe (cor anglais) with darker, mellower tone |
| 71 | Bassoon | Double reed bass woodwind, orchestral foundation |
| 72 | Clarinet | Single reed woodwind with woody, warm tone |

### Group 10: Pipe (Programs 73-80)

| Program | Name | Description |
|---------|------|-------------|
| 73 | Piccolo | High flute, bright and piercing, highest woodwind |
| 74 | Flute | Concert flute with breathy attack, pure tone |
| 75 | Recorder | Wooden recorder/block flute, Renaissance character |
| 76 | Pan Flute | Panpipes/syrinx with airy, breathy tone |
| 77 | Blown Bottle | Bottle flute or similar airy, hollow sound |
| 78 | Shakuhachi | Japanese bamboo flute, meditative character |
| 79 | Whistle | Simple whistle sound, pure/piercing |
| 80 | Ocarina | Ceramic/clay flute, ancient vessel flute |

### Group 11: Synth Lead (Programs 81-88)

| Program | Name | Description |
|---------|------|-------------|
| 81 | Lead 1 (square) | Square wave synthesizer lead, hollow/clarinet-like |
| 82 | Lead 2 (sawtooth) | Sawtooth wave synthesizer lead, bright/brassy |
| 83 | Lead 3 (calliope) | Steam organ/calliope lead sound, circus character |
| 84 | Lead 4 (chiff) | Lead with characteristic attack noise/chiff |
| 85 | Lead 5 (charang) | Distorted/gritty synthesizer lead, aggressive |
| 86 | Lead 6 (voice) | Vocal-like synthesizer lead, singing quality |
| 87 | Lead 7 (fifths) | Lead with fifth interval harmony, power chord |
| 88 | Lead 8 (bass + lead) | Bass and lead combination sound, layered |

### Group 12: Synth Pad (Programs 89-96)

| Program | Name | Description |
|---------|------|-------------|
| 89 | Pad 1 (new age) | Warm synthesizer pad (new age style), ethereal |
| 90 | Pad 2 (warm) | Warm analog-style synthesizer pad, lush |
| 91 | Pad 3 (polysynth) | Polyphonic synthesizer pad, classic analog |
| 92 | Pad 4 (choir) | Synthesized choir pad, vocal-like |
| 93 | Pad 5 (bowed) | Bowed string-like synthesizer pad, sustained |
| 94 | Pad 6 (metallic) | Metallic/bell-like synthesizer pad, shimmering |
| 95 | Pad 7 (halo) | Ethereal/atmospheric synthesizer pad, airy |
| 96 | Pad 8 (sweep) | Sweeping filter synthesizer pad, evolving |

### Group 13: Synth Effects (Programs 97-104)

| Program | Name | Description |
|---------|------|-------------|
| 97 | FX 1 (rain) | Rain sound effect, atmospheric |
| 98 | FX 2 (soundtrack) | Soundtrack/film score effect, cinematic |
| 99 | FX 3 (crystal) | Crystal/glass-like effect, sparkling |
| 100 | FX 4 (atmosphere) | Atmospheric sound effect, ambient |
| 101 | FX 5 (brightness) | Bright/shimmering effect, gleaming |
| 102 | FX 6 (goblins) | Weird/otherworldly effect, mysterious |
| 103 | FX 7 (echoes) | Echo/delay effect, spacious |
| 104 | FX 8 (sci-fi) | Science fiction sound effect, futuristic |

### Group 14: Ethnic (Programs 105-112)

| Program | Name | Description |
|---------|------|-------------|
| 105 | Sitar | Indian sitar with sympathetic strings, ornamental |
| 106 | Banjo | American banjo with characteristic twang, bluegrass |
| 107 | Shamisen | Japanese three-string instrument, sharp attack |
| 108 | Koto | Japanese thirteen-string zither, traditional |
| 109 | Kalimba | African thumb piano/mbira, metallic tines |
| 110 | Bag pipe | Scottish bagpipes with continuous drone |
| 111 | Fiddle | Folk fiddle (similar to violin, different style/ornamentation) |
| 112 | Shanai | Indian double-reed instrument (shehnai), nasal |

### Group 15: Percussive (Programs 113-120)

| Program | Name | Description |
|---------|------|-------------|
| 113 | Tinkle Bell | Small bell/chime sound, delicate |
| 114 | Agogo | Brazilian agogo bells, Latin percussion |
| 115 | Steel Drums | Caribbean steel drums/steel pan, island sound |
| 116 | Woodblock | Wooden percussion block, sharp attack |
| 117 | Taiko Drum | Japanese taiko drum, powerful low tone |
| 118 | Melodic Tom | Melodic tom-tom drums, pitched percussion |
| 119 | Synth Drum | Synthesized drum sound, electronic |
| 120 | Reverse Cymbal | Reversed cymbal effect, building crescendo |

### Group 16: Sound Effects (Programs 121-128)

| Program | Name | Description |
|---------|------|-------------|
| 121 | Guitar Fret Noise | Fret sliding/finger noise, realistic |
| 122 | Breath Noise | Wind instrument breath sound, realistic |
| 123 | Seashore | Ocean waves sound effect, ambient |
| 124 | Bird Tweet | Bird chirping sound, nature |
| 125 | Telephone Ring | Telephone ringing sound, classic |
| 126 | Helicopter | Helicopter rotor sound, mechanical |
| 127 | Applause | Audience applause sound, crowd |
| 128 | Gunshot | Gunshot sound effect, percussive |

---

## Implementation Requirements

### Program Change Processing

```c
/**
 * Handle GM1 Program Change message
 * 
 * @param channel  MIDI channel (0-15, where 9 = Channel 10 = Percussion)
 * @param program  Program number (0-127, representing GM programs 1-128)
 * 
 * Implements: REQ-F-GM1-001
 * Verified by: TEST-GM1-PROGRAM-CHANGE-001
 */
void handle_program_change(uint8_t channel, uint8_t program) {
    // Validate program number (0-127, representing programs 1-128)
    if (program > 127) {
        return;  // Invalid program number - ignore
    }
    
    // Convert to 1-based GM program number for display/lookup
    uint8_t gm_program = program + 1;  // Programs 1-128
    
    // Channel 10 (index 9) is reserved for percussion - ignore Program Change
    if (channel == 9) {
        // Channel 10 reserved for percussion (GM1 Drum Kit)
        // Program Change messages are ignored per GM1 specification
        return;
    }
    
    // Set instrument for melodic channel (channels 1-9, 11-16)
    set_channel_instrument(channel, gm_program);
    
    // Load appropriate sound bank/samples for GM program
    load_gm_instrument_samples(gm_program);
}
```

### GM1 Program Validation

```c
/**
 * Validate if program number is within GM1 range
 * 
 * @param program  1-based GM program number (1-128)
 * @return true if valid GM1 program, false otherwise
 * 
 * Implements: REQ-F-GM1-001
 * Verified by: TEST-GM1-VALIDATION-001
 */
bool is_valid_gm1_program(uint8_t program) {
    // GM1 supports programs 1-128 (MIDI Program Change 0-127)
    return (program >= 1 && program <= 128);
}
```

### GM1 Program Name Lookup

```c
/**
 * Get standard GM1 instrument name for a program number
 * 
 * @param program  1-based GM program number (1-128)
 * @return Pointer to instrument name string, or "Unknown" if invalid
 * 
 * Implements: REQ-F-GM1-001
 * Verified by: TEST-GM1-NAME-LOOKUP-001
 */
const char* get_gm1_program_name(uint8_t program) {
    static const char* gm1_names[128] = {
        // Group 1: Piano (1-8)
        "Acoustic Grand Piano",   // 1
        "Bright Acoustic Piano",  // 2
        "Electric Grand Piano",   // 3
        "Honky-tonk Piano",       // 4
        "Electric Piano 1",       // 5
        "Electric Piano 2",       // 6
        "Harpsichord",            // 7
        "Clavi",                  // 8
        
        // Group 2: Chromatic Percussion (9-16)
        "Celesta",                // 9
        "Glockenspiel",           // 10
        "Music Box",              // 11
        "Vibraphone",             // 12
        "Marimba",                // 13
        "Xylophone",              // 14
        "Tubular Bells",          // 15
        "Dulcimer",               // 16
        
        // Group 3: Organ (17-24)
        "Drawbar Organ",          // 17
        "Percussive Organ",       // 18
        "Rock Organ",             // 19
        "Church Organ",           // 20
        "Reed Organ",             // 21
        "Accordion",              // 22
        "Harmonica",              // 23
        "Tango Accordion",        // 24
        
        // Group 4: Guitar (25-32)
        "Acoustic Guitar (nylon)", // 25
        "Acoustic Guitar (steel)", // 26
        "Electric Guitar (jazz)", // 27
        "Electric Guitar (clean)", // 28
        "Electric Guitar (muted)", // 29
        "Overdriven Guitar",      // 30
        "Distortion Guitar",      // 31
        "Guitar Harmonics",       // 32
        
        // Group 5: Bass (33-40)
        "Acoustic Bass",          // 33
        "Electric Bass (finger)", // 34
        "Electric Bass (pick)",   // 35
        "Fretless Bass",          // 36
        "Slap Bass 1",            // 37
        "Slap Bass 2",            // 38
        "Synth Bass 1",           // 39
        "Synth Bass 2",           // 40
        
        // Group 6: Strings (41-48)
        "Violin",                 // 41
        "Viola",                  // 42
        "Cello",                  // 43
        "Contrabass",             // 44
        "Tremolo Strings",        // 45
        "Pizzicato Strings",      // 46
        "Orchestral Harp",        // 47
        "Timpani",                // 48
        
        // Group 7: Ensemble (49-56)
        "String Ensemble 1",      // 49
        "String Ensemble 2",      // 50
        "SynthStrings 1",         // 51
        "SynthStrings 2",         // 52
        "Choir Aahs",             // 53
        "Voice Oohs",             // 54
        "Synth Voice",            // 55
        "Orchestra Hit",          // 56
        
        // Group 8: Brass (57-64)
        "Trumpet",                // 57
        "Trombone",               // 58
        "Tuba",                   // 59
        "Muted Trumpet",          // 60
        "French Horn",            // 61
        "Brass Section",          // 62
        "SynthBrass 1",           // 63
        "SynthBrass 2",           // 64
        
        // Group 9: Reed (65-72)
        "Soprano Sax",            // 65
        "Alto Sax",               // 66
        "Tenor Sax",              // 67
        "Baritone Sax",           // 68
        "Oboe",                   // 69
        "English Horn",           // 70
        "Bassoon",                // 71
        "Clarinet",               // 72
        
        // Group 10: Pipe (73-80)
        "Piccolo",                // 73
        "Flute",                  // 74
        "Recorder",               // 75
        "Pan Flute",              // 76
        "Blown Bottle",           // 77
        "Shakuhachi",             // 78
        "Whistle",                // 79
        "Ocarina",                // 80
        
        // Group 11: Synth Lead (81-88)
        "Lead 1 (square)",        // 81
        "Lead 2 (sawtooth)",      // 82
        "Lead 3 (calliope)",      // 83
        "Lead 4 (chiff)",         // 84
        "Lead 5 (charang)",       // 85
        "Lead 6 (voice)",         // 86
        "Lead 7 (fifths)",        // 87
        "Lead 8 (bass + lead)",   // 88
        
        // Group 12: Synth Pad (89-96)
        "Pad 1 (new age)",        // 89
        "Pad 2 (warm)",           // 90
        "Pad 3 (polysynth)",      // 91
        "Pad 4 (choir)",          // 92
        "Pad 5 (bowed)",          // 93
        "Pad 6 (metallic)",       // 94
        "Pad 7 (halo)",           // 95
        "Pad 8 (sweep)",          // 96
        
        // Group 13: Synth Effects (97-104)
        "FX 1 (rain)",            // 97
        "FX 2 (soundtrack)",      // 98
        "FX 3 (crystal)",         // 99
        "FX 4 (atmosphere)",      // 100
        "FX 5 (brightness)",      // 101
        "FX 6 (goblins)",         // 102
        "FX 7 (echoes)",          // 103
        "FX 8 (sci-fi)",          // 104
        
        // Group 14: Ethnic (105-112)
        "Sitar",                  // 105
        "Banjo",                  // 106
        "Shamisen",               // 107
        "Koto",                   // 108
        "Kalimba",                // 109
        "Bag pipe",               // 110
        "Fiddle",                 // 111
        "Shanai",                 // 112
        
        // Group 15: Percussive (113-120)
        "Tinkle Bell",            // 113
        "Agogo",                  // 114
        "Steel Drums",            // 115
        "Woodblock",              // 116
        "Taiko Drum",             // 117
        "Melodic Tom",            // 118
        "Synth Drum",             // 119
        "Reverse Cymbal",         // 120
        
        // Group 16: Sound Effects (121-128)
        "Guitar Fret Noise",      // 121
        "Breath Noise",           // 122
        "Seashore",               // 123
        "Bird Tweet",             // 124
        "Telephone Ring",         // 125
        "Helicopter",             // 126
        "Applause",               // 127
        "Gunshot"                 // 128
    };
    
    if (program >= 1 && program <= 128) {
        return gm1_names[program - 1];
    }
    return "Unknown";
}
```

### GM1 Group Lookup

```c
/**
 * Get GM1 instrument group name for a program number
 * 
 * @param program  1-based GM program number (1-128)
 * @return Pointer to group name string, or "Unknown" if invalid
 * 
 * Implements: REQ-F-GM1-001
 * Verified by: TEST-GM1-GROUP-LOOKUP-001
 */
const char* get_gm1_group_name(uint8_t program) {
    static const char* gm1_groups[16] = {
        "Piano",              // Programs 1-8
        "Chromatic Percussion", // Programs 9-16
        "Organ",              // Programs 17-24
        "Guitar",             // Programs 25-32
        "Bass",               // Programs 33-40
        "Strings",            // Programs 41-48
        "Ensemble",           // Programs 49-56
        "Brass",              // Programs 57-64
        "Reed",               // Programs 65-72
        "Pipe",               // Programs 73-80
        "Synth Lead",         // Programs 81-88
        "Synth Pad",          // Programs 89-96
        "Synth Effects",      // Programs 97-104
        "Ethnic",             // Programs 105-112
        "Percussive",         // Programs 113-120
        "Sound Effects"       // Programs 121-128
    };
    
    if (program >= 1 && program <= 128) {
        uint8_t group_index = (program - 1) / 8;
        return gm1_groups[group_index];
    }
    return "Unknown";
}

/**
 * Get GM1 group number for a program number
 * 
 * @param program  1-based GM program number (1-128)
 * @return Group number (1-16), or 0 if invalid
 * 
 * Implements: REQ-F-GM1-001
 * Verified by: TEST-GM1-GROUP-NUMBER-001
 */
uint8_t get_gm1_group_number(uint8_t program) {
    if (program >= 1 && program <= 128) {
        return ((program - 1) / 8) + 1;
    }
    return 0;
}
```

---

## Channel Assignment Rules

### Melodic Channels

The system **SHALL** support all 128 GM1 programs on the following melodic channels:

| Channel Range | MIDI Index | Description |
|---------------|------------|-------------|
| Channels 1-9 | 0-8 | Full GM1 instrument access (Programs 1-128) |
| Channels 11-16 | 10-15 | Full GM1 instrument access (Programs 1-128) |
| **Channel 10** | **9** | **RESERVED for percussion** (GM1 drum set only) |

### Channel Assignment Example

```
Channel 1 + Program 1 → Acoustic Grand Piano
Channel 2 + Program 25 → Acoustic Guitar (nylon)
Channel 3 + Program 57 → Trumpet
Channel 10 → Percussion set (ignores Program Change messages)
Channel 11 + Program 41 → Violin
```

### Program Change Behavior

| Aspect | Behavior |
|--------|----------|
| **Valid Range** | Program Change 0-127 (representing GM programs 1-128) |
| **Invalid Programs** | Ignore or clamp to valid range (implementation choice) |
| **Default Program** | Program 1 (Acoustic Grand Piano) on all melodic channels at GM Reset |
| **Channel 10 Exception** | Program Change messages ignored (percussion channel) |

---

## Acceptance Criteria

### AC1: Complete 128-Program Support

**Scenario**: System responds to all 128 GM1 programs

```gherkin
Scenario: AC1 - Complete 128-Program Support
  Given a GM1-compliant MIDI implementation
  When Program Change messages 0-127 are sent on any melodic channel (1-9, 11-16)
  Then the system shall load the correct GM1 instrument for all 128 programs
  And the instrument timbre shall match the GM1 specification category

Test Method: Exhaustive program test
Success Criteria: All 128 programs respond correctly
Verified by: TEST-GM1-COMPLETE-128-001
```

### AC2: 16-Group Organization

**Scenario**: Programs are organized into correct instrument groups

```gherkin
Scenario: AC2 - 16-Group Organization
  Given GM1 program numbers 1-128
  When programs are organized by group
  Then programs 1-8 shall be Piano family
  And programs 9-16 shall be Chromatic Percussion family
  And programs 17-24 shall be Organ family
  And programs 25-32 shall be Guitar family
  And programs 33-40 shall be Bass family
  And programs 41-48 shall be Strings family
  And programs 49-56 shall be Ensemble family
  And programs 57-64 shall be Brass family
  And programs 65-72 shall be Reed family
  And programs 73-80 shall be Pipe family
  And programs 81-88 shall be Synth Lead family
  And programs 89-96 shall be Synth Pad family
  And programs 97-104 shall be Synth Effects family
  And programs 105-112 shall be Ethnic family
  And programs 113-120 shall be Percussive family
  And programs 121-128 shall be Sound Effects family

Test Method: Category verification
Success Criteria: Correct grouping confirmed
Verified by: TEST-GM1-GROUPS-001
```

### AC3: Cross-Device Consistency

**Scenario**: Same GM program produces recognizably similar sounds across devices

```gherkin
Scenario: AC3 - Cross-Device Consistency
  Given the same GM1 program number sent to different GM-compliant devices
  When comparing timbres produced by the same program number
  Then the instruments shall be recognizably in the same instrument family
  And Program 1 shall always be piano-family sound
  And Program 25 shall always be nylon guitar sound
  And Program 57 shall always be trumpet sound

Test Method: Multi-device comparison
Success Criteria: Consistent categorization across devices
Verified by: TEST-GM1-CONSISTENCY-001
```

### AC4: Channel 10 Exclusion

**Scenario**: Channel 10 ignores Program Change for percussion

```gherkin
Scenario: AC4 - Channel 10 Exclusion
  Given Channel 10 (MIDI channel 9, 0-indexed) is reserved for percussion
  When a Program Change message is sent on Channel 10
  Then the Program Change shall be ignored
  And the channel shall continue to play GM1 percussion set
  And no melodic instrument shall be assigned

Test Method: Channel 10 test
Success Criteria: Program Change has no effect on Channel 10
Verified by: TEST-GM1-CHANNEL10-001
```

### AC5: Default Program Assignment

**Scenario**: All melodic channels default to Program 1

```gherkin
Scenario: AC5 - Default Program Assignment
  Given a GM Reset or system initialization
  When no explicit Program Change has been sent
  Then all melodic channels (1-9, 11-16) shall default to Program 1
  And Program 1 shall produce Acoustic Grand Piano sound

Test Method: Initialization test
Success Criteria: Default to Program 1 (Acoustic Grand Piano)
Verified by: TEST-GM1-DEFAULT-001
```

### AC6: Invalid Program Handling

**Scenario**: Invalid program numbers are handled gracefully

```gherkin
Scenario: AC6 - Invalid Program Handling
  Given a Program Change message with value > 127
  When the system processes the message
  Then the system shall either ignore the invalid value
  Or clamp the value to the valid range 0-127
  And no crash or undefined behavior shall occur

Test Method: Error handling test
Success Criteria: Graceful handling without crash
Verified by: TEST-GM1-INVALID-001
```

### AC7: Program Name Resolution

**Scenario**: Program names can be resolved from numbers

```gherkin
Scenario: AC7 - Program Name Resolution
  Given a GM1 program number 1-128
  When querying the program name
  Then the system shall return the standard GM1 instrument name
  And Program 1 returns "Acoustic Grand Piano"
  And Program 25 returns "Acoustic Guitar (nylon)"
  And Program 128 returns "Gunshot"

Test Method: Name lookup test
Success Criteria: Correct names returned for all 128 programs
Verified by: TEST-GM1-NAMES-001
```

### AC8: Melodic Channel Accessibility

**Scenario**: All 128 programs accessible on each melodic channel

```gherkin
Scenario: AC8 - Melodic Channel Accessibility
  Given melodic channels 1-9 and 11-16 (15 channels total)
  When sending Program Change messages 0-127 to each channel
  Then all 128 programs shall be accessible on each of the 15 melodic channels
  And the correct instrument shall be loaded for each channel/program combination

Test Method: Channel/program matrix test (15 channels × 128 programs = 1,920 combinations)
Success Criteria: Full accessibility confirmed
Verified by: TEST-GM1-MATRIX-001
```

---

## Dependencies

| Dependency | Description | Type |
|------------|-------------|------|
| MIDI 1.0 Program Change | Cn pp message parsing | Technical |
| GM1 Sound Library | Sound bank/sample sets for 128 instruments | Resource |
| Channel Voice Allocation | Voice management system | Technical |
| Program Change Parser | Status byte 0xCn recognition | Technical |

---

## References

| Reference | Description |
|-----------|-------------|
| **MMA RP-003** | General MIDI System Level 1 Specification |
| **STR-GM1-Level-1** | Stakeholder Requirement for GM1 compliance |
| **REQ-F-GM1-002** | Related requirement: GM1 Percussion Map (Channel 10) |
| **MIDI 1.0 Detailed Specification** | Program Change message format |

---

## Quality Checklist

- [x] Uses 'shall' language (mandatory)
- [x] Is testable and verifiable
- [x] Has clear acceptance criteria (8 scenarios)
- [x] Traces to stakeholder requirement
- [x] Dependencies identified
- [x] No ambiguous terms
- [x] Implementation guidance provided
- [x] Complete 128-instrument specification
- [x] 16-group organization documented
- [x] Channel assignment rules defined

---

## Revision History

| Version | Date | Author | Description |
|---------|------|--------|-------------|
| 1.0 | 2025-11-28 | Copilot | Initial specification based on GitHub Issue #REQ-F-GM1-001 |
