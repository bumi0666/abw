# Exact sample mixed-bucket certificate: AVX2 skip-only comparison

## What changed

Experimental copy only. Same frozen ABW decision, sample positions/seeds,
thresholds and selected bit interval. After the original entropy decision,
if the selected active interval has nonzero shift, reuse the already collected
sample and the now-consumed entropy histogram. Clear that histogram and store
size_t(value)+1 as the first value per bucket, with0 denoting empty. The existing
64-bit size_t contract safely represents UINT32_MAX+1. Two different actual
sample values in one final bucket prove exact direct output is impossible.
Then call the whole-input AVX2 backend immediately, without the full count/
purity pass. No extra allocation, sample draw, whole-input scan or threshold.

Zero-shift buckets already identify exact values, so do not run the proof there.
Native/skip paths retain their decisions; entropy rejection does not run the
extra proof. Missing sample evidence retains the original full count/purity
verification. The existing width scan is still performed where required to
determine the final bucket interval. Full mixed ABW->pdq, its policy and
workspace implementation are unchanged. This experiment targets skip-only
fallback cost; it does not change IPv4's full-partition policy.

## Checks and measurement

Pre-run safety:79 inputs/711 full-output comparisons across pdq, Spreadsort,
AVX2 and old/new wrappers. Independent std::map bucket witness checks,
UINT32_MAX, boundary lengths, rare mixed values not seen in samples (3 cases)
and all66 original strategy/bit-interval comparisons pass. Missing evidence
correctly falls back to the original full inspection.

Windows11/i5-14500; LLVM-MinGW Clang23.1.5/libc++ UCRT; Boost1.90.0,
x86-simd-sort v7.0 commit8a7208187d99f0cb67e38c57ab1c7c85aad07aca. Common
C++17 -O3 -DNDEBUG -mavx2 -mno-avx512f -mbmi -static -pthread. Single-thread,
AVX2 only. Same unchanged66 inputs: real9+new real9+synthetic42+IPv4_6.
Same binary, shared fixed uint32 scratch/common noinline pointer timer. Load,
restore and audits outside timing; complete frontend/backend API costs inside.
Every sorted array is checked against the full hashed expected array. Four
ABBA/BAAB blocks per pair, shuffled comparator order, three fresh-process
sessions/input:198 processes/6,336 timed sorts. Warm repeated-call condition.
Source/executable/input/expected hashes pass before and after; choices, direct
output and evidence are stable across processes. No input filtering or tuning.

Primary ratios: same-pair call medians/process, median3 process ratios/input,
equal-input geometric mean. Original skip-only/backend ratios are computed
from each method's same-process medians in the two pairs, then median3; they
are separately labeled. Times are median3 process medians within each pair.
All-input ratios weight42 synthetics equally with each other column. No
workload prevalence or other-platform claim is attached to this average.

## Geometric mean time ratios

Below1 means faster. Old and candidate refer to skip-only AVX2 wrappers.

| Group | N | Old/AVX2 | Candidate/AVX2 | Candidate/old |
|---|---:|---:|---:|---:|
| development_real | 9 | 1.00159 | 0.99032 | 0.98719 |
| new_real | 9 | 1.33232 | 1.21525 | 0.90835 |
| real18 | 18 | 1.15517 | 1.09704 | 0.94695 |
| synthetic | 42 | 1.02712 | 0.97206 | 0.94469 |
| ctu_ipv4 | 6 | 1.65772 | 1.00972 | 0.61260 |
| all66 | 66 | 1.10774 | 1.00813 | 0.90881 |
| direct | 18 | 0.96099 | 0.96001 | 0.99295 |
| mixed | 27 | 1.29827 | 1.03396 | 0.79713 |
| native_skip | 21 | 1.02027 | 1.01765 | 0.99707 |
| witness | 27 | 1.29827 | 1.03396 | 0.79713 |
| no_witness | 39 | 0.99247 | 0.99063 | 0.99516 |

## All66 input times and paired ratios

| Input | Path | Sample witness | AVX2 ms | Candidate paired/AVX2 ms | Candidate/AVX2 | Old ms | Candidate paired/old ms | Candidate/old |
|---|---|---|---:|---:|---:|---:|---:|---:|
| nasa_status | native/skip | False | 0.78950 | 0.79455 | 1.00640 | 0.82570 | 0.80300 | 0.99893 |
| nasa_timestamp | native/skip | False | 6.96305 | 6.96825 | 0.99903 | 7.07125 | 7.14370 | 1.00793 |
| nasa_bytes | native/skip | False | 3.48385 | 3.71595 | 1.08751 | 3.78960 | 3.74650 | 0.98863 |
| covtype_cover | direct | False | 0.28430 | 0.35765 | 1.26563 | 0.35995 | 0.35575 | 0.99632 |
| covtype_elevation | direct | False | 1.29980 | 0.36325 | 0.27957 | 0.36780 | 0.36700 | 0.99449 |
| covtype_road_distance | mixed fallback | True | 1.75980 | 1.87495 | 1.06543 | 2.11850 | 1.83415 | 0.89315 |
| census_caseid | native/skip | False | 9.13545 | 9.31905 | 1.02010 | 9.45305 | 9.34470 | 1.01340 |
| census_industry | direct | False | 1.69595 | 1.48010 | 0.87273 | 1.48870 | 1.50550 | 0.99227 |
| census_sex | direct | False | 0.66185 | 1.62875 | 2.49668 | 1.61630 | 1.59760 | 1.00529 |
| synthetic_100000_uniform | mixed fallback | True | 0.25245 | 0.26195 | 1.03763 | 0.30080 | 0.26365 | 0.87650 |
| synthetic_100000_normal | mixed fallback | True | 0.26040 | 0.27075 | 1.04215 | 0.31645 | 0.27355 | 0.86491 |
| synthetic_100000_exponential | mixed fallback | True | 0.25205 | 0.26555 | 1.05040 | 0.30290 | 0.26465 | 0.87372 |
| synthetic_100000_narrow | mixed fallback | True | 0.27030 | 0.28795 | 1.06530 | 0.33020 | 0.28710 | 0.86947 |
| synthetic_100000_duplicates | direct | False | 0.04135 | 0.07290 | 1.75441 | 0.07265 | 0.07300 | 1.00482 |
| synthetic_100000_duplicates_narrow | direct | False | 0.04055 | 0.04465 | 1.08685 | 0.04390 | 0.04445 | 0.99292 |
| synthetic_100000_sorted | native/skip | False | 0.23025 | 0.23835 | 1.00624 | 0.23175 | 0.23135 | 0.99854 |
| synthetic_100000_partial | native/skip | False | 0.26190 | 0.26095 | 0.99637 | 0.26675 | 0.26490 | 1.00722 |
| synthetic_100000_reverse | native/skip | False | 0.24140 | 0.24365 | 1.00932 | 0.24225 | 0.24395 | 0.99618 |
| synthetic_100000_upper_only | direct | False | 0.28445 | 0.12555 | 0.44833 | 0.12120 | 0.11955 | 0.99448 |
| synthetic_100000_lower_only | mixed fallback | True | 0.26505 | 0.28365 | 1.07300 | 0.32580 | 0.28330 | 0.87144 |
| synthetic_100000_discontinuous | native/skip | False | 0.26015 | 0.27355 | 1.05212 | 0.27460 | 0.27465 | 0.99398 |
| synthetic_100000_extreme_skew | native/skip | False | 0.06160 | 0.06240 | 1.01057 | 0.06225 | 0.06235 | 1.00161 |
| synthetic_100000_mixture | mixed fallback | True | 0.22965 | 0.23935 | 1.04051 | 0.30745 | 0.23775 | 0.79362 |
| synthetic_1000000_uniform | mixed fallback | True | 3.47505 | 3.57580 | 1.04221 | 3.98860 | 3.56160 | 0.89350 |
| synthetic_1000000_normal | mixed fallback | True | 3.49935 | 3.59080 | 1.02601 | 4.03535 | 3.57265 | 0.88534 |
| synthetic_1000000_exponential | mixed fallback | True | 3.52775 | 3.53565 | 1.01777 | 3.98245 | 3.67305 | 0.89198 |
| synthetic_1000000_narrow | mixed fallback | True | 3.48490 | 3.86620 | 1.07545 | 4.30870 | 3.75720 | 0.88373 |
| synthetic_1000000_duplicates | direct | False | 0.68050 | 0.76885 | 1.12990 | 0.76930 | 0.76540 | 1.00387 |
| synthetic_1000000_duplicates_narrow | direct | False | 0.67640 | 0.61675 | 0.89931 | 0.63615 | 0.60065 | 0.98242 |
| synthetic_1000000_sorted | native/skip | False | 3.29965 | 3.28500 | 0.99325 | 3.28695 | 3.27225 | 0.99553 |
| synthetic_1000000_partial | native/skip | False | 3.72600 | 3.77160 | 1.01224 | 3.85450 | 3.69830 | 1.00155 |
| synthetic_1000000_reverse | native/skip | False | 3.47225 | 3.47740 | 1.00259 | 3.48900 | 3.46235 | 1.00264 |
| synthetic_1000000_upper_only | direct | False | 2.77595 | 0.84720 | 0.29730 | 0.80660 | 0.82000 | 1.02042 |
| synthetic_1000000_lower_only | mixed fallback | True | 3.52070 | 3.78795 | 1.07591 | 4.20700 | 3.75430 | 0.88586 |
| synthetic_1000000_discontinuous | native/skip | False | 2.69460 | 2.94715 | 1.09372 | 2.99410 | 2.88770 | 0.98411 |
| synthetic_1000000_extreme_skew | native/skip | False | 1.47750 | 1.48835 | 0.97015 | 1.50485 | 1.49635 | 1.00648 |
| synthetic_1000000_mixture | mixed fallback | True | 3.04350 | 3.07335 | 1.03184 | 3.71765 | 3.11150 | 0.82937 |
| synthetic_10000000_uniform | mixed fallback | True | 41.83050 | 42.75810 | 1.02163 | 45.81320 | 42.45525 | 0.93348 |
| synthetic_10000000_normal | mixed fallback | True | 43.56320 | 43.32715 | 1.01078 | 48.05130 | 43.58355 | 0.90702 |
| synthetic_10000000_exponential | mixed fallback | True | 43.57410 | 44.50245 | 0.99999 | 46.77525 | 42.13305 | 0.90076 |
| synthetic_10000000_narrow | mixed fallback | True | 39.99055 | 41.70050 | 1.04276 | 46.06555 | 42.68785 | 0.92668 |
| synthetic_10000000_duplicates | direct | False | 7.30820 | 8.71100 | 1.22843 | 8.62205 | 8.52000 | 0.98816 |
| synthetic_10000000_duplicates_narrow | direct | False | 6.55975 | 7.39090 | 1.12670 | 7.39160 | 7.35525 | 0.99185 |
| synthetic_10000000_sorted | native/skip | False | 44.29835 | 44.50170 | 0.99906 | 42.92400 | 42.88925 | 0.99620 |
| synthetic_10000000_partial | native/skip | False | 46.43620 | 46.09155 | 1.00658 | 46.19780 | 44.85655 | 0.97922 |
| synthetic_10000000_reverse | native/skip | False | 43.70295 | 43.54605 | 0.99641 | 43.96835 | 43.20115 | 0.98255 |
| synthetic_10000000_upper_only | direct | False | 23.57085 | 8.73870 | 0.36061 | 8.95335 | 8.84800 | 0.98244 |
| synthetic_10000000_lower_only | mixed fallback | True | 44.02450 | 45.59890 | 1.02327 | 49.49580 | 46.04870 | 0.93488 |
| synthetic_10000000_discontinuous | native/skip | False | 22.88380 | 24.83110 | 1.08510 | 24.42275 | 24.45680 | 1.00139 |
| synthetic_10000000_extreme_skew | native/skip | False | 18.35990 | 18.48220 | 0.99966 | 18.87485 | 18.20395 | 0.99018 |
| synthetic_10000000_mixture | mixed fallback | True | 39.13435 | 39.04620 | 0.97518 | 45.90495 | 38.67130 | 0.84242 |
| skin_red | direct | False | 0.40940 | 0.11340 | 0.27524 | 0.11385 | 0.11285 | 0.99206 |
| skin_label | direct | False | 0.05350 | 0.13915 | 2.59276 | 0.13960 | 0.13970 | 1.00072 |
| chest_sequence | native/skip | False | 0.42735 | 0.42850 | 1.00082 | 0.42455 | 0.42485 | 0.99457 |
| chest_x | mixed fallback | True | 0.21590 | 0.22885 | 1.05998 | 0.38085 | 0.23040 | 0.60496 |
| chest_activity | direct | False | 0.04770 | 0.09070 | 1.90147 | 0.09025 | 0.09065 | 0.99584 |
| taxi_vendor | direct | False | 0.84780 | 2.14285 | 2.46408 | 2.15725 | 2.16050 | 0.96066 |
| taxi_payment | direct | False | 1.43540 | 2.09945 | 1.44371 | 2.12425 | 2.09055 | 0.97554 |
| taxi_pickup_zone | mixed fallback | True | 3.95970 | 4.41125 | 1.09146 | 5.84285 | 4.36230 | 0.75646 |
| taxi_pickup_time | native/skip | False | 13.00320 | 13.18680 | 1.03423 | 13.27170 | 13.03190 | 0.99830 |
| ctu1_srcaddr_ipv4 | mixed fallback | True | 6.92920 | 7.50070 | 1.00669 | 9.81575 | 7.37350 | 0.74367 |
| ctu1_dstaddr_ipv4 | mixed fallback | True | 3.94195 | 3.98500 | 1.01162 | 7.39670 | 3.96905 | 0.53837 |
| ctu2_srcaddr_ipv4 | mixed fallback | True | 4.34180 | 4.41025 | 1.02070 | 5.89575 | 4.40280 | 0.74678 |
| ctu2_dstaddr_ipv4 | mixed fallback | True | 2.09015 | 2.17110 | 1.03873 | 4.26135 | 2.17405 | 0.49670 |
| ctu3_srcaddr_ipv4 | mixed fallback | True | 9.69380 | 9.48920 | 0.98245 | 15.04825 | 9.82775 | 0.64865 |
| ctu3_dstaddr_ipv4 | mixed fallback | True | 6.92215 | 6.94435 | 0.99903 | 12.53475 | 6.91655 | 0.54866 |

## Largest regressions retained

| Comparator | Input | Candidate/comparator | Three process ratios |
|---|---|---:|---|
| old_skip_avx2 | synthetic_1000000_upper_only | 1.02042 | 1.0204, 1.0247, 1.0046 |
| old_skip_avx2 | census_caseid | 1.01340 | 1.0186, 1.0134, 0.9763 |
| old_skip_avx2 | nasa_timestamp | 1.00793 | 1.0079, 1.0102, 0.9959 |
| old_skip_avx2 | synthetic_100000_partial | 1.00722 | 1.0090, 1.0072, 0.9773 |
| old_skip_avx2 | synthetic_1000000_extreme_skew | 1.00648 | 0.9319, 1.0065, 1.0082 |
| avx2 | skin_label | 2.59276 | 2.6107, 2.5928, 2.5206 |
| avx2 | census_sex | 2.49668 | 2.4967, 2.3972, 2.8172 |
| avx2 | taxi_vendor | 2.46408 | 2.3704, 6.5289, 2.4641 |
| avx2 | chest_activity | 1.90147 | 1.9229, 1.8642, 1.9015 |
| avx2 | synthetic_100000_duplicates | 1.75441 | 1.7642, 1.7544, 1.7482 |

## Separate sample-bucket diagnosis of the full mixed policy

The pre-run diagnostic used exactly the final selected bits/shift and existing
sample on the27 mixed inputs from the earlier full-ABW run:22 faster than skip,
5 slower. Maximum sample occupancy ranges overlap: winners0.195-67.822%
(median0.415%), losers21.094-83.594% (median80.518%). IPv4 destinations occupy
80.5-83.6%, sources57.0-67.8%. All six use8-bit buckets/shift24 and entropy
cutoff1. The destination skew is directly observed; no threshold was added.
Mixed evidence is found on all27. See sample_bucket_diagnosis.csv and JSON
for exact witness values and the linked earlier full/skip timings. This is
separate from the performance effects of the skip-only certificate.

## Reproduction

From the original workspace root after compiling using comparison_build.json:

```
python work/abw_sample_certificate/run_sample_certificate_comparison.py
```

The output directory must not already exist. The archive includes raw sessions,
all-input CSV, frozen headers, exact certificate source, build commands, short
safety tests/results, source/input hashes, earlier bucket diagnosis and pinned
x86 source ZIP. Boost/compiler/dataset binaries are not bundled. Plan status
is the frozen prepared state; completion.json records the completed run.
Public ABW remains unchanged and no release is issued by this experiment.
