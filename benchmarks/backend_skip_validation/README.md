# Three-backend skip-only comparison

Frozen experimental frontend before pdqsort, Spreadsort and x86-simd-sort AVX2.
Direct output succeeds or the whole untouched input goes to its backend.
No mixed scatter/bucket sorting and no workspace. The count pass still completes
after the first mixed value. Existing66 inputs,198 processes/9504 correct sorts,
Windows11/i5-14500, C++17 LLVM-MinGW Clang23.1.5, AVX2 only.

All66 time changes versus the respective backend: pdq -26.21%, Spread -38.25%,
AVX2 +10.37%. On real18: -35.99%, -56.09%, +14.53%. Direct success does not
imply a gain: skin_label is +35.11% versus pdq and +147.58% versus AVX2.
All raw data are retained. Public code and policies were not changed.

- [Report and all input times](report.md)
- [Input CSV](input_results.csv)
- [Completion checks](completion.json)
- [Raw data/source ZIP](abw_backend_skip_comparison_results.zip)
- [Follow-up full ABW/AVX2 comparison](../full_backend_validation/README.md)
