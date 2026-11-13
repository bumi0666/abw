# ABW

Experimental header-only sorting wrapper for `std::vector<std::uint32_t>`.
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
3. Determine a contiguous bucket bit range using an exact variation scan.
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
- Uses allocation; `std::bad_alloc` can propagate. No reuse-buffer API.
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
