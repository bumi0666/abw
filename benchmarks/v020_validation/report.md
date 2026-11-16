# Integrated v0.2.0 candidate: final66 validation

2026-11-16. Candidate based on released v0.1.3 commit
3797feafb7bd3e2b230e815fd1aa69df16cf0141. Adds caller-owned workspace and exact
sampled bit31 width certificate. Both overloads share one templated partition
body with owning/borrowing temporary factories. No dispatch seed/threshold,
histogram, purity, scatter, pdqsort or copy-policy change. This comparison uses
the actual integrated public candidate header; earlier prototype times are not
pooled. No main merge, tag or release is performed by this validation.

## Inputs and correctness

All original real9, new real9 and synthetic42 remain unchanged. Six CTU-13
IPv4 columns are added without selection by performance. Existing60 and IPv4
results are reported separately; the all66 average weights each input equally
and is not a production workload mixture. CTU columns are correlated from one
dataset, not six independent datasets. Address conversion is lossless network
numeric IPv4 to uint32; original retained row order preserved, no replication,
hashing or shuffle. IPv6/non-IPv4 entries were omitted as documented. See
reference/input manifests for extraction, source URLs, licenses and SHA256.

Every timed output is compared element-by-element with the hashed expected
array. Both ABW overloads agree with v0.1.3 on strategies, active intervals and
direct/mixed results. Source/executable/input/expected hashes are checked before
and after timing. Conformance, frequency collisions/bounds, exact certificates,
grow/shrink, move/release, injected bad_alloc and separate-workspace concurrency
are tested separately in C++17. Local all66 tests and GCC/Clang/MSVC CI results
are retained in validation files and CI logs. CI uses generated correctness
inputs; the external datasets are checked locally.

## Conditions and memory lifetimes

Intel i5-14500, Windows11 x64, 32GiB installed-class physical memory; exact
reported bytes in environment.json. LLVM-MinGW Clang23.1.5/libc++ UCRT,
Boost1.90.0, CAFS commit13488afef446ae88e7ad631ec6f862def896094a. All sorters
use C++20 -O3 -DNDEBUG -mavx2 -mbmi -static -pthread because CAFS needs C++20.
Public API and correctness matrix use C++17. First allowed CPU affinity.

One binary/shared fixed u32 scratch/common noinline pointer-call timer. i32
uses separate fixed signed scratch containing the exact same supported values;
conversion is outside timing. Input restore -> optional64MiB sequential read
into observable sink -> timer -> complete sorting API -> stop -> full equality.
Load/restore/disturbance/audits are excluded, sorting analysis/alloc/init/count/
scatter/internal sort/copy/free are included. Both conditions allocate the same
64MiB harness array. Disturbance does not guarantee an empty cache, a realistic
production stream or a lower/upper performance bound.

W_reuse prewarms and retains caller-owned mixed-path storage across calls;
release is separately logged. W_fresh constructs/destroys workspace inside each
call. v020_default and v013 keep per-call local temporary lifetimes. Competitors
use their original per-call APIs; competitor workspace reuse is not measured.
Each process repeats the same input; changing-content/size streams are unmeasured.
Repeated warm full-function medians are not first-application cold latency.
Retained workspace coexists with comparator allocations, so a near CAFS average
must always be accompanied by the default/fresh comparison. Direct/native/skip
do not use workspace; an earlier larger buffer can remain in a variable-size
application, outside these per-input timing processes.

Memory records are capacity payload (4*capacity), not total requested heap,
allocator overhead or peak RSS. Other per-sort allocations remain. No new
all-sorter peak-memory claim is made. Native/direct processes start empty and
retain0; mixed processes retain4n here. Growth policy and release/concurrency
contract are documented in docs/workspace.md.

CAFS i32 supports only24 exactly representable inputs. Frozen Counting uses
runtime min/max plus dense initialized uint64 counters capped at64MiB:26
eligible inputs. All IPv4 columns are unsupported by these two methods.
Unsupported means omitted, not a loss. Counting is this specific implementation,
not every optimized histogram sorter.

## Repetition and estimators

Three fresh processes/input/condition:396 processes. Four alternating ABBA/BAAB
blocks of W_reuse versus each eligible comparator; shuffled pair order and
randomized input/condition order. No outlier filtering, input removal or tuning.
Primary reuse/comparator ratio uses same-pair call medians/process, median3
process ratios/input, equal-input geometric mean. Block-paired ratios average
two calls/method/block and are retained alongside. Displayed method times pool
that method's calls/process, then median3 processes. pdq speedups and default/
v013 comparisons use same-process pooled medians; their ratios can differ from
primary pair estimates. No estimator is chosen by which favors ABW. Lowest
displayed median is nominal winner, not a statistical significance finding.

The separate Taxi payment A/A diagnostic uses the exact same baseline function
pointer under two names, three processes per condition and the same pair design.
It runs after the full comparison in a separate binary, does not establish the
cause of a regression and is not used to discard measurements.

## Results

**All following66-input performance results are Windows11/i5-14500 results.
Linux performance has not been replicated over these66 inputs.**

All 42,816 timed sorts in396 processes passed.

### warm: geometric mean speedup over pdq

| Method | Original9 | New9 | Synthetic42 | IPv4_6 | Existing60 | All66 |
|---|---:|---:|---:|---:|---:|---:|
| W_reuse | 1.89082 (9) | 1.56049 (9) | 1.54442 (42) | 0.86953 (6) | 1.59449 (60) | 1.50897 (66) |
| W_fresh | 1.84484 (9) | 1.47634 (9) | 1.50704 (42) | 0.78949 (6) | 1.54867 (60) | 1.45666 (66) |
| v020_default | 1.85037 (9) | 1.51465 (9) | 1.49873 (42) | 0.78308 (6) | 1.54932 (60) | 1.45614 (66) |
| v013 | 1.86695 (9) | 1.49447 (9) | 1.47520 (42) | 0.76736 (6) | 1.53122 (60) | 1.43801 (66) |
| pdq | 1.00000 (9) | 1.00000 (9) | 1.00000 (42) | 1.00000 (6) | 1.00000 (60) | 1.00000 (66) |
| spread | 0.72872 (9) | 0.68033 (9) | 0.64324 (42) | 0.47907 (6) | 0.66093 (60) | 0.64187 (66) |
| CAFS_u32 | 1.79876 (9) | 1.60928 (9) | 1.53571 (42) | 0.66153 (6) | 1.58365 (60) | 1.46283 (66) |
| CAFS_i32 | 1.86254 (9) | 1.70164 (9) | 2.31565 (6) | unsupported (0) | 1.90122 (24) | 1.90122 (24) |
| counting | 0.70820 (9) | 0.60453 (8) | 1.52239 (9) | unsupported (0) | 0.87914 (26) | 0.87914 (26) |

### warm: time ratios (lower is faster)

| Group | Default/v013 | Reuse/default | Reuse/v013 | Reuse/CAFS u32 | Default/CAFS u32 |
|---|---:|---:|---:|---:|---:|
| development_real | 1.01320 | 0.98067 | 0.97594 | 0.96640 | 0.97198 |
| new_real | 0.99273 | 0.96478 | 0.95445 | 1.02437 | 1.06882 |
| synthetic | 0.98587 | 0.97182 | 0.95333 | 0.99799 | 1.02293 |
| ctu_ipv4 | 0.98173 | 0.90077 | 0.87872 | 0.77126 | 0.84767 |
| all60 | 0.99095 | 0.97208 | 0.95686 | 0.99708 | 1.02183 |
| all66 | 0.99011 | 0.96537 | 0.94947 | 0.97407 | 1.00462 |

### warm: all input medians (ms)

| Input | Path | Reuse | Fresh ws | Default | v013 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| nasa_status | native/skip | 2.42395 | 2.41640 | 2.43210 | 2.35820 | 2.41885 | 5.72540 | 3.77725 | 3.68950 | 4.09380 | v013 |
| nasa_timestamp | native/skip | 1.35570 | 1.38355 | 1.35755 | 1.37020 | 1.33320 | 0.59530 | 0.44405 | 0.43650 | 16.42550 | CAFS_i32 |
| nasa_bytes | native/skip | 12.39165 | 12.54135 | 12.23975 | 12.60025 | 12.13690 | 17.10280 | 10.99410 | 7.36585 | 19.72520 | CAFS_i32 |
| covtype_cover | direct | 0.35240 | 0.34555 | 0.35405 | 0.35225 | 0.82685 | 1.77390 | 0.40170 | 0.40335 | 0.94950 | W_fresh |
| covtype_elevation | direct | 0.36085 | 0.35925 | 0.36255 | 0.36895 | 4.21720 | 3.67165 | 1.12510 | 1.17575 | 0.92370 | W_fresh |
| covtype_road_distance | mixed | 2.45695 | 2.87500 | 2.92685 | 2.97925 | 5.15695 | 5.14995 | 1.15310 | 1.16235 | 0.66515 | counting |
| census_caseid | native/skip | 1.23660 | 1.27575 | 1.27430 | 1.20220 | 1.21530 | 0.80130 | 0.55485 | 0.57795 | 8.85360 | CAFS_u32 |
| census_industry | direct | 1.65325 | 1.72235 | 1.59705 | 1.58620 | 7.40645 | 12.63820 | 7.01100 | 6.84225 | 6.29185 | v013 |
| census_sex | direct | 1.63490 | 1.66555 | 1.64610 | 1.61225 | 2.13795 | 12.66340 | 1.78670 | 1.89575 | 7.30785 | v013 |
| synthetic_100000_uniform | mixed | 1.22810 | 1.29190 | 1.30455 | 1.31460 | 1.71495 | 1.60625 | 1.78000 | unsupported | unsupported | W_reuse |
| synthetic_100000_normal | mixed | 1.28125 | 1.37495 | 1.38490 | 1.38290 | 1.74480 | 1.58015 | 1.69625 | unsupported | unsupported | W_reuse |
| synthetic_100000_exponential | mixed | 1.31680 | 1.40730 | 1.40130 | 1.39170 | 1.71765 | 1.57025 | 1.74005 | unsupported | unsupported | W_reuse |
| synthetic_100000_narrow | mixed | 1.14845 | 1.25475 | 1.13415 | 1.15845 | 1.65470 | 1.54250 | 1.70235 | 1.77470 | 0.66225 | counting |
| synthetic_100000_duplicates | direct | 0.06880 | 0.06915 | 0.07150 | 0.07685 | 0.16960 | 0.39745 | 0.06500 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_duplicates_narrow | direct | 0.04325 | 0.04360 | 0.04380 | 0.04365 | 0.16775 | 0.37350 | 0.06505 | 0.06555 | 0.27560 | W_reuse |
| synthetic_100000_sorted | native/skip | 0.03350 | 0.03345 | 0.03345 | 0.03375 | 0.03290 | 0.02560 | 0.01310 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_partial | native/skip | 0.41965 | 0.41945 | 0.41830 | 0.41935 | 0.41530 | 0.36465 | 0.43375 | unsupported | unsupported | spread |
| synthetic_100000_reverse | native/skip | 0.08320 | 0.08255 | 0.08330 | 0.08280 | 0.08240 | 0.60305 | 0.01880 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_upper_only | direct | 0.13020 | 0.12425 | 0.13350 | 0.13505 | 1.00195 | 0.83510 | 0.31685 | unsupported | unsupported | W_fresh |
| synthetic_100000_lower_only | mixed | 1.24695 | 1.31590 | 1.31160 | 1.35050 | 1.79105 | 1.61185 | 1.77240 | unsupported | 3.25100 | W_reuse |
| synthetic_100000_discontinuous | native/skip | 1.04050 | 1.03270 | 1.02270 | 1.02915 | 1.04605 | 0.79540 | 0.44065 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_extreme_skew | native/skip | 0.03805 | 0.03810 | 0.03815 | 0.03800 | 0.03735 | 0.49125 | 0.09495 | unsupported | unsupported | pdq |
| synthetic_100000_mixture | mixed | 1.17985 | 1.27410 | 1.28805 | 1.29795 | 1.48990 | 1.41145 | 1.48580 | unsupported | unsupported | W_reuse |
| synthetic_1000000_uniform | mixed | 15.07380 | 15.87785 | 16.06055 | 16.18620 | 18.91885 | 19.91360 | 18.83920 | unsupported | unsupported | W_reuse |
| synthetic_1000000_normal | mixed | 16.41110 | 17.55250 | 17.01475 | 17.54460 | 19.68755 | 21.62350 | 19.41265 | unsupported | unsupported | W_reuse |
| synthetic_1000000_exponential | mixed | 15.80780 | 16.29755 | 17.17835 | 16.60700 | 19.11380 | 20.00540 | 19.26615 | unsupported | unsupported | W_reuse |
| synthetic_1000000_narrow | mixed | 10.13505 | 11.01170 | 10.94555 | 10.91245 | 13.50130 | 14.59625 | 11.69405 | 8.23400 | 3.48470 | counting |
| synthetic_1000000_duplicates | direct | 0.77510 | 0.77095 | 0.82435 | 0.93065 | 2.52295 | 4.27830 | 0.70910 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_duplicates_narrow | direct | 0.56860 | 0.56555 | 0.57455 | 0.56805 | 2.42990 | 3.82270 | 0.67045 | 0.72100 | 2.89155 | W_fresh |
| synthetic_1000000_sorted | native/skip | 0.44970 | 0.44900 | 0.44010 | 0.44350 | 0.43495 | 0.31545 | 0.22385 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_partial | native/skip | 5.49905 | 5.52950 | 5.60080 | 5.49280 | 5.51420 | 8.91680 | 5.46720 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_reverse | native/skip | 0.81345 | 0.80130 | 0.80565 | 0.80170 | 0.80965 | 6.83980 | 0.38140 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_upper_only | direct | 0.84865 | 0.84705 | 0.87830 | 1.00715 | 7.92915 | 9.13470 | 1.58750 | unsupported | unsupported | W_fresh |
| synthetic_1000000_lower_only | mixed | 15.99385 | 16.83845 | 17.38850 | 17.65960 | 19.98875 | 20.99090 | 19.73900 | unsupported | 12.04510 | counting |
| synthetic_1000000_discontinuous | native/skip | 8.14635 | 8.27455 | 7.93470 | 8.33040 | 7.94680 | 7.85025 | 1.96580 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_extreme_skew | native/skip | 0.44130 | 0.43565 | 0.44590 | 0.45115 | 0.43245 | 7.34135 | 0.97025 | unsupported | unsupported | pdq |
| synthetic_1000000_mixture | mixed | 14.67235 | 15.31100 | 15.26790 | 15.41010 | 16.40995 | 18.87300 | 24.47235 | unsupported | unsupported | W_reuse |
| synthetic_10000000_uniform | mixed | 181.65465 | 186.80280 | 195.33560 | 188.26280 | 212.49340 | 234.88155 | 219.06870 | unsupported | unsupported | W_reuse |
| synthetic_10000000_normal | mixed | 181.76735 | 186.82520 | 191.96995 | 190.13560 | 221.63940 | 228.97430 | 213.14675 | unsupported | unsupported | W_reuse |
| synthetic_10000000_exponential | mixed | 180.55850 | 190.08840 | 189.39540 | 194.65980 | 211.74025 | 229.39265 | 212.36030 | unsupported | unsupported | W_reuse |
| synthetic_10000000_narrow | mixed | 67.75435 | 75.52565 | 75.90035 | 75.63965 | 103.76510 | 179.16535 | 72.77595 | 34.38695 | 31.67200 | counting |
| synthetic_10000000_duplicates | direct | 8.57850 | 8.47775 | 8.90525 | 10.26025 | 29.83930 | 42.96255 | 8.28950 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_duplicates_narrow | direct | 7.79850 | 7.34535 | 7.40925 | 7.54235 | 30.73690 | 40.47795 | 8.16270 | 8.26835 | 31.69125 | W_fresh |
| synthetic_10000000_sorted | native/skip | 5.55450 | 5.50405 | 5.40040 | 5.38540 | 5.49800 | 3.25545 | 2.33840 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_partial | native/skip | 67.36640 | 69.96290 | 66.82080 | 68.99195 | 69.37690 | 94.25655 | 67.65055 | unsupported | unsupported | v020_default |
| synthetic_10000000_reverse | native/skip | 10.33095 | 10.39415 | 10.26525 | 9.92135 | 10.65015 | 91.49545 | 3.90510 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_upper_only | direct | 8.47470 | 8.46660 | 8.77430 | 10.60465 | 75.98665 | 184.62225 | 14.52470 | unsupported | unsupported | W_fresh |
| synthetic_10000000_lower_only | mixed | 142.53285 | 152.49055 | 152.17010 | 150.30190 | 181.72850 | 169.63645 | 178.44010 | unsupported | 62.32255 | counting |
| synthetic_10000000_discontinuous | native/skip | 78.57960 | 77.25765 | 79.97205 | 78.28790 | 75.45645 | 98.54360 | 17.78505 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_extreme_skew | native/skip | 5.64945 | 5.64315 | 5.39235 | 5.39345 | 5.62375 | 75.31110 | 11.67290 | unsupported | unsupported | v020_default |
| synthetic_10000000_mixture | mixed | 158.26015 | 164.86810 | 167.85535 | 165.49310 | 175.33170 | 174.28560 | 302.35825 | unsupported | unsupported | W_reuse |
| skin_red | direct | 0.11000 | 0.10980 | 0.10910 | 0.11165 | 1.29330 | 0.97185 | 0.60455 | 0.57865 | 0.44580 | v020_default |
| skin_label | direct | 0.13580 | 0.13585 | 0.13635 | 0.13600 | 0.09880 | 0.06280 | 0.03615 | 0.03630 | 0.52905 | CAFS_u32 |
| chest_sequence | native/skip | 0.05630 | 0.05620 | 0.05655 | 0.05630 | 0.05540 | 0.04345 | 0.02220 | 0.02290 | 0.50640 | CAFS_u32 |
| chest_x | mixed | 0.87770 | 1.01125 | 0.90735 | 0.96900 | 0.88110 | 0.61250 | 0.46145 | 0.32265 | 0.35175 | CAFS_i32 |
| chest_activity | direct | 0.08850 | 0.08830 | 0.08775 | 0.08880 | 0.08915 | 0.72915 | 0.15320 | 0.15490 | 0.33945 | v020_default |
| taxi_vendor | direct | 2.06455 | 2.11520 | 2.04925 | 2.06155 | 2.49330 | 14.60510 | 2.40895 | 2.40225 | 8.20340 | v020_default |
| taxi_payment | direct | 2.09990 | 2.10845 | 2.11380 | 2.12755 | 6.20070 | 13.83365 | 2.21405 | 2.39915 | 7.13245 | W_reuse |
| taxi_pickup_zone | mixed | 7.11165 | 9.27315 | 9.36415 | 9.24500 | 13.10880 | 11.85640 | 5.99615 | 5.94060 | 7.25895 | CAFS_i32 |
| taxi_pickup_time | native/skip | 60.94550 | 60.63865 | 61.71695 | 61.03325 | 60.67670 | 75.56995 | 85.96995 | 68.51990 | unsupported | W_fresh |
| ctu1_srcaddr_ipv4 | mixed | 28.71220 | 31.03050 | 30.83400 | 31.43120 | 30.32110 | 41.75465 | 49.75020 | unsupported | unsupported | W_reuse |
| ctu1_dstaddr_ipv4 | mixed | 17.61375 | 20.37505 | 19.89440 | 20.88170 | 13.23680 | 38.47370 | 23.56165 | unsupported | unsupported | pdq |
| ctu2_srcaddr_ipv4 | mixed | 18.33375 | 19.69840 | 20.30375 | 20.09355 | 18.75880 | 26.97105 | 27.28410 | unsupported | unsupported | W_reuse |
| ctu2_dstaddr_ipv4 | mixed | 11.28720 | 12.33465 | 13.66015 | 13.03430 | 8.53670 | 24.59740 | 14.76320 | unsupported | unsupported | pdq |
| ctu3_srcaddr_ipv4 | mixed | 37.93365 | 41.83565 | 41.40340 | 41.90020 | 35.61495 | 63.53205 | 48.92265 | unsupported | unsupported | pdq |
| ctu3_dstaddr_ipv4 | mixed | 29.90260 | 32.85170 | 33.66480 | 35.00785 | 22.80290 | 64.19705 | 26.21815 | unsupported | unsupported | pdq |
### perturbed: geometric mean speedup over pdq

| Method | Original9 | New9 | Synthetic42 | IPv4_6 | Existing60 | All66 |
|---|---:|---:|---:|---:|---:|---:|
| W_reuse | 1.87708 (9) | 1.52381 (9) | 1.51449 (42) | 0.87530 (6) | 1.56549 (60) | 1.48489 (66) |
| W_fresh | 1.85455 (9) | 1.45147 (9) | 1.46687 (42) | 0.78846 (6) | 1.51698 (60) | 1.42937 (66) |
| v020_default | 1.84684 (9) | 1.44156 (9) | 1.45822 (42) | 0.78769 (6) | 1.50822 (60) | 1.42174 (66) |
| v013 | 1.85312 (9) | 1.43140 (9) | 1.43380 (42) | 0.77483 (6) | 1.48968 (60) | 1.40373 (66) |
| pdq | 1.00000 (9) | 1.00000 (9) | 1.00000 (42) | 1.00000 (6) | 1.00000 (60) | 1.00000 (66) |
| spread | 0.73638 (9) | 0.68863 (9) | 0.64866 (42) | 0.47543 (6) | 0.66707 (60) | 0.64685 (66) |
| CAFS_u32 | 1.78819 (9) | 1.54704 (9) | 1.50861 (42) | 0.66803 (6) | 1.55342 (60) | 1.43871 (66) |
| CAFS_i32 | 1.85579 (9) | 1.56883 (9) | 2.26519 (6) | unsupported (0) | 1.83154 (24) | 1.83154 (24) |
| counting | 0.70580 (9) | 0.62172 (8) | 1.47646 (9) | unsupported (0) | 0.87637 (26) | 0.87637 (26) |

### perturbed: time ratios (lower is faster)

| Group | Default/v013 | Reuse/default | Reuse/v013 | Reuse/CAFS u32 | Default/CAFS u32 |
|---|---:|---:|---:|---:|---:|
| development_real | 1.00484 | 0.97987 | 0.98205 | 0.95381 | 0.97535 |
| new_real | 1.00173 | 0.94770 | 0.94815 | 1.02145 | 1.08742 |
| synthetic | 0.98212 | 0.96806 | 0.95028 | 0.99944 | 1.03407 |
| ctu_ipv4 | 0.98546 | 0.89766 | 0.88934 | 0.76464 | 0.84940 |
| all60 | 0.98842 | 0.96673 | 0.95466 | 0.99570 | 1.03281 |
| all66 | 0.98815 | 0.96024 | 0.94853 | 0.97209 | 1.01461 |

### perturbed: all input medians (ms)

| Input | Path | Reuse | Fresh ws | Default | v013 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| nasa_status | native/skip | 2.40920 | 2.35430 | 2.39020 | 2.35250 | 2.33645 | 5.60405 | 3.80460 | 3.68510 | 4.09815 | pdq |
| nasa_timestamp | native/skip | 1.37195 | 1.35440 | 1.36705 | 1.39130 | 1.41545 | 0.60370 | 0.42525 | 0.42455 | 16.03595 | CAFS_i32 |
| nasa_bytes | native/skip | 12.60835 | 12.32070 | 12.62915 | 12.41320 | 12.82520 | 17.41120 | 11.23645 | 7.35880 | 20.12900 | CAFS_i32 |
| covtype_cover | direct | 0.37900 | 0.37835 | 0.38060 | 0.37770 | 0.84535 | 1.78655 | 0.42630 | 0.42260 | 0.95505 | v013 |
| covtype_elevation | direct | 0.38105 | 0.37820 | 0.38630 | 0.38695 | 4.08250 | 3.63625 | 1.17025 | 1.33320 | 0.99410 | W_fresh |
| covtype_road_distance | mixed | 2.49460 | 2.96300 | 3.02355 | 2.96575 | 5.21330 | 5.48070 | 1.22175 | 1.20445 | 0.69930 | counting |
| census_caseid | native/skip | 1.21130 | 1.27235 | 1.22270 | 1.24055 | 1.26720 | 0.76585 | 0.54590 | 0.54725 | 8.51155 | CAFS_u32 |
| census_industry | direct | 1.62260 | 1.59505 | 1.60180 | 1.59125 | 7.41670 | 12.74715 | 6.81860 | 6.74050 | 6.32010 | v013 |
| census_sex | direct | 1.76075 | 1.72860 | 1.77465 | 1.76770 | 2.06480 | 12.40225 | 1.92225 | 1.94970 | 7.34500 | W_fresh |
| synthetic_100000_uniform | mixed | 1.15385 | 1.27630 | 1.28615 | 1.28585 | 1.65765 | 1.53440 | 1.68095 | unsupported | unsupported | W_reuse |
| synthetic_100000_normal | mixed | 1.25330 | 1.36220 | 1.38065 | 1.37695 | 1.67320 | 1.51720 | 1.70720 | unsupported | unsupported | W_reuse |
| synthetic_100000_exponential | mixed | 1.31035 | 1.43865 | 1.42300 | 1.44980 | 1.69770 | 1.54325 | 1.70530 | unsupported | unsupported | W_reuse |
| synthetic_100000_narrow | mixed | 1.15895 | 1.29140 | 1.27000 | 1.27755 | 1.65220 | 1.55930 | 1.70300 | 1.77315 | 0.76825 | counting |
| synthetic_100000_duplicates | direct | 0.08270 | 0.08205 | 0.08690 | 0.09640 | 0.17390 | 0.41785 | 0.07395 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_duplicates_narrow | direct | 0.06005 | 0.06155 | 0.05980 | 0.06070 | 0.16950 | 0.37195 | 0.07545 | 0.07385 | 0.27290 | v020_default |
| synthetic_100000_sorted | native/skip | 0.04755 | 0.04730 | 0.04690 | 0.04735 | 0.04310 | 0.02960 | 0.02100 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_partial | native/skip | 0.43280 | 0.42380 | 0.42735 | 0.43015 | 0.42310 | 0.36870 | 0.44565 | unsupported | unsupported | spread |
| synthetic_100000_reverse | native/skip | 0.08470 | 0.08670 | 0.08640 | 0.08445 | 0.08090 | 0.59080 | 0.02610 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_upper_only | direct | 0.14445 | 0.14400 | 0.14805 | 0.15945 | 1.03510 | 0.84890 | 0.35930 | unsupported | unsupported | W_fresh |
| synthetic_100000_lower_only | mixed | 1.20150 | 1.30790 | 1.32170 | 1.33380 | 1.64865 | 1.56670 | 1.72350 | unsupported | 3.27100 | W_reuse |
| synthetic_100000_discontinuous | native/skip | 1.05815 | 1.07260 | 1.06790 | 1.02645 | 1.00695 | 0.85460 | 0.47585 | unsupported | unsupported | CAFS_u32 |
| synthetic_100000_extreme_skew | native/skip | 0.04745 | 0.04775 | 0.04780 | 0.04810 | 0.04265 | 0.47705 | 0.10155 | unsupported | unsupported | pdq |
| synthetic_100000_mixture | mixed | 1.15585 | 1.25500 | 1.26445 | 1.26670 | 1.44605 | 1.38955 | 1.49695 | unsupported | unsupported | W_reuse |
| synthetic_1000000_uniform | mixed | 16.36430 | 16.45970 | 16.69705 | 17.43620 | 20.30665 | 20.98565 | 20.11655 | unsupported | unsupported | W_reuse |
| synthetic_1000000_normal | mixed | 16.22355 | 17.51685 | 16.99750 | 16.99180 | 19.85830 | 20.65875 | 19.69285 | unsupported | unsupported | W_reuse |
| synthetic_1000000_exponential | mixed | 16.20960 | 17.20885 | 17.24140 | 17.13595 | 19.85485 | 21.11880 | 19.37455 | unsupported | unsupported | W_reuse |
| synthetic_1000000_narrow | mixed | 10.51800 | 11.29510 | 11.26970 | 11.15130 | 14.31515 | 15.12050 | 11.21605 | 8.55615 | 3.54520 | counting |
| synthetic_1000000_duplicates | direct | 0.76725 | 0.75910 | 0.81425 | 0.91640 | 2.35920 | 4.22280 | 0.73000 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_duplicates_narrow | direct | 0.64915 | 0.65715 | 0.66720 | 0.63850 | 2.47430 | 3.93965 | 0.74995 | 0.76045 | 3.00090 | v013 |
| synthetic_1000000_sorted | native/skip | 0.49055 | 0.49215 | 0.48530 | 0.48520 | 0.49120 | 0.31275 | 0.22390 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_partial | native/skip | 5.52415 | 5.73875 | 5.48110 | 5.47575 | 5.62185 | 9.35150 | 5.65590 | unsupported | unsupported | v013 |
| synthetic_1000000_reverse | native/skip | 0.89105 | 0.88385 | 0.90555 | 0.88690 | 0.88620 | 7.21350 | 0.33905 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_upper_only | direct | 0.88005 | 0.90045 | 0.95155 | 1.02995 | 8.02345 | 9.11300 | 1.62475 | unsupported | unsupported | W_reuse |
| synthetic_1000000_lower_only | mixed | 15.97400 | 17.17250 | 16.63060 | 16.96075 | 19.23875 | 21.23845 | 19.61640 | unsupported | 11.64610 | counting |
| synthetic_1000000_discontinuous | native/skip | 7.94355 | 8.12275 | 8.04550 | 7.91665 | 7.69025 | 7.44045 | 1.94190 | unsupported | unsupported | CAFS_u32 |
| synthetic_1000000_extreme_skew | native/skip | 0.48320 | 0.48870 | 0.47845 | 0.48420 | 0.47545 | 7.27970 | 1.04090 | unsupported | unsupported | pdq |
| synthetic_1000000_mixture | mixed | 14.38730 | 15.07325 | 15.15650 | 15.44720 | 16.49370 | 18.64365 | 24.53090 | unsupported | unsupported | W_reuse |
| synthetic_10000000_uniform | mixed | 173.46545 | 181.61855 | 184.06430 | 184.44795 | 217.13715 | 244.00890 | 215.69110 | unsupported | unsupported | W_reuse |
| synthetic_10000000_normal | mixed | 184.19545 | 189.17050 | 184.84170 | 192.11955 | 214.71190 | 233.21070 | 209.51885 | unsupported | unsupported | W_reuse |
| synthetic_10000000_exponential | mixed | 181.78935 | 190.38995 | 195.68505 | 196.93230 | 220.79560 | 237.63245 | 213.45080 | unsupported | unsupported | W_reuse |
| synthetic_10000000_narrow | mixed | 65.65370 | 73.74845 | 70.88765 | 71.88400 | 106.84890 | 170.61275 | 70.68475 | 31.87845 | 31.42235 | counting |
| synthetic_10000000_duplicates | direct | 8.60410 | 8.81985 | 8.74250 | 10.68240 | 31.57780 | 44.31985 | 8.43645 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_duplicates_narrow | direct | 7.93970 | 7.53290 | 8.11385 | 7.74275 | 30.85345 | 40.11325 | 8.54270 | 8.39750 | 32.47890 | W_fresh |
| synthetic_10000000_sorted | native/skip | 5.36820 | 5.17385 | 5.43260 | 5.41645 | 5.22140 | 3.31310 | 2.27170 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_partial | native/skip | 64.85630 | 65.07115 | 64.81095 | 64.01080 | 65.29145 | 90.01755 | 66.62525 | unsupported | unsupported | v013 |
| synthetic_10000000_reverse | native/skip | 10.26510 | 10.43355 | 10.46515 | 10.20385 | 10.40970 | 88.96010 | 3.92945 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_upper_only | direct | 8.44700 | 8.47185 | 8.77040 | 10.25355 | 76.99365 | 189.76980 | 14.55490 | unsupported | unsupported | W_reuse |
| synthetic_10000000_lower_only | mixed | 139.33660 | 148.30330 | 147.67265 | 147.72390 | 178.75470 | 163.25350 | 175.82060 | unsupported | 60.26535 | counting |
| synthetic_10000000_discontinuous | native/skip | 75.88760 | 76.07875 | 76.40135 | 76.28170 | 74.94460 | 92.82395 | 17.82110 | unsupported | unsupported | CAFS_u32 |
| synthetic_10000000_extreme_skew | native/skip | 5.58045 | 5.41130 | 5.52730 | 5.59235 | 5.55985 | 72.87780 | 11.30400 | unsupported | unsupported | W_fresh |
| synthetic_10000000_mixture | mixed | 160.25785 | 170.26200 | 167.83065 | 173.04560 | 180.50040 | 178.62370 | 300.12625 | unsupported | unsupported | W_reuse |
| skin_red | direct | 0.12405 | 0.12560 | 0.12270 | 0.12575 | 1.30015 | 1.04560 | 0.64770 | 0.61995 | 0.46125 | v020_default |
| skin_label | direct | 0.14310 | 0.14335 | 0.14360 | 0.14395 | 0.10740 | 0.06570 | 0.04060 | 0.05380 | 0.49775 | CAFS_u32 |
| chest_sequence | native/skip | 0.06690 | 0.06425 | 0.06815 | 0.06940 | 0.06340 | 0.04665 | 0.03090 | 0.03675 | 0.50420 | CAFS_u32 |
| chest_x | mixed | 0.89200 | 1.07690 | 1.10210 | 1.07275 | 0.84615 | 0.61535 | 0.47510 | 0.32670 | 0.36260 | CAFS_i32 |
| chest_activity | direct | 0.10020 | 0.09995 | 0.09950 | 0.10185 | 0.09960 | 0.72985 | 0.16155 | 0.16890 | 0.34110 | v020_default |
| taxi_vendor | direct | 2.08510 | 2.04290 | 2.07640 | 2.09275 | 2.61980 | 14.24050 | 2.35135 | 2.42400 | 8.33840 | W_fresh |
| taxi_payment | direct | 2.24900 | 2.19690 | 2.32355 | 2.28620 | 6.04760 | 13.83755 | 2.46490 | 2.49665 | 7.40070 | W_fresh |
| taxi_pickup_zone | mixed | 7.15080 | 9.37245 | 9.53980 | 9.48900 | 12.53680 | 11.94280 | 5.98565 | 6.07100 | 7.28245 | CAFS_u32 |
| taxi_pickup_time | native/skip | 59.43490 | 59.85655 | 59.86375 | 60.54540 | 58.46860 | 71.95780 | 80.00925 | 62.79055 | unsupported | pdq |
| ctu1_srcaddr_ipv4 | mixed | 29.16985 | 31.26965 | 31.88355 | 32.34550 | 31.26750 | 43.26620 | 51.58780 | unsupported | unsupported | W_reuse |
| ctu1_dstaddr_ipv4 | mixed | 17.84990 | 19.68340 | 20.21865 | 20.33300 | 13.34515 | 40.10180 | 23.61755 | unsupported | unsupported | pdq |
| ctu2_srcaddr_ipv4 | mixed | 18.94245 | 20.60145 | 20.26725 | 20.78040 | 19.08905 | 28.15195 | 28.44385 | unsupported | unsupported | W_reuse |
| ctu2_dstaddr_ipv4 | mixed | 10.99945 | 12.44880 | 12.78250 | 13.07370 | 8.50045 | 24.48150 | 14.38645 | unsupported | unsupported | pdq |
| ctu3_srcaddr_ipv4 | mixed | 38.85065 | 41.41295 | 42.60960 | 43.06125 | 36.80185 | 64.98685 | 49.42185 | unsupported | unsupported | pdq |
| ctu3_dstaddr_ipv4 | mixed | 28.40720 | 32.89090 | 32.28765 | 32.74775 | 21.93110 | 62.10655 | 24.95790 | unsupported | unsupported | pdq |

## Largest regressions retained

### warm: reuse/v013

| Input | Ratio | Block ratio | Three process ratios | Path |
|---|---:|---:|---|---|
| synthetic_1000000_partial | 1.06272 | 1.02743 | 0.98738, 1.11311, 1.06272 | native/skip |
| synthetic_10000000_sorted | 1.03846 | 1.04773 | 1.03846, 1.18099, 1.03154 | native/skip |
| nasa_status | 1.03144 | 1.02877 | 0.98855, 1.06648, 1.03144 | native/skip |
| nasa_timestamp | 1.02657 | 1.06557 | 1.04215, 1.01959, 1.02657 | native/skip |
| synthetic_10000000_duplicates_narrow | 1.01469 | 1.01615 | 0.99435, 1.01469, 1.01618 | direct |

### warm: default/v013

| Input | Ratio | Three process ratios |
|---|---:|---|
| synthetic_1000000_partial | 1.07122 | 0.97797, 1.07255, 1.07122 |
| synthetic_10000000_reverse | 1.03874 | 0.94454, 1.03874, 1.09502 |
| synthetic_10000000_uniform | 1.03757 | 1.03757, 1.04355, 0.99662 |
| nasa_timestamp | 1.03638 | 0.99077, 1.04346, 1.03638 |
| ctu2_dstaddr_ipv4 | 1.03135 | 1.03135, 0.94024, 1.04802 |
### perturbed: reuse/v013

| Input | Ratio | Block ratio | Three process ratios | Path |
|---|---:|---:|---|---|
| census_industry | 1.05688 | 1.06903 | 1.09131, 1.05688, 1.04267 | direct |
| synthetic_10000000_extreme_skew | 1.03687 | 0.99135 | 0.94919, 1.03687, 1.04601 | native/skip |
| synthetic_100000_duplicates_narrow | 1.02451 | 1.01543 | 1.02451, 0.99341, 1.03311 | direct |
| synthetic_1000000_reverse | 1.02056 | 1.00961 | 1.02295, 0.96265, 1.02056 | native/skip |
| synthetic_1000000_duplicates_narrow | 1.01911 | 1.05457 | 1.01911, 0.97390, 1.03138 | direct |

### perturbed: default/v013

| Input | Ratio | Three process ratios |
|---|---:|---|
| synthetic_1000000_duplicates_narrow | 1.03386 | 1.00243, 1.03386, 1.04939 |
| synthetic_100000_discontinuous | 1.02874 | 1.06439, 1.02874, 1.01412 |
| synthetic_10000000_duplicates_narrow | 1.02796 | 0.97767, 1.02796, 1.06148 |
| synthetic_10000000_reverse | 1.02773 | 1.05595, 1.02773, 0.98313 |
| chest_x | 1.02736 | 1.01559, 1.03228, 1.02736 |

## Path subsets (primary reuse/default ratio)

| Mode | Path | N | Ratio |
|---|---|---:|---:|
| warm | mixed | 27 | 0.92274 |
| warm | direct | 18 | 0.99062 |
| warm | native/skip | 21 | 1.00069 |
| perturbed | mixed | 27 | 0.91289 |
| perturbed | direct | 18 | 0.98883 |
| perturbed | native/skip | 21 | 0.99930 |

## CPU scope and limitations

The full66 performance comparison is one local Intel/Windows machine. GitHub
Ubuntu GCC13.3 on AMD EPYC7763 and Clang18.1 on Intel Xeon6975P-C pass repeated
call correctness with three generated1M inputs, plus growth/failure/thread
tests. Those shared virtual machines use different compilers and environments;
the small diagnostics are not a full66 cross-CPU performance replication, a
compiler ranking, or a guarantee of workspace speedup. GCC mixed diagnostics
show little or no timing gain; all unfiltered records are included. Performance
is not a CI pass condition. Existing competitive weaknesses, historical direct
path successes, distribution selection and sample-based dispatch limitations
remain. No change is automatically released.

## Exact witness audit

Full scan omitted on24/66 inputs:18 synthetic plus all six CTU IPv4 columns. Older real18 omit none. See audit.json.

## Separate Taxi payment control

| Session | Mode | Same-function alias/v013 | Default/v013 | Reuse/v013 |
|---|---|---:|---:|---:|
| 0 | warm | 0.99683 | 1.00068 | 0.98540 |
| 0 | perturbed | 1.00446 | 0.98985 | 0.97379 |
| 1 | warm | 0.96966 | 1.09777 | 1.03160 |
| 1 | perturbed | 0.96050 | 0.98072 | 1.06778 |
| 2 | warm | 1.01186 | 0.97833 | 0.97539 |
| 2 | perturbed | 0.98802 | 0.98920 | 0.99808 |

All288 control sorts pass. Separate binary, exact same baseline function pointer; not a causal explanation for another input or the main competition.

## Linux repeated-call values

Linux repeated-call timings do not establish the Windows-sized benefit.
For the two generated 1M mixed inputs (eight repetitions), the CI artifacts give:

| Hosted environment | Input | v0.1.3 ms | New default ms | Workspace ms | Workspace/v0.1.3 time change | Workspace/default time change |
|---|---|---:|---:|---:|---:|---:|
| GCC 13.3 / AMD EPYC 7763 | Full-range uint32 | 20.028 | 19.567 | 19.908 | -0.59% | +1.74% |
| GCC 13.3 / AMD EPYC 7763 | 20-bit uint32 | 19.499 | 19.349 | 19.713 | +1.09% | +1.88% |
| Clang 18.1 / Intel Xeon 6975P-C | Full-range uint32 | 18.094 | 17.956 | 17.605 | -2.70% | -1.95% |
| Clang 18.1 / Intel Xeon 6975P-C | 20-bit uint32 | 17.942 | 17.797 | 17.597 | -1.92% | -1.13% |

Positive time changes mean slower. GCC workspace is slower than the new default
on both inputs; Clang has small gains. These short shared-VM diagnostics use
different compilers and CPUs and cannot rank platforms or establish statistical
performance conclusions. Linux may show smaller gains, no gain, or a slowdown.
Allocator behavior is an unverified explanation, not a measured cause. The 66-input
Windows performance results must not be generalized to other platforms.
