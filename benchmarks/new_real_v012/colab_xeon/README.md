# Separate Colab CPU verification

Run completed 2026-11-13 on **Intel(R) Xeon(R) CPU @ 2.20GHz**, Ubuntu GCC 13.3.0,
Google Compute Engine hosted runtime. Exact CPU model string, flags, memory,
affinity, OS/compiler and source hashes: [environment.json](environment.json).
This is a different runtime CPU from the local i5-14500. CPU, OS, compiler and
standard library all differ; changes cannot be attributed only to hardware.
This is not an independently controlled bare-metal replication.

All 27 processes / 2,544 timed sorts passed full-array correctness checks.
All nine input/expected hashes and route/bits/shift/direct fields match the
local run. Public v0.1.2 header bytes match. No sorting-code or policy changes.
Only Windows process-affinity glue was ported to Linux sched affinity; all
implementations use the same GCC flags and timing boundaries within Colab.

## Aggregate results, kept separate

| Metric, new9 only | Local i5-14500 Windows/Clang | Colab Xeon Linux/GCC |
|---|---:|---:|
| v0.1.2 GM speedup vs pdq | 1.4357x | 1.5283x |
| v0.1.1 GM speedup vs pdq | 1.2531x | 1.2012x |
| v0.1.2/v0.1.1 time | 0.8786 | 0.7856 |
| v0.1.2/CAFS u32 time | 1.1040 | 0.9635 |
| v0.1.2/CAFS i32 time | 1.1583 | 1.0704 |
| v0.1.2 vs pdq wins/near ties/losses (5% band) | 4/3/2 | 5/3/1 |
| Direct-output columns | 5/9 | 5/9 |

Counting was eligible on eight columns, all other implementations on nine;
do not compare unequal-subset averages. Original local and Colab records are
not pooled. Same estimator as local: median within process, median of three
processes per input; median same-process ratios then geometric mean. See the
[raw Colab report](report.md) for every sorter and input time.

## Per-input ratios

| Input | Local v0.1.2/pdq time | Colab v0.1.2/pdq time | Local v0.1.2/v0.1.1 | Colab v0.1.2/v0.1.1 |
|---|---:|---:|---:|---:|
| skin_red | 0.0979 | 0.1113 | 0.8054 | 0.7717 |
| skin_label | 1.3933 | 0.8580 | 1.3807 | 0.8630 |
| chest_sequence | 1.0117 | 1.0191 | 1.0018 | 0.9777 |
| chest_x | 1.1982 | 1.0389 | 1.0123 | 1.0106 |
| chest_activity | 1.0000 | 0.6785 | 0.9916 | 0.6873 |
| taxi_vendor | 0.8479 | 0.6102 | 0.8197 | 0.6077 |
| taxi_payment | 0.3665 | 0.4643 | 0.3411 | 0.4131 |
| taxi_pickup_zone | 0.7458 | 1.1127 | 0.9997 | 0.9994 |
| taxi_pickup_time | 1.0066 | 1.0166 | 0.9979 | 1.0052 |

## What reproduced and what changed

- Skin red remains a strong all-comparator win: 0.28951 ms versus pdq 2.59898,
  CAFS i32 1.23454 and Counting (full table retained). v0.1.2 improves over v0.1.1
  by 22.8% by aggregated time ratios.
- Taxi provider/payment retain improvements versus v0.1.1: time ratios 0.6077
  and 0.4131. They cease to be all-comparator winners: provider ABW 6.94816 ms
  versus CAFS u32 6.21193; payment ABW 5.58197 versus CAFS u32 5.40707 (near tie).
- Chest activity becomes an all-comparator ABW win: 0.19980 ms versus pdq 0.29446.
  Colab nominal/clear non-ABW wins are Skin red and Chest activity, two columns.
- The sorted Skin-label regression does not reproduce in this environment:
  v0.1.2/v0.1.1 time is 0.8630, versus 1.3807 locally. The original local loss
  remains valid and published. This change does not justify deleting it or
  treating the sorted low-cardinality selection issue as resolved.
- Pickup zone changes from a pdq win locally to a loss in Colab: v0.1.2/pdq
  time ratio 1.1127 (see exact CSV), while the dispatch remains mixed active8/shift1.
  Specialized sorters remain stronger there. Chest x remains noncompetitive
  against CAFS i32; relative pdq loss is smaller in Colab.

Overall v0.1.2 improvement over v0.1.1 reproduced on a different runtime CPU,
with a 21.4% GM time reduction on these nine columns. Specific winners and the
sorted-label regression are environment-dependent. CAFS u32 is close in the
Colab aggregate (ABW time 3.65% lower); CAFS i32 remains ahead (ABW 7.04% higher).
These small aggregate differences do not establish broad superiority.

Keep the limited-scope experimental positioning. If optimization is requested
later, first investigate direct-output economics on already ordered low-
cardinality inputs while retaining both environments' evidence. No new guard,
threshold, implementation change or release was introduced here.

[Raw result ZIP](raw_records.zip) contains all three sessions, source snapshots,
checks, timing tables and machine metadata. ZIP entry hashes were verified after
download. [Notebook](../colab_cpu.ipynb) embeds the exact reference bundle and
regenerates the same inputs; source downloads, extraction and compilation are
outside sort timing. This run measured new9 only, not the older development9
or synthetic42. No new memory instrumentation was run on Colab.
