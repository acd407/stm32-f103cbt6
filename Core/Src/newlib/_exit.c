//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscalls.h>

// ----------------------------------------------------------------------------

extern void __attribute__ ((noreturn)) __reset_hardware (void);

// ----------------------------------------------------------------------------

// Forward declaration

void _exit (int code);

// ----------------------------------------------------------------------------

// On Release, call the hardware reset procedure.
// On Debug we just enter an infinite loop, to be used as landmark when halting
// the debugger.
//
// It can be redefined in the application, if more functionality
// is required.

void __attribute__ ((weak, noreturn))
_exit (int code __attribute__ ((unused))) {
    Error_Handler();
    // TODO: write on trace
    while (1)
        ;
}

// ----------------------------------------------------------------------------

void __attribute__ ((weak, noreturn)) abort (void) {
    puts ("abort(), exiting...");

    _exit (1);
}

// ----------------------------------------------------------------------------
