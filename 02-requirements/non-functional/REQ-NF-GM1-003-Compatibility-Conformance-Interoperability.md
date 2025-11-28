# REQ-NF-GM1-003: Compatibility, Conformance, and Interoperability Assurance

## Metadata
- **Type**: Non-Functional Requirement
- **Category**: Compatibility / Conformance / Interoperability / Diagnostics
- **Phase**: 02-requirements
- **Spec**: GM1
- **Status**: Draft
- **Priority**: P1
- **Created**: 2025-11-28

## Traceability
- **Traces to**: STR-GM1-Compat-01 (Cross-device playback), STR-GM1-QA-01 (Quality assurance), STR-GM1-Trace-01 (Traceability)  
- **Depends on**: REQ-F-GM1-006..018 (Complete functional base)  
- **Verified by (planned)**: TEST-GM1-CONF-001 (Conformance matrix), TEST-GM1-COMP-001 (Cross-reference playback), TEST-GM1-DIAG-001 (Diagnostic emission)

## Description
Establish interoperability, conformance validation, and diagnostic reporting requirements to ensure consistent GM1 behavior across devices and facilitate certification and troubleshooting.

## Requirements
1. NFR1: Implementation MUST produce a machine-readable conformance report (JSON) enumerating support status for each GM1 functional requirement (IDs REQ-F-GM1-001..018) with fields: {id, status: supported|partial|na, notes}.  
2. NFR2: Mandatory controllers (REQ-F-GM1-009) MUST indicate operational range compliance (0–127) and scaling method (log volume, equal-power pan) in report.  
3. NFR3: GM On/Off SysEx handling (REQ-F-GM1-013) MUST log each mode transition with timestamp and outcome (success|ignored|error).  
4. NFR4: Conformance test harness MUST replay a reference GM1 test MIDI file set (at least 10 patterns) and produce summary metrics: {voices_used_peak, polyphony_policy_hits, controller_events, tuning_events, anomalies}.  
5. NFR5: Interoperability: Playback of reference file set MUST match expected event-derived frequency profile within ±1 cent and controller curve thresholds (volume within ±1 step post-scaling).  
6. NFR6: Diagnostic logging MUST be toggleable (levels: ERROR, WARN, INFO, TRACE) and incur <5% performance overhead in INFO mode during stress test.  
7. NFR7: All diagnostic messages MUST include monotonic timestamp (µs resolution) and module tag (voice, ctrl, rpn, mode, bend, sysx).  
8. NFR8: Conformance JSON MUST include build metadata: {version, commit_sha, build_time_utc}.  
9. NFR9: Provide API: `gm1_get_conformance_report(char* buf, size_t buf_len)` returning serialized JSON; MUST be stable ordering for diffing.  
10. NFR10: Any unsupported functional requirement MUST return status `partial|na` with rationale (e.g., feature flag disabled).  
11. NFR11: Cross-device determinism: Running reference test on two compliant builds (same version) MUST yield identical conformance JSON except for timestamps.  
12. NFR12: Error conditions (malformed SysEx, out-of-range key) MUST emit WARN/ERROR with structured code (enum) and never produce ambiguous free-form only text.  
13. NFR13: Report generation MUST complete < 10 ms (desktop) and < 50 ms (embedded) and be callable multiple times without state mutation.  
14. NFR14: Provide diff utility (optional script) to compare two reports and highlight capability changes (add, remove, status change).  
15. NFR15: Interoperability test MUST validate percussion channel neutrality in pitch calculations (per REQ-F-GM1-016) and log non-neutral anomalies.

## Conformance Report Example (Excerpt)
```json
{
  "gm1_version": "1.0",
  "commit_sha": "abc1234",
  "generated_utc": "2025-11-28T10:32:15Z",
  "requirements": [
    {"id": "REQ-F-GM1-009", "status": "supported", "notes": "Log volume; equal-power pan"},
    {"id": "REQ-F-GM1-016", "status": "supported", "notes": "440Hz reference; C4=60"},
    {"id": "REQ-F-GM1-018", "status": "supported", "notes": "±2 default bend; dynamic RPN"}
  ],
  "metrics": {"voices_used_peak": 22, "controller_events": 1400}
}
```

## Acceptance Criteria
1. AC1: Generated report lists all functional IDs (001..018) with non-empty status fields.  
2. AC2: Reference playback test shows frequency deviations ≤ ±1 cent across entire file set.  
3. AC3: Two successive report generations diff identical except timestamp.  
4. AC4: Mode transitions log sequence for GM On/Off including ignored duplicates.  
5. AC5: Diagnostic overhead profiling shows <5% CPU delta in INFO mode.  
6. AC6: Unsupported feature (if any disabled) documented with rationale (verified by intentional flag toggle).  
7. AC7: Error/WARN emissions carry structured code and tag; parser test confirms format.  
8. AC8: Report generation time within defined bounds (timing log).  
9. AC9: Diff utility highlights intentional change (e.g., enabling MPE extension) and no false positives.

## Risks / Considerations
- Excessive logging can skew timing; ensure level gating early.  
- JSON ordering stability requires deterministic iteration order (avoid hash map non-determinism).  
- Cross-platform time source differences may affect microsecond resolution; normalize UTC formatting.

## References
- GM1 functional requirements set  
- Real-time diagnostics best practices  
- Cross-device interoperability goals
