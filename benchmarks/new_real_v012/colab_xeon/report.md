# ABW v0.1.2 Colab CPU validation

CPU: Intel(R) Xeon(R) CPU @ 2.20GHz

Compiler: g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0

27 processes, 2,544 sorts, every full-array check passed. All source input and expected hashes and routes match the Windows run. This is a hosted/virtualized runtime with a different OS/compiler; no hardware-only causal claim. No policy changes.

| Method | N | GM speedup vs pdq | v0.1.2/method time |
|---|---:|---:|---:|
| v012 | 9 | 1.5283 | 1.0000 |
| v011 | 9 | 1.2012 | 0.7856 |
| pdq | 9 | 1.0000 | 0.6543 |
| spread | 9 | 0.7366 | 0.4818 |
| CAFS_u32 | 9 | 1.4749 | 0.9635 |
| CAFS_i32 | 9 | 1.6429 | 1.0704 |
| counting | 8 | 0.8960 | 0.5556 |

| Input | v0.1.2 ms | v0.1.1 ms | pdq ms | CAFS u32 ms | CAFS i32 ms | v0.1.2/v0.1.1 | Direct |
|---|---:|---:|---:|---:|---:|---:|---|
| skin_red | 0.28951 | 0.37616 | 2.59898 | 1.58003 | 1.23454 | 0.7717 | True |
| skin_label | 0.29355 | 0.33709 | 0.34126 | 0.12636 | 0.13757 | 0.8630 | True |
| chest_sequence | 0.21885 | 0.22385 | 0.21647 | 0.08174 | 0.08175 | 0.9777 | False |
| chest_x | 1.78036 | 1.74912 | 1.71378 | 1.37209 | 0.64300 | 1.0106 | False |
| chest_activity | 0.19980 | 0.28996 | 0.29446 | 0.35506 | 0.39718 | 0.6873 | True |
| taxi_vendor | 6.94816 | 11.43446 | 11.57726 | 6.21193 | 6.53901 | 0.6077 | True |
| taxi_payment | 5.58197 | 13.34172 | 11.95556 | 5.40707 | 5.44842 | 0.4131 | True |
| taxi_pickup_zone | 28.52694 | 28.54530 | 25.64622 | 20.23253 | 12.89469 | 0.9994 | False |
| taxi_pickup_time | 123.28932 | 123.31706 | 120.97603 | 230.68896 | 304.61312 | 1.0052 | False |
