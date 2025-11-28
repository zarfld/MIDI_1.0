# REQ-F-MTC-004: Interoperability with MIDI Clock (24 PPQN) and Transport

## Type
- Functional Requirement (Extension)
- Phase: 02-requirements

## Description
Define interoperability rules between MTC and MIDI Clock, including precedence, coexistence, and transport controls (Start/Stop/Continue) without conflicting timing domains.

## Requirements
1. Document precedence policy when both MTC and MIDI Clock present.
2. Ensure Start/Stop/Continue harmonize with timecode domain; no illegal discontinuities.
3. Provide selectable sync source (MTC vs MIDI Clock) and runtime switch without undefined states.
4. Diagnostics: consistent event tagging; no duplicate transport actions.

## Acceptance Criteria
- Precedence policy yields deterministic results in mixed streams.
- Runtime sync-source switch preserves continuity per policy.
- Transport commands align to expected boundaries in both domains.

## Traceability
- Traces to: TRACK-MTC-Extension-Specification
- Depends on: GM1 clock handling
- Verified by: TEST-MTC-CLK-001..003

## References
- MIDI Time Code Specification; MIDI Clock behaviors (reference only)
