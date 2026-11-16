# Three-backend skip-only ABW comparison

## What this measures

Experimental copy of the frozen v0.2 candidate's preprocessing. Same sample
positions/seeds, order/entropy gates, exact width/certificate, zero-shift
histograms and nonzero-shift purity front half. It is not the full ABW mixed
partition algorithm. When exact direct output succeeds, values/counts are
written into the input. When any mixed bucket is found, the original input is
left unchanged and the backend sorts the whole array. Native/skip decisions
retain their existing behavior. No n-element temporary, scatter or bucket sort;
the original count/start/representative allocations in the copied front half
are retained. After the first mixed value only further purity comparisons stop;
the original count pass continues to the end before fallback. The measured
loss therefore describes this frozen front-half implementation, not a lower
bound for every possible skip-only wrapper. This experiment does not test workspace reuse. No threshold or
backend-specific dispatch tuning. Published ABW and GitHub remain unchanged.

## Implementations and conditions

Boost1.90.0 pdqsort and integer Spreadsort; x86-simd-sort v7.0 commit
8a7208187d99f0cb67e38c57ab1c7c85aad07aca, static uint32 qsort API. Exact official
source archive/hash is recorded. LLVM-MinGW Clang23.1.5/libc++ UCRT, C++17
-O3 -DNDEBUG -mavx2 -mno-avx512f -mbmi -static -pthread for all methods.
Preprocessor checks AVX2 enabled, AVX-512/OpenMP disabled. All sorters are
single-threaded. One local Windows11/i5-14500 machine, first allowed CPU
affinity. This is not Linux/cross-CPU validation.

All unchanged66 inputs: original real9, new real9, synthetic42, CTU IPv4 six.
No selection by results, shuffle of values, duplication or data transformation.
Source/input/expected hashes checked before and after. Source datasets,
extraction meaning, eligibility and hashes remain in the protocol's case list.
The old competition_methods fields in those reused input records are historical;
only the six methods described here execute. All six handle the same uint32
ascending key-only problem; no restricted-input omissions.

Same binary, fixed shared input scratch, common noinline function-pointer
timer. Restore outside timing; full frontend and backend API cost inside.
Full-array expected equality after every call. Per-input route/direct/fallback
audit outside timing verifies mixed inspection leaves the input unchanged.
One untimed warmup per method. Four ABBA/BAAB blocks per backend pair, pair order
shuffled. Three fresh-process sessions/input with randomized input order:198
processes and9,504 timed sorts. Warm repeated-call condition only; no cache
disturbance or workspace. No outlier removal, A/A control or significance claim.

Time ratio is wrapped/backend: same-pair call medians/process, median3 process
ratios/input, equal-input geometric mean by group. Displayed times are median3
process medians; their quotient can differ from the primary ratio. Below1 is
faster. Descriptive +/-5% counts are not statistical confidence intervals.
All66 equally weights42 synthetics and is not a production workload mixture.
Existing real18 and correlated IPv4 columns are reported separately. No measured
maximum is a bound for unseen inputs. Allocator/cache/assembly causes are not
inferred from timing alone. Additional peak heap/RSS is not measured here.

## Geometric mean wrapped/backend time ratios

| Input group | N | pdqsort wrapper | Spreadsort wrapper | AVX2 wrapper |
|---|---:|---:|---:|---:|
| development_real | 9 | 0.57265 | 0.41526 | 0.99317 |
| new_real | 9 | 0.71556 | 0.46423 | 1.32063 |
| real18 | 18 | 0.64013 | 0.43906 | 1.14525 |
| synthetic | 42 | 0.73209 | 0.65923 | 1.02131 |
| ctu_ipv4 | 6 | 1.19519 | 1.08699 | 1.69976 |
| all66 | 66 | 0.73794 | 0.61750 | 1.10366 |
| direct | 18 | 0.28994 | 0.15518 | 0.95065 |
| mixed_fallback | 27 | 1.07981 | 1.05599 | 1.30091 |
| native_skip | 21 | 1.00742 | 1.01196 | 1.01526 |

## Input-level win/loss counts

| Wrapper | Faster | Slower | >5% faster | >5% slower |
|---|---:|---:|---:|---:|
| ABW_skip_pdq | 24 | 42 | 16 | 11 |
| ABW_skip_spread | 19 | 47 | 17 | 10 |
| ABW_skip_avx2 | 15 | 51 | 9 | 40 |

## All input results

| Input | Path | pdq ms | +wrapper ms | Ratio | Spread ms | +wrapper ms | Ratio | AVX2 ms | +wrapper ms | Ratio |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| nasa_status | native/skip | 2.42940 | 2.42305 | 0.96637 | 5.37495 | 5.46880 | 1.00351 | 0.74830 | 0.74005 | 0.94872 |
| nasa_timestamp | native/skip | 1.50245 | 1.51915 | 0.96847 | 0.62235 | 0.64825 | 1.04162 | 6.89935 | 6.88400 | 0.99638 |
| nasa_bytes | native/skip | 12.43605 | 12.46840 | 1.03469 | 17.13235 | 17.69685 | 1.02908 | 3.35570 | 3.62350 | 1.08965 |
| covtype_cover | direct | 0.89440 | 0.35680 | 0.41937 | 1.76170 | 0.35305 | 0.20146 | 0.28170 | 0.35025 | 1.24334 |
| covtype_elevation | direct | 4.33115 | 0.41350 | 0.09547 | 3.93355 | 0.39385 | 0.10047 | 1.31495 | 0.37805 | 0.28841 |
| covtype_road_distance | mixed fallback | 5.02025 | 5.45330 | 1.08626 | 5.15470 | 5.33070 | 1.03532 | 1.62720 | 2.06450 | 1.22213 |
| census_caseid | native/skip | 1.29330 | 1.28050 | 0.98827 | 0.78720 | 0.82090 | 1.02134 | 9.70130 | 9.63515 | 0.99318 |
| census_industry | direct | 7.35795 | 1.50520 | 0.20457 | 12.28290 | 1.51215 | 0.12126 | 1.73755 | 1.53590 | 0.87287 |
| census_sex | direct | 2.07975 | 1.61765 | 0.77781 | 12.58445 | 1.65520 | 0.13153 | 0.66805 | 1.60500 | 2.40251 |
| synthetic_100000_uniform | mixed fallback | 1.65515 | 1.70385 | 1.02942 | 1.56390 | 1.54575 | 1.02900 | 0.26940 | 0.31535 | 1.17516 |
| synthetic_100000_normal | mixed fallback | 1.68160 | 1.68725 | 1.01588 | 1.47340 | 1.55655 | 1.04291 | 0.26800 | 0.31310 | 1.16828 |
| synthetic_100000_exponential | mixed fallback | 1.61990 | 1.66105 | 1.02352 | 1.49685 | 1.54455 | 1.03739 | 0.26905 | 0.32720 | 1.17580 |
| synthetic_100000_narrow | mixed fallback | 1.67215 | 1.78100 | 1.02686 | 1.59585 | 1.60695 | 1.00821 | 0.28930 | 0.34620 | 1.19548 |
| synthetic_100000_duplicates | direct | 0.16295 | 0.06945 | 0.42620 | 0.39805 | 0.06925 | 0.17339 | 0.04455 | 0.06930 | 1.49832 |
| synthetic_100000_duplicates_narrow | direct | 0.16475 | 0.04180 | 0.25584 | 0.38110 | 0.04195 | 0.11008 | 0.04655 | 0.04180 | 0.90333 |
| synthetic_100000_sorted | native/skip | 0.03300 | 0.03390 | 1.04905 | 0.02585 | 0.02645 | 1.02846 | 0.23665 | 0.23975 | 1.00487 |
| synthetic_100000_partial | native/skip | 0.42555 | 0.43605 | 1.01728 | 0.35670 | 0.35940 | 1.02054 | 0.27290 | 0.27895 | 0.99359 |
| synthetic_100000_reverse | native/skip | 0.07995 | 0.08020 | 1.01022 | 0.57655 | 0.57980 | 1.02168 | 0.23685 | 0.23640 | 0.99564 |
| synthetic_100000_upper_only | direct | 1.00055 | 0.12530 | 0.12523 | 0.80675 | 0.12340 | 0.15296 | 0.27785 | 0.12210 | 0.45365 |
| synthetic_100000_lower_only | mixed fallback | 1.74410 | 1.76655 | 1.03243 | 1.53925 | 1.60565 | 1.04314 | 0.27620 | 0.33525 | 1.20329 |
| synthetic_100000_discontinuous | native/skip | 1.01505 | 1.01435 | 1.01286 | 0.78280 | 0.79070 | 1.01009 | 0.26750 | 0.28215 | 1.04673 |
| synthetic_100000_extreme_skew | native/skip | 0.03470 | 0.03550 | 1.02305 | 0.45260 | 0.45685 | 1.00199 | 0.06040 | 0.06105 | 1.01076 |
| synthetic_100000_mixture | mixed fallback | 1.46140 | 1.54690 | 1.04523 | 1.38255 | 1.43945 | 1.03816 | 0.24780 | 0.32710 | 1.34019 |
| synthetic_1000000_uniform | mixed fallback | 20.63925 | 20.33685 | 1.00094 | 20.49460 | 21.73645 | 1.01651 | 3.51455 | 4.05005 | 1.17100 |
| synthetic_1000000_normal | mixed fallback | 19.79550 | 20.76530 | 1.04899 | 21.48870 | 21.68175 | 1.03117 | 3.74040 | 4.17710 | 1.13507 |
| synthetic_1000000_exponential | mixed fallback | 18.73495 | 18.95150 | 1.02302 | 19.89360 | 20.51745 | 1.03819 | 3.45165 | 4.06290 | 1.17890 |
| synthetic_1000000_narrow | mixed fallback | 14.30745 | 15.28660 | 1.11498 | 15.48250 | 15.97310 | 1.03169 | 3.45380 | 4.29530 | 1.21492 |
| synthetic_1000000_duplicates | direct | 2.53300 | 0.82260 | 0.33093 | 4.30695 | 0.80605 | 0.18715 | 0.63880 | 0.81695 | 1.27888 |
| synthetic_1000000_duplicates_narrow | direct | 2.46420 | 0.57975 | 0.23527 | 3.85320 | 0.58425 | 0.15300 | 0.63085 | 0.57695 | 0.87970 |
| synthetic_1000000_sorted | native/skip | 0.47075 | 0.45265 | 1.00239 | 0.31740 | 0.32305 | 1.00605 | 3.34725 | 3.36940 | 1.00662 |
| synthetic_1000000_partial | native/skip | 5.61870 | 5.72635 | 1.01566 | 9.00915 | 9.15415 | 0.98112 | 3.66405 | 3.68720 | 1.01662 |
| synthetic_1000000_reverse | native/skip | 0.80975 | 0.81425 | 1.00705 | 6.81430 | 7.14545 | 1.00074 | 3.43085 | 3.31450 | 0.95437 |
| synthetic_1000000_upper_only | direct | 8.13510 | 0.84620 | 0.10876 | 9.11375 | 0.86265 | 0.09465 | 2.77555 | 0.81975 | 0.29535 |
| synthetic_1000000_lower_only | mixed fallback | 19.66055 | 20.13130 | 1.02394 | 20.72735 | 21.71980 | 1.05061 | 3.65605 | 4.18070 | 1.12939 |
| synthetic_1000000_discontinuous | native/skip | 8.20140 | 8.25250 | 0.99364 | 7.94285 | 7.96790 | 1.00315 | 2.73340 | 2.93475 | 1.09390 |
| synthetic_1000000_extreme_skew | native/skip | 0.47620 | 0.46435 | 0.98286 | 7.07235 | 7.15130 | 1.00932 | 1.41230 | 1.39950 | 1.00453 |
| synthetic_1000000_mixture | mixed fallback | 16.82055 | 17.14345 | 1.04090 | 19.21750 | 19.60245 | 1.02003 | 3.01680 | 3.70305 | 1.22688 |
| synthetic_10000000_uniform | mixed fallback | 210.93715 | 215.29385 | 1.02065 | 236.34465 | 242.16115 | 1.02050 | 43.33970 | 47.77750 | 1.10128 |
| synthetic_10000000_normal | mixed fallback | 212.90170 | 218.93950 | 1.02132 | 224.75915 | 235.53570 | 1.04162 | 45.06305 | 48.61825 | 1.09299 |
| synthetic_10000000_exponential | mixed fallback | 217.16455 | 231.18180 | 1.03762 | 239.18100 | 241.11930 | 1.01724 | 44.68000 | 48.64560 | 1.08876 |
| synthetic_10000000_narrow | mixed fallback | 104.57945 | 106.61485 | 1.04549 | 174.75845 | 179.39200 | 1.02651 | 42.13195 | 47.23185 | 1.12819 |
| synthetic_10000000_duplicates | direct | 31.30550 | 8.85905 | 0.28376 | 43.68880 | 8.54270 | 0.19130 | 7.02035 | 8.81820 | 1.25625 |
| synthetic_10000000_duplicates_narrow | direct | 32.70385 | 8.47030 | 0.25900 | 42.10105 | 8.48945 | 0.19983 | 7.56405 | 8.35575 | 1.18133 |
| synthetic_10000000_sorted | native/skip | 5.32595 | 5.13770 | 0.97643 | 3.22525 | 3.28035 | 1.01708 | 44.25890 | 44.31155 | 0.99657 |
| synthetic_10000000_partial | native/skip | 69.39165 | 69.30930 | 1.01566 | 98.27190 | 95.12820 | 1.00223 | 47.83945 | 47.14725 | 1.00212 |
| synthetic_10000000_reverse | native/skip | 10.53905 | 10.63275 | 1.00889 | 91.41110 | 92.96670 | 1.00860 | 43.90365 | 45.07885 | 1.02677 |
| synthetic_10000000_upper_only | direct | 76.94780 | 8.68120 | 0.11410 | 200.85815 | 8.73225 | 0.04416 | 23.62100 | 8.84605 | 0.38315 |
| synthetic_10000000_lower_only | mixed fallback | 183.35415 | 193.83400 | 1.04016 | 181.05000 | 184.51510 | 1.01914 | 46.83565 | 52.59425 | 1.13537 |
| synthetic_10000000_discontinuous | native/skip | 75.18635 | 80.42355 | 1.04314 | 92.48315 | 93.98560 | 1.02149 | 23.57385 | 25.08560 | 1.07258 |
| synthetic_10000000_extreme_skew | native/skip | 5.79565 | 6.02905 | 0.99833 | 77.90085 | 78.13315 | 0.98453 | 19.21445 | 20.51215 | 1.03096 |
| synthetic_10000000_mixture | mixed fallback | 177.64905 | 180.95545 | 1.01861 | 177.44800 | 178.80760 | 1.01139 | 37.51715 | 44.25125 | 1.18505 |
| skin_red | direct | 1.27870 | 0.11190 | 0.08532 | 0.98860 | 0.10900 | 0.10950 | 0.39235 | 0.11115 | 0.27692 |
| skin_label | direct | 0.09470 | 0.12795 | 1.35111 | 0.06030 | 0.12795 | 2.12189 | 0.05170 | 0.12925 | 2.47582 |
| chest_sequence | native/skip | 0.05430 | 0.05645 | 1.03959 | 0.04260 | 0.04340 | 1.01878 | 0.42795 | 0.41575 | 1.00233 |
| chest_x | mixed fallback | 0.88350 | 1.05460 | 1.17298 | 0.62865 | 0.80995 | 1.27432 | 0.23460 | 0.40920 | 1.74425 |
| chest_activity | direct | 0.08790 | 0.08390 | 0.95449 | 0.71690 | 0.08470 | 0.11691 | 0.04565 | 0.08400 | 1.83220 |
| taxi_vendor | direct | 2.49015 | 2.07550 | 0.84975 | 15.33430 | 2.10055 | 0.13950 | 0.89885 | 2.14920 | 2.35413 |
| taxi_payment | direct | 6.15965 | 2.23560 | 0.36294 | 13.96675 | 2.32700 | 0.16661 | 1.46100 | 2.23165 | 1.55890 |
| taxi_pickup_zone | mixed fallback | 12.85470 | 14.61990 | 1.18003 | 11.33580 | 13.12475 | 1.19547 | 4.01985 | 5.73135 | 1.44744 |
| taxi_pickup_time | native/skip | 58.87225 | 59.29860 | 1.00724 | 73.18115 | 77.96175 | 1.02191 | 13.38170 | 13.90595 | 1.04739 |
| ctu1_srcaddr_ipv4 | mixed fallback | 31.02335 | 33.41630 | 1.07830 | 44.63580 | 45.36280 | 1.05548 | 7.03540 | 10.09965 | 1.40819 |
| ctu1_dstaddr_ipv4 | mixed fallback | 13.31665 | 17.04395 | 1.30079 | 38.90140 | 43.08160 | 1.10438 | 3.94695 | 7.80815 | 1.96291 |
| ctu2_srcaddr_ipv4 | mixed fallback | 19.25600 | 20.69960 | 1.07922 | 27.86860 | 29.52190 | 1.06141 | 4.39910 | 6.18150 | 1.40517 |
| ctu2_dstaddr_ipv4 | mixed fallback | 8.39450 | 10.52370 | 1.29104 | 23.87470 | 26.07085 | 1.09199 | 2.12860 | 4.49345 | 2.14351 |
| ctu3_srcaddr_ipv4 | mixed fallback | 36.45190 | 41.99895 | 1.15217 | 64.30870 | 71.10340 | 1.10566 | 9.39970 | 14.90590 | 1.55570 |
| ctu3_dstaddr_ipv4 | mixed fallback | 22.70085 | 28.75945 | 1.29449 | 66.06620 | 72.89195 | 1.10426 | 6.95770 | 13.07285 | 1.86200 |

## Largest regressions, with all process ratios

| Wrapper | Input | Ratio | Backend ms | Wrapped ms | Three process ratios |
|---|---|---:|---:|---:|---|
| ABW_skip_pdq | skin_label | 1.35111 | 0.09470 | 0.12795 | 1.3511, 1.3692, 1.3511 |
| ABW_skip_pdq | ctu1_dstaddr_ipv4 | 1.30079 | 13.31665 | 17.04395 | 1.3344, 1.2413, 1.3008 |
| ABW_skip_pdq | ctu3_dstaddr_ipv4 | 1.29449 | 22.70085 | 28.75945 | 1.2945, 1.3127, 1.2669 |
| ABW_skip_pdq | ctu2_dstaddr_ipv4 | 1.29104 | 8.39450 | 10.52370 | 1.2447, 1.2910, 1.2954 |
| ABW_skip_pdq | taxi_pickup_zone | 1.18003 | 12.85470 | 14.61990 | 1.1800, 1.1373, 1.1813 |
| ABW_skip_spread | skin_label | 2.12189 | 0.06030 | 0.12795 | 2.1219, 2.1442, 2.1219 |
| ABW_skip_spread | chest_x | 1.27432 | 0.62865 | 0.80995 | 1.2743, 1.2920, 1.2021 |
| ABW_skip_spread | taxi_pickup_zone | 1.19547 | 11.33580 | 13.12475 | 1.1457, 1.1955, 1.2626 |
| ABW_skip_spread | ctu3_srcaddr_ipv4 | 1.10566 | 64.30870 | 71.10340 | 1.0930, 1.1097, 1.1057 |
| ABW_skip_spread | ctu1_dstaddr_ipv4 | 1.10438 | 38.90140 | 43.08160 | 1.1008, 1.1044, 1.1075 |
| ABW_skip_avx2 | skin_label | 2.47582 | 0.05170 | 0.12925 | 2.4758, 2.4024, 2.5195 |
| ABW_skip_avx2 | census_sex | 2.40251 | 0.66805 | 1.60500 | 2.3634, 2.4774, 2.4025 |
| ABW_skip_avx2 | taxi_vendor | 2.35413 | 0.89885 | 2.14920 | 2.5251, 2.2204, 2.3541 |
| ABW_skip_avx2 | ctu2_dstaddr_ipv4 | 2.14351 | 2.12860 | 4.49345 | 2.1452, 2.0370, 2.1435 |
| ABW_skip_avx2 | ctu1_dstaddr_ipv4 | 1.96291 | 3.94695 | 7.80815 | 1.9629, 2.0251, 1.8571 |

## Actual real18 gains above the descriptive5% band

### ABW_skip_pdq

- covtype_cover: ratio 0.41937, backend 0.89440ms, wrapped 0.35680ms.
- covtype_elevation: ratio 0.09547, backend 4.33115ms, wrapped 0.41350ms.
- census_industry: ratio 0.20457, backend 7.35795ms, wrapped 1.50520ms.
- census_sex: ratio 0.77781, backend 2.07975ms, wrapped 1.61765ms.
- skin_red: ratio 0.08532, backend 1.27870ms, wrapped 0.11190ms.
- taxi_vendor: ratio 0.84975, backend 2.49015ms, wrapped 2.07550ms.
- taxi_payment: ratio 0.36294, backend 6.15965ms, wrapped 2.23560ms.
### ABW_skip_spread

- covtype_cover: ratio 0.20146, backend 1.76170ms, wrapped 0.35305ms.
- covtype_elevation: ratio 0.10047, backend 3.93355ms, wrapped 0.39385ms.
- census_industry: ratio 0.12126, backend 12.28290ms, wrapped 1.51215ms.
- census_sex: ratio 0.13153, backend 12.58445ms, wrapped 1.65520ms.
- skin_red: ratio 0.10950, backend 0.98860ms, wrapped 0.10900ms.
- chest_activity: ratio 0.11691, backend 0.71690ms, wrapped 0.08470ms.
- taxi_vendor: ratio 0.13950, backend 15.33430ms, wrapped 2.10055ms.
- taxi_payment: ratio 0.16661, backend 13.96675ms, wrapped 2.32700ms.
### ABW_skip_avx2

- nasa_status: ratio 0.94872, backend 0.74830ms, wrapped 0.74005ms.
- covtype_elevation: ratio 0.28841, backend 1.31495ms, wrapped 0.37805ms.
- census_industry: ratio 0.87287, backend 1.73755ms, wrapped 1.53590ms.
- skin_red: ratio 0.27692, backend 0.39235ms, wrapped 0.11115ms.

## Reproduction

The saved plan was frozen while prepared; its run_status is a historical
pre-run label. completion.json records actual completion. From the original
workspace root run `python work/abw_backends/run_backend_skip_comparison.py`
after restoring all hashed inputs and compiling with build_command/comparison_build
options; output directory must not already exist. It refuses overwriting results.
The evidence archive includes sources, pinned x86 source ZIP, hashes, all raw
sessions and the report generator. Dataset binaries, Boost and compiler are not
bundled. The executable uses Windows affinity APIs. Source/executable hashes
identify the measured build; no cross-platform performance promise.
