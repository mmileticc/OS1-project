# RISC-V Synchronous Kernel (Operating Systems 1)

A low-level synchronous kernel implementation for the **RISC-V architecture**, developed as the primary project for the **Operating Systems 1** course (2023/2024).

## Project Scope
The core objective of this project was to design and implement a functional kernel capable of managing system resources and process execution on a RISC-V processor. It covers the fundamental building blocks of an operating system, focusing on stability and synchronous execution.

## Technical Implementation
- **Architecture:** Target platform is the **RISC-V** instruction set architecture (ISA).
- **Core Features:**
    - **Kernel-Level Context Switching:** Manual management of CPU registers and stack frames.
    - **System Call Interface (ABI):** Implementation of synchronous system calls for user-space to kernel-space communication.
    - **Memory Management:** Basic allocation and protection mechanisms.
    - **Process/Thread Synchronization:** Handling execution flow within a synchronous environment.
- **Languages:** Written in a combination of **C++** and **RISC-V Assembly**.

## Engineering Challenges
Implementing a kernel from scratch required a deep understanding of:
1. **Privilege Modes:** Navigating between User and Machine/Supervisor modes.
2. **Trap Handling:** Managing interrupts and exceptions via specific RISC-V CSRs (Control and Status Registers).
3. **Low-Level Debugging:** Working without standard libraries, relying on direct memory manipulation and register tracking.

## Academic Context
This project represents the advanced (20-point) segment of the **Operating Systems 1** curriculum at the **School of Electrical Engineering (ETF)**, University of Belgrade.

---
**Author:** Milinko Miletić
