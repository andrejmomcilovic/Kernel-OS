# OS1 Kernel — RISC-V Bare Metal Operating System

A bare-metal operating system kernel implemented in C++ and RISC-V assembly, running on QEMU's RISC-V virtual machine. Developed as a university project for the Operating Systems 1 course at the School of Electrical Engineering, University of Belgrade.

## Overview

This project implements a preemptive multitasking kernel with user/supervisor mode separation, built from scratch without any standard library support. The kernel runs on a modified xv6 base targeting the RV64IMA architecture.

## Features

### Memory Management (Z1)
- First-fit dynamic memory allocator
- Free block coalescing
- Custom `operator new` / `operator delete`

### Thread Management (Z2)
- Kernel threads with cooperative and preemptive context switching
- User mode thread execution (SPP=0, SPIE=1 via `sret`)
- Round-robin scheduler (FIFO)
- C and C++ thread APIs

### Synchronization (Z3)
- Counting semaphores with blocking wait
- Bulk wait/signal (`sem_wait_n`, `sem_signal_n`)
- Safe `sem_close` with error propagation to waiting threads

### Time-sharing and I/O (Z4)
- Timer-driven preemptive scheduling (SSIP via SBI)
- `time_sleep` with delta-encoded sleep queue
- Console I/O with circular buffers and semaphore synchronization
- Dedicated kernel TX thread for non-blocking `putc`
- `getc` with blocking wait on keyboard interrupt

### C++ API
- `Thread` — base class with virtual `run()`, cooperative and preemptive dispatch
- `Semaphore` — RAII wrapper around kernel semaphore
- `PeriodicThread` — periodic activation via `time_sleep`
- `Console` — static `getc`/`putc` wrappers

## Architecture

```
User Mode (S-mode, SPP=0)        Supervisor Mode (S-mode, SPP=1)
┌─────────────────────┐          ┌──────────────────────────────┐
│  User Threads       │  ecall   │  Syscall Handler             │
│  (taskA, taskB...)  │ ──────►  │  InterruptHandler            │
│                     │  sret    │  Scheduler / Semaphores      │
│  C / C++ API        │ ◄──────  │  MemoryAllocator             │
└─────────────────────┘          │  Console (TX/RX buffers)     │
                                 │  SleepQueue                  │
                                 └──────────────────────────────┘
```

## Syscall ABI

| Code | Function | Description |
|------|----------|-------------|
| 0x01 | `mem_alloc` | Allocate memory |
| 0x02 | `mem_free` | Free memory |
| 0x11 | `thread_create` | Create and start thread |
| 0x12 | `thread_exit` | Terminate current thread |
| 0x13 | `thread_dispatch` | Yield processor |
| 0x21 | `sem_open` | Create semaphore |
| 0x22 | `sem_close` | Destroy semaphore |
| 0x23 | `sem_wait` | Wait on semaphore |
| 0x24 | `sem_signal` | Signal semaphore |
| 0x25 | `sem_wait_n` | Wait for N tokens |
| 0x26 | `sem_signal_n` | Signal N tokens |
| 0x31 | `time_sleep` | Sleep for N timer periods |
| 0x41 | `getc` | Read character from console |
| 0x42 | `putc` | Write character to console |

## Running it

The project is compiled with a RISC-V `gcc` toolchain and run in a QEMU RISC-V emulator, via the provided `Makefile`:

```
make qemu       # build + run in the emulator
make qemu-gdb   # build + run in debug mode (GDB remote)
make clean      # remove build artifacts
```

It also runs from CLion (the "Make" target, choosing the right goal), and supports remote debugging via `gdb` (QEMU prints the port, e.g. `localhost:26000`)

## Project Structure

```
├── h/                  # Header files
│   ├── k_thread.h      # Thread class
│   ├── k_semaphore.h   # Semaphore class
│   ├── Scheduler.h     # Round-robin scheduler
│   ├── MemoryAllocator.h
│   ├── Console.h       # Console I/O buffers
│   ├── sleepQueue.h    # Delta sleep queue
│   ├── syscall_c.hpp   # C API declarations
│   └── syscall_cpp.hpp # C++ API declarations
├── src/                # Source files
│   ├── main.cpp        # Kernel entry, interrupt handler, syscalls
│   ├── k_thread.cpp    # Thread implementation
│   ├── k_semaphore.cpp # Semaphore implementation
│   ├── Scheduler.cpp
│   ├── MemoryAllocator.cpp
│   ├── Console.cpp     # Circular buffer console
│   ├── sleepQueue.cpp  # Sleep queue implementation
│   ├── syscall_c.cpp   # C API (ecall wrappers)
│   ├── syscall_cpp.cpp # C++ API implementation
│   ├── interrupt.S     # Trap handler (save/restore context)
│   └── yield.S         # Context switch
└── lib/                # Provided libraries (hw.lib, mem.lib, console.lib)
```

## Technical Notes

- **Context switch**: Full register save/restore in `interrupt.S` (240 bytes, including `sepc`)
- **User mode**: First thread activation via `sret` with SPP=0, SPIE=1
- **Timer**: SSIP cleared manually via `sip` register; `currTime` tracks time slice
- **Console**: PLIC-driven RX interrupt fills `rxBuf`; dedicated kernel TX thread drains `txBuf`
- **Sleep queue**: Delta-encoded list; `update()` decrements only the head node

## License

Based on xv6 (MIT License) — Copyright (c) 2006-2019 MIT
Student implementation — Andrej Momčilović, 2026
