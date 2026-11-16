# Exact sample-certificate AVX2 experiment

The public ABW header is unchanged. This separate skip-only copy uses the exact
witness described in the root README. No full mixed partition optimization,
cardinality/occupancy threshold or release change.

- [All input times, process regressions, implementation and measurement](sample_certificate_report.md)
- [Input CSV](sample_certificate_input_results.csv)
- [Earlier sample bucket diagnosis](sample_bucket_diagnosis.csv)
- [Completion checks](completion.json)
- [Source/raw data reproduction ZIP](abw_sample_certificate_results.zip)

On the same66 inputs, mixed27 overhead versus AVX2 is29.83% for old skip-only
and3.40% for the certificate. Candidate is20.29% faster than old on mixed27,
9.12% faster overall,0.81% slower than AVX2 overall. IPv4_6 overhead is0.97%.
Real18 remains9.70% slower than AVX2. Maximum input-median regression versus old
is2.04% (1M upper-only); every input/process result is preserved. Windows11/
i5-14500, LLVM-MinGW Clang23.1.5 C++17 AVX2-only, x86-simd-sort v7.0, no OpenMP.
198 processes/6336 full-output/hash/choice checks pass, plus the earlier79-input/
711-output safety test and66 original-choice audits. Prepared and earlier raw
archives are preserved. Different backend/frontend comparisons remain separate.

The separate27-input diagnostic shows IPv4 destination max sample occupancy
80.5-83.6%, sources57.0-67.8%. Winners22/losers5 ranges overlap. No threshold
is added. The prior full ABW/pdq IPv4 mean loss27.01% and max47.70% remain in
[the earlier same-run comparison](../full_backend_validation/README.md).
