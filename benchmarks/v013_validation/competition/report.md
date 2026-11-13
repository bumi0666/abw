# ABW v0.1.3 BD candidate: final competition comparison

Date: 2026-11-13. Frozen B+D only versus byte-identical public v0.1.2 and unchanged
Boost pdqsort/Spreadsort, pinned CAFS u32/i32 and Counting. No C/C-prime, algorithm
or threshold changes. All development9 + new9 + synthetic42 retained; no new
datasets or diagnostic inputs. No GitHub/public-header/release changes.

## Conditions

Original competition harness reused: one binary, one fixed uint32 scratch and
common noinline function-pointer timer. CAFS i32 uses separate fixed int32
scratch with the exact same representable values, conversion outside timer.
Candidate BD anchored ABBA/BAAB against each competitor, four blocks each,
randomized pair order; three fresh processes per input. Original warmups.
Loading/restoration/conversion/equality/route audits outside timer; all actual
sort scans, sample allocation, histogram initialization/merge, scatter/sort,
reconstruction/copy and free inside. Uninstrumented full-call times, no outlier
filtering or timing-based exclusions.

Same local Intel i5-14500 Windows x64 first-allowed-CPU affinity, LLVM-MinGW
Clang23.1.5/libc++/UCRT, Boost1.90.0. Original competition C++20 -O3 -DNDEBUG
-mavx2 -mbmi -static -pthread flags retained for all methods (CAFS requires
C++20). This run's matched BD/v0.1.2 comparison is the evidence; no
historical timings are pooled.
CAFS commit13488afef446ae88e7ad631ec6f862def896094a, supported scalar u32 and
specialized i32 APIs with unchanged pdq fallback. Counting is the original dense
uint64 histogram, runtime min/max and initialization inside timer, 64MiB cap.
CAFS i32 unsupported when values exceed INT32_MAX; no compression/remapping.

B+D source bytes equal the previously tested D.hpp. v0.1.2 source bytes equal the
released public header. BD must preserve v0.1.2's executable sort route, active
bits/shift and direct/mixed result. Unused native bits/shift metadata may differ
and are separately recorded. Every timed output equals the complete expected
array. Source and input/expected SHA-256 checked before/after; one CPU only.

## Estimators

Times: median method calls within process, then median of three process medians.
Primary speedups and time ratios: median same-process method-median ratios,
then geometric mean over eligible inputs. BD's main median pools its anchored
calls across competitors, matching prior competition tables. A small BD/v0.1.2
effect also has two same-pair views: ratio of the BD/v0.1.2 pair's own call
medians, and median block ratios of two-call means. All views are retained;
none is substituted after seeing the outcome. Ratios need not equal displayed
median-time quotients. Descriptive 5% bands are not significance tests.

Nominal winner = lowest displayed median among all eligible methods including
v0.1.2. Clear best = main time ratio below0.95 against every eligible method.
Clear non-ABW best excludes the near-identical v0.1.2; these categories must
not be conflated. Unsupported methods are not losses; subset denominators below.

**13,920 timed sorts in180 fresh processes; all full-array, route and direct/mixed checks passed.**

## Geometric mean speedup versus pdqsort

| Method | Development9 | New9 | Synthetic eligible subset (N) | All eligible subset (N) |
|---|---:|---:|---:|---:|
| BD | 1.8526 | 1.4995 | 1.4794 (42) | 1.5333 (60) |
| v012 | 1.8057 | 1.4594 | 1.4385 (42) | 1.4916 (60) |
| pdq | 1.0000 | 1.0000 | 1.0000 (42) | 1.0000 (60) |
| spread | 0.7268 | 0.6772 | 0.6404 (42) | 0.6582 (60) |
| CAFS_u32 | 1.8265 | 1.6093 | 1.5234 (42) | 1.5783 (60) |
| CAFS_i32 | 1.8709 | 1.6935 | 2.3081 (6) | 1.8995 (24) |
| counting | 0.7063 | 0.6034 | 1.5026 (9) | 0.8738 (26) |

Do not compare CAFS i32/Counting subset averages with the all60 ABW average as if the input sets matched. See their paired time ratios on the same subsets in summary.json.

## BD/v0.1.2 time ratios

| Group | Primary pooled-anchor | Pair-only medians | Block-paired |
|---|---:|---:|---:|
| development_real | 0.97639 | 0.97368 | 0.96813 |
| new_real | 0.97716 | 0.97971 | 0.98934 |
| synthetic | 0.97150 | 0.97113 | 0.97081 |
| all60 | 0.97308 | 0.97279 | 0.97316 |

## B ordered-bypass inputs (not all D-affected inputs; same dispatch)

| Input | v0.1.2 ms | BD ms | BD/v0.1.2 | Pair-only ratio | Block-paired ratio |
|---|---:|---:|---:|---:|---:|
| covtype_cover | 0.37200 | 0.35730 | 0.9605 | 0.9746 | 0.9630 |
| skin_label | 0.13365 | 0.13020 | 0.9742 | 0.9727 | 0.9705 |
| chest_activity | 0.09850 | 0.09000 | 0.9558 | 0.9538 | 0.9485 |
| taxi_vendor | 2.08995 | 2.05005 | 0.9687 | 0.9600 | 1.0307 |

## Every input time (ms)

| Input | BD | v0.1.2 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest median |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| nasa_status | 2.44295 | 2.49820 | 2.46770 | 5.53700 | 3.62935 | 3.75845 | 4.11255 | BD |
| nasa_timestamp | 1.38355 | 1.36675 | 1.33570 | 0.60955 | 0.42910 | 0.42900 | 16.40220 | CAFS_i32 |
| nasa_bytes | 12.02860 | 12.36710 | 11.97340 | 17.07790 | 10.90500 | 7.09900 | 19.89155 | CAFS_i32 |
| covtype_cover | 0.35730 | 0.37200 | 0.84735 | 1.82085 | 0.39245 | 0.40805 | 0.97050 | BD |
| covtype_elevation | 0.36935 | 0.41490 | 4.18755 | 3.55335 | 1.14050 | 1.17655 | 0.93040 | BD |
| covtype_road_distance | 2.96840 | 3.00060 | 5.16880 | 5.42110 | 1.15570 | 1.13560 | 0.67075 | counting |
| census_caseid | 1.27630 | 1.25760 | 1.20820 | 0.77545 | 0.54810 | 0.57355 | 8.40980 | CAFS_u32 |
| census_industry | 1.53095 | 1.54910 | 7.40730 | 12.47810 | 6.63450 | 6.57530 | 6.02980 | BD |
| census_sex | 1.64500 | 1.61215 | 2.16770 | 12.49295 | 1.76680 | 1.90195 | 7.20285 | v012 |
| synthetic_100000_uniform | 1.23200 | 1.28365 | 1.66390 | 1.51985 | 1.68130 | unsupported | unsupported | BD |
| synthetic_100000_normal | 1.32545 | 1.35375 | 1.69600 | 1.52145 | 1.67955 | unsupported | unsupported | BD |
| synthetic_100000_exponential | 1.35545 | 1.42355 | 1.63430 | 1.53910 | 1.65085 | unsupported | unsupported | BD |
| synthetic_100000_narrow | 1.06980 | 1.09470 | 1.63195 | 1.50630 | 1.65440 | 1.75140 | 0.61190 | counting |
| synthetic_100000_duplicates | 0.07725 | 0.08985 | 0.16570 | 0.39775 | 0.06580 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_duplicates_narrow | 0.04420 | 0.04570 | 0.17155 | 0.37705 | 0.06635 | 0.06675 | 0.27645 | BD |
| synthetic_100000_sorted | 0.03495 | 0.03500 | 0.03420 | 0.02565 | 0.01310 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_partial | 0.42295 | 0.42070 | 0.42200 | 0.36665 | 0.43100 | unsupported | unsupported | spread |
| synthetic_100000_reverse | 0.08340 | 0.08275 | 0.08180 | 0.60555 | 0.01880 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_upper_only | 0.14305 | 0.17515 | 1.04085 | 0.86815 | 0.34720 | unsupported | unsupported | BD |
| synthetic_100000_lower_only | 1.21355 | 1.27205 | 1.65300 | 1.51595 | 1.68480 | unsupported | 3.25405 | BD |
| synthetic_100000_discontinuous | 1.03740 | 1.10100 | 1.00995 | 0.82365 | 0.46680 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_extreme_skew | 0.03730 | 0.03720 | 0.03660 | 0.49615 | 0.09545 | unsupported | unsupported | pdq |
| synthetic_100000_mixture | 1.23575 | 1.26885 | 1.44145 | 1.33325 | 1.50425 | unsupported | unsupported | BD |
| synthetic_1000000_uniform | 16.45445 | 16.68385 | 19.26215 | 20.96295 | 19.28815 | unsupported | unsupported | BD |
| synthetic_1000000_normal | 16.73200 | 17.04610 | 19.41070 | 20.54490 | 19.39400 | unsupported | unsupported | BD |
| synthetic_1000000_exponential | 16.22195 | 16.48640 | 18.56045 | 20.17080 | 18.97900 | unsupported | unsupported | BD |
| synthetic_1000000_narrow | 10.93110 | 11.00620 | 13.60490 | 14.64630 | 11.77395 | 7.94120 | 3.37275 | counting |
| synthetic_1000000_duplicates | 0.89275 | 0.97490 | 2.38420 | 4.16965 | 0.67265 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_duplicates_narrow | 0.59505 | 0.57990 | 2.42845 | 3.80900 | 0.68380 | 0.72040 | 2.92110 | v012 |
| synthetic_1000000_sorted | 0.44360 | 0.44445 | 0.44395 | 0.31125 | 0.22080 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_partial | 5.53245 | 5.57515 | 5.44410 | 8.66605 | 5.44640 | unsupported | unsupported | pdq |
| synthetic_1000000_reverse | 0.80190 | 0.82125 | 0.79380 | 6.83815 | 0.37865 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_upper_only | 1.02695 | 1.27180 | 7.83550 | 9.17360 | 1.64300 | unsupported | unsupported | BD |
| synthetic_1000000_lower_only | 16.16975 | 16.44060 | 18.85890 | 20.20260 | 18.94365 | unsupported | 11.63775 | counting |
| synthetic_1000000_discontinuous | 8.32580 | 8.56990 | 8.01300 | 7.79230 | 2.01795 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_extreme_skew | 0.45295 | 0.42775 | 0.44030 | 7.32250 | 0.97045 | unsupported | unsupported | v012 |
| synthetic_1000000_mixture | 15.42690 | 15.84785 | 16.99860 | 18.64130 | 24.67025 | unsupported | unsupported | BD |
| synthetic_10000000_uniform | 180.71410 | 179.11405 | 210.03760 | 240.73515 | 208.79380 | unsupported | unsupported | v012 |
| synthetic_10000000_normal | 187.67840 | 188.89450 | 210.89820 | 226.56725 | 212.71265 | unsupported | unsupported | BD |
| synthetic_10000000_exponential | 183.74650 | 184.59940 | 206.00550 | 221.36595 | 205.24330 | unsupported | unsupported | BD |
| synthetic_10000000_narrow | 74.09935 | 75.74990 | 102.09245 | 166.93745 | 70.55265 | 33.83840 | 31.58950 | counting |
| synthetic_10000000_duplicates | 10.44065 | 10.77995 | 30.61795 | 44.23690 | 8.17000 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_duplicates_narrow | 7.89370 | 7.77425 | 30.78435 | 41.29195 | 8.96505 | 8.76950 | 32.66250 | v012 |
| synthetic_10000000_sorted | 5.26945 | 5.31680 | 5.43180 | 3.15815 | 2.30525 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_partial | 64.89945 | 65.63615 | 65.24785 | 92.76805 | 66.86055 | unsupported | unsupported | BD |
| synthetic_10000000_reverse | 9.98505 | 9.93525 | 10.59795 | 91.23440 | 3.96280 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_upper_only | 10.03280 | 10.52050 | 75.05085 | 185.46850 | 14.65855 | unsupported | unsupported | BD |
| synthetic_10000000_lower_only | 149.20520 | 151.33580 | 178.68810 | 171.10590 | 177.97345 | unsupported | 63.94985 | counting |
| synthetic_10000000_discontinuous | 78.02655 | 77.29785 | 76.36585 | 94.09810 | 18.68450 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_extreme_skew | 5.42865 | 5.52495 | 5.52285 | 76.55200 | 11.57065 | unsupported | unsupported | BD |
| synthetic_10000000_mixture | 167.93330 | 171.04430 | 178.58850 | 185.45015 | 300.24420 | unsupported | unsupported | BD |
| skin_red | 0.11350 | 0.12835 | 1.27375 | 1.01065 | 0.62905 | 0.60520 | 0.45765 | BD |
| skin_label | 0.13020 | 0.13365 | 0.10300 | 0.06285 | 0.03610 | 0.03830 | 0.51000 | CAFS_u32 |
| chest_sequence | 0.05620 | 0.05630 | 0.05530 | 0.04340 | 0.02220 | 0.02225 | 0.51430 | CAFS_u32 |
| chest_x | 0.91815 | 0.91410 | 0.88760 | 0.60600 | 0.46725 | 0.32335 | 0.36050 | CAFS_i32 |
| chest_activity | 0.09000 | 0.09850 | 0.09135 | 0.76445 | 0.16160 | 0.16015 | 0.35405 | BD |
| taxi_vendor | 2.05005 | 2.08995 | 2.51880 | 14.52270 | 2.27395 | 2.39730 | 7.97260 | BD |
| taxi_payment | 2.09430 | 2.09615 | 5.89960 | 13.53055 | 2.18400 | 2.42345 | 6.92020 | BD |
| taxi_pickup_zone | 9.62125 | 9.45100 | 13.44615 | 11.91595 | 6.01905 | 6.04300 | 7.46745 | CAFS_u32 |
| taxi_pickup_time | 56.13245 | 57.51165 | 56.25460 | 68.74440 | 74.76065 | 60.40460 | unsupported | BD |

## Nominal and clear winners

- development_real: nominal BD winners = nasa_status, covtype_cover, covtype_elevation, census_industry; clear versus all = covtype_elevation; clear versus non-ABW = covtype_cover, covtype_elevation, census_industry, census_sex.
- new_real: nominal BD winners = skin_red, chest_activity, taxi_vendor, taxi_payment, taxi_pickup_time; clear versus all = skin_red; clear versus non-ABW = skin_red, taxi_vendor.
- synthetic: nominal BD winners = synthetic_100000_uniform, synthetic_100000_normal, synthetic_100000_exponential, synthetic_100000_duplicates_narrow, synthetic_100000_upper_only, synthetic_100000_lower_only, synthetic_100000_mixture, synthetic_1000000_uniform, synthetic_1000000_normal, synthetic_1000000_exponential, synthetic_1000000_upper_only, synthetic_1000000_mixture, synthetic_10000000_normal, synthetic_10000000_exponential, synthetic_10000000_partial, synthetic_10000000_upper_only, synthetic_10000000_extreme_skew, synthetic_10000000_mixture; clear versus all = synthetic_100000_upper_only, synthetic_1000000_upper_only; clear versus non-ABW = synthetic_100000_uniform, synthetic_100000_normal, synthetic_100000_exponential, synthetic_100000_duplicates_narrow, synthetic_100000_upper_only, synthetic_100000_lower_only, synthetic_100000_mixture, synthetic_1000000_uniform, synthetic_1000000_normal, synthetic_1000000_exponential, synthetic_1000000_duplicates_narrow, synthetic_1000000_upper_only, synthetic_1000000_mixture, synthetic_10000000_uniform, synthetic_10000000_normal, synthetic_10000000_exponential, synthetic_10000000_duplicates_narrow, synthetic_10000000_upper_only.
- all60: nominal BD winners = nasa_status, covtype_cover, covtype_elevation, census_industry, synthetic_100000_uniform, synthetic_100000_normal, synthetic_100000_exponential, synthetic_100000_duplicates_narrow, synthetic_100000_upper_only, synthetic_100000_lower_only, synthetic_100000_mixture, synthetic_1000000_uniform, synthetic_1000000_normal, synthetic_1000000_exponential, synthetic_1000000_upper_only, synthetic_1000000_mixture, synthetic_10000000_normal, synthetic_10000000_exponential, synthetic_10000000_partial, synthetic_10000000_upper_only, synthetic_10000000_extreme_skew, synthetic_10000000_mixture, skin_red, chest_activity, taxi_vendor, taxi_payment, taxi_pickup_time; clear versus all = covtype_elevation, synthetic_100000_upper_only, synthetic_1000000_upper_only, skin_red; clear versus non-ABW = covtype_cover, covtype_elevation, census_industry, census_sex, synthetic_100000_uniform, synthetic_100000_normal, synthetic_100000_exponential, synthetic_100000_duplicates_narrow, synthetic_100000_upper_only, synthetic_100000_lower_only, synthetic_100000_mixture, synthetic_1000000_uniform, synthetic_1000000_normal, synthetic_1000000_exponential, synthetic_1000000_duplicates_narrow, synthetic_1000000_upper_only, synthetic_1000000_mixture, synthetic_10000000_uniform, synthetic_10000000_normal, synthetic_10000000_exponential, synthetic_10000000_duplicates_narrow, synthetic_10000000_upper_only, skin_red, taxi_vendor.

## Final decision: recommend frozen B+D for v0.1.3 integration

The final seven-method competition retains the approximately 2% improvement
target: whole60 B+D/v0.1.2 geometric mean time ratio **0.97308**, a **2.69%**
decrease. Same-pair medians give0.97279 (2.72% decrease) and block-paired0.97316
(2.68% decrease). Original real9 decreases2.36%, new real9 2.28%, synthetic42
2.85%. These are this run's matched results; no historical aggregate is pooled.
The choice is to retain B+D without any further optimization or threshold tuning.
Public header integration, platform CI and release were not performed here.

### Competitive interpretation

Against pdqsort, B+D reaches1.8526x on original real9,1.4995x on new real9,
1.4794x on synthetic42 and1.5333x overall. CAFS u32 remains faster overall:
B+D's time is2.96% longer, reduced from v0.1.2's5.82% gap on the same60.
B+D is1.85% faster on original real9,7.41% slower on new real9 and3.09%
slower on synthetic42 than CAFS u32. Against CAFS i32 it is4.47% slower on
the supported24; that subset must not be compared with all60 as if identical.

B+D has the lowest displayed median on9/18 real inputs (4 original,5 new)
and18/42 synthetic inputs. Many are near ties. With the descriptive >5% margin
against every eligible non-ABW implementation, real winners are Covertype cover,
elevation, Census industry, sex, Skin red and Taxi vendor. Including near-identical
v0.1.2, only elevation and Skin red meet that real-input margin. These counts
describe this local sample, not statistical superiority across CPUs.

### Regression checks (all measurements retained)

The prior NASA bytes4.8% loss was D versus B-only. This run compares B+D with
released v0.1.2, so it does not directly repeat that exact contrast. The loss
does not recur versus the current release: main ratio0.97585 (-2.42%), pair-only
0.97646, block-paired0.98778. Its three primary process ratios are1.01938,
0.97585,0.96759; pair-only1.00141,0.97646,0.95307. The input is still slower
than CAFS (u32 ratio1.10594, i32 ratio1.70112), so this is not a repaired
competitive weakness.

Maximum primary regression versus v0.1.2 is10M sorted: **+3.61%**, process
ratios1.03611,0.97516,1.05008. The pair-only median is+5.24% (process ratios
1.06995,0.99762,1.05243), block-paired+3.44%. Slowdown is present in2/3
processes; retain it as a small observed regression. This input uses the native
route and skips both changed analysis paths. No cause is established, and no
same-function A/A control was added to this competition. Do not attribute the
loss to hash-table work or dismiss it as measured noise.

Other previously flagged cases:1M narrow duplicates primary+2.06%, pair-only
-0.60%, block-paired-0.15% (no consistent prior6.8% loss);1M partial primary
-0.77%, pair-only-2.05%;1M lower-only primary-2.34%, pair-only-0.21%.
Taxi pickup zone is the largest new-real primary loss at+1.72%, pair-only
+1.80%, block-paired+3.83%; process direction varies. No input has a primary
median regression above5%; pair-only sorted exceeds5% as disclosed above.
This is not a claim that every input or process improves.

The direct-path sample-analysis gains remain clear: Covertype elevation main
time reduction13.21%, Skin red11.57%, both improve in all three processes.
The B-only bypass inputs improve3.95% (cover),2.58% (Skin label),4.42%
(Chest activity),3.13% (Taxi vendor). No new dispatch strategy is used.

### Scope, memory and limitations

All180 process audits preserve route, actual active bits/shift and direct/mixed
result. Full timed arrays match expected arrays on all13,920 calls. No unused
native metadata differences occurred on these60 inputs. The raw `changed_branch`
flag labels only B's four ordered-low-byte bypass inputs; D replaces frequency
analysis on40 inputs, not four. It is not a total-candidate-change flag.

D trades unordered_map node allocation for a fixed4096-slot table:16KiB uint32
keys plus8KiB uint16 counts in a separate noinline sample helper. The original
sample-values vector remains. The added24KiB stack payload is relevant to
small-stack callers; linear probing has a collision-dependent worst case. The
prior D exact-frequency/collision tests are retained as separate evidence, not
new measurements in this run. No fresh peak-memory profile, new collision
policy, cross-CPU validation of B+D, or statistical equivalence claim is made.

## Regression detail: process ratios

Ratios below are B+D/v0.1.2 (lower is better), in session0/1/2 order.

| Input | Primary process ratios | Same-pair process ratios |
|---|---|---|
| nasa_bytes | 1.01938, 0.97585, 0.96759 | 1.00141, 0.97646, 0.95307 |
| synthetic_10000000_sorted | 1.03611, 0.97516, 1.05008 | 1.06995, 0.99762, 1.05243 |
| synthetic_1000000_duplicates_narrow | 1.02058, 0.97002, 1.04484 | 1.02616, 0.98592, 0.99396 |
| synthetic_1000000_partial | 0.96792, 0.99379, 0.99234 | 0.96427, 0.98375, 0.97946 |
| synthetic_1000000_lower_only | 0.95890, 0.97657, 0.99844 | 0.97266, 0.99791, 1.00656 |
| taxi_pickup_zone | 0.93305, 1.01718, 1.01801 | 0.90724, 1.04510, 1.01801 |

## Largest remaining competitive losses

| Input | B+D/pdq time | B+D/CAFS u32 time | Lowest displayed median |
|---|---:|---:|---|
| synthetic_100000_reverse | 1.0196 | 4.4596 | CAFS_u32 |
| synthetic_10000000_discontinuous | 1.0190 | 4.3132 | CAFS_u32 |
| synthetic_1000000_discontinuous | 1.0510 | 4.2565 | CAFS_u32 |
| skin_label | 1.3189 | 3.6066 | CAFS_u32 |
| nasa_timestamp | 1.0363 | 3.2243 | CAFS_i32 |
| synthetic_10000000_reverse | 0.9457 | 2.6188 | CAFS_u32 |
| synthetic_100000_sorted | 1.0219 | 2.5511 | CAFS_u32 |
| chest_sequence | 1.0126 | 2.5315 | CAFS_u32 |

## Reproduction and artifacts

Raw `session0/1/2.jsonl`, per-call times, process medians, paired blocks,
per-input results, summary, complete source/input SHA-256 protocol, compiler
command and source snapshots are included. No raw record was filtered/edited.
The input arrays and Boost/toolchain are not bundled. Restore the original
real/synthetic inputs and new-real9 from the public repository documentation;
validate against all60 hashes before timing. Source provenance and manifests
are copied into input_provenance/. All original order/values/seeds are retained.

Original Windows workspace commands (fresh outputs required):

```powershell
python work/abw_v013_bd_competition/setup.py
python work/abw_v013_bd_competition/run.py
python work/abw_v013_bd_competition/analyze.py
python work/abw_v013_bd_competition/finalize.py
```

setup.py reconstructs this candidate comparison from the included prior B-only
competition scripts/source/build, released header and accepted D.hpp. Recreate
their original paths (reference_paths.json) or adapt paths only. Preserve source
bytes, flags, eligibility, seeds and timing boundaries. The build command also
permits compiling benchmark.cpp directly with the included headers and CAFS,
external Boost1.90.0 and LLVM-MinGW. This harness is Windows-specific.

The measured runner completed all180 jobs and post-run source/input hash checks,
then its artifact-copy step referred to absent prepare.py. That packaging-only
reference was corrected to setup.py; all source/hash audits were independently
rechecked during finalization. No timing rerun or raw-data edit was needed.
The initial analyzer report is supplemented by this authored assessment.
No follow-up timing run or additional optimizer was introduced.
