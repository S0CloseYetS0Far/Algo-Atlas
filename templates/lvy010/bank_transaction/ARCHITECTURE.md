## Bank Transaction System (High-Concurrency C++ Architecture Design)

### Goals
- Safely process account deposits, withdrawals, and transfers under high concurrency with multiple threads.
- Guarantee data consistency (no lost balances, no double deductions, no negative/out-of-range balances).
- Provide good extensibility (thread pool scaling, batch transaction processing, asynchrony).

### Overall Architecture
- Thread pool (ThreadPool)
  - A fixed number of worker threads; tasks are submitted through a blocking queue, and `submit` returns a `std::future`.
  - Exception-safe tasks: exceptions are caught inside the thread and propagated to the future.
- Concurrent account model (Bank)
  - Uses fine-grained per-account mutexes (one `std::mutex` per account).
  - Deposit/withdrawal: holds the exclusive lock of that account; transfer: "locks in order" on both accounts to avoid deadlock.
  - Balances are stored as `long long`, and all modifications happen under mutex protection.
- Transaction model (Transaction)
  - Three kinds: Deposit, Withdraw, Transfer.
  - Carries a unique ID, source/destination accounts, and an amount.
  - The result (TransactionResult) contains the ID, whether it succeeded, an error message, etc.
- Transaction processor (TransactionProcessor)
  - Submits transactions in batches to the thread pool for concurrent execution.
  - Aggregates the execution results; independent of input order, but each result can be matched by ID.

### Concurrency Control and Consistency
- Per-account lock granularity:
  - Single-account operations (deposit/withdraw): hold a single lock, keep the critical section as small as possible.
  - Two-account operations (transfer): always lock the smaller account ID first, then the larger one, to avoid deadlock.
- Funds validation:
  - Withdrawals and transfers must check that the balance is sufficient; if not, they fail without modifying state.
  - Deposits allow any non-negative amount (the example requires amount > 0).
- Atomicity:
  - Within its critical section, each transaction either fully succeeds or fails without changing state.

### Scalability Considerations
- Thread pool size:
  - CPU-bound tasks: ≈ number of CPU cores.
  - I/O-bound or mixed: can be scaled up appropriately (2x~4x).
- Partitioning and scaling (for very large numbers of accounts):
  - Shard by account ID, with independent locks within each shard. Cross-shard transfers can use "two-phase locking" or message middleware.
- Multi-machine deployment (beyond the scope of this example):
  - Use distributed transactions or eventual consistency (event-driven).

### Exceptions and Fault Tolerance
- Exceptions thrown by thread tasks are caught and propagated to the caller via `std::future`.
- Business failures (insufficient balance, nonexistent account) are returned as business errors, not thrown as exceptions.

### Performance Trade-offs
- Pros of per-account locks: fine lock granularity, few conflicts; cons: transfers need two locks.
- Avoid a single global lock (low throughput).
- We don't use `std::atomic<long long>` to update balances directly, because transfers involve a compound invariant across two accounts, so mutexes are required to guarantee consistency.

### Data Validation and Reconciliation
- Before batch processing, record the total balance `S0` and tally the net cash flow `Δ` (deposits +, withdrawals -); finally verify `S1 == S0 + Δ`.
- Transfers have no effect on the total balance.

### Directory Structure (Planned)
```
bank_transaction/
  ├─ include/
  │   ├─ thread_pool.hpp
  │   ├─ bank.hpp
  │   ├─ transaction.hpp
  │   └─ transaction_processor.hpp
  ├─ src/
  │   └─ main.cpp
  ├─ v1.0.cpp            # original simple implementation (single-threaded example)
  ├─ CMakeLists.txt
  ├─ README.md
  └─ ARCHITECTURE.md
```

### Testing Approach
- Randomly generate a large number of transactions (a mix of deposits, withdrawals, and transfers) and execute them concurrently.
- Tally the expected net cash flow and reconcile it against the final total balance.
- Observe throughput and latency under high thread counts and high contention (hot accounts accessed very frequently).

