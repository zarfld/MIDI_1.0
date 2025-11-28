# REQ-F-MTC-001: MTC Quarter Frame Message Handling

## Type
- Functional Requirement (Extension)
- Phase: 02-requirements

## Description
Handle MIDI Time Code Quarter Frame messages, updating timecode state nibble-by-nibble with correct frame rate semantics and deterministic application order.

## Requirements
1. Parse and validate Quarter Frame status and data format; ignore malformed.
2. Maintain current timecode across 8 nibbles; apply atomically on completion or per accepted strategy.
3. Support frame rates: 24, 25, 29.97 (drop), 30 fps; expose selected rate.
4. Provide generator follower roles; follower updates from incoming stream; generator emits at correct cadence.
5. Deterministic ordering with other timing events; document precedence with MIDI Clock.
6. Diagnostic logging (tag: mtc) with timestamps; optional.

## Acceptance Criteria
- Nibble sequence reconstructs correct timecode for all frame rates.
- Malformed sequences are discarded without state corruption.
- Timing jitter of emitted Quarter Frames within stated bounds.

## Traceability
- Traces to: TRACK-MTC-Extension-Specification
- Depends on: GM1 timing base
- Verified by: TEST-MTC-QF-001..003

## References
- MIDI Time Code Specification (reference only)
