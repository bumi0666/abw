# v0.1.2 validation

The public header contains the measured selective candidate without algorithm
changes; namespace and explanatory comment are the only source differences.
The test-only frozen candidate is tests/reference/v012_candidate.hpp.

## Packaging check

C++17 LLVM-MinGW Clang 23.1.5, Boost 1.90.0, Intel i5-14500 Windows x64.
Conformance passed 140 cases: 89 built-in and all original 51 input files.
Includes shared nonzero prefixes, low-entropy ordered low-byte inputs, all four
histogram tails, and high-bit outliers missed by the order sample. Full sorted
arrays, candidate/public choices, bits, shifts and direct/mixed execution agree.

The 816-call short packaging check used the existing same-buffer, common-callsite
ABBA/BAAB harness: 51 fresh processes, two blocks per pair, frozen candidate and
same-public-function alias control. Public/candidate geometric mean time ratios:
real 0.9577, synthetic 1.0088; input ratios ranged
0.8079 to 1.0822. This short check detects packaging issues;
it does not replace the full candidate comparisons or prove statistical equivalence.
[Raw packaging records](header_raw_records.zip) include commands and hashes.

## Full evidence

- [Competition report](competition/report.md), [all input times](competition/input_results.csv),
  [source/input hashes](competition/protocol.json), [complete reproduction archive](competition/raw_records.zip).
  153 fresh processes; 11,376 timed sorts; all full-array checks passed.
- [Full 51-input paired candidate/baseline comparison](abw_paired_full/report.md)
  and [raw archive](abw_paired_full/raw_records.zip).
- [Earlier paired 13-input diagnosis](abw_paired_check/report.md)
  and [raw archive](abw_paired_check/raw_records.zip).

Unzip each archive into its containing directory to restore relative paths.
The competition archive is the original byte-verified reproduction bundle,
including pinned CAFS sources/licenses, original v0.1.1 header, frozen candidate,
harness, runners, source/input hashes and raw records. Input arrays and compiler
are external prerequisites. Fetch/extract inputs with ../README.md instructions
and verify hashes in competition/protocol.json. Original scripts retain local
research paths; adapt paths only, and write new runs to a fresh directory.

CAFS int32 supports 6/42 synthetic inputs; Counting was eligible on 9/42 under
the existing 64 MiB cap. Do not compare unequal-subset aggregate speeds. Scalar
CAFS u32, AVX2 CAFS i32, one CPU, no fresh memory profiling. Existing large losses
are retained in the report. Timings are not CI pass conditions.
