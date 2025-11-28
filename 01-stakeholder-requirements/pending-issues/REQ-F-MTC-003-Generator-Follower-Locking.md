# REQ-F-MTC-003: MTC Generator and Follower Locking Behavior

## Type
- Functional Requirement (Extension)
- Phase: 02-requirements

## Description
Define behaviors for generator and follower roles including lock acquisition, loss, re-lock strategies, freewheel behavior, and dropout handling.

## Requirements
1. Follower detects lock within defined threshold after receiving consistent QF sequence.
2. On dropout, freewheel for bounded duration; expose status (locked, searching, freewheel).
3. Generator emits QF at exact cadence for selected frame rate; jitter bounded.
4. Provide callbacks/events on lock gain/loss with timestamps.
5. Optional smoothing when rate flags change; no retroactive drift.

## Acceptance Criteria
- Lock achieved within target frames; logged.
- Dropout triggers freewheel and status transitions; re-lock within bounds.
- Emission jitter within specified ms.

## Traceability
- Traces to: TRACK-MTC-Extension-Specification
- Depends on: REQ-F-MTC-001, REQ-F-MTC-002
- Verified by: TEST-MTC-LOCK-001..003

## References
- MIDI Time Code Specification (reference only)
