# Caller-owned workspace (v0.2.0 candidate)

This candidate adds an explicit reusable temporary buffer and an exact sampled
bit31 certificate. Sorting choices, active intervals and results are unchanged
from v0.1.3. This is not a published release. Requires C++17/x64 and Boost.

```cpp
abw::workspace ws;
for (auto& values : batches) {
    abw::sort(values, ws);
}
const auto retained = ws.retained_bytes();
ws.release();

// Existing call keeps its original behavior and per-call temporary lifetime.
abw::sort(values);
```

## Minimal interface

- `workspace()`: empty, no temporary payload allocation.
- Noncopyable; noexcept movable. A moved-from object remains valid and can be
  released or used again; do not assume an exact moved-from capacity.
- `capacity() const noexcept`: retained uint32 element capacity, not bytes.
- `retained_bytes() const noexcept`: capacity multiplied by four; allocator
  bookkeeping and RSS are not included.
- `release() noexcept`: discard retained vector storage, returning capacity
  and retained bytes to zero. Repeated release is allowed.
- `sort(vector<uint32_t>&, workspace&)`: same v0.1.3 choice, sample, thresholds,
  active interval, histogram, purity and mixed execution; uses the caller's
  vector only on the mixed path.

Native, skip and exact direct-output paths do not allocate or resize workspace
storage. If a workspace already retains an earlier mixed buffer, these paths
leave that capacity unchanged until explicit release or destruction.

## Growth and lifetime

Mixed calls use `std::vector<uint32_t>::resize(n)`. The logical size becomes n;
smaller calls retain capacity, equal-size calls reuse initialized elements,
and growth within capacity can value-initialize newly enlarged logical elements.
If n exceeds capacity, normal std::vector growth allocates a larger buffer.
Its growth factor is implementation-dependent; capacity is at least n and can
exceed n. Thus retained bytes are not promised to equal four times the current
input size, particularly after larger prior inputs. Query actual capacity or
call release when memory should be returned. No automatic shrinking, reservation
API, hidden static/thread_local cache or input-dependent reuse guard is added.
Growth can temporarily require old plus new storage. Other per-sort allocations
for samples/counts/starts/cursors remain; this is only temporary-array reuse.

## Exceptions and concurrency

For these uint32 elements, a failed vector allocation during workspace growth
preserves the old workspace storage. Allocations in choose/counting/workspace
growth occur before writes to the caller's input; allocation failure there
leaves that input unchanged. Local tests inject a bad_alloc at large growth,
verify input and old capacity, then successfully reuse the same workspace.
This does not replace a general library/platform exception-safety review.

One workspace must not participate in overlapping sort/release/move operations.
Use one workspace per concurrent sorting context, or externally serialize access.
Separate workspace objects may be used concurrently; the implementation has no shared
cache. Sequential ownership transfer between threads is possible with external
synchronization. Same-workspace concurrent use is not supported or detected.

## Implementation and validation

Both overloads use one templated partition kernel. A local owning-vector factory
preserves the one-argument API's per-call temporary lifetime. A borrowing factory
uses workspace storage for mixed paths. This shares histogram, purity, scatter,
internal pdqsort and copy code without runtime per-element ownership branches.

A bit31 difference already observed by the existing order probe proves width32.
In that case the normal full OR scan is omitted. A missing witness still performs
the scan. Ordered dispatch, thresholds and zero/single-value handling are retained.
The width certificate does not prove sortedness or buffer purity.

Correctness tests cover both overloads, strategy/active interval/direct-output
agreement, growth/shrink, move/release, injected allocation failure, independent
concurrent workspaces, exact certificates and adverse sample collisions. Linux
CI records repeated-call diagnostics without performance pass/fail thresholds.
The final integrated-header comparison is recorded separately from earlier
workspace-only prototype measurements.
