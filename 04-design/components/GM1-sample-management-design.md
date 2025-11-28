# GM1 Sample Management Component Design

**Component ID**: DES-C-GM1-SAMPLE-001  
**Phase**: 04-Design  
**Standards**: IEEE 1016-2009 (Software Design Descriptions)  
**Version**: 1.0.0  
**Author**: Design Team  
**Date**: 2025-11-28  
**Status**: Draft

## Traceability

- **Implements**: #540 (REQ-F-GM1-005: GM1 Instrument Loading and Sample Management)
- **Depends on**: 
  - #537 (REQ-F-GM1-001: GM1 Melodic Instrument Sound Set)
  - #538 (REQ-F-GM1-002: GM1 Percussion Sound Set)
  - #539 (REQ-F-GM1-003: GM1 Program Change Processing)
- **Verified by**: TEST-GM1-LOADING-* (pending)
- **Architecture**: ADR-GM1-SAMPLE-001 (pending)

## Purpose

Manage loading and availability of GM1 instrument samples and synthesis parameters. This component ensures all 128 melodic programs and 47 percussion sounds are accessible without user intervention, supporting efficient sample management for memory-constrained systems while maintaining real-time performance.

## Responsibilities

1. **Sample Data Access**: Provide unified access to GM1 sample data (ROM or disk-based)
2. **Cache Management**: Implement LRU caching for memory-constrained environments
3. **Priority Loading**: Support priority-based loading strategies
4. **Background Loading**: Pre-load anticipated samples during idle time
5. **Error Recovery**: Graceful fallback when loading fails
6. **Format Support**: Handle multiple sample formats (WAV, AIFF, compressed)

## Domain Model (DDD Patterns)

### Entities

#### GM1SampleManager (Aggregate Root)
```c
/**
 * GM1 Sample Manager - Aggregate Root
 * ID: ENTITY-GM1-SAMPLE-MGR-001
 * Traceability: Implements #540
 */
typedef struct {
    // Identity
    uint32_t            id;
    
    // Strategy selection
    loading_strategy_t  strategy;           // ROM, DYNAMIC, or HYBRID
    
    // Sample database
    union {
        gm1_rom_sample_t*      rom_samples;      // ROM-based samples
        gm1_dynamic_sample_t*  dynamic_samples;  // Disk-based samples
    };
    
    // Cache management (for DYNAMIC and HYBRID strategies)
    sample_cache_t*     cache;
    
    // Configuration
    gm1_sample_config_t config;
    
    // State
    bool                initialized;
    uint8_t             loaded_programs_count;
    uint8_t             loaded_percussion_count;
} gm1_sample_manager_t;
```

#### GM1Sample (Entity)
```c
/**
 * GM1 Sample - Internal Entity
 * ID: ENTITY-GM1-SAMPLE-001
 * Traceability: Supports #540
 */
typedef struct {
    // Identity
    uint8_t             program_number;     // 1-128 for melodic, 35-81 for percussion
    bool                is_percussion;
    
    // Sample metadata
    uint32_t            sample_length;
    uint16_t            sample_rate;
    uint8_t             bit_depth;
    uint8_t             channels;
    
    // Loop points
    uint32_t            loop_start;
    uint32_t            loop_end;
    loop_type_t         loop_type;          // NONE, FORWARD, BIDIRECTIONAL
    
    // Pitch information
    uint8_t             root_key;           // MIDI note number
    int8_t              fine_tune;          // Cents (-99 to +99)
    
    // Loading state
    sample_state_t      state;              // UNLOADED, LOADING, LOADED, ERROR
    load_priority_t     priority;
    
    // Data pointer (NULL if unloaded)
    uint8_t*            data;
} gm1_sample_t;
```

### Value Objects

#### SampleIdentifier
```c
/**
 * Sample Identifier - Value Object
 * ID: VO-SAMPLE-ID-001
 */
typedef struct {
    uint8_t     program_number;     // 1-128 or 35-81
    bool        is_percussion;      // True for percussion samples
} sample_identifier_t;

// Factory method with validation
sample_identifier_t create_sample_id(uint8_t program, bool is_percussion) {
    sample_identifier_t id = {0};
    
    if (is_percussion) {
        // Percussion keys 35-81
        if (program >= 35 && program <= 81) {
            id.program_number = program;
            id.is_percussion = true;
        }
    } else {
        // Melodic programs 1-128
        if (program >= 1 && program <= 128) {
            id.program_number = program;
            id.is_percussion = false;
        }
    }
    
    return id;
}

// Equality based on attributes
bool sample_id_equals(sample_identifier_t a, sample_identifier_t b) {
    return (a.program_number == b.program_number) && 
           (a.is_percussion == b.is_percussion);
}
```

#### LoadingResult
```c
/**
 * Loading Result - Value Object
 * ID: VO-LOAD-RESULT-001
 */
typedef enum {
    LOAD_SUCCESS = 0,
    LOAD_IN_PROGRESS,
    LOAD_ERROR_FILE_NOT_FOUND,
    LOAD_ERROR_OUT_OF_MEMORY,
    LOAD_ERROR_CORRUPT_DATA,
    LOAD_ERROR_UNSUPPORTED_FORMAT
} load_result_t;

typedef struct {
    load_result_t   result;
    uint32_t        bytes_loaded;
    uint32_t        load_time_ms;
    const char*     error_message;
} loading_result_t;
```

### Enumerations

```c
/**
 * Loading Strategy - Strategy Pattern
 * ID: ENUM-LOAD-STRATEGY-001
 */
typedef enum {
    STRATEGY_ROM = 0,       // Pre-loaded ROM (hardware synthesizers)
    STRATEGY_DYNAMIC,       // On-demand loading (software/disk-based)
    STRATEGY_HYBRID         // Combination approach (optimized systems)
} loading_strategy_t;

/**
 * Sample State
 * ID: ENUM-SAMPLE-STATE-001
 */
typedef enum {
    SAMPLE_UNLOADED = 0,
    SAMPLE_LOADING,
    SAMPLE_LOADED,
    SAMPLE_ERROR
} sample_state_t;

/**
 * Load Priority
 * ID: ENUM-LOAD-PRIORITY-001
 */
typedef enum {
    PRIORITY_ESSENTIAL = 0,  // Always keep loaded (Piano, basic drums)
    PRIORITY_HIGH = 1,       // Load quickly, keep cached (Guitar, Bass)
    PRIORITY_MEDIUM = 2,     // Load on-demand, normal caching
    PRIORITY_LOW = 3         // Load on-demand, evict quickly (Sound effects)
} load_priority_t;

/**
 * Loop Type
 * ID: ENUM-LOOP-TYPE-001
 */
typedef enum {
    LOOP_NONE = 0,
    LOOP_FORWARD,
    LOOP_BIDIRECTIONAL
} loop_type_t;
```

## Repository Interface

```c
/**
 * GM1 Sample Repository Interface (Domain Layer)
 * ID: REPO-GM1-SAMPLE-001
 * Traceability: Supports #540
 */
typedef struct {
    // Retrieval methods
    gm1_sample_t*   (*get_melodic_sample)(uint8_t program);
    gm1_sample_t*   (*get_percussion_sample)(uint8_t key);
    
    // Query methods
    bool            (*is_sample_loaded)(sample_identifier_t id);
    uint8_t         (*get_loaded_count)(void);
    
    // Loading operations
    load_result_t   (*load_sample)(sample_identifier_t id);
    load_result_t   (*load_sample_async)(sample_identifier_t id);
    void            (*unload_sample)(sample_identifier_t id);
    
    // Batch operations
    void            (*load_all_essential)(void);
    void            (*unload_all)(void);
    
    // Cache management
    void            (*evict_oldest)(void);
    uint32_t        (*get_cache_usage)(void);
} gm1_sample_repository_t;
```

## Component Interfaces

### Public API

```c
/**
 * GM1 Sample Manager Public Interface
 * ID: INT-GM1-SAMPLE-001
 * Traceability: Implements #540
 */

// Initialization
gm1_sample_manager_t* gm1_sample_manager_create(loading_strategy_t strategy);
void                  gm1_sample_manager_destroy(gm1_sample_manager_t* mgr);
void                  gm1_sample_manager_init(gm1_sample_manager_t* mgr);

// Sample access (main API)
const gm1_sample_t*   gm1_get_program_sample(gm1_sample_manager_t* mgr, uint8_t program);
const gm1_sample_t*   gm1_get_percussion_sample(gm1_sample_manager_t* mgr, uint8_t key);

// Loading control
load_result_t         gm1_request_program(gm1_sample_manager_t* mgr, uint8_t program);
load_result_t         gm1_ensure_program_loaded(gm1_sample_manager_t* mgr, uint8_t program);
void                  gm1_preload_programs(gm1_sample_manager_t* mgr, const uint8_t* programs, uint8_t count);

// Cache management
void                  gm1_set_cache_limit(gm1_sample_manager_t* mgr, uint32_t max_bytes);
uint32_t              gm1_get_cache_usage(gm1_sample_manager_t* mgr);
void                  gm1_clear_cache(gm1_sample_manager_t* mgr);

// Background loading
void                  gm1_start_background_loader(gm1_sample_manager_t* mgr);
void                  gm1_stop_background_loader(gm1_sample_manager_t* mgr);
void                  gm1_analyze_midi_for_preload(gm1_sample_manager_t* mgr, 
                                                    const uint8_t* midi_data, uint32_t length);

// Status and diagnostics
bool                  gm1_is_program_available(gm1_sample_manager_t* mgr, uint8_t program);
bool                  gm1_is_percussion_available(gm1_sample_manager_t* mgr, uint8_t key);
void                  gm1_get_loading_status(gm1_sample_manager_t* mgr, gm1_loading_status_t* status);
```

### Internal Interfaces

```c
/**
 * Internal Cache Manager Interface
 * ID: INT-CACHE-001
 */
typedef struct sample_cache {
    gm1_sample_t*   samples[MAX_LOADED_SAMPLES];
    uint8_t         count;
    uint32_t        total_memory_used;
    uint32_t        max_memory;
    
    // Operations
    bool            (*add)(struct sample_cache* cache, gm1_sample_t* sample);
    void            (*remove)(struct sample_cache* cache, sample_identifier_t id);
    void            (*update_access_time)(struct sample_cache* cache, sample_identifier_t id);
    gm1_sample_t*   (*find_oldest)(struct sample_cache* cache);
    void            (*evict_oldest)(struct sample_cache* cache);
} sample_cache_t;

/**
 * Internal File Loader Interface
 * ID: INT-FILE-LOADER-001
 */
typedef struct {
    load_result_t   (*load_wav)(const char* path, gm1_sample_t* sample);
    load_result_t   (*load_aiff)(const char* path, gm1_sample_t* sample);
    load_result_t   (*load_compressed)(const char* path, gm1_sample_t* sample);
    void            (*start_async_load)(const char* path, gm1_sample_t* sample, 
                                        void (*callback)(load_result_t, void*), void* context);
} file_loader_t;
```

## Sequence Diagrams

### Program Change with Sample Loading
```
sequenceDiagram
    participant MIDI as MIDI Parser
    participant CH as Channel Manager
    participant SM as Sample Manager
    participant CACHE as Sample Cache
    participant FS as File System
    participant SYN as Synthesizer

    MIDI->>CH: handleProgramChange(channel, program)
    CH->>SM: gm1_ensure_program_loaded(program)
    
    alt Sample already loaded
        SM->>CACHE: find(program)
        CACHE-->>SM: sample (loaded)
        SM-->>CH: LOAD_SUCCESS
    else Sample not loaded
        SM->>CACHE: find(program)
        CACHE-->>SM: NULL
        
        alt Cache has space
            SM->>FS: load_sample_async(program)
            FS-->>SM: LOAD_IN_PROGRESS
            SM-->>CH: LOAD_IN_PROGRESS
            
            Note over FS: Asynchronous loading
            FS->>SM: callback(LOAD_SUCCESS)
            SM->>CACHE: add(sample)
        else Cache full
            SM->>CACHE: evict_oldest()
            CACHE-->>SM: evicted sample
            SM->>FS: load_sample_async(program)
        end
    end
    
    CH->>SYN: set_program(channel, program)
    CH-->>MIDI: success
```

### Background Pre-loading
```
sequenceDiagram
    participant BG as Background Thread
    participant SM as Sample Manager
    participant CACHE as Sample Cache
    participant FS as File System

    loop While system running
        BG->>SM: get_next_priority_program()
        SM-->>BG: program (ESSENTIAL priority)
        
        alt Cache has space
            BG->>SM: load_sample_async(program)
            SM->>FS: load_wav(path)
            FS-->>SM: sample data
            SM->>CACHE: add(sample)
        else Cache full
            BG->>BG: sleep(100ms)
        end
    end
```

## Error Handling Strategy

### Loading Failure Recovery
```c
/**
 * Error Handling - Fallback Strategy
 * ID: ERR-FALLBACK-001
 * Traceability: AC4 of #540
 */

typedef enum {
    FALLBACK_SILENT = 0,        // No sound output
    FALLBACK_SUBSTITUTE,        // Use substitute instrument
    FALLBACK_SYNTHESIZED        // Generate simple waveform
} fallback_strategy_t;

void handle_program_load_failure(gm1_sample_manager_t* mgr, 
                                  uint8_t program, 
                                  load_result_t error) {
    switch (error) {
        case LOAD_ERROR_FILE_NOT_FOUND:
            // Use substitute instrument from same family
            use_substitute_program(mgr, program);
            log_warning("Sample file not found for program %d, using substitute", program);
            break;
            
        case LOAD_ERROR_OUT_OF_MEMORY:
            // Evict old samples and retry
            evict_oldest_sample(mgr->cache);
            load_result_t retry = gm1_request_program(mgr, program);
            if (retry != LOAD_SUCCESS) {
                use_substitute_program(mgr, program);
            }
            break;
            
        case LOAD_ERROR_CORRUPT_DATA:
            // Use synthesized fallback
            use_synthesized_fallback(mgr, program);
            log_error("Corrupt sample data for program %d", program);
            break;
            
        default:
            use_substitute_program(mgr, program);
            break;
    }
}

uint8_t get_substitute_program(uint8_t failed_program) {
    // Return substitute from same instrument family
    // Piano family (1-8)
    if (failed_program >= 1 && failed_program <= 8) return 1;
    // Chromatic percussion (9-16)
    if (failed_program >= 9 && failed_program <= 16) return 9;
    // Organ (17-24)
    if (failed_program >= 17 && failed_program <= 24) return 17;
    // Guitar (25-32)
    if (failed_program >= 25 && failed_program <= 32) return 25;
    // Bass (33-40)
    if (failed_program >= 33 && failed_program <= 40) return 33;
    // Strings (41-48)
    if (failed_program >= 41 && failed_program <= 48) return 41;
    // Ensemble (49-56)
    if (failed_program >= 49 && failed_program <= 56) return 49;
    // Brass (57-64)
    if (failed_program >= 57 && failed_program <= 64) return 57;
    // Reed (65-72)
    if (failed_program >= 65 && failed_program <= 72) return 65;
    // Pipe (73-80)
    if (failed_program >= 73 && failed_program <= 80) return 73;
    // Synth Lead (81-88)
    if (failed_program >= 81 && failed_program <= 88) return 81;
    // Synth Pad (89-96)
    if (failed_program >= 89 && failed_program <= 96) return 89;
    // Synth Effects (97-104)
    if (failed_program >= 97 && failed_program <= 104) return 97;
    // Ethnic (105-112)
    if (failed_program >= 105 && failed_program <= 112) return 105;
    // Percussive (113-120)
    if (failed_program >= 113 && failed_program <= 120) return 113;
    // Sound Effects (121-128)
    if (failed_program >= 121 && failed_program <= 128) return 121;
    
    // Default fallback: Piano
    return 1;
}
```

## Configuration

### Default Priority Assignment
```c
/**
 * Default Program Priorities
 * ID: CONFIG-PRIORITIES-001
 * Based on typical usage statistics
 */
const load_priority_t gm1_default_priorities[128] = {
    // Piano (1-8)
    PRIORITY_ESSENTIAL, PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_MEDIUM,
    PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    
    // Chromatic Percussion (9-16)
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_LOW, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_LOW,
    
    // Organ (17-24)
    PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_LOW,
    
    // Guitar (25-32)
    PRIORITY_ESSENTIAL, PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_HIGH,
    PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_LOW,
    
    // Bass (33-40)
    PRIORITY_ESSENTIAL, PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_HIGH,
    
    // Strings (41-48)
    PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_MEDIUM,
    
    // Ensemble (49-56)
    PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_LOW, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    
    // Brass (57-64)
    PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    
    // Reed (65-72)
    PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    PRIORITY_MEDIUM, PRIORITY_LOW, PRIORITY_MEDIUM, PRIORITY_MEDIUM,
    
    // Pipe (73-80)
    PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    
    // Synth Lead (81-88)
    PRIORITY_HIGH, PRIORITY_HIGH, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    
    // Synth Pad (89-96)
    PRIORITY_MEDIUM, PRIORITY_MEDIUM, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    
    // Synth Effects (97-104)
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    
    // Ethnic (105-112)
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    
    // Percussive (113-120)
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_MEDIUM, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_MEDIUM, PRIORITY_LOW, PRIORITY_LOW,
    
    // Sound Effects (121-128)
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW,
    PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW, PRIORITY_LOW
};
```

## Performance Considerations

### Latency Targets
| Operation | ROM Strategy | Dynamic Strategy | Hybrid Strategy |
|-----------|--------------|------------------|-----------------|
| Program Change (cached) | < 1ms | < 5ms | < 5ms |
| Program Change (uncached) | N/A | < 2000ms | < 500ms |
| Note Playback Start | < 1ms | < 1ms | < 1ms |
| Background Load | N/A | < 5000ms per sample | < 2000ms per sample |

### Memory Targets
| Configuration | ROM Size | Dynamic Cache | Typical Usage |
|---------------|----------|---------------|---------------|
| Minimal | 8 MB | 2 MB (8 samples) | Embedded systems |
| Standard | 32 MB | 8 MB (32 samples) | Desktop software |
| Full | 128 MB | 32 MB (all samples) | Professional DAWs |

## Dependencies

### Internal Dependencies
- MIDI Parser (#18-25): Receives Program Change messages
- Channel Manager (#539): Routes program change requests
- Voice Allocation: Provides sample data for synthesis
- Audio Engine: Sample playback

### External Dependencies
- File System: Sample file storage and loading
- Memory Manager: Dynamic allocation
- Threading: Background loading thread
- Compression Library: For compressed sample support

## Testing Strategy

See: [TEST-GM1-LOADING-*](../../07-verification-validation/test-cases/TEST-GM1-LOADING.md)

### Unit Testing
- Sample identifier validation
- Priority assignment logic
- Substitute program selection
- Cache eviction algorithm

### Integration Testing
- Program change latency measurement
- Background loading coordination
- Error recovery scenarios
- Multi-threaded access

## Design Rationale

### Why Strategy Pattern for Loading?
Different deployment targets have vastly different constraints:
- **ROM**: Hardware synthesizers with fixed ROM
- **Dynamic**: Software synthesizers with disk access
- **Hybrid**: Professional systems with SSD and RAM

Using Strategy Pattern allows the same code to adapt to all platforms.

### Why LRU for Cache Eviction?
LRU (Least Recently Used) is simple, predictable, and effective for typical MIDI usage patterns where recently used instruments are likely to be used again soon.

### Why Priority-Based Loading?
Musical compositions typically use a predictable set of instruments (Piano, Bass, Drums, Guitar). Pre-loading these high-priority instruments reduces latency for common use cases.

## Compliance Mapping

| Standard Clause | How Addressed |
|-----------------|---------------|
| IEEE 1016 §4 (Design Entities) | Component structure with clear responsibilities |
| IEEE 1016 §5 (Design Attributes) | Interfaces, data structures, algorithms defined |
| IEEE 1016 §6 (Design Relationships) | Dependencies and traceability documented |
| ISO 42010 (Architecture) | Views and concerns addressed |

---

**Review Status**: Draft - Pending architecture review  
**Next Steps**: 
1. Create ADR for loading strategy selection
2. Implement unit tests
3. Review with audio engine team
