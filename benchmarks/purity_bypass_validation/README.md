# v0.1.1: exact zero-shift histogram

When the unchanged policy selects partitioning with shift==0, the exact whole-input variation scan has proved that all bits above the mask are common. A bucket uniquely identifies a key. Save a[0]&~mask before overwriting input, count keys with one histogram, then output prefix|bucket in bucket order. This removes representative comparisons and unused starts/representative allocations. No four-way histogram, policy/threshold change, or uninitialized-temporary variant is included. The public implementation equals the previously measured candidate except for its namespace.

## Existing performance evidence

The full comparison in `comparison/` used all nine existing real columns and 42 synthetic inputs, the original seven competitors plus public v0.1.0 and the candidate, three fresh-process sessions: **9,072 timed sorts**, 153 timing processes, plus 102 separate public/candidate heap-measurement processes. Source/input hashes, all raw records, route audits, per-input tables and separately retained follow-ups are included. No historical file was overwritten. `comparison/protocol.json` preserves hashes and records its original manifest hash; machine-local build paths are replaced by `${WORKSPACE}`.

Candidate/public-v0.1.0 geometric mean time ratios: **0.949625** across real inputs and **0.977181** across synthetic inputs. Covertype elevation's paired time ratio was **0.695801**; Census industry's was **0.974282**. Most real-input average improvement comes from Covertype. The 46 unchanged-path inputs have geometric mean ratio **1.003103**. The candidate still loses overall to CAFS; real-input geometric mean speedup is 0.783445 vs CAFS i32 and 0.835018 vs CAFS u32. The Census sex input retains about 19.5% input-level time regression vs pdqsort in this run.

Primary maximum candidate/public input-level regression was 6.05% on 100k upper-only, which does not enter the new path. Five new diagnostic processes did not reproduce it (median ratio 0.9810). Both primary and follow-up records are retained, not pooled or filtered. Covertype requested peak heap decreased from 81,928 to 50,280 bytes; Census industry's peak remained 10,840 bytes because analysis allocations dominate.

## Public-header release check

Existing conformance tests plus 16 explicit cases with nonzero common prefixes, active 8/12-bit zero-shift selection and sizes 100000..100003: **77 cases**. Including all 51 existing input files: **128 cases**, full equality against independently sorted expected arrays and frozen route checks. New tests explicitly assert the intended branch is reached; they are not merely sorted-output checks on native fallback.

A short packaging check compares tagged v0.1.0, the new public API and the measured candidate in one binary: all nine real columns, three fresh processes each, three Latin rotations of three methods, **243 timed sorts**. Input copy/validation excluded; all sort-owned work included. Identical Clang 23.1.5/C++20/O3/AVX2/BMI/static/pthread conditions on the existing Intel i5-14500 Windows x64 machine. Compiler inlining/layout may differ across namespaces. The check is scoped to packaging parity; it does not replace the full experiment or claim statistical equivalence.

Public/candidate geometric mean time ratio: **1.003430**. All nine paired input-level ratios are within a descriptive +/-5% band.

| Input | v0.1.0 ms | Public v0.1.1 ms | Candidate ms | Public/candidate | Public/v0.1.0 |
|---|---:|---:|---:|---:|---:|
| census_caseid | 1.3033 | 1.2819 | 1.3106 | 0.9781 | 1.0063 |
| census_industry | 2.5198 | 2.3460 | 2.3265 | 1.0098 | 0.9323 |
| census_sex | 2.6807 | 2.6658 | 2.6941 | 0.9961 | 1.0027 |
| covtype_cover | 0.8823 | 0.8811 | 0.8739 | 1.0092 | 0.9986 |
| covtype_elevation | 0.6852 | 0.4484 | 0.4546 | 0.9915 | 0.6631 |
| covtype_road_distance | 3.1011 | 3.1319 | 3.0685 | 1.0221 | 0.9998 |
| nasa_bytes | 12.7381 | 12.8727 | 12.6991 | 1.0264 | 1.0123 |
| nasa_status | 2.4077 | 2.4476 | 2.4413 | 1.0057 | 1.0093 |
| nasa_timestamp | 1.4569 | 1.4480 | 1.4347 | 0.9930 | 1.0030 |

Times are medians of process medians; ratios are medians of paired process ratios, so their quotients need not match the displayed median times. Raw packaging records, hashes and comparator source are in `release_check/`. Both old/candidate comparator headers differ from their source versions only in namespace. All underlying value selection and sorting code is retained.

## Reproduce

Obtain existing inputs using `benchmarks/README.md`; all input hashes are in the comparison protocol. To build the full comparison on Windows, compile `comparison/benchmark.cpp` with the original flags plus include roots for `include/`, `comparison/`, `benchmarks/frozen/outputs/`, Boost 1.90.0 and the pinned CAFS headers fetched by `benchmarks/fetch_sources.py`; link psapi. Pass `bench INPUT EXPECTED methods_csv SEED`, or `audit INPUT EXPECTED methods_csv SEED`. A separate `-DABW_MEMPROFILE` build supports `memory` mode; never use it for timing. Eligible methods are recorded per input in the protocol.

For the short release check, compile `release_check/benchmark.cpp` with C++20/O3/NDEBUG/AVX2/BMI/static/pthread and include `include/`, `tests/`, `release_check/` and Boost. Pass `INPUT EXPECTED SEED bench`. It emits all nine timed calls and verifies every output. For cross-platform correctness use the repository's existing CMake/CTest commands; CI does not gate on performance.

## Scope

One performance CPU only. GCC/Clang/MSVC CI validates builds and correctness, not cross-CPU performance. Existing workload-specific regressions, uint32/vector-only interface, mixed-path temporary memory and Windows benchmark limitations remain. Future policy research is separate from this release.

Raw JSONL records are distributed in **raw_records.zip**, retaining their exact bytes and relative paths (`comparison/` and `release_check/`). Extract the archive here to inspect the referenced JSONL files. Other tables, manifests and sources are directly browsable.

Existing [GCC, Clang and MSVC C++17 CI](https://github.com/bumi0666/abw/actions/runs/43179871456) passed all three jobs on this implementation and the strengthened 77-case test. No workflow change was required.
