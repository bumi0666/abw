# Third-party notices and data attribution

## Runtime dependency

Boost.Sort / pdqsort, tested with Boost 1.90.0, is a header-only dependency.
Boost uses the Boost Software License 1.0:
https://www.boost.org/LICENSE_1_0.txt
Boost source is not vendored into the ABW runtime header.

## Benchmark-only competitor

CAFS: https://github.com/kexibq-official/cafs-lib
Pinned commit: 13488afef446ae88e7ad631ec6f862def896094a
License: MIT, retained with downloaded source by the reproduction script.
No CAFS code is required to include or call ABW.

## Public source datasets

- NASA HTTP July 1995: https://ita.ee.lbl.gov/html/contrib/NASA-HTTP.html
  KSC NASA server log, contributed by Jim Dumoulin, Martin Arlitt and Carey
  Williamson. Publisher permits free redistribution, with a request to limit
  analysis to general traffic patterns. No named SPDX license was established.
- Covertype: Jock Blackard (1998), DOI 10.24432/C50K5N,
  https://archive.ics.uci.edu/dataset/31/covertype — CC BY 4.0.
- US Census Data (1990): Meek, Thiesson and Heckerman (2001),
  DOI 10.24432/C5VP42,
  https://archive.ics.uci.edu/dataset/116/us+census+data+1990 — CC BY 4.0.
  This is the publisher's processed release, including upstream recoding and
  randomization. Published order and codes are retained.

CC BY 4.0: https://creativecommons.org/licenses/by/4.0/
The extraction changes representation to integer arrays and excludes documented
missing/unparseable records. No replication, favorable reordering, or lossy
numeric conversion was used. Details and input hashes are in the full report
and benchmark metadata. Dataset licenses are not replaced by the project's MIT
license. Raw source data are downloaded separately, not included in the repo.
