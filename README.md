# KVStore 

A Redis-inspired in-memory key-value database built entirely from scratch in **C++**.

KVStore is a lightweight database server that supports concurrent TCP clients, custom hash-table storage, persistence, key expiration, and LRU-based eviction—implemented without relying on STL hash maps or external networking frameworks.

Repo Link: https://github.com/Vaibhav260606/KV-store

> **Goal:** Learn systems programming by implementing the core building blocks of a database server from first principles.
---

# Features

### Storage Engine
- Custom hash table implementation (without `std::unordered_map`)
- Separate chaining for collision handling
- Automatic dynamic rehashing
- Average-case **O(1)** lookup, insertion and deletion

### Networking
- TCP server built using POSIX sockets
- Custom text-based request protocol
- Multiple simultaneous client connections

### Concurrency
- Multi-threaded server
- Thread-per-connection architecture
- Mutex-protected shared data structure
- Thread-safe CRUD operations

### Persistence
- Snapshot-based persistence
- Automatic loading of saved data on startup
- Graceful shutdown with data flush

### Cache Features
- Time-To-Live (TTL) support
- Automatic key expiration
- LRU eviction policy
- Memory-bounded storage

---

# Architecture

```
                +----------------------+
                |      TCP Clients     |
                +----------+-----------+
                           |
                           |
                    POSIX Socket Server
                           |
                    Command Parser
                           |
                  -------------------
                  |                 |
            Hash Table         Persistence
                  |                 |
                  |            Snapshot File
                  |
         TTL + LRU Eviction
```

---

# Project Structure

```
kvstore/
│
├── include/
│   ├── hashtable.h
│   ├── server.h
│   ├── parser.h
│   ├── persistence.h
│   └── cache.h
│
├── src/
│   ├── main.cpp
│   ├── hashtable.cpp
│   ├── server.cpp
│   ├── parser.cpp
│   ├── persistence.cpp
│   └── cache.cpp
│
├── tests/
│
├── data/
│   └── snapshot.db
│
├── Makefile
└── README.md
```

---

# Supported Commands

| Command | Description |
|----------|-------------|
| `SET key value` | Insert or update a key |
| `GET key` | Retrieve value |
| `DEL key` | Delete key |
| `EXISTS key` | Check key existence |
| `EXPIRE key seconds` | Set TTL |
| `TTL key` | Remaining lifetime |
| `SAVE` | Force snapshot |
| `EXIT` | Close client connection |

---

# Example Session

```
$ nc localhost 6379

SET name Vaibhav
OK

GET name
Vaibhav

EXISTS name
1

DEL name
OK

GET name
NULL
```

---

# Core Data Structures

### Hash Table

- Custom implementation
- Separate chaining
- Dynamic resizing
- Polynomial rolling hash

### LRU Cache

Implemented using

- Doubly Linked List
- Hash Table

to provide

- O(1) insertion
- O(1) deletion
- O(1) eviction

---

# Design Decisions

## Why Separate Chaining?

Instead of open addressing, separate chaining allows

- simpler deletion
- dynamic resizing
- lower clustering
- easier concurrent access

---

## Why Dynamic Rehashing?

As the load factor increases, lookup time degrades.

The table automatically doubles its capacity whenever

```
load_factor > 0.75
```

to maintain near O(1) performance.

---

## Why Raw POSIX Sockets?

The networking layer is implemented directly using

- socket()
- bind()
- listen()
- accept()
- recv()
- send()

to understand low-level network programming without abstractions.

---

## Why Manual Memory Management?

Nodes are allocated using

```cpp
new
```

and freed using

```cpp
delete
```

to understand heap allocation, ownership and destructors.

---

# Complexity

| Operation | Complexity |
|-----------|------------|
| SET | O(1) Average |
| GET | O(1) Average |
| DEL | O(1) Average |
| EXISTS | O(1) Average |
| Rehash | O(n) |
| LRU Update | O(1) |
| Expiry Check | O(1) |

---

# Build

```
git clone https://github.com/Vaibhav260606/KV-store

cd kvstore

make
```

---

# Run

```
./kvstore 6379
```

Connect using

```
nc localhost 6379
```

---

# Technologies Used

- C++17
- POSIX Sockets
- std::thread
- std::mutex
- File I/O
- Makefile

---

# Future Improvements

- Thread Pool
- Read-Write Locks
- Publish / Subscribe
- Transactions
- Replication
- Append Only File (AOF)
- Benchmark Suite
- Custom Memory Allocator

---

# Learning Outcomes

This project explores several fundamental systems programming concepts:

- Hash Tables
- Linked Lists
- Dynamic Memory Management
- TCP Networking
- Multi-threading
- Synchronization
- File Persistence
- Cache Design
- Database Internals

---

# Inspiration

This project is inspired by the architecture of Redis, but is implemented entirely from scratch.
