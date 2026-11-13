# Candidate paired comparison and identical-function control

Date: 2026-11-13. No algorithm, selection rule, threshold or GitHub file was changed. Candidate.hpp is byte-identical to the prior experiment.

## Clarification

The previous experiment already put baseline/candidate in one binary and interleaved them within each fresh process. Its variability cannot be explained by saying they were benchmarked only in separate processes. This experiment tightens the comparison rather than introducing within-process comparison for the first time.

## Focused changes to the harness

- One fixed scratch vector per process. Before every timed sort, restore the same original values with std::copy; copying is outside timing. All baseline/candidate/control calls receive the exact same data pointer, which is checked after every call.
- Baseline/candidate are called through the same noinline timing wrapper and function-pointer call site. This normalizes the call boundary; it also changes inlining/code layout relative to previous direct calls, so this is not a one-factor causal isolation experiment.
- Each candidate block is ABBA or BAAB, alternating orientation. Compare mean candidate time of the block with mean baseline time from that block, then take median of eight block ratios per process.
- A second ABBA/BAAB control block calls exactly the same baseline function under two labels. Function-pointer equality is asserted. No separate rewritten 'equivalent' algorithm is used. Candidate/control block order is randomized each cycle.
- Sort-owned allocations remain fresh inside each sort; only external input copying storage is fixed. Required analysis, allocation, zeroing, merge and output are timed; input reset and full-array verification are excluded.

13 inputs: all seven primary >5% regressions from the previous run, four improvement targets, NASA status and Census caseID. This includes regressions with unchanged branches and synthetic cases. Three fresh processes per input, eight blocks per pair, 64 timed calls/process: 39 processes and **2,496 timed calls**. Each call passes full comparison with the original expected array. All selected source/input hashes are verified. Same Intel i5-14500 Windows x64 first-allowed-CPU affinity, LLVM-MinGW Clang23.1.5/Boost1.90.0 and C++20/O3/NDEBUG/AVX2/BMI/static/pthread. Three original warmup families plus one warmup per method on actual input; no outlier filtering or all-process victory gate. One CPU only.

This is a 13-input diagnostic subset, not a rerun of the full 42-input synthetic group. It cannot replace the previous all-input geometric mean or establish that the prior +1.1% synthetic aggregate is gone.

## Results

Ratio below is candidate/baseline; control is baseline-alias/baseline. Lower candidate ratio is better. Times are medians of process medians from candidate pairs. Ratios are medians of the eight ABBA block ratios, then medians across three processes; they need not equal displayed time quotients.

| Input | Baseline ms | Candidate ms | Candidate/A | Process ratio range | Exact same-function control range |
|---|---:|---:|---:|---|---|
| census_caseid | 1.1997 | 1.2001 | 1.0092 | 0.9979..1.0135 | 0.9761..1.0324 |
| census_industry | 2.2115 | 1.5195 | 0.6923 | 0.6690..0.7243 | 0.9915..1.0693 |
| census_sex | 2.6076 | 1.6593 | 0.6313 | 0.5585..0.6575 | 0.9717..1.0595 |
| covtype_cover | 0.8366 | 0.3771 | 0.4416 | 0.4379..0.4570 | 0.9921..1.0085 |
| covtype_elevation | 0.4312 | 0.4407 | 1.0133 | 1.0068..1.0369 | 0.9941..1.0045 |
| nasa_status | 2.4790 | 2.3544 | 0.9558 | 0.9290..1.0176 | 0.9458..1.0175 |
| nasa_timestamp | 1.4352 | 1.4127 | 0.9652 | 0.9620..1.0485 | 0.9385..1.0112 |
| synthetic_10000000_duplicates_narrow | 8.1174 | 8.0076 | 0.9845 | 0.9691..1.0127 | 0.9629..1.0247 |
| synthetic_10000000_upper_only | 10.4940 | 10.6662 | 1.0082 | 0.9986..1.0288 | 0.9822..1.0221 |
| synthetic_1000000_duplicates | 0.9575 | 0.9727 | 1.0166 | 0.9945..1.0214 | 0.9747..1.0108 |
| synthetic_1000000_reverse | 0.7943 | 0.7957 | 0.9938 | 0.9834..1.0143 | 0.9932..1.0174 |
| synthetic_100000_exponential | 1.4353 | 1.4340 | 0.9940 | 0.9867..0.9941 | 0.9941..1.0173 |
| synthetic_100000_upper_only | 0.1588 | 0.1639 | 1.0202 | 1.0162..1.0476 | 0.9986..1.0198 |

## Interpretation

The large gains repeat in all three processes on Census industry (ratios0.6690/0.6923/0.7243), Census sex(0.5585/0.6575/0.6313) and Covertype cover(0.4379/0.4570/0.4416). The previously observed 20..40% reversal on these inputs did not recur in this tighter comparison. Identical-function control process ratios over the 13 inputs ranged approximately0.9385..1.0693, not20..40%. This is not a proof that controls never fluctuate; raw block records retain all variation.

Same-route Covertype elevation candidate ratios are1.0133/1.0068/1.0369, control0.9941..1.0045: no large candidate speedup appears when the one-histogram path is retained. The remaining small difference can include added probe arithmetic and code generation, not only timing error. NASA status and protected caseID remain native, with no large partition regression.

Representative prior synthetic regressions are near baseline here. The largest candidate process-median ratio in this diagnostic set is about1.0485 on NASA timestamp; 100k upper-only reaches1.0476. Those values are much smaller than the prior20%+ extremes, though they do not prove absence of smaller regressions. Narrow10M duplicates ratios0.9691..1.0127; no incremental benefit should be claimed for them.

No offset sweep, padding variant, extra selector or hardware model was added. The old fluctuation's exact cause is not identified: fixed scratch storage, tighter time pairing and normalized out-of-line call boundary changed together. Allocator/cache/code placement/scheduling remain possible influences. Scratch low12-bit addresses and function pointers are logged as observations, not proof of causality. Assigning all previous variation to CPU cache or input alignment would exceed the evidence.

Conclusion: the candidate's substantial gains on the three target inputs are reproducible under this paired harness. It should be retained as a promising follow-up implementation, not discarded due to the earlier unstable run. Existing v0.1.1 remains the public release; this diagnostic is not automatic authorization to publish v0.1.2 or to claim universal improvement. The prior full-input evidence and this focused check remain separate.

## Files and reproduction

session0..2.jsonl retain every timed call, function-pointer identity, routes and scratch-address observations; block_pairs.csv contains each ABBA ratio; process_summary.csv and input_results.csv provide aggregates. protocol.json records selection, input/source hashes and flags. Source, candidate and the unchanged public header snapshot are included. Original data arrays and Boost/compiler installations are not bundled.

Build benchmark.cpp on Windows x64 with the recorded flags and include roots include/ and Boost1.90.0. Invoke `benchmark.exe INPUT EXPECTED SEED`; original runner uses the existing research-workspace paths and refuses to overwrite session records. Use a new output location for another run. Do not pool these short diagnostic records into previous full-input averages.
