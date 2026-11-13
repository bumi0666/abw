# New real-data validation: v0.1.2

This set is separate from the nine development columns and 42 synthetic inputs.
The released header was frozen; no workload-dependent changes were made.

- [Report](report.md), [all times](input_results.csv), [input metadata](input_manifest.json).
- [Raw records, sources, hashes and reproduction](raw_records.zip): extract here.
- [Requested-heap audit](memory.json), [order diagnostics](order_diagnostics.json).
- [Colab CPU notebook](colab_cpu.ipynb), [standalone Linux runner](colab_run.py).

Open the notebook in Colab, select a CPU runtime, and Run all:
https://colab.research.google.com/github/bumi0666/abw/blob/main/benchmarks/new_real_v012/colab_cpu.ipynb

It embeds the exact frozen reference bundle, downloads official datasets and
verified Boost 1.90.0, verifies every source/input hash, compiles all methods
with the same GCC C++20/O3/AVX2/BMI options and compares them in 27 fresh
processes. Only Windows affinity glue is replaced with Linux sched affinity.
It records the actual CPU model, compiler, OS and raw results separately, then
downloads a result ZIP. Original sort code and dispatch stay unchanged. Linux
results require an actual completed run; supplying this notebook is not CPU
validation. AVX2/BMI1 support is checked before executing the benchmark.

The local validation used one Intel i5-14500 Windows CPU. New9 results are not
pooled with prior data. Colab is a shared hosted runtime with a different
OS/compiler; algorithm ratios within each machine matter, and differences
cannot be attributed solely to CPU microarchitecture. No timing is a CI gate.

## Completed second-environment run

[Colab Xeon verification and local comparison](colab_xeon/README.md): 27 processes,
2,544 correct sorts, same input hashes/routes, actual Intel Xeon @ 2.20GHz,
GCC 13.3.0. Keep local and hosted results separate. This verifies the new nine
columns on another CPU/OS/compiler environment, not all prior 51 inputs.
