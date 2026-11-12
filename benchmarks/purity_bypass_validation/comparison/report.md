# Single-histogram purity-bypass comparison

Compare all original seven implementations plus the public v0.1.0 header and the previously tested shift-zero single-histogram candidate, in one current run. No four-sub-histograms, dispatch changes, entropy threshold changes or uninitialized-temporary variant. The candidate file is byte-identical to the preceding quick experiment. The new fast path also avoids unused starts and representative metadata; its total gain must not be attributed to comparison instructions alone.

Same existing nine real columns and 42 synthetic input files, source order and expected arrays. Exact input/source hashes checked before and after. Same compiler/options and one binary for all methods; full-array equality per timed call. Three fresh-process sessions per input, eligible-method count Latin rotations, randomized method order and randomized input order. Original three 100k warmup families retained. Loading, input copying and signed-key preparation excluded; full sorting including analysis/allocations/output/free included. The two new public header methods have outer whole-call timers and no route instrumentation. Raw B_direct timing flag is meaningful only for frozen ABW_B; separate audits verify public-header dispatch and direct/mixed decisions. All 51 route audits passed. Historical raw files and sources are preserved.

Intel i5-14500 Windows x64, first permitted CPU affinity; LLVM-MinGW Clang 23.1.5, Boost 1.90.0, C++20/O3/NDEBUG/AVX2/BMI/static/pthread. CAFS sources, supported u32/i32 handling and the Counting sort 64MiB range budget remain identical to the historical experiment. No unsupported competitor cases included. Repeated process sessions are not independent datasets or independent hardware. No outlier filtering or statistical significance claim.

Requested sort-owned peak heap for public_v010/shift0 was measured separately, one process per input/method, with original allocation hooks; 102 extra processes. This excludes input copies, stack, allocator overhead and OS metadata; it is not RSS. All retained heap counters are zero.

Timed sorts: 9072 in 153 processes.

- real: 9 inputs; GM candidate/public time ratio 0.949625; candidate/pdq speedup 1.483792.
- synthetic: 42 inputs; GM candidate/public time ratio 0.977181; candidate/pdq speedup 1.421530.
- changed: 5 inputs; GM candidate/public time ratio 0.729464; candidate/pdq speedup 4.303011.
- unchanged: 46 inputs; GM candidate/public time ratio 1.003103; candidate/pdq speedup 1.270908.

## Real-input times (ms)

| Input | pdq | E_pdq | Frozen B | Public v0.1.0 | Candidate | Spread | CAFS u32 | CAFS i32 | Counting | Candidate/public |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| nasa_status | 2.4558 | 2.4467 | 2.4230 | 2.4181 | 2.4408 | 5.6490 | 3.7898 | 3.5124 | 4.2591 | 1.0123 |
| nasa_timestamp | 1.4526 | 1.4383 | 1.4674 | 1.4409 | 1.3741 | 0.6260 | 0.4893 | 0.4301 | 17.1468 | 0.9931 |
| nasa_bytes | 12.2945 | 12.9031 | 13.0287 | 12.8649 | 13.0123 | 17.7046 | 11.4626 | 7.5123 | 20.3593 | 1.0115 |
| covtype_cover | 0.8647 | 0.8804 | 0.8777 | 0.8787 | 0.8727 | 1.8114 | 0.4202 | 0.4278 | 1.0044 | 0.9927 |
| covtype_elevation | 4.2742 | 2.0652 | 0.6842 | 0.7080 | 0.5025 | 3.7837 | 1.2304 | 1.2588 | 0.9799 | 0.6958 |
| covtype_road_distance | 5.2170 | 3.1549 | 3.0936 | 3.1384 | 3.1341 | 5.3411 | 1.1974 | 1.1841 | 0.7328 | 0.9789 |
| census_caseid | 1.2870 | 1.2699 | 1.2476 | 1.2687 | 1.2496 | 0.8166 | 0.5681 | 0.5684 | 8.6658 | 0.9849 |
| census_industry | 7.4212 | 7.8796 | 2.3755 | 2.3478 | 2.3379 | 12.5837 | 6.8468 | 6.8192 | 6.4839 | 0.9743 |
| census_sex | 2.1778 | 2.6708 | 2.6400 | 2.6796 | 2.6035 | 12.7841 | 1.9490 | 1.8709 | 7.6007 | 0.9519 |

## All input-level candidate comparisons

| Input | Public ms | Candidate ms | Candidate/public | Session ratio range | Direct | New path | Public heap bytes | Candidate heap bytes |
|---|---:|---:|---:|---|---|---|---:|---:|
| nasa_status | 2.4181 | 2.4408 | 1.0123 | 1.0094..1.0259 | False | False | 0 | 0 |
| nasa_timestamp | 1.4409 | 1.3741 | 0.9931 | 0.9522..1.0450 | False | False | 0 | 0 |
| nasa_bytes | 12.8649 | 13.0123 | 1.0115 | 1.0038..1.0653 | False | False | 34600 | 34600 |
| covtype_cover | 0.8787 | 0.8727 | 0.9927 | 0.9834..1.0226 | False | False | 0 | 0 |
| covtype_elevation | 0.7080 | 0.5025 | 0.6958 | 0.6508..0.7220 | True | True | 81928 | 50280 |
| covtype_road_distance | 3.1384 | 3.1341 | 0.9789 | 0.8582..1.0242 | False | False | 2438752 | 2438752 |
| census_caseid | 1.2687 | 1.2496 | 0.9849 | 0.9819..0.9972 | False | False | 0 | 0 |
| census_industry | 2.3478 | 2.3379 | 0.9743 | 0.9577..0.9972 | True | True | 10840 | 10840 |
| census_sex | 2.6796 | 2.6035 | 0.9519 | 0.9497..1.1007 | False | False | 10320 | 10320 |
| synthetic_100000_uniform | 1.3566 | 1.3466 | 0.9877 | 0.9866..1.0380 | False | False | 514704 | 514704 |
| synthetic_100000_normal | 1.4776 | 1.4407 | 1.0092 | 0.9742..1.0216 | False | False | 514704 | 514704 |
| synthetic_100000_exponential | 1.4651 | 1.4077 | 0.9901 | 0.9547..0.9945 | False | False | 514704 | 514704 |
| synthetic_100000_narrow | 1.3276 | 1.2978 | 0.9743 | 0.9620..1.0103 | False | False | 514704 | 514704 |
| synthetic_100000_duplicates | 0.0898 | 0.0932 | 1.0390 | 1.0245..1.0407 | True | False | 5128 | 5128 |
| synthetic_100000_duplicates_narrow | 0.0799 | 0.0468 | 0.5958 | 0.5857..0.6042 | True | True | 5128 | 4440 |
| synthetic_100000_sorted | 0.0377 | 0.0378 | 1.0025 | 1.0000..1.0027 | False | False | 0 | 0 |
| synthetic_100000_partial | 0.4193 | 0.4266 | 1.0095 | 1.0070..1.0174 | False | False | 0 | 0 |
| synthetic_100000_reverse | 0.0788 | 0.0792 | 1.0051 | 0.9884..1.0126 | False | False | 0 | 0 |
| synthetic_100000_upper_only | 0.2048 | 0.2137 | 1.0605 | 1.0234..1.0851 | True | False | 81928 | 81928 |
| synthetic_100000_lower_only | 1.2886 | 1.2986 | 1.0135 | 0.9765..1.0138 | False | False | 514704 | 514704 |
| synthetic_100000_discontinuous | 1.0398 | 1.0398 | 1.0000 | 0.9681..1.0074 | False | False | 56584 | 56584 |
| synthetic_100000_extreme_skew | 0.0367 | 0.0366 | 0.9973 | 0.9809..0.9974 | False | False | 0 | 0 |
| synthetic_100000_mixture | 1.3220 | 1.3121 | 0.9925 | 0.9882..1.0190 | False | False | 514704 | 514704 |
| synthetic_1000000_uniform | 16.7086 | 16.3858 | 1.0127 | 0.9447..1.0157 | False | False | 4114704 | 4114704 |
| synthetic_1000000_normal | 17.7352 | 17.4409 | 0.9957 | 0.9834..1.0019 | False | False | 4114704 | 4114704 |
| synthetic_1000000_exponential | 17.4637 | 17.5042 | 1.0221 | 0.9636..1.0579 | False | False | 4114704 | 4114704 |
| synthetic_1000000_narrow | 11.8010 | 11.6355 | 0.9911 | 0.9565..0.9917 | False | False | 4114704 | 4114704 |
| synthetic_1000000_duplicates | 1.0593 | 1.0585 | 1.0524 | 0.9242..1.0841 | True | False | 10584 | 10584 |
| synthetic_1000000_duplicates_narrow | 0.9770 | 0.6564 | 0.6771 | 0.6696..0.7015 | True | True | 10584 | 10584 |
| synthetic_1000000_sorted | 0.5094 | 0.5318 | 1.0420 | 1.0049..1.0440 | False | False | 0 | 0 |
| synthetic_1000000_partial | 5.3270 | 5.7560 | 1.0424 | 1.0152..1.1019 | False | False | 0 | 0 |
| synthetic_1000000_reverse | 0.8965 | 0.9003 | 1.0182 | 0.9525..1.0201 | False | False | 0 | 0 |
| synthetic_1000000_upper_only | 1.3225 | 1.3013 | 0.9789 | 0.9766..1.0132 | True | False | 117944 | 117944 |
| synthetic_1000000_lower_only | 16.8131 | 16.8963 | 0.9993 | 0.9941..1.0102 | False | False | 4114704 | 4114704 |
| synthetic_1000000_discontinuous | 8.4306 | 8.6521 | 1.0263 | 0.9619..1.0809 | False | False | 117784 | 117784 |
| synthetic_1000000_extreme_skew | 0.5244 | 0.5227 | 0.9937 | 0.9124..0.9968 | False | False | 0 | 0 |
| synthetic_1000000_mixture | 15.8129 | 15.8663 | 0.9877 | 0.9724..1.0034 | False | False | 4114704 | 4114704 |
| synthetic_10000000_uniform | 187.8612 | 189.6307 | 0.9889 | 0.9832..1.0094 | False | False | 40114704 | 40114704 |
| synthetic_10000000_normal | 196.7289 | 196.0831 | 1.0034 | 0.9967..1.0048 | False | False | 40114704 | 40114704 |
| synthetic_10000000_exponential | 196.6749 | 198.1602 | 0.9717 | 0.9644..1.0217 | False | False | 40114704 | 40114704 |
| synthetic_10000000_narrow | 79.2028 | 77.3271 | 0.9702 | 0.9628..0.9763 | False | False | 40114704 | 40114704 |
| synthetic_10000000_duplicates | 10.8597 | 10.8789 | 1.0034 | 0.9841..1.0150 | True | False | 10584 | 10584 |
| synthetic_10000000_duplicates_narrow | 10.7125 | 7.9960 | 0.7553 | 0.7268..0.7778 | True | True | 10584 | 10584 |
| synthetic_10000000_sorted | 5.7016 | 5.5901 | 0.9628 | 0.9614..1.0743 | False | False | 0 | 0 |
| synthetic_10000000_partial | 70.8306 | 69.5675 | 1.0289 | 0.9779..1.0317 | False | False | 0 | 0 |
| synthetic_10000000_reverse | 10.3069 | 10.6304 | 1.0238 | 0.9787..1.0314 | False | False | 0 | 0 |
| synthetic_10000000_upper_only | 11.1955 | 11.1154 | 0.9928 | 0.9854..1.0076 | True | False | 117784 | 117784 |
| synthetic_10000000_lower_only | 153.3029 | 153.0231 | 0.9944 | 0.9935..1.0359 | False | False | 40114704 | 40114704 |
| synthetic_10000000_discontinuous | 80.3022 | 78.9527 | 0.9949 | 0.9755..0.9954 | False | False | 118232 | 118232 |
| synthetic_10000000_extreme_skew | 5.8489 | 5.9013 | 1.0566 | 0.9287..1.0571 | False | False | 0 | 0 |
| synthetic_10000000_mixture | 178.6013 | 178.2269 | 0.9947 | 0.9716..1.0463 | False | False | 40114704 | 40114704 |

All times are medians of process medians. Ratios are medians of paired process ratios and may differ from displayed-time quotients. Geometric means use those input-level ratios; competitor subset sizes are in summary.json. These observations do not expand prior one-CPU performance claims. Raw sessions, separate memory records, route audits, protocol hashes and sources accompany the report. No public code or release is updated automatically.

## Assessment and focused follow-up

The primary run reproduces a substantial Covertype elevation benefit (median paired candidate/public time ratio 0.6958) and a smaller Census industry benefit (0.9743). All five changed-path inputs improved in input-level median ratio; the other 46 inputs have geometric mean ratio 1.0031. The original native-fallback and mixed-path weaknesses remain: real-input geometric mean candidate speedup over CAFS_i32 is only 0.7834, and over CAFS_u32 only 0.8350. This remains a limited-scope candidate.

Peak requested heap on Covertype elevation decreases from 81,928 to 50,280 bytes. Census industry remains 10,840 bytes because peak analysis allocation dominates the smaller execution-phase histogram; avoiding execution metadata does not guarantee a lower whole-call peak.

The largest primary input-level candidate/public regression is 6.05% on synthetic 100k upper-only. It uses shift=20 and does not enter the new shift-zero path. This input and Census industry were followed up in five new processes each with unchanged binary, methods and interleaving. Raw follow-up records are retained separately and never pooled with primary results. Upper-only follow-up ratios were 1.0015, 0.9710, 0.9637, 0.9810, 0.9916 (median 0.9810): the regression did not reproduce in these processes. Census industry ratios were 0.9573, 0.9599, 0.9519, 0.9528, 0.9102 (median 0.9528). Exact causes of process variation or code layout effects are not established; no outlier was removed and no further optimization was added.

Decision: retain the single-histogram shortcut as a promising follow-up implementation, with unchanged policy. Evidence is sufficient to justify a compatibility/CI review for a later version; this experiment does not itself update v0.1.0 or establish multi-CPU generality. No histogram-four variant is adopted. Existing regressions versus pdqsort and specialized competitors remain disclosed.
