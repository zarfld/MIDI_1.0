# Test Cases: GM1 Instrument Loading and Sample Management

**Test Suite ID**: TEST-GM1-LOADING  
**Phase**: 07-Verification-Validation  
**Standards**: IEEE 1012-2016 (Verification and Validation)  
**Version**: 1.0.0  
**Author**: QA Team  
**Date**: 2025-11-28  
**Status**: Draft

## Traceability

- **Verifies**: #540 (REQ-F-GM1-005: GM1 Instrument Loading and Sample Management)
- **Related Design**: DES-C-GM1-SAMPLE-001
- **Depends on**: 
  - #537 (REQ-F-GM1-001: GM1 Melodic Instruments)
  - #538 (REQ-F-GM1-002: GM1 Percussion)
  - #539 (REQ-F-GM1-003: GM1 Program Change)

---

## Test Case: TEST-GM1-LOADING-001

### Complete GM1 Availability Test

**Test ID**: TEST-GM1-LOADING-001  
**Trace to**: #540 AC1 (Complete GM1 Availability)  
**Priority**: Critical (P0)  
**Type**: Functional  

#### Objective
Verify that all 128 melodic programs and 47 percussion sounds are available without user intervention when GM1 mode is active.

#### Preconditions
- System initialized in GM1 mode
- Sample manager fully initialized
- Appropriate loading strategy configured (ROM/Dynamic/Hybrid)

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Initialize GM1 Sample Manager | Manager returns initialized state | |
| 2 | For each program 1-128, call `gm1_is_program_available(program)` | Returns `true` for all 128 programs | |
| 3 | For each percussion key 35-81, call `gm1_is_percussion_available(key)` | Returns `true` for all 47 keys | |
| 4 | Attempt to get sample for each melodic program | Valid sample pointer returned (non-NULL) | |
| 5 | Attempt to get sample for each percussion key | Valid sample pointer returned (non-NULL) | |

#### Test Data
```c
// Melodic programs to test
uint8_t melodic_programs[128];  // 1-128

// Percussion keys to test
uint8_t percussion_keys[47] = {
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    77, 78, 79, 80, 81
};
```

#### Success Criteria
- All 128 melodic programs report available
- All 47 percussion sounds report available
- Total sounds accessible: 175

---

## Test Case: TEST-GM1-LOADING-002

### Program Change Latency Test (ROM Strategy)

**Test ID**: TEST-GM1-LOADING-002  
**Trace to**: #540 AC2 (Program Change Latency)  
**Priority**: High (P1)  
**Type**: Performance  

#### Objective
Verify that program changes complete within 100ms when using ROM-based loading strategy.

#### Preconditions
- System initialized with `STRATEGY_ROM`
- Performance measurement tools available

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Initialize with ROM strategy | Strategy set to ROM | |
| 2 | Start timer | Timer at 0ms | |
| 3 | Request program change from 1 to 64 | Program change initiated | |
| 4 | Stop timer when program ready | Time recorded | |
| 5 | Verify latency < 100ms | Latency within target | |
| 6 | Repeat for 20 random program changes | All latencies recorded | |
| 7 | Calculate P95 latency | P95 < 100ms | |

#### Test Data
```c
// Random program changes to test
typedef struct {
    uint8_t from_program;
    uint8_t to_program;
} program_change_test_t;

program_change_test_t test_changes[20] = {
    {1, 64}, {64, 128}, {128, 1}, {25, 33}, {33, 57},
    {57, 81}, {81, 97}, {97, 113}, {113, 25}, {1, 127},
    // ... additional test cases
};
```

#### Success Criteria
- All individual latencies < 100ms
- P95 latency < 100ms
- No program change failures

---

## Test Case: TEST-GM1-LOADING-003

### Program Change Latency Test (Dynamic Strategy)

**Test ID**: TEST-GM1-LOADING-003  
**Trace to**: #540 AC2 (Program Change Latency)  
**Priority**: High (P1)  
**Type**: Performance  

#### Objective
Verify that program changes complete within 2000ms (2 seconds) when using dynamic disk-based loading strategy.

#### Preconditions
- System initialized with `STRATEGY_DYNAMIC`
- Sample files available on disk
- Cache cleared before test

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Initialize with Dynamic strategy | Strategy set to DYNAMIC | |
| 2 | Clear sample cache | Cache empty | |
| 3 | Start timer | Timer at 0ms | |
| 4 | Request program change (uncached program) | Loading initiated | |
| 5 | Wait for load completion | Sample loaded | |
| 6 | Stop timer | Time recorded | |
| 7 | Verify latency < 2000ms | Latency within target | |
| 8 | Request same program again (cached) | Latency < 100ms | |

#### Success Criteria
- Uncached load latency < 2000ms
- Cached load latency < 100ms
- No loading errors

---

## Test Case: TEST-GM1-LOADING-004

### Memory Management and LRU Eviction Test

**Test ID**: TEST-GM1-LOADING-004  
**Trace to**: #540 AC3 (Memory Management)  
**Priority**: High (P1)  
**Type**: Functional  

#### Objective
Verify efficient cache management with LRU eviction when memory is constrained.

#### Preconditions
- System initialized with limited cache (e.g., MAX_LOADED_SAMPLES = 8)
- Dynamic loading strategy enabled

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Set cache limit to 8 samples | Limit configured | |
| 2 | Load programs 1-8 (fill cache) | 8 samples loaded, cache full | |
| 3 | Record access time for program 1 | Time T1 recorded | |
| 4 | Access programs 2-8 (update access times) | Access times updated | |
| 5 | Load program 9 (trigger eviction) | Program 1 evicted (oldest) | |
| 6 | Verify program 9 loaded | Program 9 in cache | |
| 7 | Verify program 1 unloaded | Program 1 not in cache | |
| 8 | Verify cache count = 8 | No memory leak | |
| 9 | Get cache memory usage | Within configured limit | |

#### Test Data
```c
#define TEST_CACHE_LIMIT 8
uint8_t programs_to_load[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
```

#### Success Criteria
- LRU eviction correctly identifies oldest sample
- Cache count never exceeds limit
- No memory leaks (verify with memory profiler)
- Memory usage within configured limit

---

## Test Case: TEST-GM1-LOADING-005

### Loading Error Recovery Test

**Test ID**: TEST-GM1-LOADING-005  
**Trace to**: #540 AC4 (Loading Error Recovery)  
**Priority**: High (P1)  
**Type**: Fault Tolerance  

#### Objective
Verify graceful fallback behavior when sample loading fails.

#### Preconditions
- System initialized with dynamic loading
- Test infrastructure to simulate loading failures

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Simulate FILE_NOT_FOUND for program 5 | Error detected | |
| 2 | Attempt to load program 5 | Loading fails | |
| 3 | Verify substitute program used | Program 1 (Piano) loaded as substitute | |
| 4 | Verify no crash | System stable | |
| 5 | Simulate OUT_OF_MEMORY error | Error detected | |
| 6 | Verify eviction triggered | Cache eviction performed | |
| 7 | Verify retry attempted | Load retried | |
| 8 | Simulate CORRUPT_DATA error | Error detected | |
| 9 | Verify synthesized fallback used | Simple waveform fallback active | |

#### Error Injection
```c
typedef enum {
    INJECT_FILE_NOT_FOUND,
    INJECT_OUT_OF_MEMORY,
    INJECT_CORRUPT_DATA
} error_injection_t;

void inject_loading_error(uint8_t program, error_injection_t error);
void clear_error_injection(void);
```

#### Success Criteria
- No silent failures (always have audio output)
- Appropriate substitute instrument selected
- System remains stable after errors
- Error logged for diagnostics

---

## Test Case: TEST-GM1-LOADING-006

### Background Loading Test

**Test ID**: TEST-GM1-LOADING-006  
**Trace to**: #540 AC5 (Background Loading)  
**Priority**: Medium (P2)  
**Type**: Functional  

#### Objective
Verify background loading of priority programs during idle time.

#### Preconditions
- System initialized with hybrid or dynamic strategy
- Background loader thread available
- Cache initially empty

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Start background loader | Loader thread running | |
| 2 | Wait 5 seconds (idle time) | Background loading occurs | |
| 3 | Check if ESSENTIAL programs loaded | Program 1, 25, 33 loaded | |
| 4 | Stop background loader | Thread stopped cleanly | |
| 5 | Verify no race conditions | No data corruption | |
| 6 | Verify essential programs ready | Instant access available | |

#### Success Criteria
- ESSENTIAL priority programs loaded during idle
- Background loading doesn't affect performance
- Thread cleanup is clean (no leaks)

---

## Test Case: TEST-GM1-LOADING-007

### Platform Adaptability Test

**Test ID**: TEST-GM1-LOADING-007  
**Trace to**: #540 AC6 (Platform Adaptability)  
**Priority**: Medium (P2)  
**Type**: Compatibility  

#### Objective
Verify appropriate loading strategy selection for different platform constraints.

#### Preconditions
- Test harness to simulate different platform configurations

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Configure as embedded system (low RAM) | Minimal cache configured | |
| 2 | Verify ROM strategy recommended | STRATEGY_ROM selected | |
| 3 | Configure as desktop (moderate RAM) | Standard cache configured | |
| 4 | Verify Dynamic strategy works | STRATEGY_DYNAMIC functional | |
| 5 | Configure as DAW (high RAM) | Full cache configured | |
| 6 | Verify Hybrid strategy works | STRATEGY_HYBRID functional | |
| 7 | Verify all configurations stable | No crashes or errors | |

#### Platform Configurations
```c
typedef struct {
    const char* name;
    uint32_t available_ram_mb;
    bool has_fast_storage;
    loading_strategy_t expected_strategy;
} platform_config_t;

platform_config_t test_platforms[] = {
    {"Embedded", 2, false, STRATEGY_ROM},
    {"Desktop", 64, true, STRATEGY_DYNAMIC},
    {"DAW", 256, true, STRATEGY_HYBRID}
};
```

#### Success Criteria
- Each platform configuration runs without errors
- Memory usage appropriate for platform
- Performance meets platform-specific targets

---

## Test Case: TEST-GM1-LOADING-008

### File Format Support Test

**Test ID**: TEST-GM1-LOADING-008  
**Trace to**: #540 AC7 (File Format Support)  
**Priority**: Medium (P2)  
**Type**: Compatibility  

#### Objective
Verify support for common sample file formats.

#### Preconditions
- Test samples available in multiple formats
- Dynamic loading strategy enabled

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Load WAV format sample (16-bit) | Sample loaded successfully | |
| 2 | Load WAV format sample (24-bit) | Sample loaded successfully | |
| 3 | Load AIFF format sample | Sample loaded successfully | |
| 4 | Load compressed format (OGG) | Sample loaded and decompressed | |
| 5 | Verify audio quality for each | Playback matches reference | |
| 6 | Load unsupported format | LOAD_ERROR_UNSUPPORTED_FORMAT returned | |

#### Test Files
```
test-samples/
├── piano-16bit.wav
├── piano-24bit.wav
├── piano.aiff
├── piano.ogg
└── piano.unsupported
```

#### Success Criteria
- WAV (16/24-bit) loading works
- AIFF loading works
- Compressed format decompression works
- Unsupported formats return proper error

---

## Test Case: TEST-GM1-LOADING-009

### Compression Efficiency Test

**Test ID**: TEST-GM1-LOADING-009  
**Trace to**: #540 AC8 (Compression Efficiency)  
**Priority**: Low (P3)  
**Type**: Performance  

#### Objective
Verify efficient compression without unacceptable quality loss.

#### Preconditions
- Uncompressed reference samples available
- Compression algorithms available

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Measure uncompressed size (full GM1 set) | Size recorded (e.g., 100MB) | |
| 2 | Apply ADPCM compression | 4:1 compression ratio | |
| 3 | Measure compressed size | ~25MB | |
| 4 | Compare audio quality (A/B test) | No audible artifacts | |
| 5 | Measure decompression time | Within latency targets | |

#### Quality Assessment
```c
typedef struct {
    compression_type_t type;
    float expected_ratio;
    float max_quality_loss_db;
    float max_decompression_ms;
} compression_test_t;

compression_test_t tests[] = {
    {COMPRESSION_ADPCM, 4.0, -60.0, 10.0},
    {COMPRESSION_OGG_VORBIS, 10.0, -40.0, 50.0}
};
```

#### Success Criteria
- Compression ratio meets targets
- Audio quality acceptable (SNR within limits)
- Decompression doesn't exceed latency targets

---

## Test Case: TEST-GM1-LOADING-010

### Concurrent Access Stress Test

**Test ID**: TEST-GM1-LOADING-010  
**Trace to**: #540 (General robustness)  
**Priority**: Medium (P2)  
**Type**: Stress  

#### Objective
Verify thread-safe concurrent access to sample manager.

#### Preconditions
- Multi-threaded test environment
- Sample manager initialized

#### Test Steps

| Step | Action | Expected Result | Status |
|------|--------|-----------------|--------|
| 1 | Create 4 concurrent threads | Threads running | |
| 2 | Each thread performs 100 program changes | All changes processed | |
| 3 | Monitor for race conditions | No data corruption | |
| 4 | Monitor for deadlocks | No hangs | |
| 5 | Verify cache integrity | Cache consistent | |
| 6 | Run for 60 seconds | Stable operation | |

#### Success Criteria
- No race conditions detected
- No deadlocks
- Cache remains consistent
- All threads complete successfully

---

## Test Summary Matrix

| Test ID | AC# | Priority | Type | Status |
|---------|-----|----------|------|--------|
| TEST-GM1-LOADING-001 | AC1 | P0 | Functional | Pending |
| TEST-GM1-LOADING-002 | AC2 | P1 | Performance | Pending |
| TEST-GM1-LOADING-003 | AC2 | P1 | Performance | Pending |
| TEST-GM1-LOADING-004 | AC3 | P1 | Functional | Pending |
| TEST-GM1-LOADING-005 | AC4 | P1 | Fault Tolerance | Pending |
| TEST-GM1-LOADING-006 | AC5 | P2 | Functional | Pending |
| TEST-GM1-LOADING-007 | AC6 | P2 | Compatibility | Pending |
| TEST-GM1-LOADING-008 | AC7 | P2 | Compatibility | Pending |
| TEST-GM1-LOADING-009 | AC8 | P3 | Performance | Pending |
| TEST-GM1-LOADING-010 | - | P2 | Stress | Pending |

## Test Environment Requirements

### Hardware
- Minimum: 4GB RAM, dual-core CPU
- Storage: SSD recommended for dynamic loading tests

### Software
- Test framework (e.g., Unity, CUnit)
- Performance measurement tools
- Memory profiler (e.g., Valgrind)
- Thread sanitizer

### Test Data
- Complete GM1 sample set in multiple formats
- Error injection test harness
- Platform simulation environment

---

**Review Status**: Draft - Pending review  
**Next Steps**:
1. Set up test environment
2. Create test automation scripts
3. Execute tests during implementation phase
