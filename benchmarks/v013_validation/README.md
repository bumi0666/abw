# v0.1.3 B+D validation

This release adopts the frozen B+D candidate, with only its namespace changed
to `abw`. B skips unused detailed frequency sampling on the ordered low-byte
exception. D replaces required `unordered_map` frequencies with a fixed table.
No policy, seed, threshold, histogram choice or mixed-bucket algorithm is changed.
Native-only bits/shift metadata is not an executed bit interval and can differ.

## Final competition evidence (separate from v0.1.2 measurements)

- [Report and all-input times](competition/report.md)
- [Input results](competition/input_results.csv), [summary](competition/summary.json)
- [Protocol and hashes](competition/protocol.json), [validation](competition/validation.json)
- [Raw records, source snapshots and reproduction ZIP](competition/reproduction.zip)

All60 inputs (original real9, new real9, synthetic42) and all eligible competitors
were retained. There were13,920 timed sorts in180 fresh processes. Overall GM
execution time decreased2.69% versus v0.1.2; same-pair views give2.68–2.72%.
Group decreases were2.36%,2.28%,2.85%. B+D remains2.96% slower than CAFS u32
overall and4.47% slower than CAFS i32 on its supported24-input subset.

Maximum primary regression versus v0.1.2 was+3.61% on10M sorted values,
pair-only+5.24%; it was slower in2/3 processes. NASA bytes primary ratio was
0.97585, not a repeat of the earlier D-versus-B-only4.8% loss. Report all views,
including existing losses to competitors; do not infer universal superiority.
These timings use one Intel i5-14500 Windows CPU. Prior v0.1.2 Colab results do
not validate B+D on another CPU. No further performance run was added for release.

## Public-header integration and safety

The public header is byte-equivalent as text to the measured `D.hpp` after the
single opening namespace substitution. See [local validation](local_validation.json)
for both SHA-256 hashes, complete commands and actual test output. No instruction
change was introduced while extracting the header.

The table has4096 slots and production `choose()` makes at most2048 increments:
512 samples below1M,2048 otherwise. Distinct count cannot exceed increment count,
so occupancy is at most50%. At every insertion an empty slot exists, including
under a single collision chain; masked linear probing visits every slot and
terminates. Full-table insertion is outside this internal helper's precondition
and cannot occur through `abw::sort()`. There is no public arbitrary-size table API.

Counts start at zero. A key is written before its count becomes nonzero, and is
read only when that count is nonzero, so uninitialized key storage is never read.
Zero count is an occupancy marker, leaving all uint32 keys valid, including0 and
UINT32_MAX. Each count is at most2048, within uint16. Hash multiplication uses
unsigned32-bit wraparound; capacity is a power of two. This preserves exact
frequencies, distinct count and mode; the original sample-values vector remains.

The 24KiB key/count array payload is in a separate noinline helper; bypass paths
do not enter it. Small-stack users should budget for this. Collision patterns can
make probing quadratic in sample count; this change does not claim adversarial
constant-time hashing. Mixed-path4*n temporary storage and its initialization
are unchanged. No uninitialized temporary-array optimization is combined here.

Local C++17 checks:

- `abw_frequency`:58 cases, including maximum supported2048 distinct keys sharing
  the last initial slot, wraparound, reverse insertion, repeated0/UINT32_MAX,
  maximum2048 frequency and exact sample positions/summary at both sample sizes.
- `abw_conformance`:149 cases including all existing60 workloads. Complete arrays,
  executable strategies, active bit intervals and direct/mixed results match the
  frozen v0.1.2 reference. Native metadata is intentionally not equated to execution.
- GCC, Clang and MSVC C++17 run both tests through the existing
  [correctness workflow](../../.github/workflows/ci.yml). Timing is not a CI gate.

Build and test:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 -DABW_BOOST_INCLUDE_DIR=/path/to/boost_1_90_0
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

For the performance reproduction ZIP, restore the baseline public header from
tag v0.1.2, not current main, at the recorded baseline paths before running.
Input arrays, Boost and compiler binaries are not bundled. Restore source data
and generated arrays from the original and new-real benchmark instructions,
verify protocol hashes, and adapt filesystem paths only. The recorded runner's
artifact-copy filename correction is disclosed in the report. Existing raw
measurements and historical evidence are unchanged.
