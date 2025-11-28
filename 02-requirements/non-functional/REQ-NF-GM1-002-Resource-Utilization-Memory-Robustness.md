# REQ-NF-GM1-002: Memory Footprint, Resource Utilization, and Robustness

## Metadata
- **Type**: Non-Functional Requirement
- **Category**: Resource / Memory / Robustness / Reliability
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: P1
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Deploy-01 (Embeddable implementation), STR-GM1-Reliability-01
- **Depends on**: REQ-F-GM1-006 (voice pool), REQ-F-GM1-013..015 (mode state), REQ-F-GM1-016..018 (pitch core)  
- **Verified by (planned)**: TEST-GM1-MEM-001 (Static footprint), TEST-GM1-ROBUST-001 (Boundary behaviors)

## Description
Define target memory footprint, resource ceilings, and robustness constraints (error handling, boundary safety, integrity preservation under stress) for a GM1 compliant implementation.

## Requirements
1. NFR1: Static RAM footprint for core GM1 state (24 voices, controller states, RPN, mode) SHOULD be ≤ 32 KB (excluding sample/audio buffers).  
2. NFR2: Heap usage after initialization MUST be zero for real-time critical code paths (voice allocation, controller update, pitch calc).  
3. NFR3: Each channel MUST maintain integrity under invalid inputs (out-of-range keys, malformed SysEx) without memory corruption or crash.  
4. NFR4: Voice pool index bounds MUST be validated; no out-of-range access allowed (defensive asserts optional in debug builds).  
5. NFR5: All arrays (voices, channels, controller states) MUST initialize explicitly (no reliance on zeroed memory assumptions from allocator).  
6. NFR6: Error codes MUST be standardized (enum) and never return ambiguous negative values (< -128) that risk wrap on narrow types.  
7. NFR7: Memory fragmentation MUST be avoided; if dynamic allocation used for optional features, pool or arena strategy MUST ensure deterministic reclaim.  
8. NFR8: Robustness under malformed or rapid SysEx stream: parser MUST discard invalid sequences and recover sync without state leak (no incremental heap growth).  
9. NFR9: Over-allocation attempts (requesting > max voices) MUST return defined error (e.g., GM1_ERR_LIMIT) without partial state modification.  
10. NFR10: Snapshot/restore operations (mode state) MUST produce identical state after round-trip (hash equality of serialized segment).  
11. NFR11: Pitch computation for invalid key (<0 or >127) MUST clamp safely and signal GM1_WARN_KEY_RANGE.  
12. NFR12: Memory guard tests MUST detect no read/write outside allocated regions using instrumentation (address sanitizer or custom sentinel pattern in embedded).  
13. NFR13: CPU spike resilience: under burst load memory usage remains constant (peak fluctuations < 1%).  
14. NFR14: Provide metrics API: {static_bytes, dynamic_bytes, heap_ops_after_init, snapshot_size_bytes}.  
15. NFR15: Data structures MUST avoid false sharing (cache line overlap) for high-frequency fields (voice state vs controller state) in multicore contexts.

## Acceptance Criteria
1. AC1: Measured static footprint ≤ 32 KB for baseline configuration.  
2. AC2: Heap operation counter after initialization remains 0 during 60 s stress test.  
3. AC3: Boundary key tests produce clamped values and warnings without faults.  
4. AC4: SysEx fuzzing (random noise + valid frames) yields 0 crashes, stable memory usage.  
5. AC5: Snapshot→restore hash equality confirmed across 10 iterations.  
6. AC6: Address sanitizer / sentinel test reports 0 out-of-bounds violations.  
7. AC7: Metrics API returns consistent values across repeated queries (no unbounded growth).  
8. AC8: Burst load memory delta < 1% relative to baseline.  
9. AC9: False sharing mitigation test (sampling performance counters) shows no >10% stall penalty vs separated layout baseline.

## Risks / Considerations
- Cache line optimization may complicate struct readability; balance clarity vs performance.  
- Pool strategies must handle optional feature enable/disable toggles gracefully.  
- Fuzz testing required to uncover latent parser issues.

## References
- Embedded resource constraints guidance  
- Robust MIDI parsing principles (resynchronization, bounds)  
- GM1 mode snapshot functional requirements
