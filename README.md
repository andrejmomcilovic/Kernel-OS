# Kernel-OS
Educational multithreaded operating system kernel for RISC-V (RV64), running on a modified xv6 host under QEMU.
RISC-V Operating System Kernel
An educational, library-style OS kernel for the RISC-V (RV64IMA) architecture, developed as a course project. The kernel and user application share a single address space and are statically linked into one program, as is typical for embedded systems. It runs on a stripped-down xv6 host inside the QEMU emulator.
Features

Continuous-allocation memory allocator (first-fit with coalescing)
Thread abstraction with synchronous and asynchronous context switching
Counting semaphores
Time-sharing with timer-based preemption
Layered system-call interface: ABI (via ecall), C API, and C++ OO API

Built with
C++ and RISC-V assembly. No standard library.
