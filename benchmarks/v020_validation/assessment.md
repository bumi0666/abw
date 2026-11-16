# Assessment: retain the v0.2.0 candidate for review

The integrated header passes all66 full-array/strategy/interval/direct-output
checks and the C++17 GCC/Clang/MSVC matrix. Growth failure, recovery, release,
move and independent-workspace concurrency pass. No algorithm/policy changes
were made during comparison. No further long-running test or optimization is
proposed by this report. Main and the public release remain v0.1.3.

## Measured effect (warm / 64MiB-disturbed)

- All66 reused-workspace time versus v0.1.3: -5.05% / -5.15%.
- All66 default-call time versus v0.1.3: -0.99% / -1.18%.
- Reuse versus the new default call: -3.46% / -3.98% overall.
- Mixed27 reuse versus new default: -7.73% / -8.71%; all27 input medians improve
  in both conditions. This does not mean every process/call improves.
- Existing60 reuse versus v0.1.3: -4.31% / -4.53%; keep separate from IPv4.
- IPv4_6 reuse versus v0.1.3: -12.13% / -11.07%; versus new default -9.92% /
  -10.23%. Default versus v0.1.3: -1.83% / -1.45%.
- All66 reuse versus per-call CAFS u32: -2.59% / -2.79%; new default versus CAFS:
  +0.46% / +1.46%. These are separate lifetime/estimator contrasts, not a claim
  that ABW universally beats CAFS or that all ratios multiply exactly.
- Existing60 default versus CAFS u32 remains +2.18% / +3.28%; reused workspace
  is close (-0.29% / -0.43%). Include both when describing the familiar60.
- CAFS i32 uses24 inputs; Counting uses26. Their subset averages cannot be
  compared as though their denominator were all66. Large competitive losses
  on some native/mixed workloads remain; see all-input tables.

## Regressions and attribution

Largest reused/v013 primary median: +6.27% on1M partial (warm; 2/3 processes
slower); +5.69% on Census industry (disturbed; 3/3 slower). These are native
and direct paths respectively, with no retained temporary used. Largest default
median: +7.12% on1M partial (warm; 2/3 slower), +3.39% on1M duplicates_narrow
(disturbed; 3/3 slower). Raw individual-process changes can exceed these medians.
Do not describe these losses as measured workspace allocation costs or dismiss
them as proven noise/layout effects: their cause is unconfirmed.

The previously reported Taxi payment +4.55% is not reproduced in this full run:
reused/v013 -0.83% / +0.56%; default/v013 -0.65% / +1.63%. In the separate
preselected control, the exact same baseline function alias spans0.96050–1.01186
across six processes. Candidate/default ratios and fluctuations remain in
control.json; one default control process is +9.78%. This control describes
variation in that binary/experiment; it does not explain unrelated regressions.

## Certificate and memory

The unchanged probe exactly certifies bit31 variation on24/66 inputs: all six
CTU IPv4 columns and18 synthetics. Original real18 do not omit the full scan.
No missed witness is treated as proof: absent witness retains the scan.

Reused mixed processes retain exactly4n bytes here (27 inputs); direct/native
processes retain0. These are payload capacities, not total allocation/RSS.
Variable-size streams can retain a larger prior capacity. release() explicitly
returns it. No hidden cache or competitor workspace API was introduced. No
phase-by-phase attribution to allocation/initialization/page faults was measured.

## Linux scope and readiness

Ubuntu GCC13.3/AMD EPYC7763 and Clang18.1/Intel Xeon6975P-C hosted VM diagnostics
pass repeated-call correctness/capacity/release for three generated1M forms.
GCC timing has little or no reuse benefit; Clang shows small mixed gains.
These unfiltered diagnostics validate Linux operation, not all66 cross-CPU
performance or a compiler comparison. Full66 timing remains one local CPU.

The candidate is ready for code/release review with these limitations recorded.
A main merge/tag/Experimental Release requires the user's release decision;
none is performed automatically. Preserve this implementation rather than
adding guards or tuning around the remaining regressions.
