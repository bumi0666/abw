# Full51 paired release-candidate check

Date: 2026-11-13. The candidate, public v0.1.1 and paired benchmark executable are unchanged from the preceding 13-input check, verified byte-for-byte by SHA-256 before execution. The executable was copied, not rebuilt. Only the input list expands to all original nine real and 42 synthetic inputs. No tuning, new optimization, GitHub edit or release.

Measurement remains the exact same fixed-buffer, common noinline function-pointer call site, eight alternating ABBA/BAAB blocks for candidate comparison plus eight exact same-function A/A_alias control blocks in each process. Candidate/control block order is randomized. One original-value reset into the same scratch data pointer precedes every call, outside timing. Full-array comparison with unchanged expected files follows every call, outside timing. Sort-owned analysis, allocation, zeroing, merge, sort/output and free remain inside. Original warmup families and actual-input warmup retained. All control function pointers are identical and each scratch address is checked stable.

51 inputs x3 fresh processes x64 timed calls = **9,792 timed calls**, consisting of 4,896 candidate-comparison calls (2,448 candidate and 2,448 baseline) plus 4,896 identical-function control calls. These repeated calls are not 9,792 independent datasets. Input and expected hashes are checked before/after; source and binary hashes before/after. All calls pass exact output correctness. Same Intel i5-14500 Windows x64 first-permitted-CPU affinity; LLVM-MinGW Clang23.1.5, Boost1.90.0, C++20/O3/NDEBUG/AVX2/BMI/static/pthread. No memory profiling or offset sweeps.

Estimator: average the two candidate and two baseline times in each ABBA block, take their ratio, median eight block ratios per process, then median the three process ratios per input. Group geometric means use these input-level ratios. Times below are medians of process medians; time quotients may differ from paired ratios. No outlier filtering. Process min/max are descriptive ranges, not confidence intervals. The existing 5% descriptive column is a reporting label, not a requirement that every process must win. One performance CPU and one existing synthetic data seed remain limitations.

## Aggregates

```json
{
  "real": {
    "inputs": 9,
    "gm_candidate_over_v011": 0.8582617862233598,
    "gm_control": 0.9859571536478144,
    "worst": {
      "name": "covtype_road_distance",
      "group": "real",
      "n": 581012,
      "baseline_route": 3,
      "candidate_route": 3,
      "baseline_bits": 12,
      "candidate_bits": 12,
      "baseline_shift": 1,
      "candidate_shift": 1,
      "choice_changed": false,
      "candidate_ratio": 1.0572746793817345,
      "candidate_delta_ms": 0.1777000000000002,
      "candidate_A_ms": 3.049,
      "candidate_other_ms": 3.05465,
      "candidate_process_min": 0.9813638402683075,
      "candidate_process_max": 1.0807750511629668,
      "control_ratio": 0.9940827509935236,
      "control_delta_ms": -0.01782499999999998,
      "control_A_ms": 3.1066000000000003,
      "control_other_ms": 3.0406,
      "control_process_min": 0.9560024868658683,
      "control_process_max": 1.0394401239375521,
      "all_three_over_5pct": false
    },
    "best": {
      "name": "covtype_cover",
      "ratio": 0.440026643919166
    },
    "regressions_over_5pct": [
      "covtype_road_distance"
    ],
    "all_three_over_5pct": []
  },
  "synthetic": {
    "inputs": 42,
    "gm_candidate_over_v011": 0.9972225879277961,
    "gm_control": 0.9979570996334028,
    "worst": {
      "name": "synthetic_100000_upper_only",
      "group": "synthetic_regression",
      "n": 100000,
      "baseline_route": 3,
      "candidate_route": 3,
      "baseline_bits": 12,
      "candidate_bits": 12,
      "baseline_shift": 20,
      "candidate_shift": 20,
      "choice_changed": false,
      "candidate_ratio": 1.0369906798133437,
      "candidate_delta_ms": 0.006274999999999989,
      "candidate_A_ms": 0.16165000000000002,
      "candidate_other_ms": 0.1679,
      "candidate_process_min": 1.0324112742529632,
      "candidate_process_max": 1.0472005208333335,
      "control_ratio": 1.0013431122788106,
      "control_delta_ms": 0.00019999999999999185,
      "control_A_ms": 0.1594,
      "control_other_ms": 0.1594,
      "control_process_min": 0.9976675764124732,
      "control_process_max": 1.010036923076923,
      "all_three_over_5pct": false
    },
    "best": {
      "name": "synthetic_10000000_sorted",
      "ratio": 0.9568756279213273
    },
    "regressions_over_5pct": [],
    "all_three_over_5pct": []
  },
  "all": {
    "inputs": 51,
    "gm_candidate_over_v011": 0.9711607179556777,
    "gm_control": 0.9958288998216174,
    "worst": {
      "name": "covtype_road_distance",
      "group": "real",
      "n": 581012,
      "baseline_route": 3,
      "candidate_route": 3,
      "baseline_bits": 12,
      "candidate_bits": 12,
      "baseline_shift": 1,
      "candidate_shift": 1,
      "choice_changed": false,
      "candidate_ratio": 1.0572746793817345,
      "candidate_delta_ms": 0.1777000000000002,
      "candidate_A_ms": 3.049,
      "candidate_other_ms": 3.05465,
      "candidate_process_min": 0.9813638402683075,
      "candidate_process_max": 1.0807750511629668,
      "control_ratio": 0.9940827509935236,
      "control_delta_ms": -0.01782499999999998,
      "control_A_ms": 3.1066000000000003,
      "control_other_ms": 3.0406,
      "control_process_min": 0.9560024868658683,
      "control_process_max": 1.0394401239375521,
      "all_three_over_5pct": false
    },
    "best": {
      "name": "covtype_cover",
      "ratio": 0.440026643919166
    },
    "regressions_over_5pct": [
      "covtype_road_distance"
    ],
    "all_three_over_5pct": []
  }
}
```

## Every input

| Input | v011 ms | Candidate ms | Candidate/v011 | Additional ms | Process ratio range | Exact control range | Routes A/C |
|---|---:|---:|---:|---:|---|---|---|
| census_caseid | 1.2183 | 1.2105 | 1.0187 | +0.0240 | 1.0107..1.0400 | 0.8834..1.0183 | 0/0 |
| census_industry | 2.2427 | 1.5821 | 0.7758 | -0.4973 | 0.7078..0.7827 | 0.8063..0.9878 | 2/2 |
| census_sex | 2.4756 | 1.6155 | 0.6603 | -0.8745 | 0.6383..0.6690 | 1.0030..1.0272 | 0/2 |
| covtype_cover | 0.8392 | 0.3664 | 0.4400 | -0.4692 | 0.4335..0.4411 | 0.9236..1.0084 | 0/2 |
| covtype_elevation | 0.4244 | 0.4269 | 1.0240 | +0.0100 | 1.0050..1.0333 | 0.9880..1.0142 | 3/3 |
| covtype_road_distance | 3.0490 | 3.0547 | 1.0573 | +0.1777 | 0.9814..1.0808 | 0.9560..1.0394 | 3/3 |
| nasa_bytes | 12.6209 | 12.5574 | 0.9927 | -0.0869 | 0.9738..1.0016 | 0.9872..1.0145 | 0/0 |
| nasa_status | 2.3678 | 2.3661 | 0.9889 | -0.0269 | 0.9675..1.0369 | 0.9627..1.0462 | 0/0 |
| nasa_timestamp | 1.2886 | 1.3302 | 1.0355 | +0.0473 | 0.9949..1.0370 | 0.9095..1.0326 | 0/0 |
| synthetic_10000000_discontinuous | 77.5892 | 77.9173 | 0.9970 | -0.2492 | 0.9941..0.9971 | 0.9797..0.9930 | 0/0 |
| synthetic_10000000_duplicates | 10.4727 | 10.4170 | 1.0080 | +0.0780 | 1.0016..1.0659 | 0.9642..1.0061 | 2/2 |
| synthetic_10000000_duplicates_narrow | 7.7079 | 7.6879 | 0.9904 | -0.0716 | 0.9736..1.0112 | 0.9661..1.0325 | 2/2 |
| synthetic_10000000_exponential | 193.5206 | 191.9339 | 0.9919 | -1.5332 | 0.9852..1.0039 | 0.9820..1.0068 | 3/3 |
| synthetic_10000000_extreme_skew | 5.5448 | 5.5096 | 0.9603 | -0.2421 | 0.9393..1.0282 | 0.9198..1.0626 | 0/0 |
| synthetic_10000000_lower_only | 149.2055 | 148.6312 | 0.9928 | -1.1367 | 0.9873..0.9958 | 0.9632..1.0064 | 3/3 |
| synthetic_10000000_mixture | 172.4400 | 169.4393 | 1.0026 | +0.4004 | 0.9841..1.0117 | 0.9969..1.0211 | 3/3 |
| synthetic_10000000_narrow | 78.5627 | 76.5472 | 0.9890 | -0.8091 | 0.9810..1.0003 | 0.9678..1.0189 | 3/3 |
| synthetic_10000000_normal | 201.7882 | 204.4074 | 0.9963 | -0.7300 | 0.9738..1.0176 | 0.9940..1.0187 | 3/3 |
| synthetic_10000000_partial | 63.6653 | 63.0793 | 1.0077 | +0.4866 | 0.9713..1.0152 | 0.9343..1.0171 | 0/0 |
| synthetic_10000000_reverse | 11.1101 | 10.7555 | 0.9763 | -0.2497 | 0.9690..0.9886 | 0.9713..1.1100 | 0/0 |
| synthetic_10000000_sorted | 5.2626 | 5.2542 | 0.9569 | -0.2323 | 0.9532..1.0031 | 0.9712..1.0808 | 0/0 |
| synthetic_10000000_uniform | 184.3711 | 183.6214 | 0.9842 | -2.8614 | 0.9821..0.9911 | 0.9972..1.0110 | 3/3 |
| synthetic_10000000_upper_only | 10.6840 | 10.3566 | 1.0064 | +0.0648 | 0.9889..1.0247 | 0.9779..1.0089 | 3/3 |
| synthetic_1000000_discontinuous | 8.8627 | 8.5425 | 0.9979 | -0.0171 | 0.9923..1.0078 | 0.9679..0.9975 | 0/0 |
| synthetic_1000000_duplicates | 0.9652 | 0.9858 | 1.0142 | +0.0136 | 0.9957..1.0267 | 0.9908..1.0315 | 2/2 |
| synthetic_1000000_duplicates_narrow | 0.6021 | 0.5958 | 0.9834 | -0.0100 | 0.9737..0.9841 | 0.9881..1.0195 | 2/2 |
| synthetic_1000000_exponential | 16.7391 | 16.6828 | 1.0059 | +0.0973 | 0.9312..1.0059 | 0.9874..1.0034 | 3/3 |
| synthetic_1000000_extreme_skew | 0.4315 | 0.4385 | 1.0018 | +0.0008 | 1.0006..1.0245 | 0.9970..1.0051 | 0/0 |
| synthetic_1000000_lower_only | 17.1656 | 17.2493 | 0.9980 | -0.0324 | 0.9878..1.0055 | 0.9933..1.0274 | 3/3 |
| synthetic_1000000_mixture | 15.9573 | 15.3705 | 0.9942 | -0.0873 | 0.9772..0.9985 | 0.9688..1.0028 | 3/3 |
| synthetic_1000000_narrow | 11.2465 | 11.4032 | 1.0141 | +0.1543 | 0.9873..1.0398 | 0.9940..1.0482 | 3/3 |
| synthetic_1000000_normal | 17.3183 | 17.4732 | 1.0012 | +0.0196 | 0.9893..1.0023 | 0.9597..1.0107 | 3/3 |
| synthetic_1000000_partial | 5.4055 | 5.3204 | 1.0052 | +0.0273 | 0.9711..1.0070 | 0.9883..0.9999 | 0/0 |
| synthetic_1000000_reverse | 0.8132 | 0.8163 | 1.0018 | +0.0014 | 0.9968..1.0134 | 0.9898..1.0067 | 0/0 |
| synthetic_1000000_sorted | 0.4416 | 0.4452 | 1.0061 | +0.0028 | 1.0032..1.0099 | 0.9975..1.0913 | 0/0 |
| synthetic_1000000_uniform | 16.6380 | 17.1534 | 0.9929 | -0.1170 | 0.9706..1.0439 | 0.9624..1.0021 | 3/3 |
| synthetic_1000000_upper_only | 1.2805 | 1.2823 | 1.0050 | +0.0052 | 0.9028..1.0119 | 0.9959..1.0040 | 3/3 |
| synthetic_100000_discontinuous | 1.0699 | 1.0545 | 0.9925 | -0.0077 | 0.9900..1.0124 | 0.9830..0.9926 | 0/0 |
| synthetic_100000_duplicates | 0.0799 | 0.0809 | 1.0102 | +0.0008 | 1.0072..1.0235 | 0.9900..0.9982 | 2/2 |
| synthetic_100000_duplicates_narrow | 0.0445 | 0.0432 | 0.9657 | -0.0015 | 0.9596..0.9697 | 0.9989..1.0022 | 2/2 |
| synthetic_100000_exponential | 1.4311 | 1.4211 | 0.9881 | -0.0168 | 0.9652..0.9994 | 1.0033..1.0139 | 3/3 |
| synthetic_100000_extreme_skew | 0.0379 | 0.0382 | 1.0080 | +0.0003 | 1.0066..1.0086 | 0.9940..1.0000 | 0/0 |
| synthetic_100000_lower_only | 1.3236 | 1.3188 | 0.9976 | -0.0032 | 0.9189..1.0256 | 0.9809..1.0146 | 3/3 |
| synthetic_100000_mixture | 1.3388 | 1.3348 | 0.9997 | -0.0005 | 0.9898..1.0112 | 0.9789..1.0300 | 3/3 |
| synthetic_100000_narrow | 1.2893 | 1.2931 | 1.0181 | +0.0229 | 0.9825..1.0182 | 0.9974..1.0064 | 3/3 |
| synthetic_100000_normal | 1.4357 | 1.4282 | 0.9953 | -0.0067 | 0.9680..0.9984 | 0.9969..1.0099 | 3/3 |
| synthetic_100000_partial | 0.4178 | 0.4200 | 0.9911 | -0.0039 | 0.9861..0.9981 | 0.9842..1.0128 | 0/0 |
| synthetic_100000_reverse | 0.0789 | 0.0793 | 1.0038 | +0.0003 | 0.9840..1.0051 | 0.9972..0.9994 | 0/0 |
| synthetic_100000_sorted | 0.0333 | 0.0335 | 1.0072 | +0.0003 | 1.0052..1.0080 | 0.9948..1.0023 | 0/0 |
| synthetic_100000_uniform | 1.2940 | 1.3054 | 0.9919 | -0.0106 | 0.9875..1.0023 | 0.9955..0.9991 | 3/3 |
| synthetic_100000_upper_only | 0.1617 | 0.1679 | 1.0370 | +0.0063 | 1.0324..1.0472 | 0.9977..1.0100 | 3/3 |

Routes:0=native,1=skip,2=active8,3=active12. Raw session0..2.jsonl retains all times/addresses; block_pairs.csv, process_summary.csv and input_results.csv provide every aggregation level. No measurement from earlier runs is substituted or pooled.

## Findings and adoption recommendation

The three target gains persist in every process:

- Census industry: paired process ratios0.70783..0.78273, input median0.77580 (22.42% less time).
- Census sex:0.63828..0.66898, input median0.66027 (33.97% less time).
- Covertype cover:0.43347..0.44108, input median0.44003 (56.00% less time).

Covertype elevation retains one histogram; paired time ratio1.02398 (+2.40%, +0.00998ms), process range1.00497..1.03328. NASA status and Census caseID remain native, preserving protection against the earlier large forced-partition losses.

Real9 geometric mean time ratio is0.858262 (-14.17%); synthetic42 is0.997223 (-0.28%); all51 is0.971161 (-2.88%). The former +1.1% synthetic average regression is not reproduced in this full paired run. The -0.28% synthetic change is too small to emphasize as an independent improvement, particularly with the exact-function control geometric mean0.997957 in that group. Treat synthetic average as approximately unchanged on this machine. The aggregate remains an equal-input benchmark summary, not an assumed production benefit.

Largest input-level candidate regression is Covertype road distance: ratio1.057275 (+5.73%), paired added time+0.1777ms; process range0.98136..1.08078. It retains the same mixed12-bit path. The regression is disclosed, not filtered, and its exact cause is not established. It is the only input whose input-level median ratio exceeds1.05. None exceeds1.05 in all three processes; this is a descriptive observation, not an adoption requirement. Largest synthetic input-level regression is100k upper-only at+3.70%, +0.00628ms, process range1.03241..1.04720.

The identical-function controls still fluctuate. Census industry's control ratios include0.80626 in one process; one must not claim that exact-function differences never reach20% merely because the earlier13-input check had a narrower range. This limits precision of individual percentage estimates. Candidate industrial-code ratios nevertheless favor it in all three processes, while sex/cover gains are large and repeat. All raw control/candidate records are retained. No offset sweeps or additional variants were added; no causal cache/alignment attribution is made.

Recommendation: **adopt this unchanged candidate as the v0.1.2 implementation candidate**. The large repeated gains in three real inputs, approximately unchanged synthetic average and bounded observed regression support the tradeoff. This is a recommendation for the limited-scope experimental project, not a universal improvement or regression-free guarantee. Keep the existing v0.1.1 release until public-header integration and existing correctness/CI checks are completed under a separate user instruction. No new optimization, threshold or rule is needed for that integration. GitHub/public releases remain untouched in this task.

## Reproduction

The executable used for this run is byte-identical to the preceding13-input paired check. The original runner and build command are supplied with source hashes. An exact public v0.1.1 header snapshot is under include/; candidate.hpp is unchanged. To rebuild on Windows x64, use Boost1.90.0 and recorded Clang options, include include/ and the Boost root, compile benchmark.cpp, then pass INPUT EXPECTED SEED. Existing original input/expected arrays must match protocol.json hashes. Rebuilds are new binaries and should be tracked by their own hashes. The original run.py expects the research workspace layout; change only paths/output directory to reproduce elsewhere and never overwrite the supplied raw records.
