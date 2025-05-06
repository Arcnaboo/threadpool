# 🧵 threadpool.c

A minimal, fast, and easy-to-understand thread pool implementation in **C**, designed for learning, small projects, or embedded usage. It uses **POSIX threads** to manage a fixed pool of worker threads and a circular task queue.

---

## 📂 File Overview

- `threadpool.c` — Main source file with full thread pool logic.
- No dependencies beyond `pthread`.

---

## ⚙️ Compilation

Use `gcc` with the `-lpthread` flag:

```bash
gcc threadpool.c -o threadpool -lpthread
