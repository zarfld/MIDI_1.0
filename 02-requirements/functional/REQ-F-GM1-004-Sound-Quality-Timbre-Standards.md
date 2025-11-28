# REQ-F-GM1-004: GM1 Sound Quality and Timbre Standards

## Metadata
- **ID**: REQ-F-GM1-004
- **Type**: Functional Requirement
- **Phase**: 02-requirements
- **Priority**: Medium
- **Status**: Draft
- **Specification**: GM1 (General MIDI System Level 1)
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (pending GitHub issue creation)
- **Depends on**: zarfld/MIDI_1.0#537-539 (GM1 instruments, percussion, program changes)
- **Verified by**: TBD (TEST-GM1-SOUND-QUALITY-*)

## Source Specification
**STR-GM1-Level-1** (RP-003: General MIDI System Level 1 Specification):
- Timbre aesthetics: Manufacturer freedom within category constraints
- Predictable playback more important than identical timbres
- Modulation Wheel (CC#1) affects sound nature in expected way
- Synthesis method up to manufacturer (ROM wavetable, PCM sample playback, software)

## Description
Define sound quality and timbre standards for GM1 instruments ensuring recognizable instrument categories while allowing manufacturer aesthetic freedom. Establish consistency requirements for cross-device playback predictability. Support various synthesis methods (wavetable, sample playback, software synthesis) while maintaining GM1 compatibility.

## Timbre Recognition Standards

### Primary Requirement: Category Recognition
```
Each GM1 program must be recognizably within its instrument category:
- Program 1 (Acoustic Grand Piano): Clearly identifiable as piano timbre
- Program 25 (Acoustic Guitar nylon): Clearly identifiable as nylon-string guitar
- Program 57 (Trumpet): Clearly identifiable as trumpet brass instrument
- Program 38 (Acoustic Snare): Clearly identifiable as snare drum

Recognition Test: Trained musician should identify instrument family (80%+ accuracy)
```

### Timbre Flexibility Guidelines
```
Manufacturers have aesthetic freedom for:
- Specific sample content (Steinway vs Yamaha piano character)
- Synthesis parameters (filter settings, envelope shapes)
- Effects processing (reverb, chorus, EQ)
- Dynamic response curves (velocity to volume/brightness mapping)

Manufacturers must maintain:
- Instrument category identity (piano sounds like piano)
- Playability characteristics (expected response to controllers)
- Harmonic content appropriate to instrument family
```

## Synthesis Method Support

### Acceptable Synthesis Methods
```
GM1 specification is synthesis-agnostic:
1. ROM Wavetable Synthesis (most common 1994-1999)
2. PCM Sample Playback (higher-end devices)
3. Software Synthesis (computer-based GM)
4. FM Synthesis (Yamaha legacy support)
5. Additive/Subtractive Synthesis
6. Physical modeling (advanced implementations)
7. Hybrid combinations
```

### Quality Baseline Requirements
```
Minimum quality standards:
- Sample rate: 22.05 kHz minimum (44.1 kHz recommended)
- Bit depth: 8-bit minimum (16-bit recommended)
- Frequency response: 20 Hz - 11 kHz minimum (20 Hz - 20 kHz recommended)
- Dynamic range: 48 dB minimum (72 dB recommended)
- Polyphony: 24 voices minimum as specified elsewhere
```

## Controller Response Standards

### Modulation Wheel (CC#1) Response
```c
void handle_modulation_wheel(uint8_t channel, uint8_t value) {
    // CC#1 must affect sound in musically appropriate way for each instrument
    uint8_t program = get_channel_program(channel);
    
    switch (get_gm1_instrument_category(program)) {
        case GM1_STRINGS:
            // Strings: Modulation wheel → Vibrato depth
            set_vibrato_depth(channel, value);
            break;
            
        case GM1_BRASS:
            // Brass: Modulation wheel → Vibrato + brightness
            set_vibrato_depth(channel, value);
            set_brightness(channel, value);
            break;
            
        case GM1_SYNTH_LEAD:
            // Synth leads: Modulation wheel → LFO depth/speed
            set_lfo_depth(channel, value);
            break;
            
        case GM1_PERCUSSION:
            // Percussion: Modulation wheel minimal effect or none
            break;
            
        default:
            // General case: Subtle vibrato
            set_vibrato_depth(channel, value * 0.5);
            break;
    }
}
```

### Volume Controller (CC#7) Response
```c
void handle_volume_controller(uint8_t channel, uint8_t value) {
    // CC#7 must provide smooth volume control across full range
    float volume_db = midi_value_to_db(value);  // 0-127 → -∞ to 0 dB
    set_channel_volume(channel, volume_db);
    
    // Response curve requirements:
    // - CC#7 = 0: Silence (or very quiet)
    // - CC#7 = 64: Nominal volume (-6 dB or similar)
    // - CC#7 = 127: Maximum volume (0 dB)
    // - Smooth logarithmic response (not linear)
}

float midi_value_to_db(uint8_t midi_value) {
    if (midi_value == 0) return -60.0f;  // Near silence
    return 20.0f * log10f((float)midi_value / 127.0f);  // Logarithmic scaling
}
```

## Cross-Device Consistency Standards

### Playback Predictability Requirements
```
GM1 MIDI file playback consistency across devices:

1. Melodic Recognition: Same program number yields recognizable instrument family
2. Rhythmic Integrity: Percussion patterns maintain rhythmic character
3. Dynamic Response: Volume controllers produce similar relative changes
4. Harmonic Compatibility: Chord voicings sound musically appropriate
5. Tempo Stability: MIDI timing preserved without artifacts

Acceptable Variations:
- Specific timbre character (warm vs bright piano)
- Reverb/effects processing choices
- Slight tuning variations within equal temperament
- Dynamic range differences (within 12 dB)
```

### Quality Assurance Testing
```c
typedef struct {
    uint8_t program;
    float   recognition_score;      // 0.0 - 1.0 (human listening test)
    bool    controller_response;    // CC#1, CC#7 work as expected
    bool    velocity_sensitive;     // Notes respond to velocity
    float   frequency_accuracy;     // Tuning accuracy (cents deviation)
} gm1_quality_metrics_t;

bool validate_gm1_program_quality(uint8_t program) {
    gm1_quality_metrics_t metrics = test_gm1_program(program);
    
    return (metrics.recognition_score >= 0.8f &&  // 80% recognition
            metrics.controller_response == true &&
            metrics.velocity_sensitive == true &&
            fabs(metrics.frequency_accuracy) <= 10.0f);  // ±10 cents tuning
}
```

## Implementation Quality Guidelines

### ROM/Sample Efficiency
```
GM1 implementations should optimize for:
- Memory usage: Efficient sample compression and looping
- CPU usage: Real-time synthesis within performance budgets
- Storage: Complete GM1 sound set within reasonable ROM/disk space
- Loading time: Quick program changes (<100ms switching time)

Recommended optimizations:
- Shared samples between similar programs (multiple piano variations)
- Velocity layering for expressive response (minimum 2 layers per program)
- Loop points for sustained instruments (strings, pads, organs)
- Multi-sampling across keyboard range (minimum every octave)
```

### Velocity Response Implementation
```c
typedef struct {
    uint8_t  velocity_layers;       // Number of velocity layers (2 minimum)
    float    velocity_curve;        // Response curve (0.5 = linear, 1.0 = exponential)
    bool     velocity_affects_timbre; // Velocity changes brightness/character
} velocity_config_t;

void configure_program_velocity(uint8_t program, velocity_config_t config) {
    // All GM1 programs must respond to velocity
    set_program_velocity_layers(program, config.velocity_layers);
    set_program_velocity_curve(program, config.velocity_curve);
    
    // Recommended: Velocity affects both volume and timbre
    if (config.velocity_affects_timbre) {
        enable_velocity_brightness_control(program);
    }
}
```

## Platform-Specific Considerations

### Desktop Computer Implementation
```
Software GM synthesizers:
- Higher quality samples/synthesis (CD-quality audio)
- More sophisticated effects processing
- Real-time parameter control via GUI
- Higher polyphony (64+ voices)
- Support for SoundFont banks
```

### Embedded/Hardware Implementation  
```
ROM-based sound modules:
- Optimized wavetable synthesis
- Fixed polyphony (24 voices minimum)
- Hardware effects processing (reverb, chorus)
- MIDI-only control interface
- Power-efficient operation
```

### Hybrid Implementations
```
Sound cards (Creative SoundBlaster era):
- Hardware wavetable + software mixing
- Downloadable samples (SoundFont support)
- Game-optimized latency
- Background loading of samples
```

## Acceptance Criteria

### AC1: Instrument Category Recognition
**Given** GM1 program 1-128  
**When** playing program  
**Then** instrument recognizable as correct category (80%+ accuracy)

**Test Method**: Human listening test  
**Success Criteria**: Category identification

### AC2: Modulation Wheel Response
**Given** CC#1 modulation wheel on any program  
**When** adjusting modulation  
**Then** sound changes in musically appropriate way

**Test Method**: Controller response test  
**Success Criteria**: Expected modulation effects

### AC3: Volume Controller Response
**Given** CC#7 volume controller 0-127  
**When** adjusting volume  
**Then** smooth volume change from silence to maximum

**Test Method**: Volume response test  
**Success Criteria**: Smooth logarithmic response

### AC4: Velocity Sensitivity
**Given** Note On velocity 1-127  
**When** playing notes  
**Then** volume and/or timbre varies with velocity

**Test Method**: Velocity test  
**Success Criteria**: Minimum 2 velocity levels

### AC5: Cross-Device Consistency
**Given** same GM1 MIDI file on different GM devices  
**When** comparing playback  
**Then** recognizably similar musical result

**Test Method**: Multi-device comparison  
**Success Criteria**: Consistent musical interpretation

### AC6: Synthesis Method Flexibility
**Given** various synthesis implementations  
**When** implementing GM1  
**Then** any synthesis method acceptable if meets quality standards

**Test Method**: Implementation variety test  
**Success Criteria**: Multiple synthesis approaches work

### AC7: Quality Baseline Compliance
**Given** GM1 implementation  
**When** measuring technical specs  
**Then** meets minimum quality requirements (sample rate, bit depth, etc.)

**Test Method**: Technical measurement  
**Success Criteria**: Baseline specifications met

### AC8: Program Switching Performance
**Given** Program Change message  
**When** switching programs  
**Then** new program available within 100ms

**Test Method**: Switching latency test  
**Success Criteria**: <100ms program changes

## Dependencies
- zarfld/MIDI_1.0#537-539 (GM1 instruments, percussion, program changes)
- MIDI controller processing (CC#1, CC#7)
- Audio synthesis/playback engine
- Quality testing framework

## References
- **STR-GM1-Level-1**: RP-003 General MIDI System Level 1 Specification
- **MMA Developer Guidelines**: GM1 implementation recommendations
- **Audio Engineering Standards**: Sample rates, bit depths, frequency response
