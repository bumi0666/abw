# ABW v0.1.2: new public-data validation

2026-11-13. Released implementation frozen; no optimization, policy tuning,
GitHub edits or new release. Sources/columns were selected in design.json before
timing. Original row order and values retained, no input replication or shuffle.
Nine columns from three public datasets, none used in the earlier Covertype,
Census or NASA experiments. This is a small new-workload validation on the same
CPU, not an independent hardware replication or random sample of all workloads.

## Data and extraction

- **skin**: [https://archive.ics.uci.edu/dataset/229/skin+segmentation](https://archive.ics.uci.edu/dataset/229/skin+segmentation); [skin.zip](https://archive.ics.uci.edu/static/public/229/skin+segmentation.zip); CC BY 4.0 (UCI dataset page). Download SHA-256 `8556117952ed3f23231e8bb10ef5dec037f30f632c71aaf0b5fa16e6493002e7` (3,400,948 bytes).
- **chest**: [https://archive.ics.uci.edu/dataset/287/activity+recognition+from+single+chest+mounted+acceleromete](https://archive.ics.uci.edu/dataset/287/activity+recognition+from+single+chest+mounted+acceleromete); [chest.zip](https://archive.ics.uci.edu/ml/machine-learning-databases/00287/Activity%20Recognition%20from%20Single%20Chest-Mounted%20Accelerometer.zip); CC BY 4.0 (UCI dataset page). Download SHA-256 `0a056f0032e2e06e7af2309998af21bd3bdeee43bb3c0eff877f54e944198eed` (11,256,835 bytes).
- **taxi**: [https://www.nyc.gov/site/tlc/about/tlc-trip-record-data.page](https://www.nyc.gov/site/tlc/about/tlc-trip-record-data.page); [yellow_tripdata_2023-01.parquet](https://d37ci6vzurychx.cloudfront.net/trip-data/yellow_tripdata_2023-01.parquet); NYC public TLC trip records; NYC.gov Terms of Use, no separate CC license asserted. Download SHA-256 `32df6f67578fa86c484a6b5ef23a5281992ff085521082340b0f9e5889e9a572` (47,673,370 bytes).

UCI Skin is a published sample of B/G/R pixels from FERET/PAL face images, not a
new image-processing run or original raster scan. Red and class columns are kept
in released text-file order. UCI Chest uses the complete participant 1 file chosen
by identifier, not fastest participant. Sensor x is the original uncalibrated
integer reading, not a rounded continuous float. Sequence tokens include
scientific notation: Decimal parses them exactly, preserving source duplicates.
This sequence is not an artificially repaired unique ID. Activity code 0 retained.

TLC January 2023 Yellow Taxi uses full published Parquet columns with zero nulls
in the selected fields. All microsecond timestamp values were checked divisible
by 1,000,000, then represented exactly as timezone-unspecified wall-clock seconds
since 1970-01-01. No local-to-UTC interpretation, truncation, modulo, offset
subtraction or clipping. This tests chronological key sorting at source second
resolution; ABW does not gain a uint64/microsecond datetime API. Records as early
as 2009 remain, rather than filtering to the advertised month. Numeric integer
columns safely fit uint32 without remapping. No rows or columns were excluded.
No data arrays are redistributed in the reproduction archive. UCI licenses are
recorded as stated by UCI; original face-image rights are not relicensed here.
TLC is public data under [NYC.gov Terms of Use](https://www.nyc.gov/main/terms-of-use),
not asserted CC BY. [TLC dictionary](https://www.nyc.gov/assets/tlc/downloads/pdf/data_dictionary_trip_records_yellow.pdf)
provides field meanings; current provider code names need not describe historical vendors.

| Column | N | Unique | Min | Max | Duplicate fraction | v0.1.2 route | Bits/shift | Direct |
|---|---:|---:|---:|---:|---:|---|---|---|
| skin_red | 245,057 | 256 | 0 | 255 | 0.998955 | active8 | 8/0 | True |
| skin_label | 245,057 | 2 | 1 | 2 | 0.999992 | active8 | 8/0 | True |
| chest_sequence | 162,501 | 106,251 | 0 | 162500 | 0.346152 | native | 0/0 | False |
| chest_x | 162,501 | 499 | 1455 | 2356 | 0.996929 | active8 | 8/4 | False |
| chest_activity | 162,501 | 8 | 0 | 7 | 0.999951 | active8 | 8/0 | True |
| taxi_vendor | 3,066,766 | 2 | 1 | 2 | 0.999999 | active8 | 8/0 | True |
| taxi_payment | 3,066,766 | 5 | 0 | 4 | 0.999998 | active8 | 8/0 | True |
| taxi_pickup_zone | 3,066,766 | 257 | 1 | 265 | 0.999916 | active8 | 8/1 | False |
| taxi_pickup_time | 3,066,766 | 1,610,975 | 1230764502 | 1675213013 | 0.474699 | native | 12/18 | False |

## Measurement

Same Intel i5-14500 / Windows x64 / first allowed CPU affinity, LLVM-MinGW
Clang 23.1.5, Boost 1.90.0. All methods same C++20 -O3 -DNDEBUG -mavx2 -mbmi
-static -pthread options to support pinned CAFS (commit
13488afef446ae88e7ad631ec6f862def896094a). v0.1.2 is a byte-identical public
header snapshot; v0.1.1 differs from its original header only in namespace.
Same fixed uint32 scratch and common noinline function-pointer timer for unsigned
methods; CAFS i32 uses separate fixed signed scratch with exactly identical values.
All nine columns fit INT32_MAX. Conversion/load/input restoration/warmup/path
audit/full-array equality excluded; all sort-owned scans, allocation/init,
histogram merge, scatter, sort, reconstruction/copy/free included. No timing
instrumentation in kernels. Full-array correctness checked every measured call.
Four ABBA/BAAB blocks per competitor, shuffled pair order; three fresh processes
per column. Same three original 100k warmup families and one actual-input warmup
per method. No outlier filtering. Sessions are repeated measurements, not new
datasets or new machines. Input and source hashes verified before and after.

CAFS u32 supported scalar API and specialized AVX2 i32 API both have pdq fallback.
Counting is the unchanged simple uint64 dense histogram including runtime min/max
and initialization, only where counter range fits the original 64 MiB cap.
Timestamp requires 3.56 GB of counters and is excluded from Counting, not counted
as a loss. Counting totals therefore cover eight columns, other totals nine.

Tables: median of method calls per process, then median of three process medians.
Ratios: median same-process median-time ratio, then geometric mean over eligible
columns. Ratios need not equal ratios of displayed time medians. ABW anchor times
pool calls across competitor pairs. Block-paired data are retained separately.
Descriptive 5% wins/ties/losses are not statistical significance claims.

**2,544 timed sorts in 27 processes; every full-array check passed.**

| Method | Eligible columns | Geometric mean speedup vs pdq | v0.1.2/method time | v0.1.2 wins/ties/losses |
|---|---:|---:|---:|---|
| v012 | 9 | 1.4357 | 1.0000 | 0/9/0 |
| v011 | 9 | 1.2531 | 0.8786 | 3/5/1 |
| pdq | 9 | 1.0000 | 0.6965 | 4/3/2 |
| spread | 9 | 0.6780 | 0.4739 | 6/0/3 |
| CAFS_u32 | 9 | 1.5923 | 1.1040 | 5/0/4 |
| CAFS_i32 | 9 | 1.6710 | 1.1583 | 5/0/4 |
| counting | 8 | 0.5957 | 0.3975 | 6/0/2 |

## Every input time (ms)

| Input | v0.1.2 | v0.1.1 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest median |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| skin_red | 0.13015 | 0.16160 | 1.32905 | 1.04985 | 0.64710 | 0.62525 | 0.47155 | v012 |
| skin_label | 0.14390 | 0.10375 | 0.10280 | 0.06550 | 0.03770 | 0.03770 | 0.53805 | CAFS_u32 |
| chest_sequence | 0.05640 | 0.05630 | 0.05575 | 0.04350 | 0.02220 | 0.02240 | 0.50265 | CAFS_u32 |
| chest_x | 1.07320 | 1.05865 | 0.89090 | 0.64660 | 0.49390 | 0.34065 | 0.37005 | CAFS_i32 |
| chest_activity | 0.09425 | 0.09405 | 0.09185 | 0.74800 | 0.15960 | 0.15995 | 0.34935 | pdq |
| taxi_vendor | 2.11695 | 2.68495 | 2.49670 | 13.99250 | 2.32505 | 2.43055 | 8.21775 | v012 |
| taxi_payment | 2.19970 | 6.52845 | 6.00970 | 14.08000 | 2.41050 | 2.43610 | 7.38005 | v012 |
| taxi_pickup_zone | 9.49370 | 9.66890 | 12.94025 | 11.82090 | 5.93475 | 6.04545 | 7.65335 | CAFS_u32 |
| taxi_pickup_time | 57.95700 | 59.10090 | 57.63485 | 68.94735 | 81.60325 | 68.46605 | not eligible | pdq |

## Subgroups

| Subgroup | N columns | v0.1.2 speedup vs pdq | v0.1.2/v0.1.1 time |
|---|---:|---:|---:|
| direct | 5 | 1.8816 | 0.7903 |
| not_direct | 4 | 1.0239 | 1.0029 |
| skin | 2 | 2.7072 | 1.0545 |
| chest | 3 | 0.9379 | 1.0018 |
| taxi | 4 | 1.4389 | 0.7267 |

## Interpretation: what generalized and what did not

v0.1.2/pdq geometric-mean speedup is **1.4357x** (4 wins / 3 near ties / 2 losses,
descriptive 5% band). v0.1.1 was 1.2531x on these same new inputs. Median-process
ratio aggregation gives v0.1.2/v0.1.1 time 0.8786: **12.14% lower**. This is not
pooled with the earlier nine development columns.

Against CAFS u32/i32, ABW wins on five and loses on four columns, with aggregate
time **10.40% / 15.83% higher**. CAFS is the stronger aggregate choice on this
specific set. Counting covers eight columns; its denominator is not nine.

Three distinct new columns are lowest-median ABW wins, also more than 5% ahead of
every eligible non-ABW competitor by same-process time ratios:

- Skin red: 0.13015 ms vs pdq 1.32905, CAFS i32 0.62525 and Counting 0.47155.
  pdq speedup 10.21x; shared low-byte histogram fast path. All 256 byte values
  occur. Original adjacent red value changes: 140,675 / 245,056 pairs.
- Taxi provider: 2.11695 ms vs pdq 2.49670, CAFS u32 2.32505. pdq speedup 1.18x.
  v0.1.1 took 2.68495 ms and selected native; v0.1.2 exactly certifies low-byte
  direct output. Two values, 660,478 adjacent changes, original order retained.
- Taxi payment: 2.19970 ms vs pdq 6.00970 and CAFS u32 2.41050. pdq speedup 2.73x.
  v0.1.1 took 6.52845 ms and selected native; v0.1.2 uses exact direct output.
  Five values, 895,893 adjacent changes.

These repeat the earlier benefit on actual image measurements and transport
codes outside Census/Covertype. They do not show superiority on arbitrary integer
data, every low-cardinality column, or every sensor signal.

Important counterexamples:

- **Skin label** is already nondecreasing, with one change in 245,056 adjacent
  pairs (all label 1 followed by label 2). v0.1.2 reopens native and emits direct
  output, taking 0.14390 ms vs v0.1.1 0.10375 and pdq 0.10280. Main aggregated
  ratios are **+38.07% vs v0.1.1, +39.33% vs pdq**; displayed absolute deltas
  are about 40 and 41 microseconds. Regression vs pdq repeated in all processes
  (33.6% to 40.0%). CAFS u32 took 0.03770 ms. This is a newly observed real-data
  regression introduced by the accepted limited direct-output exception.
  No policy adjustment or extra guard is made in this validation.
- Chest x uses the existing mixed active8/shift4 path. 1.07320 ms vs pdq 0.89090
  (aggregated +19.82%) and CAFS i32 0.34065 (3.16x time). v0.1.1 and v0.1.2
  are near tied. A narrow integer measurement range alone did not reproduce
  the Covertype elevation gain.
- Chest activity uses direct output but is near pdq: 0.09425 vs 0.09185 ms,
  aggregate ratio 1.0000. Original labels change only nine times. Direct output
  does not by itself establish a useful net gain on block-structured inputs.
- Chest sequence remains native, near pdq and about 2.54x CAFS u32 time. Existing
  sorted-input weaknesses against specialized implementations remain.
- Taxi pickup zone: mixed 8-bit/shift1, 9.49370 ms vs pdq 12.94025 (1.34x faster),
  but CAFS u32 5.93475 ms and Counting 7.65335 ms are faster than ABW.
- Taxi time: native, 57.9570 vs pdq 57.63485 ms, near tie. Here CAFS u32 takes
  81.60325 and Spread 68.94735 ms, so the prior NASA timestamp loss does not
  repeat against those implementations. Distribution and source arrangement
  differ; no general timestamp rule follows.

Direct output fires on **5/9** selected columns; this is not a production-use
frequency. The direct subgroup GM speedup vs pdq is 1.8816x, non-direct 1.0239x.
Non-direct time vs v0.1.1 is 1.0029 (near unchanged). Those are comparisons of
the whole implementations, not an isolation of the purity-check instruction
cost. Strong wins and losses within each subgroup must remain visible.
The aggregate is sensitive to column composition: these columns share three
sources and are not nine independent random populations. The large red-column
gain contributes heavily. No generalization beyond this scope is asserted.

## Separate ABW memory audit

The following is additional peak *requested C++ new/new[] bytes during sort*,
including sampling/dispatch, in separate instrumented processes. Preexisting
input/expected buffers, allocator metadata, stack and RSS are excluded. This is
not a competitor memory comparison, and these processes supply no timing data.
Every memory-audit output also matched its complete expected array.

| Input | Peak requested bytes | Total requested bytes | Allocation calls |
|---|---:|---:|---:|
| skin_red | 9,448 | 19,120 | 191 |
| skin_label | 8,192 | 10,320 | 5 |
| chest_sequence | 0 | 0 | 0 |
| chest_x | 657,188 | 668,884 | 156 |
| chest_activity | 8,192 | 10,608 | 12 |
| taxi_vendor | 8,272 | 16,464 | 5 |
| taxi_payment | 8,392 | 16,600 | 9 |
| taxi_pickup_zone | 12,274,248 | 12,288,624 | 96 |
| taxi_pickup_time | 132,088 | 157,472 | 2060 |

Direct paths use about 8–10 KiB requested peak in these cases. The taxi zone
mixed path requests about 12.27 MB, close to 4*n plus metadata, and still loses
to CAFS and Counting. The sensor mixed path requests 657 KB and also loses.
Extra memory is justified by measured gains for the three winning direct paths
on this machine; memory cost does not imply a net benefit on mixed paths.

## Conclusion

**Maintain limited-scope publication (B).** Existing direct-output advantages
generalize to some new real datasets, with three new all-comparator wins.
ABW is not the overall winner: CAFS wins the new aggregate, a sorted two-code
column newly regresses relative to v0.1.1, and continuous sensor measurements
do not automatically benefit. Direct output can be correct and still uneconomic.

This validation neither upgrades ABW to a broadly superior library nor erases
its practical successes. Existing v0.1.2 and GitHub remain unchanged. No new
optimization, threshold, policy, release or automatic follow-up experiment.

## Reproduce and audit

Files here contain frozen public v0.1.2 and namespaced v0.1.1 headers, harness,
pinned CAFS sources/licenses, raw JSONL, tables, design, input/source SHA-256,
memory audit and scripts. No downloaded datasets, compiler or binary is bundled.
The full original three source files are downloaded from official hosts by
download.py; hashes must match sources.json. Input manifest records per-column
meaning, conversion, size, cardinality, range, duplicates, missing counts and
expected-array hashes. No artificial replication, random rearrangement, value
scaling of sensor measurements, signed aliasing, fractional truncation or
performance-based row/column exclusion.

Original workspace commands (Python 3.12, NumPy 2.3.5, pyarrow 26.0.1):

```powershell
python -m pip install --target work/abw_new_real_v012/python_deps pyarrow==26.0.1
python work/abw_new_real_v012/download.py
python work/abw_new_real_v012/prepare.py
python work/abw_new_real_v012/run.py
python work/abw_new_real_v012/memory.py
python work/abw_new_real_v012/analyze.py
```

Use a fresh workspace/output location. Scripts retain original research paths;
adapt paths only. Preserve recorded comparator code, eligibility, seeds, flags
and timer boundaries. Need Boost 1.90.0, the compatible x64 AVX2/BMI C++ compiler,
the included pinned CAFS vendor sources, and public-header files at the paths
shown in build/protocol JSON. The archive vendor/header snapshots are references
for restoring those prerequisite paths; do not silently substitute newer code.
run.py creates session files exclusively and refuses to overwrite them. The
design/download and extraction steps are deterministic; verify source and input
hashes before new timing. analyze.py recreates tables/initial report; this final
interpretation is an authored snapshot, not regenerated by that script.

No cross-CPU validation was performed. There was no hardware-counter profile,
no new competitor memory measurement and no synthetic-regression rerun in this
new-data experiment, since sorting implementation itself was not modified.
All nine selected columns fit signed int32. Timestamp counting was ineligible
under the preexisting memory cap; its absence is disclosed rather than timed
with a different algorithm. Existing real/synthetic benchmark records were not
modified or pooled into these aggregates.
