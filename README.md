# ABW

Experimental header-only adaptive preprocessing wrapper for `std::vector<std::uint32_t>`.
It uses an E-derived dispatch policy, Boost pdqsort as its existing sorter,
and the B exact single-value-bucket fast path.

**Some workloads get faster; others regress.** This is a limited-scope research
implementation, not a generally faster replacement for pdqsort or specialized
integer sorting libraries.

## Use

Requires C++17, a 64-bit target, GCC, Clang or MSVC, and Boost headers. Boost 1.90.0 and
LLVM-MinGW Clang 23.1.5 were used in the recorded experiments.

```cpp
#include <abw/abw.hpp>
#include <vector>
#include <cstdint>

std::vector<std::uint32_t> keys = {7, 2, 7, 0, 5, 2};
abw::sort(keys);
```

Add this repository's `include/` directory and your Boost root to the compiler's
include paths. No CAFS dependency is needed to use ABW.

```sh
c++ -std=c++17 -O3 -Iinclude -I/path/to/boost_1_90_0 examples/basic.cpp -o example
```

CMake can build the example and frozen-reference conformance test:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DABW_BOOST_INCLUDE_DIR=/path/to/boost_1_90_0
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The public header is an API extraction of the benchmark implementation.
The historical performance numbers below belong to the frozen benchmark harness.
The v0.1.0 [separate header validation](benchmarks/header_validation/README.md) measured
all nine real and 42 synthetic inputs: header/frozen-B time ratios were 1.0067
and 0.9983 in geometric mean, respectively. All input-level ratios were within
a descriptive +/-5% band on the measured machine. Raw session variation and
follow-up results are included; this is not a statistical equivalence proof.
Tests compare decisions, selected bit ranges, direct-output decisions and full
sorted results against the frozen implementation.

## v0.2.0 candidate (unreleased)

This branch adds caller-owned temporary storage for repeated mixed-path sorts:

```cpp
abw::workspace ws;
for (auto& values : batches) abw::sort(values, ws);
auto bytes = ws.retained_bytes();  // capacity payload, not RSS
ws.release();                     // return retained storage
```

The existing `abw::sort(values)` retains its per-call temporary lifetime.
Native/skip/direct paths do not use workspace storage. Mixed calls retain at
least `4*n` bytes of capacity after growth; later smaller calls can keep more.
Use a separate workspace per concurrent sorting context. See the complete
[API, memory, growth and exception contract](docs/workspace.md).

An exact bit31 difference witnessed in the existing probe also skips the full
OR scan: it already proves width32. Missing witnesses retain the full scan.
No sampling seed, dispatch threshold, histogram selection or sorting policy is
changed. The shared partition implementation is measured in its final header.

The [integrated-header validation](benchmarks/v020_validation/README.md) includes
all existing60 plus six CTU IPv4 columns,42,816 correct timed sorts/396 processes.
**All 66-input performance figures below were measured on Windows11, Intel
i5-14500; they are not cross-platform performance results.**
Reused calls reduce all66 time5.05%/5.15% versus v0.1.3 (warm/64MiB-disturbed)
and3.46%/3.98% versus the new default. Mixed27 reuse/default gains7.73%/8.71%.
Default-call gains versus v0.1.3 are0.99%/1.18% overall. No runtime sorting
policy changes are made. The exact scan certificate fires on18 synthetics and
six IPv4 inputs, not the older18 real columns.

The new default call is0.46%/1.46% slower than CAFS u32 on all66 under per-call
storage lifetimes. When ABW retains its workspace between calls while CAFS still
allocates per call, reused ABW is2.59%/2.79% faster. On the familiar60, default
remains2.18%/3.28% slower and reuse is close. CAFS i32/Counting
have smaller supported subsets. Do not interpret one aggregate as universal
superiority. Full-input times and substantial competitive losses are retained.

**Version-to-version comparison:** maximum default/v0.1.3 median regression is7.12% on1M partial; reused calls
show6.27% on that input and5.69% on Census industry in the disturbed condition.
Their causes remain unconfirmed; native/direct paths do not use workspace.
**Backend comparison:** in the separate full-ABW/pdqsort66 run below, the maximum
observed regression versus pdqsort alone is47.70% (CTU2 destination IPv4), and
the IPv4_6 mean is27.01% slower. Version-to-version improvements are not a bound
on overhead versus the backend. The earlier IPv4 scan omission reduced ABW's
cost; it did not make full ABW faster than pdqsort on those six columns.
GCC/Clang/MSVC C++17 CI passes. Linux generated repeated-call diagnostics on
AMD/Intel hosted VMs pass correctness checks. Linux gains are small or absent
and slowdowns occur: GCC workspace is about1.7-1.9% slower than the new default
on both tested mixed inputs. Clang shows small gains. Eight-repeat VM timings
do not establish a platform-wide conclusion. See the [Linux artifact values](benchmarks/v020_validation/report.md#linux-repeated-call-values).
All66 performance on another CPU remains unverified.
[Assessment and raw evidence](benchmarks/v020_validation/assessment.md) retain
these limitations. Main and published release are not changed by this draft PR.

## Backend positioning: full ABW versus AVX2

The public ABW backend is **Boost pdqsort**. Other backend experiments use
separate copies; the public API does not select an arbitrary backend.
ABW's benefit depends on both the input and its backend.

The [same-binary comparison](benchmarks/full_backend_validation/README.md)
uses full default ABW->pdqsort, AVX2 alone, pdqsort alone and skip-only ABW->pdq
on all66 unchanged inputs. Windows11/i5-14500, LLVM-MinGW Clang23.1.5, C++17,
AVX2 only, x86-simd-sort v7.0, single-threaded. All calls include their own
per-call allocation/free; no workspace reuse.198 processes/9,504 timed sorts
pass full-output, source/input hash and route checks.

| Group | N | Full ABW/pdqsort time | Full ABW/AVX2 time | Full ABW/skip-only pdq time |
|---|---:|---:|---:|---:|
| Original real9 | 9 | 0.53578 | 0.88528 | 0.93778 |
| New real9 | 9 | 0.66335 | 1.41225 | 0.95275 |
| Real18 | 18 | 0.59616 | 1.11814 | 0.94523 |
| Synthetic42 | 42 | 0.67069 | 1.43324 | 0.90892 |
| IPv4_6 | 6 | 1.27012 | 4.58496 | 1.06896 |
| All66 | 66 | 0.68830 | 1.48875 | 0.93233 |
| Mixed27 | 27 | 0.90705 | 3.96780 | 0.84264 |

Ratios below1 mean full ABW is faster. All66 full ABW time is31.17% lower than
pdqsort,48.88% higher than AVX2, and6.77% lower than skip-only pdq. Mixed27 is
15.74% faster than skip-only pdq; IPv4_6 is6.90% slower. These are direct paired
measurements, not differences between historical experiments.

All18 real columns are listed below, including losses. Full66 tables and raw
process ratios are in the linked report. Primary ratios are medians of paired
process ratios; displayed times are medians of process medians.

| Real input | Full path | AVX2 ms | Full ABW ms | Full/AVX2 | Processes faster /3 |
|---|---|---:|---:|---:|---:|
| nasa_status | native/skip | 0.79660 | 2.30305 | 3.07050 | 0 |
| nasa_timestamp | native/skip | 6.90860 | 1.26680 | 0.18337 | 3 |
| nasa_bytes | native/skip | 3.38665 | 12.49195 | 3.69854 | 0 |
| covtype_cover | direct | 0.27530 | 0.34530 | 1.23568 | 0 |
| covtype_elevation | direct | 1.22435 | 0.33445 | 0.27277 | 3 |
| covtype_road_distance | mixed | 1.76455 | 2.94910 | 1.67130 | 0 |
| census_caseid | native/skip | 8.91145 | 1.17780 | 0.13217 | 3 |
| census_industry | direct | 1.81505 | 1.55810 | 0.87067 | 2 |
| census_sex | direct | 0.65605 | 1.58245 | 2.47429 | 0 |
| skin_red | direct | 0.40495 | 0.11175 | 0.27596 | 3 |
| skin_label | direct | 0.05320 | 0.13425 | 2.52350 | 0 |
| chest_sequence | native/skip | 0.40255 | 0.05195 | 0.12905 | 3 |
| chest_x | mixed | 0.23990 | 0.96290 | 3.96278 | 0 |
| chest_activity | direct | 0.04615 | 0.08460 | 1.83315 | 0 |
| taxi_vendor | direct | 0.87025 | 2.04635 | 2.35145 | 0 |
| taxi_payment | direct | 1.39465 | 2.08740 | 1.46700 | 0 |
| taxi_pickup_zone | mixed | 3.99455 | 9.19675 | 2.30507 | 0 |
| taxi_pickup_time | native/skip | 12.99345 | 55.93135 | 4.30458 | 0 |

Full ABW is faster on6/18 real columns. Covertype elevation and Skin red retain
large direct-output gains versus AVX2; Census industry also has a lower median
ratio, with improvement in2/3 processes. The other three winners (NASA timestamp,
Census case ID and chest sequence) use native pdqsort fallback. Their advantage
is not a direct-output gain. Across all66, full ABW is faster on20 and slower
on46. These results support a pdqsort-backed wrapper with specific useful
workloads, not default use of the same frontend ahead of AVX2.

The [three-backend skip-only experiment](benchmarks/backend_skip_validation/README.md)
separately measured preprocessing before pdqsort, Spreadsort and AVX2. Its
all66 time changes were -26.21%, -38.25% and +10.37%. Direct output itself can
lose: skin_label was35.11% slower than pdqsort and147.58% slower than AVX2.
No measured maximum is presented as a runtime bound for other inputs.

A separate27-input diagnosis finds count/purity accounts for92.44% of the sum
of per-input median instrumented frontend times; sample allocation/frequency
1.04%, width scan5.54%, probe0.11%, entropy0.54%. Instrumented frontend totals
sum to59.1328ms versus58.3637ms uninstrumented (+1.32%); the median input-level
time ratio is1.00387. These are27 separate frontend diagnoses, not one sort.
The skip-only copy in that diagnostic continued the original count pass after
the first mixed value. The independent experiment below tests avoiding it with
an exact sample witness.

### IPv4 regression and bucket diagnosis

These are the earlier same-run **full ABW** comparisons, not the new skip-only
candidate. Positive time changes mean slower.

| IPv4 input | Full ABW vs pdqsort | Full ABW vs skip-only pdq |
|---|---:|---:|
| ctu1_srcaddr_ipv4 | +1.43% | -5.51% |
| ctu1_dstaddr_ipv4 | +47.63% | +17.77% |
| ctu2_srcaddr_ipv4 | +12.52% | +0.52% |
| ctu2_dstaddr_ipv4 | +47.70% | +16.09% |
| ctu3_srcaddr_ipv4 | +15.43% | -0.50% |
| ctu3_dstaddr_ipv4 | +46.14% | +15.47% |

The separate existing-sample diagnosis uses the exact selected bits/shift.
IPv4 destinations have maximum sample bucket occupancy80.5-83.6%, sources
57.0-67.8%; all use256 buckets/shift24 and entropy cutoff1. Across mixed27,
full ABW is faster than skip-only on22 inputs and slower on5. Maximum-occupancy
ranges overlap (winners0.195-67.822%, losers21.094-83.594%). No new occupancy
threshold is introduced. [Exact sample values and per-input diagnosis](benchmarks/sample_certificate_validation/sample_bucket_diagnosis.csv).

### Experimental exact sample witness for skip-only AVX2

A pair of different actual sample values in the same **final** bucket proves
that exact single-value direct output is impossible. The experimental copy
retains the original decision, samples and bit interval. After entropy selection,
it reuses the collected values and the no-longer-needed histogram to detect the
pair; no extra allocation, full scan or numerical dispatch threshold. Native
and zero-shift paths skip this additional test. Missing evidence retains the
old whole-input count/purity check. The public header remains unchanged.

[The candidate comparison](benchmarks/sample_certificate_validation/README.md)
uses AVX2 alone, old skip-only AVX2 and sample-witness skip-only AVX2 on all66
unchanged inputs, same binary/C++17 AVX2 conditions.198 processes/6,336 correct
timed sorts pass full-array/hash/decision/direct-path checks. The proof fires on
all27 mixed inputs. Pre-run safety also covers rare mixed values missed by the
sample, independent map-based witnesses, UINT32_MAX and all66 decisions.

| Group | N | Old skip-only/AVX2 | Sample-witness/AVX2 | Candidate/old skip-only |
|---|---:|---:|---:|---:|
| Real18 | 18 | 1.15517 | 1.09704 | 0.94695 |
| Synthetic42 | 42 | 1.02712 | 0.97206 | 0.94469 |
| IPv4_6 | 6 | 1.65772 | 1.00972 | 0.61260 |
| All66 | 66 | 1.10774 | 1.00813 | 0.90881 |
| Mixed27 | 27 | 1.29827 | 1.03396 | 0.79713 |
| Direct18 | 18 | 0.96099 | 0.96001 | 0.99295 |
| Native/skip21 | 21 | 1.02027 | 1.01765 | 0.99707 |

Mixed-input overhead versus AVX2 falls from29.83% to3.40% in this run;
candidate time is20.29% lower than old skip-only. IPv4_6 overhead falls from
65.77% to0.97%. Overall candidate time is9.12% below old skip-only and0.81%
above AVX2. Real18 remains9.70% slower than AVX2. Direct-output losses such
as Census sex and Skin labels remain. Maximum candidate regression versus old
skip-only is2.04% on1M upper-only. Full input tables/process ratios are retained.

This exact proof reduces skip-only fallback cost. Full ABW's mixed partition
and its IPv4 losses remain unchanged. No public implementation or release is
changed by the experiment; no cardinality/occupancy threshold is fitted.

## v0.1.3 update

v0.1.3 preserves the v0.1.2 sorting policy while reducing preprocessing work.
The ordered low-byte exception skips detailed sample analysis that cannot
affect its decision (B). Required sample frequencies use a fixed 4,096-slot
linear-probing table instead of per-key `unordered_map` allocations (D).
Sample positions, seeds, thresholds, active bit intervals, histogram selection
and mixed-bucket execution are unchanged. Unused native bits/shift metadata
can differ. The header equals the measured B+D candidate except its namespace.

The [separate v0.1.3 comparison](benchmarks/v013_validation/competition/report.md)
retains all 60 inputs: original real9, new real9 and synthetic42. Across 13,920
correct timed sorts in 180 processes, geometric-mean execution time decreased
**2.69% versus v0.1.2** (2.36%, 2.28% and 2.85% by those groups). Same-pair
views give 2.68–2.72% overall. pdqsort speedups were 1.853x, 1.499x and 1.479x;
all60 1.533x. CAFS u32 remained 2.96% faster in execution-time terms overall
(ABW time / CAFS time = 1.0296); CAFS i32 was faster on its supported subset.

The largest primary input regression was +3.61% on 10M sorted values; the
pair-only view was +5.24%, with slowdown in two of three processes. NASA bytes
was 2.42% faster than v0.1.2 in this run; the earlier D-versus-B-only loss is a
different contrast. Small regressions and large existing competitive losses
remain. This B+D performance comparison uses one local Intel i5-14500 Windows
CPU; the older v0.1.2 Colab run does not validate v0.1.3 on another CPU.

The frequency helper adds 24 KiB of stack array payload when required. Its
maximum 2,048 samples keep the table at <=50% occupancy, and uint16 counts fit
the maximum frequency. The sample-values vector still allocates; mixed-path
temporary storage is unchanged. [Safety proof, tests and reproduction](benchmarks/v013_validation/README.md)
document collision/wraparound coverage and implementation limits. Historical
v0.1.2 results below keep their original inputs and measurement scope.

## v0.1.2 update

The validated selective histogram candidate is now the public implementation.
Zero-shift 8-bit partitions use four sub-histograms; 12-bit partitions retain
one histogram. The existing order probe also collects bit variation from the
same sampled values. A nonzero low-byte variation can reopen an ordered exit;
an exact whole-input scan must then prove variation width <= 8 before direct
output bypasses the entropy gate. Wider ordered inputs retain native fallback.
This changes the limited direct-output dispatch exception, not the remaining
sample seeds, thresholds or mixed-bucket algorithm.

In the [51-input competition comparison](benchmarks/v012_validation/competition/report.md),
11,376 timed sorts all passed full-array correctness checks. Geometric-mean
speedups versus pdqsort were 1.788x on nine real columns and 1.432x on 42 synthetic
inputs. Candidate/v0.1.1 time ratios were 0.8384 and 0.9979. On the real aggregate,
candidate time was 1.6% below CAFS uint32 and 2.6% above CAFS int32; the small
aggregate differences do not establish universal superiority. Across all 51
inputs it remained 5.3% slower than CAFS uint32.

Covertype cover, Census industry and Census sex were fastest among the tested
implementations. Large losses remain: NASA timestamp took 3.11x CAFS uint32
time, road distance 4.51x Counting time, and synthetic 10M discontinuous 4.37x
CAFS uint32 time. Maximum input-level regression versus v0.1.1 was +3.14% in
this run. [Packaging checks and reproduction](benchmarks/v012_validation/README.md)
include source/input hashes and raw records. The original competition run covers one CPU;
CAFS int32 and Counting have restricted synthetic eligibility. Historical
v0.1.0/v0.1.1 results below retain their original scope.

## v0.1.1 update

For selected zero-shift partitions, one histogram replaces purity comparisons and
unused execution metadata. The exact common prefix guarantees reconstruction;
dispatch and thresholds are unchanged. The four-sub-histogram experiment is not
included. [Validation and raw records](benchmarks/purity_bypass_validation/README.md)
cover the prior 9,072-sort comparison and a short 243-sort public-header check.
Real/synthetic candidate-v0.1.0 geometric mean time ratios were 0.9496/0.9772;
most real-input improvement came from Covertype elevation. Historical tables below
remain frozen-B measurements. One performance CPU is covered; cross-CPU performance
replication is still open.

## New real-data validation (separate from development inputs)

The frozen v0.1.2 was tested on nine new columns from UCI Skin Segmentation,
UCI chest-mounted accelerometer participant 1, and January 2023 NYC TLC Yellow
Taxi records. Original file order/values were retained, without replication or
performance-based filtering. This set is not pooled with the nine earlier
development columns or 42 synthetic inputs.

All 2,544 timed sorts in 27 processes passed full-array correctness checks.
Geometric-mean speedup versus pdqsort was 1.436x; execution time versus v0.1.1
decreased 12.1%. ABW was fastest among the tested implementations on Skin red,
taxi provider and taxi payment. CAFS uint32/int32 still won the new aggregate:
ABW time was 10.4%/15.8% higher. Direct output fired on 5/9 selected columns.

A new real-data regression is retained: already sorted two-value Skin labels
took 38.1% longer than v0.1.1 (about 40 microseconds extra) after direct output
replaced native fallback. Chest x measurements were 19.8% slower than pdqsort.
Low cardinality or successful direct output does not guarantee a performance
gain. [Full report, metadata, raw records and Colab CPU reproduction](benchmarks/new_real_v012/README.md)
include these losses. A [separate Colab Xeon/GCC run](benchmarks/new_real_v012/colab_xeon/README.md)
completed the same 2,544 sorts with matching input hashes and routes: v0.1.2
was 1.528x pdqsort and reduced time 21.4% versus v0.1.1. Specific winners and
the sorted-label regression differed across environments. CPU, OS and compiler
all changed; the two runs are not pooled. No new policy or optimization is adopted here.

## What it does

1. Apply the small-input and sampled-order checks, retaining the exact low-byte
   direct-output exception described above.
2. Sample the input for the existing distribution/duplicate policy.
3. Determine a contiguous bucket bit range using exact variation analysis.
   An already observed bit31 difference proves width32 without a full scan.
4. If splitting is selected with `shift == 0`, count and reconstruct values
   directly: the common prefix proves every bucket contains a single value.
   Otherwise count buckets and compare each bucket's values.
5. If every occupied bucket has one distinct value, write values and counts
   directly into the input. Otherwise scatter, sort each bucket with pdqsort,
   and copy back. Stop extra purity checks after the first mismatch.

The remaining E thresholds, sample seeds and bit selection are retained. Nonzero-shift
execution retains representative-array initialization. No new cost model is introduced.
A sampled low-byte variation condition can reopen the ordered exit; only an exact
whole-input width certificate authorizes the low-byte direct-output exception.

## Measured scope

Nine columns from three public datasets, in their published order, were tested
on one Intel Core i5-14500 Windows machine with three fresh-process sessions.
The frozen benchmark used the same C++20 / `-O3 -mavx2 -mbmi` flags for all
competitors to accommodate CAFS. Loading, input copies and data conversion
were outside the sort timer; analysis, allocations, scatter, sorting, output
and deallocation were inside it.

Selected median times (milliseconds; median of process medians):

| Real column | pdqsort | ABW-B | CAFS int32 | Counting |
|---|---:|---:|---:|---:|
| Covertype elevation, 581,012 keys | 4.0162 | **0.6958** | 1.1949 | 0.9830 |
| Census industry code, 2,458,285 keys | 7.1427 | **2.3751** | 6.6274 | 6.1935 |

Both columns beat every tested implementation by more than 5% in each of the
three sessions. The additional requested-heap peaks were 81,928 and 10,840
bytes. Counting used less memory; ABW was faster on these two columns.

The complete picture matters:

- Against pdqsort: 3 wins, 5 near ties, 1 loss using a descriptive ±5% time band.
- Geometric-mean speedup across the nine columns: 1.429x vs pdqsort,
  0.821x vs CAFS uint32, and 0.746x vs CAFS int32. ABW lost overall to CAFS.
- Direct output fired for 2 of 9 columns. Those counts are not a deployment
  frequency estimate.
- Largest observed pdqsort regression: **+61.7%** on a Census sex-code session.
- Controlled mixed-value placement tests also found an extra B overhead of
  about 4.56–5.25 ms for a late mismatch in a 10-million-key input.
- Real-input measurements cover one CPU, up to about 2.46 million keys.
  Repeated sessions are not independent hardware validation.

[Full real-workload report](docs/REAL_WORLD_REPORT.md),
[all input times and memory](benchmarks/results/input_results.csv),
[raw measurement records and reproduction](benchmarks/README.md).
The conclusion is **publish within a limited scope**, not expand into a
universal sorting library.

CAFS uint32 was tested through its supported scalar bucket path. The additional
int32 comparison uses exactly the same nonnegative values and four-byte keys,
with input preparation outside the timer. Counting is a documented simple
runtime-min/max dense histogram, not a claim about every optimized histogram
implementation. See the full report before interpreting the ratios.

## Limits

- Key-only, ascending, non-stable sorting. No custom comparator, negative-key
  API, attached records, payload movement, or stable-sort guarantee.
- Uses allocation; `std::bad_alloc` can propagate. Internal allocation failure
  preserves the input array. Optional caller-owned workspace retains mixed-path
  temporary storage until release or destruction.
- Required frequency analysis uses a 24 KiB stack-array payload in a separate
  helper; small-stack callers should account for this. Collision patterns can
  increase linear-probing work; the table is bounded by 2,048 samples.
- A mixed bucket path needs roughly `4*n` temporary bytes plus sample and
  bucket metadata. The direct-output path avoids the `n`-element temporary.
- Small inputs and sampled-ordered inputs outside the low-byte exception use
  pdqsort. Near-baseline performance can
  come from this fallback rather than the new fast path.
- Success on a cardinality or value range alone does not predict success on
  another arrangement of the same values.
- C++17 build and correctness CI passed GCC 13.3, Clang 18.1 and MSVC 19.51
  on x64. Performance records include the local Windows CPU and a separate Colab Xeon
  Linux/GCC run on the new nine columns; this does not isolate CPU effects.

## License

ABW project code is [MIT](LICENSE). Boost is separately licensed under the
Boost Software License 1.0. CAFS and source datasets retain their own licenses;
see [third-party notices](THIRD_PARTY_NOTICES.md). Raw datasets and benchmark
executables are not committed to this repository.
