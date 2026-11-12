# Public-header validation

The unchanged public `abw::sort()` header and frozen ABW-B produce identical
sorted arrays, dispatch decisions, bit widths, shifts and direct-output decisions
on all 51 existing workloads. This run supports practical timing parity on the
measured machine. It is not a statistical equivalence proof or a cross-CPU claim.

## Fixed design

- All nine existing real columns and all 14 synthetic distributions at 100,000,
  1,000,000 and 10,000,000 elements; existing input and expected-output SHA-256
  values checked before execution. No replacement, replication or reordering.
- Three independent processes per input, nine repetitions per method per process,
  randomized initial method order and cyclic interleaving: 153 processes and
  4,131 timed sorts. These are repeated measurements, not 153 independent datasets.
- Same binary, compiler and options for pdqsort, frozen B and public header:
  LLVM-MinGW Clang 23.1.5, C++20, O3, NDEBUG, AVX2, BMI, static, pthread;
  Boost 1.90.0. Intel i5-14500, Windows 11 x64, first permitted CPU affinity.
- Full-input warmup once per method outside timing. Loading, input copying,
  verification and separate path audits excluded. An outer steady-clock interval
  measures each entire function call. Frozen B retains its original internal
  analysis clocks and string metadata; the header has none. This difference is
  part of the shipped implementations and is not silently subtracted.
- Each output is compared element-for-element with the existing independently
  sorted expected file. Path audits run separately to avoid instrumentation in
  the public header. They invoke the same detail functions used by `abw::sort()`;
  timed header calls themselves have no route instrumentation.
- Frozen reference and historical benchmark/results files remain byte-identical.
  No algorithm, policy, thresholds or public header bytes changed.
- Descriptive input-level parity band fixed before timing: header/B time ratio
  0.95 through 1.05. Timing is never a CI acceptance condition. No outliers removed.

## Results

Ratio means **public-header time / frozen-B time**; lower is faster. Each time is
the median of three process medians. The ratio is the median of the three paired
process ratios, so it need not equal the displayed median times' quotient.

- Real inputs: geometric mean ratio **1.006671** (header time +0.67%).
- Synthetic regression inputs: geometric mean ratio **0.998334** (header time -0.17%).
- All **51/51** input-level ratios fall inside the descriptive +/-5% band.
- Real direct output: **2/9**, Covertype elevation and Census industry, unchanged.

| Input | n | pdq ms | Frozen B ms | Header ms | Header/B | Session ratio range | Path |
|---|---:|---:|---:|---:|---:|---|---|
| census_caseid | 2458285 | 1.2897 | 1.2731 | 1.2849 | 1.0151 | 0.9595–1.0196 | native |
| census_industry | 2458285 | 7.1597 | 2.4948 | 2.4885 | 1.0018 | 0.9975–1.0029 | active8/direct |
| census_sex | 2458285 | 2.1871 | 2.6540 | 2.6632 | 1.0026 | 0.9900–1.0035 | native |
| covtype_cover | 581012 | 0.8654 | 0.8711 | 0.8738 | 1.0031 | 0.9959–1.0048 | native |
| covtype_elevation | 581012 | 4.1051 | 0.6954 | 0.7027 | 1.0170 | 0.9765–1.0193 | active12/direct |
| covtype_road_distance | 581012 | 5.3030 | 3.1072 | 3.1654 | 1.0191 | 0.9757–1.0211 | active12 |
| nasa_bytes | 1871988 | 11.7314 | 12.1541 | 12.2497 | 1.0079 | 0.9947–1.0109 | native |
| nasa_status | 1891714 | 2.4090 | 2.4408 | 2.4478 | 1.0014 | 0.9986–1.0454 | native |
| nasa_timestamp | 1891714 | 1.4500 | 1.4536 | 1.4426 | 0.9924 | 0.9881–1.0595 | native |
| synthetic_10000000_discontinuous | 10000000 | 73.9797 | 76.8985 | 76.3712 | 0.9957 | 0.9922–1.0015 | native |
| synthetic_10000000_duplicates | 10000000 | 30.7363 | 10.9430 | 10.8420 | 0.9948 | 0.9884–0.9962 | active8/direct |
| synthetic_10000000_duplicates_narrow | 10000000 | 29.4617 | 10.6515 | 10.6175 | 0.9999 | 0.9954–1.0040 | active8/direct |
| synthetic_10000000_exponential | 10000000 | 207.2412 | 192.9274 | 190.7392 | 0.9933 | 0.9850–0.9939 | active12 |
| synthetic_10000000_extreme_skew | 10000000 | 5.6551 | 5.5988 | 5.6126 | 1.0025 | 0.9925–1.0106 | native |
| synthetic_10000000_lower_only | 10000000 | 176.1132 | 150.5689 | 149.0425 | 0.9827 | 0.9776–0.9899 | active12 |
| synthetic_10000000_mixture | 10000000 | 170.0059 | 165.2092 | 165.9375 | 1.0044 | 0.9881–1.0241 | active12 |
| synthetic_10000000_narrow | 10000000 | 100.4452 | 73.3474 | 74.1759 | 1.0094 | 0.9939–1.0113 | active12 |
| synthetic_10000000_normal | 10000000 | 207.5849 | 185.5218 | 183.6220 | 0.9913 | 0.9573–1.0058 | active12 |
| synthetic_10000000_partial | 10000000 | 66.6517 | 65.9322 | 65.6896 | 0.9960 | 0.9829–1.0172 | native |
| synthetic_10000000_reverse | 10000000 | 10.3165 | 10.3210 | 10.4327 | 1.0068 | 0.9870–1.0108 | native |
| synthetic_10000000_sorted | 10000000 | 5.5485 | 5.6595 | 5.6791 | 1.0035 | 0.9971–1.0529 | native |
| synthetic_10000000_uniform | 10000000 | 207.0206 | 180.4201 | 179.1703 | 0.9849 | 0.9752–0.9931 | active12 |
| synthetic_10000000_upper_only | 10000000 | 74.6323 | 10.8761 | 10.7885 | 1.0021 | 0.9919–1.0100 | active12/direct |
| synthetic_1000000_discontinuous | 1000000 | 7.7457 | 8.2704 | 8.4909 | 1.0096 | 0.9922–1.0540 | native |
| synthetic_1000000_duplicates | 1000000 | 2.5079 | 1.0736 | 1.0620 | 1.0206 | 0.9812–1.0290 | active8/direct |
| synthetic_1000000_duplicates_narrow | 1000000 | 2.4504 | 0.9913 | 0.9773 | 0.9867 | 0.9839–1.0032 | active8/direct |
| synthetic_1000000_exponential | 1000000 | 19.2310 | 17.2688 | 17.1579 | 0.9866 | 0.9816–0.9960 | active12 |
| synthetic_1000000_extreme_skew | 1000000 | 0.4981 | 0.5128 | 0.5100 | 1.0159 | 0.9645–1.0468 | native |
| synthetic_1000000_lower_only | 1000000 | 18.4370 | 16.1559 | 16.0844 | 0.9985 | 0.9943–1.0137 | active12 |
| synthetic_1000000_mixture | 1000000 | 16.4653 | 15.6464 | 15.6813 | 1.0018 | 0.9690–1.0022 | active12 |
| synthetic_1000000_narrow | 1000000 | 13.9927 | 11.4591 | 11.5233 | 1.0008 | 0.9978–1.0065 | active12 |
| synthetic_1000000_normal | 1000000 | 18.9895 | 16.9521 | 17.0000 | 1.0028 | 1.0012–1.0164 | active12 |
| synthetic_1000000_partial | 1000000 | 5.5038 | 5.5342 | 5.5230 | 1.0006 | 0.9980–1.0091 | native |
| synthetic_1000000_reverse | 1000000 | 0.9444 | 0.9102 | 0.9414 | 0.9993 | 0.9941–1.0343 | native |
| synthetic_1000000_sorted | 1000000 | 0.5141 | 0.5130 | 0.5169 | 0.9885 | 0.9815–1.0106 | native |
| synthetic_1000000_uniform | 1000000 | 18.8643 | 16.3318 | 16.2693 | 0.9947 | 0.9930–1.0030 | active12 |
| synthetic_1000000_upper_only | 1000000 | 7.7794 | 1.3325 | 1.3815 | 1.0233 | 1.0148–1.0676 | active12/direct |
| synthetic_100000_discontinuous | 100000 | 1.0489 | 1.0978 | 1.0875 | 0.9842 | 0.9701–1.0087 | native |
| synthetic_100000_duplicates | 100000 | 0.1686 | 0.0915 | 0.0909 | 0.9934 | 0.9639–1.0294 | active8/direct |
| synthetic_100000_duplicates_narrow | 100000 | 0.1614 | 0.0801 | 0.0795 | 0.9925 | 0.9922–0.9950 | active8/direct |
| synthetic_100000_exponential | 100000 | 1.7371 | 1.4836 | 1.4774 | 0.9958 | 0.9912–1.0299 | active12 |
| synthetic_100000_extreme_skew | 100000 | 0.0357 | 0.0363 | 0.0362 | 0.9972 | 0.9945–1.0027 | native |
| synthetic_100000_lower_only | 100000 | 1.6894 | 1.3180 | 1.3166 | 0.9974 | 0.9911–0.9999 | active12 |
| synthetic_100000_mixture | 100000 | 1.5201 | 1.3479 | 1.3712 | 1.0229 | 1.0173–1.0259 | active12 |
| synthetic_100000_narrow | 100000 | 1.7256 | 1.3447 | 1.3421 | 0.9981 | 0.9977–1.1854 | active12 |
| synthetic_100000_normal | 100000 | 1.7648 | 1.4601 | 1.4639 | 0.9969 | 0.9888–1.0086 | active12 |
| synthetic_100000_partial | 100000 | 0.4269 | 0.4274 | 0.4297 | 0.9978 | 0.9896–1.0290 | native |
| synthetic_100000_reverse | 100000 | 0.0818 | 0.0827 | 0.0823 | 0.9952 | 0.9843–0.9952 | native |
| synthetic_100000_sorted | 100000 | 0.0341 | 0.0350 | 0.0350 | 0.9970 | 0.9474–1.0000 | native |
| synthetic_100000_uniform | 100000 | 1.7707 | 1.3776 | 1.3726 | 1.0008 | 0.9964–1.0065 | active12 |
| synthetic_100000_upper_only | 100000 | 1.0044 | 0.1744 | 0.1756 | 0.9659 | 0.9490–1.0553 | active12/direct |

## Interpretation and limits

There is no input-level difference exceeding the predeclared 5% descriptive band.
Small differences change sign between processes, including on native paths that
use the same pdqsort. Function inlining/code layout, original metadata and clocks,
and measurement variation are possible contributors; this experiment does not
identify a causal percentage for any of them. No optimization is justified by
these data, and none was made. Individual process ratios are retained above and
in CSV even when they exceed the band.

This validates the measured header revision, not identical absolute milliseconds
on other systems. The original historical performance tables remain historical
frozen-harness measurements. The new test uses three comparison methods and full
input warmups; do not merge its times with the original seven-method experiment.
GCC/Clang/MSVC CI tests correctness and build compatibility, not timing portability.
The benchmark driver remains Windows-specific because it preserves local CPU
affinity control. CMake and correctness tests are cross-platform x64.

## Reproduce

Use the original benchmark source downloads/extractor and synthetic generator in
`benchmarks/README.md` to obtain the existing inputs. Use
`benchmarks/input_manifest.json` and `benchmarks/synthetic_manifest.json`; the full 51-case
manifest is also embedded under `inputs` in this run's `results/protocol.json`.
The runner expects a JSON object with a `cases` array (the existing
`benchmark_manifest.json`). Do not overwrite `results/`; choose a new directory.

```powershell
python benchmarks/header_validation/build.py --cxx PATH/clang++.exe --boost PATH/boost_1_90_0 --out header_validation.exe
python benchmarks/header_validation/run.py --inputs PATH/inputs --manifest PATH/benchmark_manifest.json --exe header_validation.exe --out new_header_results
python benchmarks/header_validation/analyze.py new_header_results
```

For the embedded manifest, write `{"cases": protocol["inputs"]}` to a new JSON
file using Python. Input files are not bundled in this new directory.

`results/protocol.json` records the measured source and binary SHA-256 values and
all input hashes. `validation.json` records unchanged historical artifacts,
additional adapter hashes, compiler flags and environment. `audits.jsonl` records
all exact path comparisons. `session0..2.jsonl` are unfiltered new raw measurements;
CSV files provide session and input summaries. The benchmark source is included.

## Follow-up of process variation

Eight of the 153 primary input/process ratios exceeded the descriptive +/-5%
band (seven distinct inputs). All seven were remeasured, without changing the
binary, in three new processes each: 567 additional timed sorts. These records
are in `diagnostic.jsonl`; they are not pooled with or substituted for primary
results. Follow-up median ratios ranged from 0.97625 to 1.03609, all inside the
same band. The largest primary deviation, narrow-range 100k at 1.18539, had
follow-up ratios 0.99869, 1.09035 and 0.98912. NASA timestamp again showed a
single-process deviation (1.09996), while the follow-up median was 1.01849.

Thus an input-level slowdown above 5% did not reproduce across these processes;
individual process variation remains. All decisions were equal, including native
fallback for timestamp, so no changed dispatch explains those timings. Exact
causal attribution to clock overhead, code layout, caching or OS scheduling would
require further instrumentation; none is claimed here. No policy or code change
was made in response.

```powershell
python benchmarks/header_validation/diagnose.py --results new_header_results --inputs PATH/inputs --exe header_validation.exe
```

## Build compatibility and release readiness

GitHub Actions run [43167684707](https://github.com/bumi0666/abw/actions/runs/43167684707)
passed CMake Release builds and the existing 61-case conformance test on:

- Ubuntu 24.04, GCC 13.3.0, C++17.
- Ubuntu 24.04, Clang 18.1.3, C++17.
- Windows 2025, MSVC 19.51.36260.0, x64, C++17.

The MSVC test adapter supplies an equivalent leading-zero primitive for the
archived reference's GCC builtin. It does not change the public header or frozen
source files. MSVC emits harmless C4068 warnings for archived Clang pragmas.
CMake explicitly links Threads for the Boost-based reference test. Boost 1.90.0
headers are downloaded from the official archive and SHA-256 checked in CI.
Performance is not a CI pass/fail criterion; CI does not rerun the data benchmarks.

The requested verification supports preparing **v0.1.0 as an experimental,
limited-scope release**. No release or tag was created. Existing input-dependent
regressions, uint32 key-only/vector API, allocation cost, x64 scope, Windows-only
performance harness and lack of second-CPU performance replication remain.
