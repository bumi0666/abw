# Full ABW versus AVX2: same-run comparison

The public ABW backend is **Boost pdqsort**. Other backend experiments use
separate copies; the public API does not select an arbitrary backend.
ABW's benefit depends on both the input and its backend.


- [Full input tables and phase diagnosis](full_backend_report.md)
- [Input CSV](full_input_results.csv)
- [Phase costs CSV](frontend_phase_costs.csv)
- [Completion checks](completion.json)
- [Sources and raw reproduction archive](abw_full_backend_comparison_results.zip)

| Group | N | Full ABW/pdqsort time | Full ABW/AVX2 time | Full ABW/skip-only pdq time |
|---|---:|---:|---:|---:|
| Original real9 | 9 | 0.53578 | 0.88528 | 0.93778 |
| New real9 | 9 | 0.66335 | 1.41225 | 0.95275 |
| Real18 | 18 | 0.59616 | 1.11814 | 0.94523 |
| Synthetic42 | 42 | 0.67069 | 1.43324 | 0.90892 |
| IPv4_6 | 6 | 1.27012 | 4.58496 | 1.06896 |
| All66 | 66 | 0.68830 | 1.48875 | 0.93233 |
| Mixed27 | 27 | 0.90705 | 3.96780 | 0.84264 |

Ratios below1 mean full ABW is faster. All66 full ABW time is31.17% lower than
pdqsort,48.88% higher than AVX2, and6.77% lower than skip-only pdq. Mixed27 is
15.74% faster than skip-only pdq; IPv4_6 is6.90% slower. These are direct paired
measurements, not differences between historical experiments.

All18 real columns are listed below, including losses. Full66 tables and raw
process ratios are in the linked report. Primary ratios are medians of paired
process ratios; displayed times are medians of process medians.

| Real input | Full path | AVX2 ms | Full ABW ms | Full/AVX2 | Processes faster /3 |
|---|---|---:|---:|---:|---:|
| nasa_status | native/skip | 0.79660 | 2.30305 | 3.07050 | 0 |
| nasa_timestamp | native/skip | 6.90860 | 1.26680 | 0.18337 | 3 |
| nasa_bytes | native/skip | 3.38665 | 12.49195 | 3.69854 | 0 |
| covtype_cover | direct | 0.27530 | 0.34530 | 1.23568 | 0 |
| covtype_elevation | direct | 1.22435 | 0.33445 | 0.27277 | 3 |
| covtype_road_distance | mixed | 1.76455 | 2.94910 | 1.67130 | 0 |
| census_caseid | native/skip | 8.91145 | 1.17780 | 0.13217 | 3 |
| census_industry | direct | 1.81505 | 1.55810 | 0.87067 | 2 |
| census_sex | direct | 0.65605 | 1.58245 | 2.47429 | 0 |
| skin_red | direct | 0.40495 | 0.11175 | 0.27596 | 3 |
| skin_label | direct | 0.05320 | 0.13425 | 2.52350 | 0 |
| chest_sequence | native/skip | 0.40255 | 0.05195 | 0.12905 | 3 |
| chest_x | mixed | 0.23990 | 0.96290 | 3.96278 | 0 |
| chest_activity | direct | 0.04615 | 0.08460 | 1.83315 | 0 |
| taxi_vendor | direct | 0.87025 | 2.04635 | 2.35145 | 0 |
| taxi_payment | direct | 1.39465 | 2.08740 | 1.46700 | 0 |
| taxi_pickup_zone | mixed | 3.99455 | 9.19675 | 2.30507 | 0 |
| taxi_pickup_time | native/skip | 12.99345 | 55.93135 | 4.30458 | 0 |

Full ABW is faster on6/18 real columns. Covertype elevation and Skin red retain
large direct-output gains versus AVX2; Census industry also has a lower median
ratio, with improvement in2/3 processes. The other three winners (NASA timestamp,
Census case ID and chest sequence) use native pdqsort fallback. Their advantage
is not a direct-output gain. Across all66, full ABW is faster on20 and slower
on46. These results support a pdqsort-backed wrapper with specific useful
workloads, not default use of the same frontend ahead of AVX2.

The [three-backend skip-only experiment](../backend_skip_validation/README.md)
separately measured preprocessing before pdqsort, Spreadsort and AVX2. Its
all66 time changes were -26.21%, -38.25% and +10.37%. Direct output itself can
lose: skin_label was35.11% slower than pdqsort and147.58% slower than AVX2.
No measured maximum is presented as a runtime bound for other inputs.

A separate27-input diagnosis finds count/purity accounts for92.44% of the sum
of per-input median instrumented frontend times; sample allocation/frequency
1.04%, width scan5.54%, probe0.11%, entropy0.54%. Instrumented frontend totals
sum to59.1328ms versus58.3637ms uninstrumented (+1.32%); the median input-level
time ratio is1.00387. These are27 separate frontend diagnoses, not one sort.
The skip-only copy continues the original count pass after the first mixed
value. No execution-order or implementation optimization is made here.

