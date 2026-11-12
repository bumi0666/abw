# Frozen benchmark and raw results

The recorded performance belongs to `benchmark.cpp`, which is copied unchanged
from the real-workload evaluation. It does not benchmark the cleaned public
header. The dispatch policy and B execution path are the compatibility reference
for that header.

## Inspect the existing evidence

- `results/raw/`: every real and synthetic session, separate heap measurements,
  bucket audits and preflight records. No outlier filtering.
- `results/input_results.csv`: all inputs, times, direct-output decisions and
  additional requested-heap peaks.
- `results/session_medians.csv`: each process's result, including regressions.
- `results/real_input_metadata.csv`: meanings, size, cardinality, range,
  duplicate fraction, omitted records and hashes.
- `results/provenance.json`: frozen source and measured executable hashes.
- `results/environment.json`: public machine/compiler conditions.
- `results/assessment.json`: the reviewed limited-scope verdict for this run.

There were 1,323 timed sorts on nine real columns and 3,681 on 42 synthetic
regression inputs, plus 288 separate memory processes. All outputs matched the
complete reference array. Three sessions on one CPU are not three hardware
validations. Memory instrumentation is excluded from timing builds.

## Reproduce

The frozen performance harness uses Windows process affinity and memory APIs.
It is Windows x86-64 only. The header-only library has no Windows API dependency.
A Linux performance harness is not provided or claimed as validated here.

Requirements: AVX2/BMI1-capable x86-64 CPU, Python 3 with NumPy, Node, Boost 1.90.0,
and LLVM-MinGW Clang (the recorded version was 23.1.5). The source archives total
about 200MB; synthetic input and expected arrays use about 1.3GB. Raw datasets
and generated executables are ignored by Git.

From the repository root in PowerShell:

```powershell
python benchmarks/fetch_sources.py
python benchmarks/extract_data.py
node benchmarks/generate_synthetic.cjs
python benchmarks/build.py --cxx C:/llvm-mingw/bin/clang++.exe --boost C:/boost_1_90_0
python benchmarks/run.py bench
python benchmarks/run.py memory
python benchmarks/analyze_results.py
```

Use a fresh working copy for a new run: the runner refuses to overwrite existing
session records. New results are written under `benchmarks/`, with tables in
`benchmarks/review/`. Recorded original results remain under `results/`.

The fetch script checks original data and CAFS hashes. Extraction retains source
order and uses the predeclared columns. No rows are replicated or shuffled.
Census is the publisher's processed release; it already includes upstream
recoding and randomization. See [the report](../docs/REAL_WORLD_REPORT.md) and
[attributions](../THIRD_PARTY_NOTICES.md) for licenses and transformations.

`CAFS_u32` is the supported scalar path; `CAFS_i32` uses the exact same
nonnegative values through the specialized int32 API. Both use the public
pdqsort fallback callback. Counting is a simple runtime-min/max histogram,
with a predeclared 64MiB request limit. It is not an exhaustive comparison of
all optimized counting implementations.

Input copy/conversion, loading, warmup, verification and logging are outside the
timer. The total includes analysis and all sort-owned allocation, initialization,
scatter, bucket sorting, output and deallocation. The separate profile build
tracks C++ new/delete and CAFS aligned allocations; stack, allocator overhead
and unobserved CRT allocations are excluded. OS peak working-set values are also
recorded, and should not be confused with exact requested-heap usage.

This source layout's command entry points are syntax checked. The original
components were executed locally, but this public layout has not been replayed
end-to-end on a new machine. Reanalysis computes measurements only; it does not
automatically issue a positive verdict for future runs.
