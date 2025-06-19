# Scheduler-Tools

A small library providing a lightweight task scheduler, timer, and other utilities for basic microcontroller applications. To include the library in your project, add the `src` directory to the compiler sources and the `inc` directory to the compiler include path.

The library is primarily designed and tested for the STM32 line but should work anywhere with simple modification (the only platform specific components are atomic code blocks). This project is **not** thread-safe and is only designed for single-threaded systems.