# ABW v0.1.2 handoff

Updated 2026-11-13. v0.1.2 adopts the validated selective histogram candidate.
See the root README and benchmarks/v012_validation/README.md for the current
implementation, evidence, correctness checks and limitations. Four histograms
are used only for selected zero-shift 8-bit execution. An exact width <= 8
certificate authorizes the limited direct-output dispatch exception. Wider
sampled-ordered inputs retain native fallback. No further optimization is implied.

The historical snapshot below describes v0.1.1 and its then-open questions.
For current code use main or the v0.1.2 tag. Other-CPU performance
replication remains open. Preserve existing raw records and do not automatically
restart optimization or publish further versions.

---

# ABW project handoff

Snapshot: 2026-11-12. Read this before continuing the project on another computer or in a new chat. This is a project-state summary, not a transcript of the development conversation.

## Released state

- Repository: https://github.com/bumi0666/abw
- Current release: [v0.1.1 — Experimental Release](https://github.com/bumi0666/abw/releases/tag/v0.1.1), marked pre-release.
- Release commit: `f01099fea493aafcb581c8891f43293691878373`.
- [PR #2](https://github.com/bumi0666/abw/pull/2) merged the exact zero-shift single-histogram shortcut and strengthened tests.
- Project license: MIT; Boost and comparison sources/datasets retain their own licenses.
- API: C++17, x64, `abw::sort(std::vector<std::uint32_t>&)`, ascending key-only sorting, no stability or payload support.

## Accepted and deferred decisions

Accepted in v0.1.1: after the existing policy chooses a partition with `shift == 0`, the exact whole-input variation scan proves all higher bits are common. Save the prefix, count one histogram, then reconstruct `prefix | bucket`. This avoids purity comparisons and unused execution metadata. The public header matches the measured candidate except for its namespace.

Dispatch, sample seeds, entropy thresholds and nonzero-shift execution are unchanged. Four-sub-histogram execution was not adopted. Exact single-value detection remains on the nonzero-shift B path. No new cardinality guard, cost predictor, or uninitialized temporary was adopted.

The current project position is a limited-scope experimental integer-sort wrapper. Performance gains concentrate in particular inputs. Preserve regressions and raw records. Do not claim general superiority or treat repetitions as independent datasets/hardware.

## Evidence map

- [Public API](../include/abw/abw.hpp)
- [Strengthened correctness tests](../tests/conformance.cpp): 77 built-in cases, including 16 explicit nonzero-common-prefix cases that assert active zero-shift selection at both bucket widths and four lengths.
- [v0.1.1 validation](../benchmarks/purity_bypass_validation/README.md): prior full comparison, 9,072 timed sorts on nine real and 42 synthetic inputs; 102 separate header/candidate heap processes; short 243-sort public-header packaging check. Local correctness with all existing input files: 128 cases.
- The v0.1.1 raw JSONL records are in `benchmarks/purity_bypass_validation/raw_records.zip`; extract at that directory to restore the documented relative paths.
- [v0.1.0 header parity](../benchmarks/header_validation/README.md): 4,131 primary timed sorts and separate diagnostic measurements.
- [Original real-workload report](REAL_WORLD_REPORT.md), [historical raw records](../benchmarks/results/raw/), [source/data fetch and reproduction](../benchmarks/README.md).
- [Existing GCC/Clang/MSVC C++17 CI](https://github.com/bumi0666/abw/actions/runs/43180395967) passed for the v0.1.1 change. Timing is not a CI gate.

The full v0.1.1 candidate/public-v0.1.0 time ratios were 0.9496 for real inputs and 0.9772 for synthetic inputs in geometric mean. Most real-input average improvement came from Covertype elevation. Existing losses to CAFS and input-specific regressions remain. All performance evidence here uses one Intel i5-14500 Windows CPU; other-CPU performance replication remains open.

Earlier A–M research history, Colab artifacts and some recent histogram experiments also exist in the original local research workspace. They are not all in this repository. A separately transferred research archive may contain them. Do not infer that a Git clone includes every local artifact or the conversation history.

## Continue on another computer

```sh
git clone https://github.com/bumi0666/abw.git
cd abw
git checkout v0.1.1
```

Use the tag to inspect the release; create a branch before changing it. This handoff document was added after the release and is available on `main`.

For correctness, install CMake, a C++17 x64 compiler and Boost headers, then follow the root README's CMake/CTest commands. Performance reproduction currently requires the Windows harness, a compatible AVX2/BMI x86-64 CPU, Python/NumPy, Node, Boost and the pinned CAFS sources. See `benchmarks/README.md` for commands and scope. Raw source data, generated input arrays, compiler installations and executables are not automatically supplied by Git; fetch/regenerate inputs and check hashes. Use a fresh working directory for new measurements.

## Open questions, not automatic tasks

- Existing E selection can reject inputs where B direct output could be useful. The Census sex input falls below the entropy threshold despite having only two values. Investigating that is a future policy experiment, not part of v0.1.1.
- Four histograms showed input-order/occupancy-dependent gains and regressions. Eight-bit bucket count alone was insufficient to justify adoption. Hardware-counter attribution is unverified.
- Independent CPU performance replication and external workload feedback remain useful.

Before further work, confirm the user's next objective. Do not automatically restart optimization, tune thresholds, rerun thousands of timings, or publish another release. Preserve historical inputs and raw records; record seeds, compiler/options, source hashes, correctness, route and full-call time for new experiments. Avoid renaming or moving existing files without explicit request.

## New-chat starter

> Read `docs/HANDOFF.md`, the root README and `benchmarks/purity_bypass_validation/README.md`. Treat v0.1.1 as the released baseline. First report the current code/validation state and outstanding limitations, then carry out the task I specify. Do not assume access to the previous conversation or missing local files.
