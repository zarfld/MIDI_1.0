# TRACK: MIDI 1.0 Extension — MIDI Time Code (MTC) Specification Planning

## Type
- Tracking / Epic
- Phase: 02-requirements → 03-architecture → 04-design → 05-implementation → 07-V&V

## Context
Plan and track the MIDI Time Code (MTC) extension workstream, covering functional requirements, architecture, design, implementation, and verification for MTC (Quarter Frame and Full Frame behaviors) without reproducing MMA copyrighted content.

## Objectives
- Define functional requirements for MTC Quarter Frame and Full Frame
- Specify generator and follower behavior, frame rates, and locking
- Integrate with GM1 timing/clock handling deterministically
- Provide tests and conformance criteria

## Child Issues (to create on GitHub)
- REQ-F-MTC-001: Quarter Frame Message Handling
- REQ-F-MTC-002: Full Frame SysEx Set Timecode
- REQ-F-MTC-003: Generator/Follower Locking Behavior
- REQ-F-MTC-004: Interop with MIDI Clock (24 PPQN) and Transport

## Traceability
- Traces to: STR-TIMING-01 (Timeline synchronization across devices)
- Depends on: GM1 core timing and controller base
- Will be verified by: TEST-MTC-* suite

## Acceptance Criteria
- All child issues created and linked in GitHub Issues
- Architecture + design docs added under `03-architecture/` and `04-design/`
- Implementation added under `05-implementation/` with tests in `07-verification-validation/`

## References
- MIDI Time Code Specification (MMA) — reference only (no reproduction)
- GM1 requirements for timing and system messages
