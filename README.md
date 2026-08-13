# Task Scheduler (C++)

A command-line task scheduler backed by a custom-built binary min-heap.
Tasks are added with a priority (lower number = more urgent) and run in
priority order. Any pending task can also be cancelled by id.

Built to practice core data structures and algorithms — the heap is
implemented from scratch (not `std::priority_queue`) so insertion,
extraction, and removal are all hand-rolled and testable.

## Features

- `add <id> <priority> <name...>` — add a task
- `run` — run (pop) the next most urgent task
- `cancel <id>` — remove a pending task by id, from anywhere in the queue
- `list` — list all pending tasks

## Design

- **`MinHeap`** (`include/MinHeap.h`) — a binary min-heap over `Task`,
  ordered by `priority`. Alongside the heap array, it keeps an
  `unordered_map<id, index>` so a task's position can be found without
  scanning the whole heap — that's what makes `cancel` fast.
- **`Task`** (`include/Task.h`) — plain struct: `id`, `name`, `priority`, `deadline`.

## Complexity

| Operation         | Complexity   | Why |
|-------------------|--------------|-----|
| `push`            | O(log n)     | Insert at the end, bubble up at most tree-height steps |
| `extractMin`      | O(log n)     | Swap root with last element, bubble down |
| `peek`            | O(1)         | Root of the heap |
| `remove(id)`      | O(log n)     | Hash-map lookup is O(1) average, then bubble up/down from that index |
| `contains(id)`    | O(1) average | Direct hash-map lookup |

Note: `remove(id)` is O(log n), not O(1) — a binary heap can locate the
node in O(1) via the index map, but restoring the heap property after
removing it still costs up to O(log n).

## Build & test

Requires g++ with C++17 support. No external dependencies beyond the
bundled Catch2 single-header test framework (`tests/catch.hpp`).

```bash
make        # builds ./scheduler and runs the test suite
make app    # just build the CLI
make test   # build and run the unit tests
make clean
```

## Tests

`tests/test_heap.cpp` covers:
- Empty-heap behavior (`peek`/`extractMin` throw)
- Priority ordering on `extractMin`
- Duplicate id rejection
- Cancellation from the middle, front, and end of the heap
- Cancelling a missing id is a no-op
- A larger sequence to confirm heap order holds under interleaved operations

## Example session

```
> add 1 5 write report
Added task 1 ("write report", priority 5)
> add 2 1 fix critical bug
Added task 2 ("fix critical bug", priority 1)
> run
Running task 2: fix critical bug (priority 1)
```
