# Full ABW->pdqsort versus AVX2 and frontend-cost diagnosis

## Scope

This same-run comparison measures the actual frozen v0.2 candidate's default
full ABW->pdqsort, AVX2 alone, pdqsort alone and the earlier skip-only ABW->pdq.
The default full call includes its own analysis/count/scatter/local temporary/
bucket sorts/copy/free. No workspace reuse. Skip-only uses the exact copied
front half and calls pdqsort on the untouched whole input when mixed. Its
original count pass still completes after purity failure. No source, dispatch,
threshold, histogram, seed or input changes. Historical percentages from other
runs are not subtracted to infer the mixed-path benefit; full/skip is measured
directly here. Direct output is not presumed to win on every input: the prior
pdq skip-only skin_label regression remains in its original report.

## Conditions

Windows11/i5-14500, LLVM-MinGW Clang23.1.5/libc++ UCRT, Boost1.90.0 and
x86-simd-sort v7.0 commit8a7208187d99f0cb67e38c57ab1c7c85aad07aca. Common
C++17 -O3 -DNDEBUG -mavx2 -mno-avx512f -mbmi -static -pthread. AVX2 only,
single-threaded, no OpenMP. Original unchanged66 inputs, no eligibility drops.
One binary/shared fixed input scratch/common noinline function-pointer timer.
Input load/restore outside time, complete API inside, full-array expected
equality after every call. Input/source/expected hashes checked before/after.
Three fresh processes/input, randomized input and comparator order, four
ABBA/BAAB blocks per comparison:198 processes and9,504 correct timed sorts.
Warm repeated calls; no cache-disturbance condition, external CPU test or
additional memory profile in this run. Original files and public code preserved.

Each pair has its own full-ABW call samples. Primary ratio is median3 of the
same-process paired-call-median ratios. Times shown are median3 process medians
within each pair; quotients may differ slightly from primary ratios. Equal-input
geometric means by existing real9/new real9/synthetic42/IPv4_6/all66. The all66
average is not a production input mix. Process ratios and every input remain
in the CSV. No discarded inputs/runs or timing-based tuning.

## Geometric mean full ABW time / comparator time

Below1 means full ABW is faster.

| Input group | N | /AVX2 | /pdqsort | /skip-only pdq |
|---|---:|---:|---:|---:|
| development_real | 9 | 0.88528 | 0.53578 | 0.93778 |
| new_real | 9 | 1.41225 | 0.66335 | 0.95275 |
| real18 | 18 | 1.11814 | 0.59616 | 0.94523 |
| synthetic | 42 | 1.43324 | 0.67069 | 0.90892 |
| ctu_ipv4 | 6 | 4.58496 | 1.27012 | 1.06896 |
| all66 | 66 | 1.48875 | 0.68830 | 0.93233 |
| direct | 18 | 0.94605 | 0.28746 | 1.00473 |
| mixed | 27 | 3.96780 | 0.90705 | 0.84264 |
| native_skip | 21 | 0.62264 | 1.02026 | 0.99587 |

## All66: full ABW versus AVX2 input table

| Input | Path | AVX2 ms | Full ABW ms | Full/AVX2 | Three process ratios |
|---|---|---:|---:|---:|---|
| nasa_status | native/skip | 0.79660 | 2.30305 | 3.07050 | 3.0705, 2.8911, 3.2340 |
| nasa_timestamp | native/skip | 6.90860 | 1.26680 | 0.18337 | 0.1857, 0.1834, 0.1799 |
| nasa_bytes | native/skip | 3.38665 | 12.49195 | 3.69854 | 3.6886, 3.7332, 3.6985 |
| covtype_cover | direct | 0.27530 | 0.34530 | 1.23568 | 1.2180, 1.2543, 1.2357 |
| covtype_elevation | direct | 1.22435 | 0.33445 | 0.27277 | 0.2732, 0.2728, 0.2727 |
| covtype_road_distance | mixed | 1.76455 | 2.94910 | 1.67130 | 1.6663, 1.6713, 1.7249 |
| census_caseid | native/skip | 8.91145 | 1.17780 | 0.13217 | 0.1322, 0.1333, 0.1232 |
| census_industry | direct | 1.81505 | 1.55810 | 0.87067 | 0.8707, 1.5378, 0.8584 |
| census_sex | direct | 0.65605 | 1.58245 | 2.47429 | 2.4121, 2.5493, 2.4743 |
| synthetic_100000_uniform | mixed | 0.29150 | 1.24405 | 4.34271 | 4.5214, 4.1317, 4.3427 |
| synthetic_100000_normal | mixed | 0.29640 | 1.32920 | 4.49200 | 4.7114, 4.4920, 4.4845 |
| synthetic_100000_exponential | mixed | 0.29335 | 1.36200 | 4.60992 | 4.5913, 4.6099, 4.9241 |
| synthetic_100000_narrow | mixed | 0.30875 | 1.27545 | 4.04071 | 4.0407, 4.1489, 4.0008 |
| synthetic_100000_duplicates | direct | 0.04585 | 0.07380 | 1.63459 | 1.6369, 1.6346, 1.5488 |
| synthetic_100000_duplicates_narrow | direct | 0.04245 | 0.04195 | 0.98822 | 0.9882, 1.0267, 0.9435 |
| synthetic_100000_sorted | native/skip | 0.22545 | 0.03650 | 0.16190 | 0.1619, 0.1769, 0.1448 |
| synthetic_100000_partial | native/skip | 0.28335 | 0.43655 | 1.54067 | 1.5407, 1.5456, 1.5117 |
| synthetic_100000_reverse | native/skip | 0.24545 | 0.08005 | 0.32929 | 0.3293, 0.3243, 0.3334 |
| synthetic_100000_upper_only | direct | 0.28465 | 0.12640 | 0.45257 | 0.4697, 0.4425, 0.4526 |
| synthetic_100000_lower_only | mixed | 0.30050 | 1.25990 | 4.38302 | 4.4043, 4.3830, 4.0874 |
| synthetic_100000_discontinuous | native/skip | 0.29135 | 1.04215 | 3.57697 | 3.5770, 3.5694, 3.5812 |
| synthetic_100000_extreme_skew | native/skip | 0.06270 | 0.03765 | 0.58772 | 0.5877, 0.6763, 0.5713 |
| synthetic_100000_mixture | mixed | 0.26405 | 1.25110 | 4.84453 | 4.3469, 4.8445, 4.8688 |
| synthetic_1000000_uniform | mixed | 3.41900 | 15.60650 | 4.56464 | 4.5579, 4.5646, 4.9817 |
| synthetic_1000000_normal | mixed | 3.49050 | 16.14040 | 4.61548 | 4.6155, 4.5342, 4.9596 |
| synthetic_1000000_exponential | mixed | 3.45635 | 16.35635 | 4.86716 | 4.7323, 4.8811, 4.8672 |
| synthetic_1000000_narrow | mixed | 3.56960 | 11.37630 | 3.18700 | 3.0566, 3.2731, 3.1870 |
| synthetic_1000000_duplicates | direct | 0.65070 | 0.74080 | 1.15741 | 1.1574, 1.1330, 1.2266 |
| synthetic_1000000_duplicates_narrow | direct | 0.65325 | 0.56755 | 0.86484 | 0.8648, 0.8485, 0.8725 |
| synthetic_1000000_sorted | native/skip | 3.32635 | 0.44510 | 0.13469 | 0.1364, 0.1347, 0.1338 |
| synthetic_1000000_partial | native/skip | 3.58655 | 5.29990 | 1.48386 | 1.4839, 1.5110, 1.4415 |
| synthetic_1000000_reverse | native/skip | 3.31790 | 0.79020 | 0.23816 | 0.2436, 0.2382, 0.2272 |
| synthetic_1000000_upper_only | direct | 2.72340 | 0.82520 | 0.30421 | 0.3030, 0.3042, 0.3057 |
| synthetic_1000000_lower_only | mixed | 3.56120 | 15.77805 | 4.48230 | 4.4823, 4.4305, 4.6250 |
| synthetic_1000000_discontinuous | native/skip | 2.68100 | 8.02030 | 3.06545 | 3.0655, 2.8848, 3.2765 |
| synthetic_1000000_extreme_skew | native/skip | 1.52345 | 0.46535 | 0.29870 | 0.2967, 0.2987, 0.3055 |
| synthetic_1000000_mixture | mixed | 2.99430 | 15.40980 | 4.95712 | 4.9571, 4.5929, 5.2612 |
| synthetic_10000000_uniform | mixed | 44.86900 | 181.81250 | 4.05207 | 4.0282, 4.3626, 4.0521 |
| synthetic_10000000_normal | mixed | 43.98695 | 185.21670 | 4.21072 | 4.2525, 4.1911, 4.2107 |
| synthetic_10000000_exponential | mixed | 42.37985 | 178.74345 | 4.23460 | 4.2346, 4.2026, 4.2542 |
| synthetic_10000000_narrow | mixed | 41.23050 | 71.99140 | 1.76361 | 1.7636, 1.7461, 1.7801 |
| synthetic_10000000_duplicates | direct | 7.03140 | 8.84335 | 1.24485 | 1.2425, 1.2664, 1.2449 |
| synthetic_10000000_duplicates_narrow | direct | 6.78470 | 7.27105 | 1.09331 | 1.0933, 1.0401, 1.1868 |
| synthetic_10000000_sorted | native/skip | 43.73715 | 5.20280 | 0.11721 | 0.1219, 0.1143, 0.1172 |
| synthetic_10000000_partial | native/skip | 45.67935 | 65.76740 | 1.45774 | 1.4398, 1.4577, 1.4655 |
| synthetic_10000000_reverse | native/skip | 43.41230 | 10.01055 | 0.23284 | 0.2328, 0.2306, 0.2446 |
| synthetic_10000000_upper_only | direct | 22.66055 | 8.77030 | 0.38038 | 0.3895, 0.3804, 0.3719 |
| synthetic_10000000_lower_only | mixed | 42.81730 | 143.52090 | 3.35194 | 3.3463, 3.3519, 3.4099 |
| synthetic_10000000_discontinuous | native/skip | 23.00575 | 75.73130 | 3.30594 | 3.3375, 3.2918, 3.3059 |
| synthetic_10000000_extreme_skew | native/skip | 17.96270 | 5.66845 | 0.31557 | 0.3047, 0.3156, 0.3261 |
| synthetic_10000000_mixture | mixed | 37.70355 | 159.02895 | 4.26746 | 4.1616, 4.2675, 4.3724 |
| skin_red | direct | 0.40495 | 0.11175 | 0.27596 | 0.2707, 0.2760, 0.2812 |
| skin_label | direct | 0.05320 | 0.13425 | 2.52350 | 2.5235, 2.4681, 2.5563 |
| chest_sequence | native/skip | 0.40255 | 0.05195 | 0.12905 | 0.1289, 0.1291, 0.1298 |
| chest_x | mixed | 0.23990 | 0.96290 | 3.96278 | 4.0552, 3.9628, 3.9446 |
| chest_activity | direct | 0.04615 | 0.08460 | 1.83315 | 1.8397, 1.8332, 1.8311 |
| taxi_vendor | direct | 0.87025 | 2.04635 | 2.35145 | 2.4313, 2.3515, 2.2373 |
| taxi_payment | direct | 1.39465 | 2.08740 | 1.46700 | 1.4538, 3.8675, 1.4670 |
| taxi_pickup_zone | mixed | 3.99455 | 9.19675 | 2.30507 | 2.2456, 2.3051, 2.3664 |
| taxi_pickup_time | native/skip | 12.99345 | 55.93135 | 4.30458 | 4.4015, 4.3046, 4.2989 |
| ctu1_srcaddr_ipv4 | mixed | 7.03040 | 30.58485 | 4.35037 | 4.2915, 4.3504, 4.3713 |
| ctu1_dstaddr_ipv4 | mixed | 3.99120 | 18.90805 | 4.73743 | 4.6958, 4.7374, 4.9132 |
| ctu2_srcaddr_ipv4 | mixed | 4.49020 | 21.43910 | 4.77464 | 4.3878, 4.7746, 4.9815 |
| ctu2_dstaddr_ipv4 | mixed | 2.48865 | 11.93690 | 4.83253 | 5.5288, 4.7394, 4.8325 |
| ctu3_srcaddr_ipv4 | mixed | 9.66240 | 40.71300 | 4.22808 | 4.3508, 4.2135, 4.2281 |
| ctu3_dstaddr_ipv4 | mixed | 6.93280 | 32.03260 | 4.62044 | 4.5878, 4.6204, 4.8221 |

## Same-run mixed-path contribution: full versus skip-only pdq

| Input | Path | skip-only ms | full ms | Full/skip-only | Three process ratios |
|---|---|---:|---:|---:|---|
| nasa_status | native/skip | 2.37660 | 2.28400 | 0.97457 | 0.9746, 0.9572, 0.9963 |
| nasa_timestamp | native/skip | 1.26370 | 1.33815 | 1.05891 | 1.0589, 0.9685, 1.1148 |
| nasa_bytes | native/skip | 12.36715 | 12.18660 | 0.99554 | 0.9955, 0.9779, 1.0201 |
| covtype_cover | direct | 0.34690 | 0.34650 | 0.99885 | 0.9991, 0.9988, 0.9822 |
| covtype_elevation | direct | 0.33965 | 0.33755 | 0.99759 | 0.9938, 0.9976, 1.0061 |
| covtype_road_distance | mixed | 5.49550 | 2.94890 | 0.53660 | 0.5273, 0.5366, 0.6302 |
| census_caseid | native/skip | 1.14760 | 1.16560 | 1.01166 | 1.0063, 1.0157, 1.0117 |
| census_industry | direct | 1.48510 | 1.61765 | 0.98185 | 0.9749, 0.9819, 1.0938 |
| census_sex | direct | 1.60305 | 1.58680 | 1.02799 | 0.9716, 1.0280, 1.0917 |
| synthetic_100000_uniform | mixed | 1.73830 | 1.22865 | 0.71565 | 0.7293, 0.7068, 0.7157 |
| synthetic_100000_normal | mixed | 1.72020 | 1.34050 | 0.77535 | 0.7753, 0.7559, 0.7793 |
| synthetic_100000_exponential | mixed | 1.72840 | 1.36280 | 0.79417 | 0.7814, 0.8082, 0.7942 |
| synthetic_100000_narrow | mixed | 1.72190 | 1.26870 | 0.73438 | 0.7344, 0.7414, 0.7115 |
| synthetic_100000_duplicates | direct | 0.07070 | 0.07140 | 1.03343 | 1.0334, 1.0347, 0.9917 |
| synthetic_100000_duplicates_narrow | direct | 0.04135 | 0.04195 | 1.01451 | 1.0145, 1.0038, 1.0239 |
| synthetic_100000_sorted | native/skip | 0.04045 | 0.03565 | 0.88133 | 0.8813, 0.8506, 1.0000 |
| synthetic_100000_partial | native/skip | 0.43175 | 0.42765 | 0.99050 | 0.9905, 0.9875, 1.0308 |
| synthetic_100000_reverse | native/skip | 0.08000 | 0.08000 | 1.00063 | 1.0083, 1.0000, 1.0006 |
| synthetic_100000_upper_only | direct | 0.12800 | 0.12705 | 0.99064 | 0.9856, 0.9906, 0.9977 |
| synthetic_100000_lower_only | mixed | 1.73405 | 1.23955 | 0.71108 | 0.7088, 0.7111, 0.7175 |
| synthetic_100000_discontinuous | native/skip | 1.04415 | 1.02785 | 0.98485 | 0.9844, 1.0118, 0.9848 |
| synthetic_100000_extreme_skew | native/skip | 0.03805 | 0.03800 | 0.99865 | 0.9986, 0.9962, 0.9987 |
| synthetic_100000_mixture | mixed | 1.56555 | 1.27580 | 0.81498 | 0.7849, 0.8214, 0.8150 |
| synthetic_1000000_uniform | mixed | 18.97365 | 15.69535 | 0.82475 | 0.8448, 0.8248, 0.8228 |
| synthetic_1000000_normal | mixed | 19.81695 | 16.46900 | 0.83522 | 0.8352, 0.8424, 0.8296 |
| synthetic_1000000_exponential | mixed | 19.17480 | 16.09420 | 0.85954 | 0.8287, 0.8595, 0.8635 |
| synthetic_1000000_narrow | mixed | 14.64820 | 11.49145 | 0.78450 | 0.7690, 0.8126, 0.7845 |
| synthetic_1000000_duplicates | direct | 0.73545 | 0.75880 | 1.03394 | 1.0339, 1.0550, 1.0092 |
| synthetic_1000000_duplicates_narrow | direct | 0.55950 | 0.57625 | 1.00814 | 1.0046, 1.0623, 1.0081 |
| synthetic_1000000_sorted | native/skip | 0.43740 | 0.43985 | 1.00560 | 0.9984, 1.0056, 1.0154 |
| synthetic_1000000_partial | native/skip | 5.60485 | 5.50775 | 1.00721 | 1.0072, 1.0325, 0.9827 |
| synthetic_1000000_reverse | native/skip | 0.79020 | 0.79780 | 1.00853 | 0.9615, 1.1215, 1.0085 |
| synthetic_1000000_upper_only | direct | 0.82325 | 0.81275 | 0.98688 | 0.9888, 0.9869, 0.9428 |
| synthetic_1000000_lower_only | mixed | 19.10510 | 15.59005 | 0.81602 | 0.8118, 0.8160, 0.8276 |
| synthetic_1000000_discontinuous | native/skip | 7.91465 | 7.87155 | 0.98018 | 0.9802, 0.9955, 0.9458 |
| synthetic_1000000_extreme_skew | native/skip | 0.45300 | 0.44430 | 0.98079 | 1.0040, 0.9808, 0.9773 |
| synthetic_1000000_mixture | mixed | 17.57415 | 15.53625 | 0.88009 | 0.8801, 0.8768, 0.9155 |
| synthetic_10000000_uniform | mixed | 222.62765 | 183.90725 | 0.82758 | 0.8500, 0.8276, 0.8261 |
| synthetic_10000000_normal | mixed | 215.36755 | 182.98300 | 0.85004 | 0.8654, 0.8500, 0.8496 |
| synthetic_10000000_exponential | mixed | 217.84540 | 177.72525 | 0.85651 | 0.8153, 0.8662, 0.8565 |
| synthetic_10000000_narrow | mixed | 105.33335 | 71.18155 | 0.67816 | 0.6789, 0.6758, 0.6782 |
| synthetic_10000000_duplicates | direct | 8.61005 | 8.94300 | 1.03057 | 1.0306, 1.0387, 1.0223 |
| synthetic_10000000_duplicates_narrow | direct | 7.46060 | 7.44905 | 0.99845 | 0.9958, 0.9985, 1.0798 |
| synthetic_10000000_sorted | native/skip | 5.20995 | 5.22885 | 1.03165 | 1.0316, 1.0745, 0.9576 |
| synthetic_10000000_partial | native/skip | 67.15805 | 66.31765 | 0.99252 | 0.9875, 0.9963, 0.9925 |
| synthetic_10000000_reverse | native/skip | 10.02790 | 10.18000 | 1.03306 | 0.9920, 1.0331, 1.0517 |
| synthetic_10000000_upper_only | direct | 9.00215 | 8.89785 | 0.99577 | 0.9958, 1.0007, 0.9759 |
| synthetic_10000000_lower_only | mixed | 177.89025 | 143.19180 | 0.80494 | 0.8109, 0.8049, 0.7944 |
| synthetic_10000000_discontinuous | native/skip | 75.76120 | 75.45570 | 0.99597 | 0.9945, 0.9960, 0.9962 |
| synthetic_10000000_extreme_skew | native/skip | 5.45635 | 5.53780 | 0.99405 | 0.9940, 0.9776, 1.0149 |
| synthetic_10000000_mixture | mixed | 175.60450 | 158.80585 | 0.91024 | 0.9102, 0.9043, 0.9180 |
| skin_red | direct | 0.11155 | 0.11090 | 0.99552 | 0.9955, 0.9849, 1.0018 |
| skin_label | direct | 0.13430 | 0.13375 | 0.99888 | 0.9989, 0.9989, 0.9952 |
| chest_sequence | native/skip | 0.05185 | 0.05190 | 1.00000 | 0.9899, 1.0000, 1.0010 |
| chest_x | mixed | 1.00870 | 1.02880 | 1.02022 | 1.0220, 0.8944, 1.0202 |
| chest_activity | direct | 0.08405 | 0.08435 | 1.00060 | 1.0006, 1.0036, 0.9972 |
| taxi_vendor | direct | 2.03550 | 2.03900 | 1.00172 | 1.0105, 0.9983, 1.0017 |
| taxi_payment | direct | 2.09935 | 2.08950 | 0.99207 | 1.0158, 0.8962, 0.9921 |
| taxi_pickup_zone | mixed | 14.36290 | 9.22915 | 0.64232 | 0.6426, 0.6385, 0.6423 |
| taxi_pickup_time | native/skip | 55.87690 | 55.94855 | 0.99825 | 0.9983, 1.0013, 0.9632 |
| ctu1_srcaddr_ipv4 | mixed | 31.68680 | 30.27955 | 0.94495 | 0.9422, 0.9556, 0.9449 |
| ctu1_dstaddr_ipv4 | mixed | 16.18820 | 19.06425 | 1.17766 | 1.1648, 1.1777, 1.1905 |
| ctu2_srcaddr_ipv4 | mixed | 20.47415 | 20.58130 | 1.00523 | 1.0052, 1.0424, 0.9873 |
| ctu2_dstaddr_ipv4 | mixed | 10.34480 | 12.05130 | 1.16089 | 1.1609, 1.1650, 1.1478 |
| ctu3_srcaddr_ipv4 | mixed | 41.41720 | 41.20820 | 0.99495 | 0.9999, 0.9950, 0.9917 |
| ctu3_dstaddr_ipv4 | mixed | 27.67000 | 31.62140 | 1.15471 | 1.1428, 1.1547, 1.2115 |

## Separate frontend phase diagnosis on27 mixed inputs

This diagnostic executes after the uninstrumented primary comparison, in a
separate executable. It measures probe, sample allocation/frequencies, width
scan, entropy and count/purity. Three repeats alternate normal and instrumented
frontend calls; choices and untouched-input invariants agree. No backend sort
runs in this diagnostic. Phase clocks never enter the primary API timings.
The original fixed frequency helper, sample positions, certificate and copied
count pass are retained. These component costs describe the diagnostic; they
are not subtracted from primary end-to-end measurements. A zero width-scan cost
can mean the existing bit31 certificate omitted the scan.

| Input | Normal front ms | Instrumented ms | Probe ms | Sample allocation+frequency ms | Width scan ms | Entropy ms | Count/purity ms | Instrumented/normal |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| covtype_road_distance | 0.259600 | 0.279300 | 0.001200 | 0.004100 | 0.035700 | 0.007200 | 0.221600 | 1.050761 |
| synthetic_100000_uniform | 0.069700 | 0.072400 | 0.001100 | 0.003500 | 0.000000 | 0.013700 | 0.052900 | 1.038737 |
| synthetic_100000_normal | 0.070700 | 0.070100 | 0.001100 | 0.003500 | 0.000000 | 0.012500 | 0.052000 | 0.991513 |
| synthetic_100000_exponential | 0.077600 | 0.077900 | 0.001100 | 0.003700 | 0.000000 | 0.015500 | 0.057300 | 1.003866 |
| synthetic_100000_narrow | 0.078100 | 0.077400 | 0.001200 | 0.003700 | 0.004700 | 0.014800 | 0.052400 | 0.983355 |
| synthetic_100000_lower_only | 0.080600 | 0.081900 | 0.001100 | 0.003500 | 0.004700 | 0.015900 | 0.056700 | 1.023316 |
| synthetic_100000_mixture | 0.102100 | 0.107000 | 0.001100 | 0.003600 | 0.000000 | 0.014500 | 0.088200 | 1.049563 |
| synthetic_1000000_uniform | 0.427700 | 0.429600 | 0.001200 | 0.018300 | 0.000000 | 0.019700 | 0.380600 | 1.004442 |
| synthetic_1000000_normal | 0.440900 | 0.428200 | 0.001400 | 0.017900 | 0.000000 | 0.013900 | 0.385000 | 0.971195 |
| synthetic_1000000_exponential | 0.443200 | 0.463000 | 0.001300 | 0.017700 | 0.000000 | 0.012300 | 0.425900 | 1.045147 |
| synthetic_1000000_narrow | 0.472000 | 0.481800 | 0.001400 | 0.022500 | 0.065500 | 0.020200 | 0.365300 | 1.027292 |
| synthetic_1000000_lower_only | 0.470400 | 0.475000 | 0.001200 | 0.018700 | 0.061600 | 0.019800 | 0.364000 | 0.998512 |
| synthetic_1000000_mixture | 0.668900 | 0.684800 | 0.001300 | 0.021800 | 0.000000 | 0.011200 | 0.640400 | 1.002342 |
| synthetic_10000000_uniform | 4.122900 | 4.116100 | 0.005000 | 0.048500 | 0.000000 | 0.021300 | 4.032200 | 0.988559 |
| synthetic_10000000_normal | 4.243600 | 4.478600 | 0.006200 | 0.050200 | 0.000100 | 0.015000 | 4.385000 | 1.044990 |
| synthetic_10000000_exponential | 4.534100 | 4.472100 | 0.004700 | 0.044700 | 0.000000 | 0.012400 | 4.402100 | 1.011879 |
| synthetic_10000000_narrow | 5.514100 | 5.846700 | 0.005800 | 0.051900 | 1.482700 | 0.020800 | 4.269400 | 1.073143 |
| synthetic_10000000_lower_only | 6.169400 | 5.600800 | 0.004500 | 0.048800 | 1.394100 | 0.020600 | 4.101900 | 0.907835 |
| synthetic_10000000_mixture | 7.129100 | 6.677000 | 0.005000 | 0.050000 | 0.000000 | 0.012300 | 6.590300 | 0.974307 |
| chest_x | 0.162000 | 0.157000 | 0.001100 | 0.003000 | 0.007000 | 0.000600 | 0.144300 | 0.959877 |
| taxi_pickup_zone | 1.366800 | 1.359000 | 0.001700 | 0.016700 | 0.218300 | 0.002300 | 1.118900 | 0.989299 |
| ctu1_srcaddr_ipv4 | 2.760500 | 2.705500 | 0.002900 | 0.031200 | 0.000100 | 0.003400 | 2.674600 | 0.988335 |
| ctu1_dstaddr_ipv4 | 3.548400 | 3.524900 | 0.002100 | 0.027500 | 0.000000 | 0.003900 | 3.487400 | 0.974214 |
| ctu2_srcaddr_ipv4 | 1.659600 | 1.687800 | 0.001600 | 0.018000 | 0.000000 | 0.003300 | 1.643000 | 0.989757 |
| ctu2_dstaddr_ipv4 | 2.199500 | 2.213300 | 0.001600 | 0.020800 | 0.000100 | 0.003500 | 2.186000 | 1.006189 |
| ctu3_srcaddr_ipv4 | 5.367300 | 6.177700 | 0.003400 | 0.035100 | 0.000000 | 0.003700 | 6.134800 | 1.150988 |
| ctu3_dstaddr_ipv4 | 5.924900 | 6.387900 | 0.002900 | 0.029500 | 0.000000 | 0.003800 | 6.350200 | 1.078145 |

The sum of per-input median component times (27 one-input diagnoses, not a
single application call) is recorded in full_summary.json. This preserves both
absolute sizes and the normal/instrumented total comparison.

## Reproduction files

Run from the original workspace root:

```
python work/abw_full_backend_check/run_full_abw_avx2_comparison.py
```

The output directory must not already exist. The archive includes the measured
sources/build commands, frozen ABW/front-end, pinned x86 source archive, raw
sessions, phase records, protocol/input hashes and result CSVs. Boost, compiler
and dataset binaries are not bundled. full_comparison_plan.json preserves the
prepared plan; completion.json records execution completion. Public README
updates document these results; no main merge or release is part of this run.
