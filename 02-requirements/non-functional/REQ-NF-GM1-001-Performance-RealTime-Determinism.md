# REQ-NF-GM1-001: Performance, Real-Time Determinism, and Latency Bounds

## Metadata
- **Type**: Non-Functional Requirement
- **Category**: Performance / Timing / Determinism
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: P0 (Critical)
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Level-1 (Cross-device musical compatibility), STR-GM1-RT-01 (Real-time playback integrity)
- **Depends on**: REQ-F-GM1-006..015 (polyphony, controllers, mode), REQ-F-GM1-016..018 (pitch & tuning)
- **Verified by (planned)**: TEST-GM1-PERF-001 (Allocation latency), TEST-GM1-PERF-002 (Steady tick jitter), TEST-GM1-PERF-003 (Bend update latency)

## Description
Define hard and soft performance targets, deterministic behavior expectations, and latency bounds required for GM1 compliant real-time operation on reference desktop and constrained embedded targets.

## Requirements
1. NFR1: Maximum voice allocation path (note-on to voice ACTIVE) MUST complete < 500 µs (desktop reference) and < 2000 µs (embedded baseline @ 200 MHz) for worst-case steal scenario at 24 voices.  
2. NFR2: Controller updates (CC1/7/10/11/64) MUST apply state changes before next audio block boundary; if block size = 64 frames @ 48 kHz, effective latency < 1.4 ms.  
3. NFR3: Pitch bend and RPN sensitivity changes MUST reflect in synthesis within one processing cycle (< audio block duration).  
4. NFR4: Jitter (variance between scheduled tick events, e.g., clock or modulation update) MUST remain < ±1 ms over 10 min continuous playback (desktop) and < ±2 ms (embedded).  
5. NFR5: Voice stealing decision MUST execute in O(V) where V ≤ 128; for V=24 target runtime < 50 µs average, < 150 µs worst-case.  
6. NFR6: Frequency calculation for pitched notes MUST support ≥10k queries/sec without exceeding 10% CPU on reference desktop baseline (single thread).  
7. NFR7: Allocation and controller operations MUST be lock-free or use bounded wait primitives ensuring worst-case block < 200 µs (no unbounded mutex contention).  
8. NFR8: Memory allocation in real-time paths MUST be static or pooled; no heap allocation after initialization (zero malloc/new calls in note-on fast path).  
9. NFR9: Active sensing / timing watchdog (if enabled) MUST detect >300 ms silence of expected activity and raise a diagnostic event within 50 ms thereafter.  
10. NFR10: Deterministic ordering: events with identical timestamp MUST process in stable priority sequence (Note-On before Controller before Pitch Bend) ensuring repeatable results.  
11. NFR11: Performance metrics MUST be measurable via instrumentation API returning: {avg_alloc_us, worst_alloc_us, jitter_ms, bend_latency_ms}.  
12. NFR12: Under sustained 100 notes/sec input (stress), missed allocs (dropped Note-On) MUST be 0 while CPU ≤ 70% (desktop).  
13. NFR13: Mode initialization (GM On) MUST complete < 5 ms (desktop) and < 25 ms (embedded) including controller resets.

## Measurement Methodology
- High-resolution timer (microseconds) around allocation and steal path.  
- Jitter measured as deviation from expected periodic callback (e.g., 1 ms scheduler).  
- Bench harness issues synthetic streams (note bursts + controllers) and logs metrics.  
- Determinism confirmed by repeated identical event sequences producing identical allocation ordering traces.

## Acceptance Criteria
1. AC1: Worst-case voice allocation (full pool + steal) ≤ specified limits (perf log).  
2. AC2: Controller latency < block duration; measured median < 1 ms.  
3. AC3: Bend change visible next synthesis block (frequency diff logged).  
4. AC4: Logged jitter standard deviation < 0.3 ms; max < 1 ms (desktop).  
5. AC5: 100 notes/sec stress test completes 60 s with 0 drops.  
6. AC6: No dynamic allocations detected by allocator hook after init phase.  
7. AC7: Instrumentation API reports values within specified bounds.  
8. AC8: Repeat run (same event script) yields identical allocation order sequences (hash match).  
9. AC9: GM On initialization time within limits with full controller reset.

## Risks / Considerations
- Timer precision differences across platforms; profiling harness must abstract high-res source.  
- Large polyphony expansions (>128) may require optimized data structures beyond O(V) scanning.  
- Lock-free design complexity vs portability.

## References
- Real-time MIDI performance principles (latency, jitter, determinism)
- GM1 polyphony and controller functional requirements
