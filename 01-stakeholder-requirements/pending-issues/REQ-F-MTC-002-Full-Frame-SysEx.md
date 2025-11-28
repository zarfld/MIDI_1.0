# REQ-F-MTC-002: MTC Full Frame SysEx Set Timecode

## Type
- Functional Requirement (Extension)
- Phase: 02-requirements

## Description
Support Full Frame SysEx to set absolute timecode and synchronize generator/follower state, including frame rate field handling.

## Requirements
1. Parse/validate Full Frame SysEx payload; reject invalid manufacturer or length.
2. Update internal timecode atomically; notify dependent components.
3. Honor frame rate field; switch rates safely.
4. Provide API to request Full Frame transmit (generator mode) with rate.
5. Coexistence rules: Full Frame overrides in-flight Quarter Frame assembly deterministically.

## Acceptance Criteria
- Setting time via SysEx reflected in subsequent Quarter Frame emissions.
- Invalid SysEx does not corrupt state; error logged.
- Rate changes do not produce illegal intermediate states.

## Traceability
- Traces to: TRACK-MTC-Extension-Specification
- Depends on: REQ-F-MTC-001
- Verified by: TEST-MTC-FF-001..002

## References
- MIDI Time Code Specification (reference only)
