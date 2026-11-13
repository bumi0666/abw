# ABW v0.1.2 candidate competitive comparison

Date: 2026-11-13. Current candidate is byte-identical to the full51 paired candidate. Public v0.1.1, Boost pdqsort/Spreadsort, CAFS and Counting implementations are unchanged. No optimization, policy adjustment, GitHub edit or release. Results are not pooled with historical measurements.

## Conditions

Existing nine real columns and42 synthetic files, original values/order/expected arrays and SHA-256 before/after. Same Intel i5-14500 Windows x64 first-allowed-CPU affinity; LLVM-MinGW Clang23.1.5/libc++/UCRT, Boost1.90.0, C++20/O3/NDEBUG/AVX2/BMI/static/pthread for all methods in one binary. CAFS pinned official commit13488afef446ae88e7ad631ec6f862def896094a and sources retain recorded hashes. Both supported scalar uint32 and specialized int32 APIs use the unchanged public pdqsort fallback. Counting is the unchanged uint64 dense histogram with runtime min/max inside timer, eligible only under the prior64MiB counter budget.

One fixed u32 scratch vector is shared by all u32 methods; values are restored before each timed call outside timing. CAFS_i32 uses a separate fixed int32 vector with exactly the same representable nonnegative values, converted outside timer; no reinterpret aliasing or value compression. Unsupported i32/range cases are omitted and denominators shown. One common noinline templated function-pointer timer; actual sort-owned analysis, allocations, initialization/merge/output/free inside timer; input load/restore/conversion, warmup and full output check outside. Scratch pointers are checked stable. One actual-input warmup per eligible method plus the original three100k warmup families.

Each competitor is paired against candidate in ABBA/BAAB blocks, with four blocks per competitor, pair order randomized each block; three fresh processes per input. Every call compares the complete output with the existing expected array. All51 source/input hashes rechecked. No outlier filtering. One CPU; process sessions are not independent hardware/dataset seeds. This experiment measures runtime, not a new memory/RSS profile.

Timed sorts: **11376** in153 processes. Repetition counts vary only with declared competitor eligibility.

Primary tables use per-method median call time in each process, then median of three process medians per input. pdq speedups and competitor time ratios use median of paired same-process method-median ratios, followed by geometric mean across eligible inputs. Candidate appears in multiple anchored pairs; its median pools its calls within that process. The independent block-paired ratios are also retained in paired_blocks.csv/summary.json as a robustness view; those estimates need not numerically equal ratios of medians. No estimator is substituted after seeing results.

The raw-time winner is the smallest displayed input median. Clear-best requires candidate's input-level time ratio below0.95 against every eligible competitor; 'near best' is a descriptive5% band, not statistical significance. Unsupported methods are not counted as wins/losses. Comparisons involving CAFS_i32 or Counting over synthetic data use only6 and9 of42 cases respectively; these subset geometric means must not be treated as42-input totals.

## Geometric mean speedup relative to pdqsort

| Method | Real9 | Synthetic eligible subset | N synthetic |
|---|---:|---:|---:|
| candidate | 1.7884 | 1.4322 | 42 |
| v011 | 1.4918 | 1.4263 | 42 |
| pdq | 1.0000 | 1.0000 | 42 |
| spread | 0.7157 | 0.6362 | 42 |
| CAFS_u32 | 1.7803 | 1.5293 | 42 |
| CAFS_i32 | 1.8286 | 2.3321 | 6 |
| counting | 0.6935 | 1.5087 | 9 |

## All real-column times (ms)

| Input | Candidate | v0.1.1 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest median |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| nasa_status | 2.4196 | 2.3607 | 2.3594 | 5.6793 | 3.6925 | 3.6368 | 4.2429 | pdq |
| nasa_timestamp | 1.3506 | 1.3178 | 1.2889 | 0.6033 | 0.4305 | 0.4388 | 15.8830 | CAFS_u32 |
| nasa_bytes | 12.7613 | 12.7696 | 12.3126 | 17.5211 | 11.2941 | 7.3586 | 20.1086 | CAFS_i32 |
| covtype_cover | 0.3607 | 0.8196 | 0.8269 | 1.7770 | 0.3917 | 0.4053 | 0.9505 | candidate |
| covtype_elevation | 0.4353 | 0.4335 | 4.1643 | 3.7330 | 1.1391 | 1.2307 | 0.9197 | v011 |
| covtype_road_distance | 3.0194 | 3.0412 | 5.0274 | 5.2614 | 1.2005 | 1.1728 | 0.6774 | counting |
| census_caseid | 1.2336 | 1.2601 | 1.2389 | 0.7972 | 0.5702 | 0.5621 | 8.5952 | CAFS_i32 |
| census_industry | 1.5348 | 2.1967 | 7.0686 | 12.6349 | 7.0145 | 6.8361 | 6.3044 | candidate |
| census_sex | 1.6372 | 2.7277 | 2.0960 | 12.7832 | 1.7707 | 1.9250 | 7.9523 | candidate |

## Every input, including synthetic regressions

| Input | Candidate | v0.1.1 | pdq | Spread | CAFS u32 | CAFS i32 | Counting | Lowest median |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| nasa_status | 2.4196 | 2.3607 | 2.3594 | 5.6793 | 3.6925 | 3.6368 | 4.2429 | pdq |
| nasa_timestamp | 1.3506 | 1.3178 | 1.2889 | 0.6033 | 0.4305 | 0.4388 | 15.8830 | CAFS_u32 |
| nasa_bytes | 12.7613 | 12.7696 | 12.3126 | 17.5211 | 11.2941 | 7.3586 | 20.1086 | CAFS_i32 |
| covtype_cover | 0.3607 | 0.8196 | 0.8269 | 1.7770 | 0.3917 | 0.4053 | 0.9505 | candidate |
| covtype_elevation | 0.4353 | 0.4335 | 4.1643 | 3.7330 | 1.1391 | 1.2307 | 0.9197 | v011 |
| covtype_road_distance | 3.0194 | 3.0412 | 5.0274 | 5.2614 | 1.2005 | 1.1728 | 0.6774 | counting |
| census_caseid | 1.2336 | 1.2601 | 1.2389 | 0.7972 | 0.5702 | 0.5621 | 8.5952 | CAFS_i32 |
| census_industry | 1.5348 | 2.1967 | 7.0686 | 12.6349 | 7.0145 | 6.8361 | 6.3044 | candidate |
| census_sex | 1.6372 | 2.7277 | 2.0960 | 12.7832 | 1.7707 | 1.9250 | 7.9523 | candidate |
| synthetic_100000_uniform | 1.3485 | 1.3077 | 1.7085 | 1.5495 | 1.6986 | unsupported / budget | unsupported / budget | v011 |
| synthetic_100000_normal | 1.4547 | 1.4527 | 1.7841 | 1.6324 | 1.7669 | unsupported / budget | unsupported / budget | v011 |
| synthetic_100000_exponential | 1.4738 | 1.4819 | 1.7432 | 1.5904 | 1.7800 | unsupported / budget | unsupported / budget | candidate |
| synthetic_100000_narrow | 1.1819 | 1.2083 | 1.7345 | 1.5450 | 1.7192 | 1.8054 | 0.6314 | counting |
| synthetic_100000_duplicates | 0.0844 | 0.0896 | 0.1703 | 0.4096 | 0.0675 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_100000_duplicates_narrow | 0.0446 | 0.0469 | 0.1707 | 0.3936 | 0.0678 | 0.0682 | 0.2854 | candidate |
| synthetic_100000_sorted | 0.0335 | 0.0333 | 0.0328 | 0.0258 | 0.0132 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_100000_partial | 0.4390 | 0.4343 | 0.4379 | 0.3752 | 0.4446 | unsupported / budget | unsupported / budget | spread |
| synthetic_100000_reverse | 0.0827 | 0.0826 | 0.0819 | 0.5836 | 0.0190 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_100000_upper_only | 0.1746 | 0.1745 | 1.0464 | 0.8736 | 0.3375 | unsupported / budget | unsupported / budget | v011 |
| synthetic_100000_lower_only | 1.3516 | 1.3452 | 1.7436 | 1.5956 | 1.7704 | unsupported / budget | 3.2437 | v011 |
| synthetic_100000_discontinuous | 1.0947 | 1.0722 | 1.0432 | 0.8169 | 0.4673 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_100000_extreme_skew | 0.0380 | 0.0378 | 0.0372 | 0.5563 | 0.0965 | unsupported / budget | unsupported / budget | pdq |
| synthetic_100000_mixture | 1.2968 | 1.2875 | 1.4911 | 1.3983 | 1.4806 | unsupported / budget | unsupported / budget | v011 |
| synthetic_1000000_uniform | 16.9860 | 16.9614 | 19.0461 | 20.7513 | 19.6028 | unsupported / budget | unsupported / budget | v011 |
| synthetic_1000000_normal | 18.0104 | 17.7146 | 19.3952 | 21.4405 | 20.0674 | unsupported / budget | unsupported / budget | v011 |
| synthetic_1000000_exponential | 16.8344 | 16.8908 | 18.6291 | 19.7454 | 18.4729 | unsupported / budget | unsupported / budget | candidate |
| synthetic_1000000_narrow | 11.3739 | 11.3874 | 13.6741 | 14.9946 | 11.4801 | 8.1349 | 3.3962 | counting |
| synthetic_1000000_duplicates | 1.0111 | 1.0094 | 2.4452 | 4.5722 | 0.6914 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_1000000_duplicates_narrow | 0.5967 | 0.6083 | 2.4788 | 3.9981 | 0.6950 | 0.7239 | 2.9346 | candidate |
| synthetic_1000000_sorted | 0.4444 | 0.4422 | 0.4976 | 0.3125 | 0.2253 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_1000000_partial | 5.6525 | 5.5088 | 5.6189 | 9.3008 | 5.4218 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_1000000_reverse | 0.8318 | 0.8378 | 0.8241 | 7.0302 | 0.3927 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_1000000_upper_only | 1.2574 | 1.2736 | 7.6909 | 8.9772 | 1.5486 | unsupported / budget | unsupported / budget | candidate |
| synthetic_1000000_lower_only | 16.9672 | 16.9980 | 19.1310 | 20.4817 | 19.3805 | unsupported / budget | 12.6066 | counting |
| synthetic_1000000_discontinuous | 8.7602 | 8.8576 | 8.0267 | 7.9470 | 2.0502 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_1000000_extreme_skew | 0.4697 | 0.4595 | 0.4772 | 7.2351 | 0.9902 | unsupported / budget | unsupported / budget | v011 |
| synthetic_1000000_mixture | 15.9766 | 16.1814 | 16.7920 | 18.7059 | 26.0741 | unsupported / budget | unsupported / budget | candidate |
| synthetic_10000000_uniform | 190.7635 | 192.2811 | 213.1320 | 249.0421 | 220.8360 | unsupported / budget | unsupported / budget | candidate |
| synthetic_10000000_normal | 190.7672 | 192.5935 | 221.8546 | 232.5003 | 214.3177 | unsupported / budget | unsupported / budget | candidate |
| synthetic_10000000_exponential | 197.0722 | 199.1982 | 215.8288 | 247.3563 | 220.8933 | unsupported / budget | unsupported / budget | candidate |
| synthetic_10000000_narrow | 76.7022 | 76.4281 | 105.5787 | 178.7667 | 65.3558 | 33.8852 | 32.3991 | counting |
| synthetic_10000000_duplicates | 10.7661 | 10.9597 | 31.5060 | 45.5464 | 8.0420 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_10000000_duplicates_narrow | 7.6294 | 7.4724 | 30.9545 | 39.6822 | 8.0483 | 7.9823 | 31.7659 | v011 |
| synthetic_10000000_sorted | 5.2208 | 5.3891 | 5.1719 | 3.1473 | 2.2770 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_10000000_partial | 69.6019 | 67.0653 | 68.7202 | 97.5996 | 69.7370 | unsupported / budget | unsupported / budget | v011 |
| synthetic_10000000_reverse | 10.4900 | 11.0393 | 10.4688 | 90.2069 | 4.0053 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_10000000_upper_only | 10.7250 | 10.9390 | 73.4319 | 181.7183 | 14.7887 | unsupported / budget | unsupported / budget | candidate |
| synthetic_10000000_lower_only | 152.0594 | 153.7631 | 183.5806 | 173.7640 | 180.1074 | unsupported / budget | 62.2251 | counting |
| synthetic_10000000_discontinuous | 76.5843 | 75.9899 | 75.2425 | 93.5486 | 17.5635 | unsupported / budget | unsupported / budget | CAFS_u32 |
| synthetic_10000000_extreme_skew | 5.9245 | 5.6751 | 5.6498 | 77.9013 | 12.7934 | unsupported / budget | unsupported / budget | pdq |
| synthetic_10000000_mixture | 174.0394 | 174.6595 | 171.1538 | 179.1533 | 300.1591 | unsupported / budget | unsupported / budget | pdq |

## Competitive aggregates and all losses

```json
{
  "real": {
    "candidate": {
      "eligible": 9,
      "gm_speedup_pdq": 1.7884126250252155
    },
    "v011": {
      "eligible": 9,
      "gm_speedup_pdq": 1.4917981998109044
    },
    "pdq": {
      "eligible": 9,
      "gm_speedup_pdq": 1.0
    },
    "spread": {
      "eligible": 9,
      "gm_speedup_pdq": 0.7157368329077274
    },
    "CAFS_u32": {
      "eligible": 9,
      "gm_speedup_pdq": 1.7803059163660013
    },
    "CAFS_i32": {
      "eligible": 9,
      "gm_speedup_pdq": 1.828638043975413
    },
    "counting": {
      "eligible": 9,
      "gm_speedup_pdq": 0.6934627309248247
    },
    "candidate_winners": [
      "covtype_cover",
      "census_industry",
      "census_sex"
    ],
    "candidate_clear_best": [
      "covtype_cover",
      "census_industry",
      "census_sex"
    ],
    "candidate_near_best": [
      "nasa_status",
      "covtype_cover",
      "covtype_elevation",
      "census_industry",
      "census_sex"
    ],
    "candidate_vs_v011": {
      "n": 9,
      "gm_time_ratio": 0.8383554856462364,
      "baseline_gm_time_ratio": 1.0,
      "wins": 3,
      "ties": 6,
      "losses": 0,
      "paired_gm_speedup": 1.2052723159111731
    },
    "candidate_vs_pdq": {
      "n": 9,
      "gm_time_ratio": 0.5591550775291024,
      "baseline_gm_time_ratio": 0.6703319524897917,
      "wins": 5,
      "ties": 3,
      "losses": 1,
      "paired_gm_speedup": 1.768333853903988
    },
    "candidate_vs_spread": {
      "n": 9,
      "gm_time_ratio": 0.39683602530319845,
      "baseline_gm_time_ratio": 0.4741084262343228,
      "wins": 7,
      "ties": 0,
      "losses": 2,
      "paired_gm_speedup": 2.47041877134741
    },
    "candidate_vs_CAFS_u32": {
      "n": 9,
      "gm_time_ratio": 0.9841577885281886,
      "baseline_gm_time_ratio": 1.1790265176477803,
      "wins": 5,
      "ties": 0,
      "losses": 4,
      "paired_gm_speedup": 1.0253247002803576
    },
    "candidate_vs_CAFS_i32": {
      "n": 9,
      "gm_time_ratio": 1.0261152221779233,
      "baseline_gm_time_ratio": 1.2227719441617317,
      "wins": 5,
      "ties": 0,
      "losses": 4,
      "paired_gm_speedup": 0.9593617419114171
    },
    "candidate_vs_counting": {
      "n": 9,
      "gm_time_ratio": 0.38692329874081083,
      "baseline_gm_time_ratio": 0.45979983810288355,
      "wins": 8,
      "ties": 0,
      "losses": 1,
      "paired_gm_speedup": 2.5782188067145
    },
    "worst_vs_v011": {
      "name": "nasa_status",
      "ratio": 1.024950226627695,
      "candidate_ms": 2.4196,
      "other_ms": 2.3607
    },
    "worst_vs_pdq": {
      "name": "nasa_bytes",
      "ratio": 1.0683928154886868,
      "candidate_ms": 12.761299999999999,
      "other_ms": 12.3126
    },
    "worst_vs_spread": {
      "name": "nasa_timestamp",
      "ratio": 2.2386043427813695,
      "candidate_ms": 1.3505500000000001,
      "other_ms": 0.6033
    },
    "worst_vs_CAFS_u32": {
      "name": "nasa_timestamp",
      "ratio": 3.1123674911660775,
      "candidate_ms": 1.3505500000000001,
      "other_ms": 0.4305
    },
    "worst_vs_CAFS_i32": {
      "name": "nasa_timestamp",
      "ratio": 3.0774752193232318,
      "candidate_ms": 1.3505500000000001,
      "other_ms": 0.43884999999999996
    },
    "worst_vs_counting": {
      "name": "covtype_road_distance",
      "ratio": 4.511879856545129,
      "candidate_ms": 3.01935,
      "other_ms": 0.67745
    }
  },
  "synthetic": {
    "candidate": {
      "eligible": 42,
      "gm_speedup_pdq": 1.4322021288188875
    },
    "v011": {
      "eligible": 42,
      "gm_speedup_pdq": 1.4262983491651524
    },
    "pdq": {
      "eligible": 42,
      "gm_speedup_pdq": 1.0
    },
    "spread": {
      "eligible": 42,
      "gm_speedup_pdq": 0.6362453692522853
    },
    "CAFS_u32": {
      "eligible": 42,
      "gm_speedup_pdq": 1.529271726262873
    },
    "CAFS_i32": {
      "eligible": 6,
      "gm_speedup_pdq": 2.332144202141066
    },
    "counting": {
      "eligible": 9,
      "gm_speedup_pdq": 1.5087204129411038
    },
    "candidate_winners": [
      "synthetic_100000_exponential",
      "synthetic_100000_duplicates_narrow",
      "synthetic_1000000_exponential",
      "synthetic_1000000_duplicates_narrow",
      "synthetic_1000000_upper_only",
      "synthetic_1000000_mixture",
      "synthetic_10000000_uniform",
      "synthetic_10000000_normal",
      "synthetic_10000000_exponential",
      "synthetic_10000000_upper_only"
    ],
    "candidate_clear_best": [],
    "candidate_near_best": [
      "synthetic_100000_uniform",
      "synthetic_100000_normal",
      "synthetic_100000_exponential",
      "synthetic_100000_duplicates_narrow",
      "synthetic_100000_upper_only",
      "synthetic_100000_lower_only",
      "synthetic_100000_extreme_skew",
      "synthetic_100000_mixture",
      "synthetic_1000000_uniform",
      "synthetic_1000000_normal",
      "synthetic_1000000_exponential",
      "synthetic_1000000_duplicates_narrow",
      "synthetic_1000000_partial",
      "synthetic_1000000_upper_only",
      "synthetic_1000000_extreme_skew",
      "synthetic_1000000_mixture",
      "synthetic_10000000_uniform",
      "synthetic_10000000_normal",
      "synthetic_10000000_exponential",
      "synthetic_10000000_duplicates_narrow",
      "synthetic_10000000_partial",
      "synthetic_10000000_upper_only",
      "synthetic_10000000_extreme_skew",
      "synthetic_10000000_mixture"
    ],
    "candidate_vs_v011": {
      "n": 42,
      "gm_time_ratio": 0.9978554892571203,
      "baseline_gm_time_ratio": 1.0,
      "wins": 1,
      "ties": 41,
      "losses": 0,
      "paired_gm_speedup": 0.9979970949462197
    },
    "candidate_vs_pdq": {
      "n": 42,
      "gm_time_ratio": 0.6982254668373401,
      "baseline_gm_time_ratio": 0.7011155839767498,
      "wins": 27,
      "ties": 13,
      "losses": 2,
      "paired_gm_speedup": 1.427481121950477
    },
    "candidate_vs_spread": {
      "n": 42,
      "gm_time_ratio": 0.44486293730354093,
      "baseline_gm_time_ratio": 0.4475797196898898,
      "wins": 35,
      "ties": 1,
      "losses": 6,
      "paired_gm_speedup": 2.236208913331712
    },
    "candidate_vs_CAFS_u32": {
      "n": 42,
      "gm_time_ratio": 1.0682443755560265,
      "baseline_gm_time_ratio": 1.074646774949505,
      "wins": 25,
      "ties": 4,
      "losses": 13,
      "paired_gm_speedup": 0.9269023268583485
    },
    "candidate_vs_CAFS_i32": {
      "n": 6,
      "gm_time_ratio": 1.0130244473205352,
      "baseline_gm_time_ratio": 1.0156556444991618,
      "wins": 3,
      "ties": 1,
      "losses": 2,
      "paired_gm_speedup": 0.9780063027315616
    },
    "candidate_vs_counting": {
      "n": 9,
      "gm_time_ratio": 0.8142416167672529,
      "baseline_gm_time_ratio": 0.8202651180278612,
      "wins": 4,
      "ties": 0,
      "losses": 5,
      "paired_gm_speedup": 1.2123696549346035
    },
    "worst_vs_v011": {
      "name": "synthetic_10000000_extreme_skew",
      "ratio": 1.0313789078857103,
      "candidate_ms": 5.9245,
      "other_ms": 5.6751000000000005
    },
    "worst_vs_pdq": {
      "name": "synthetic_1000000_discontinuous",
      "ratio": 1.127805497935004,
      "candidate_ms": 8.76025,
      "other_ms": 8.02665
    },
    "worst_vs_spread": {
      "name": "synthetic_10000000_sorted",
      "ratio": 1.658845023433156,
      "candidate_ms": 5.2208000000000006,
      "other_ms": 3.14725
    },
    "worst_vs_CAFS_u32": {
      "name": "synthetic_10000000_discontinuous",
      "ratio": 4.374170865715832,
      "candidate_ms": 76.58430000000001,
      "other_ms": 17.563499999999998
    },
    "worst_vs_CAFS_i32": {
      "name": "synthetic_10000000_narrow",
      "ratio": 2.2635948195596,
      "candidate_ms": 76.70224999999999,
      "other_ms": 33.88515
    },
    "worst_vs_counting": {
      "name": "synthetic_1000000_narrow",
      "ratio": 3.373528103218152,
      "candidate_ms": 11.373850000000001,
      "other_ms": 3.39615
    }
  },
  "all": {
    "candidate": {
      "eligible": 51,
      "gm_speedup_pdq": 1.4894545998698832
    },
    "v011": {
      "eligible": 51,
      "gm_speedup_pdq": 1.4376444858562318
    },
    "pdq": {
      "eligible": 51,
      "gm_speedup_pdq": 1.0
    },
    "spread": {
      "eligible": 51,
      "gm_speedup_pdq": 0.6496019979991728
    },
    "CAFS_u32": {
      "eligible": 51,
      "gm_speedup_pdq": 1.5708455225529592
    },
    "CAFS_i32": {
      "eligible": 15,
      "gm_speedup_pdq": 2.015481474536248
    },
    "counting": {
      "eligible": 18,
      "gm_speedup_pdq": 1.022859412509934
    },
    "candidate_winners": [
      "covtype_cover",
      "census_industry",
      "census_sex",
      "synthetic_100000_exponential",
      "synthetic_100000_duplicates_narrow",
      "synthetic_1000000_exponential",
      "synthetic_1000000_duplicates_narrow",
      "synthetic_1000000_upper_only",
      "synthetic_1000000_mixture",
      "synthetic_10000000_uniform",
      "synthetic_10000000_normal",
      "synthetic_10000000_exponential",
      "synthetic_10000000_upper_only"
    ],
    "candidate_clear_best": [
      "covtype_cover",
      "census_industry",
      "census_sex"
    ],
    "candidate_near_best": [
      "nasa_status",
      "covtype_cover",
      "covtype_elevation",
      "census_industry",
      "census_sex",
      "synthetic_100000_uniform",
      "synthetic_100000_normal",
      "synthetic_100000_exponential",
      "synthetic_100000_duplicates_narrow",
      "synthetic_100000_upper_only",
      "synthetic_100000_lower_only",
      "synthetic_100000_extreme_skew",
      "synthetic_100000_mixture",
      "synthetic_1000000_uniform",
      "synthetic_1000000_normal",
      "synthetic_1000000_exponential",
      "synthetic_1000000_duplicates_narrow",
      "synthetic_1000000_partial",
      "synthetic_1000000_upper_only",
      "synthetic_1000000_extreme_skew",
      "synthetic_1000000_mixture",
      "synthetic_10000000_uniform",
      "synthetic_10000000_normal",
      "synthetic_10000000_exponential",
      "synthetic_10000000_duplicates_narrow",
      "synthetic_10000000_partial",
      "synthetic_10000000_upper_only",
      "synthetic_10000000_extreme_skew",
      "synthetic_10000000_mixture"
    ],
    "candidate_vs_v011": {
      "n": 51,
      "gm_time_ratio": 0.9676527032774024,
      "baseline_gm_time_ratio": 1.0,
      "wins": 4,
      "ties": 47,
      "losses": 0,
      "paired_gm_speedup": 1.0317918277131597
    },
    "candidate_vs_pdq": {
      "n": 51,
      "gm_time_ratio": 0.6713866942217365,
      "baseline_gm_time_ratio": 0.6955822596185317,
      "wins": 32,
      "ties": 16,
      "losses": 3,
      "paired_gm_speedup": 1.4824534312142286
    },
    "candidate_vs_spread": {
      "n": 51,
      "gm_time_ratio": 0.43598406184822114,
      "baseline_gm_time_ratio": 0.4521509506898013,
      "wins": 42,
      "ties": 1,
      "losses": 8,
      "paired_gm_speedup": 2.2758633127122385
    },
    "candidate_vs_CAFS_u32": {
      "n": 51,
      "gm_time_ratio": 1.052900238589802,
      "baseline_gm_time_ratio": 1.0923707513390264,
      "wins": 30,
      "ties": 4,
      "losses": 17,
      "paired_gm_speedup": 0.9435571888325927
    },
    "candidate_vs_CAFS_i32": {
      "n": 15,
      "gm_time_ratio": 1.0208587338992476,
      "baseline_gm_time_ratio": 1.135287546470517,
      "wins": 8,
      "ties": 1,
      "losses": 6,
      "paired_gm_speedup": 0.9667765299435873
    },
    "candidate_vs_counting": {
      "n": 18,
      "gm_time_ratio": 0.5612923056052315,
      "baseline_gm_time_ratio": 0.6141317191536789,
      "wins": 12,
      "ties": 0,
      "losses": 6,
      "paired_gm_speedup": 1.7679802728091634
    },
    "worst_vs_v011": {
      "name": "synthetic_10000000_extreme_skew",
      "ratio": 1.0313789078857103,
      "candidate_ms": 5.9245,
      "other_ms": 5.6751000000000005
    },
    "worst_vs_pdq": {
      "name": "synthetic_1000000_discontinuous",
      "ratio": 1.127805497935004,
      "candidate_ms": 8.76025,
      "other_ms": 8.02665
    },
    "worst_vs_spread": {
      "name": "nasa_timestamp",
      "ratio": 2.2386043427813695,
      "candidate_ms": 1.3505500000000001,
      "other_ms": 0.6033
    },
    "worst_vs_CAFS_u32": {
      "name": "synthetic_10000000_discontinuous",
      "ratio": 4.374170865715832,
      "candidate_ms": 76.58430000000001,
      "other_ms": 17.563499999999998
    },
    "worst_vs_CAFS_i32": {
      "name": "nasa_timestamp",
      "ratio": 3.0774752193232318,
      "candidate_ms": 1.3505500000000001,
      "other_ms": 0.43884999999999996
    },
    "worst_vs_counting": {
      "name": "covtype_road_distance",
      "ratio": 4.511879856545129,
      "candidate_ms": 3.01935,
      "other_ms": 0.67745
    }
  }
}
```

Full input-level time ratios, pdq speedups and paired block comparisons are in CSV/JSON. Raw session records preserve every measured call, with no exclusions beyond type/range eligibility. A primary time-ratio>1 means candidate is slower; pdq speedup>1 means the method is faster than pdq. Per-column ratios and group means may differ from displayed-time quotients. No production workload weighting or multi-CPU generality is implied.

## Assessment: current competitive position

Candidate speedup over pdqsort in the nine real columns is1.7884 versus1.4918 for v0.1.1. Its geometric mean time is16.16% below v0.1.1 on real data,0.21% below on synthetic data and3.23% below across all51. The direct block-paired robustness estimates support the same conclusion (real candidate-v011 speedup1.2053; all51 1.0318). These estimates are reported separately, not substituted into the main tables.

### CAFS gap, measured in this same run

Using same-process method-median time ratios, v0.1.1/CAFS_u32 real-input GM time ratio is1.1790, candidate/CAFS_u32 is0.9842: the17.9% time penalty shrinks to a nominal1.6% advantage. The block-paired candidate/CAFS comparison also places them close (candidate speedup1.0253). This is near parity evidence, not a statistically established overall win.

Against CAFS_i32 over the same nine real inputs, v0.1.1 time ratio1.2228 becomes candidate1.0261: the22.3% penalty shrinks to2.6%. Block-paired estimate still favors CAFS_i32 by about4.2% time. Candidate therefore has not established the fastest overall real-data average among all methods; CAFS_i32 remains slightly ahead.

Across all51 inputs versus supported CAFS_u32, candidate still takes5.29% more time geometrically (v0.1.1 took9.24% more). Over synthetic42 alone the gap is6.82% (previous baseline7.46%). CAFS_i32 supports only6 synthetic cases here; its subset result cannot be extrapolated to42 or51. Counting supports9 synthetic cases. No unsupported case is counted as a loss.

Historical v0.1.0 evidence used another harness/run. Do not compute an exact improvement by pooling its times with this experiment. The contemporary v0.1.1 comparison above isolates the current gap reduction under shared conditions.

### Where candidate wins

Candidate is the lowest median and more than5% ahead on input-level ratios against every tested eligible method on three real columns: Covertype cover, Census industry and Census sex. At Covertype elevation candidate0.4353ms and v0.1.1 0.4335ms are near-tied; the baseline is the nominal lowest and both remain substantially faster than the other sorters. Counting is the nearest non-ABW competitor there at0.9197ms.

Covertype cover: candidate0.3607ms, CAFS_u32 0.3917ms, CAFS_i32 0.4053ms. Census industry: candidate1.5348ms, Counting6.3044ms, CAFS_i32 6.8361ms. Census sex: candidate1.6372ms, CAFS_u32 1.7707ms, CAFS_i32 1.9250ms. Counts of winners are finite benchmark observations, not production-use frequencies. Over synthetic inputs candidate has10 lowest-median results, but none is clearly faster than every other method by the descriptive5% band (v0.1.1 often ties).

Candidate vs CAFS_u32 on real data is5 wins/0 near-ties/4 losses using the descriptive5% input time band; the same counts hold versus CAFS_i32. The remaining four losses can be large, so win count alone is not a dominance claim.

### Where losses remain

- NASA timestamp: candidate1.3506ms vs CAFS_u32 0.4305ms and Spread0.6033ms. Input-level time ratios are3.11 and2.24 respectively. It still falls back to pdqsort; new direct-output gains do not apply.
- NASA bytes: candidate12.7613ms vs CAFS_i32 7.3586ms (about1.73x displayed time); candidate retains native fallback after its analysis.
- Covertype road distance: candidate3.0194ms vs CAFS_i32 1.1728ms and Counting0.6775ms. Input-level candidate/Counting time ratio4.51. It retains mixed partition execution.
- Census caseID: candidate1.2336ms vs CAFS_i32 0.5621ms (about2.19x displayed time). Native fallback remains cheaper than forced partitioning but is not the fastest specialized sorter.
- Synthetic10M discontinuous: candidate76.5843ms vs CAFS_u32 17.5635ms, input-level ratio4.37. Narrow10M: candidate76.7022ms vs CAFS_i32 33.8852ms, ratio2.26. Full losses are in the tables/CSV.

### Regression versus v0.1.1

No input-level candidate/v0.1.1 ratio exceeds1.05 in this run. Maximum is+3.14% on synthetic10M extreme-skew (5.9245ms vs5.6751ms displayed medians); real maximum+2.50% on NASA status. Both primary method-median and direct block-paired aggregate comparisons retain the candidate's improvement when competitors are present. Candidate is still up to12.78% slower than direct pdqsort on1M discontinuous, an existing workload weakness rather than universal improvement. No new severe regression versus v0.1.1 was observed in this finite sample; no regression-free guarantee or cross-CPU claim is implied.

Conclusion: candidate now competes with CAFS on the real-data aggregate and has three clear all-comparator real-column wins. It is not the universal or overall fastest sorter: CAFS_i32 remains slightly ahead on real average, CAFS_u32 ahead across all51, and specialized losses remain substantial. The result strengthens its value as a limited-scope experimental wrapper. The tested candidate is retained unchanged; no optimization, policy change, GitHub edit or release is made.

## Reproduce

Exact candidate, public v0.1.1 snapshot, pinned CAFS headers/license, benchmark source, original runner/analyzer and source/input hashes accompany this report. Boost1.90.0, the recorded compatible compiler and original input/expected arrays are external prerequisites. Benchmark sources are under the artifact root; include roots are include/, vendor/ and Boost root. Rebuild with the recorded C++20/O3/NDEBUG/AVX2/BMI/static/pthread flags on Windows x64.

Invocation: `benchmark.exe INPUT EXPECTED SEED SIGNED_OK COUNTING_OK`. The final two eligibility arguments must match protocol.json, not be chosen by timing. Input restoration, signed conversion and result checks remain outside timers; runtime min/max and Counting counter initialization remain inside. Original run.py assumes the research-workspace paths; adapt only paths and fresh output location when reproducing, preserving methods, seeds and caps. Do not overwrite raw records or merge a new machine's times into these aggregates.
