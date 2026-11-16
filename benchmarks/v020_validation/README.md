# v0.2.0 candidate validation

This evidence measures the integrated public header: caller-owned workspace,
exact sampled bit31 certificate, one shared partition core. Sorting policy is
unchanged from v0.1.3. This is an unreleased candidate.

- [Full report and all input times](report.md)
- [Assessment, gains, regressions and limits](assessment.md)
- [Input-level CSV](input_results.csv)
- [Checks and CI record](validation.json)
- [Reproduction archive](abw_v020_competition_repro.zip)

396 processes /42,816 timed sorts, existing60 plus CTU IPv4 six. Full arrays,
choices, active intervals, direct paths and source/input hashes pass. Three
processes/input/condition, warm and64MiB-disturbed, same prior ABBA design.
Default/fresh-workspace/reused-workspace calls remain separate. No timing gate.
**All66 performance figures are Windows11/i5-14500 measurements, not
cross-platform performance claims.**

Reused workspace reduces all66 time by5.05%/5.15% versus v0.1.3 and3.46%/3.98%
versus the new default. Mixed27 reuse/default reduction7.73%/8.71%. New default
versus v0.1.3 reduction0.99%/1.18%. Existing60 and IPv4_6 are separately shown.
Default is0.46%/1.46% slower than CAFS u32 across66; reused is2.59%/2.79% faster
under its retained-buffer lifetime. On existing60 default remains2.18%/3.28%
slower, reused close. Do not present these as unconditional CAFS equivalence.

Regressions remain: default1M partial +7.12% warm; reuse1M partial +6.27% warm
and Census industry +5.69% disturbed. All three reuse Census processes are
slower. These native/direct paths do not use workspace; cause unconfirmed.

GCC/Clang/MSVC C++17 [CI](https://github.com/bumi0666/abw/actions/runs/43804878261)
passes. Linux repeated diagnostics on AMD/Intel hosted VMs pass correctness.
Workspace versus v0.1.3 changes by -0.59%/+1.09% on GCC and -2.70%/-1.92% on
Clang for the two mixed inputs. Versus the new default, GCC workspace is
1.74%/1.88% slower. Linux gains can be small or absent and slowdowns occur;
these eight-repeat VM values do not reproduce all66 performance on another CPU.
See [exact artifact values](report.md#linux-repeated-call-values).
The archive contains raw sessions, source/input manifests, frozen headers,
competitor source/licenses, analysis, API contract and Linux logs. Dataset
binaries, Boost and compiler are not bundled. See REPRODUCE.md inside it.

Archive SHA256: `a33c70c2a44b64084902abaacac6fc690f97da718a0a0465c722d18d61b5668b`.

The reproduction archive remains the frozen original evidence snapshot. Current
repository documents clarify platform scope and the allocation-failure contract;
raw timings, sources and the archive were not changed.
