#ifndef SCAPULA_OS_EXCEPTION_LEVEL_H
#define SCAPULA_OS_EXCEPTION_LEVEL_H

#include <stdint.h>

// get_current_el
//
// Get the current exception level that Scapula OS is currently exceuting at
// Software components should read the current exception level through this
// function because hardware has no way of reporting when the current exception
// level is EL0
//
// @return The current exception level (0 = EL0, 1 = EL1, 2 = EL2, 3 = EL3)
uint32_t get_current_el(void);

// set_current_el
//
// Sets the current exception level that Scapula OS is currently exceuting
// at. This function should generally be used only by interrupt and exception
// handlers as a means to indicate to software that the current exception level
// has changed. This function does NOT change the hardware exception level.
//
// @param new_el The new exception level that Scapula OS is currently
//      running at (0 = EL0, 1 = EL1, 2 = EL2, 3 = EL3, other values undefined)
void set_current_el(uint32_t new_el);

// switch_to_el
//
// Change the current exception level (EL) to a new one. Upon returning, the
// exception level will be changed to target_el, preserving the current stack,
// link register, and general purpose registers
//
// @param target_el The target exception level to switch to
//      Valid values: 0 (EL0), 1 (EL1), 2 (EL2), 3 (EL3)
//
// Usage:
//      SCAPULA_PRINT("This will print from EL2");
//      switch_to_el(1);
//      SCAPULA_PRINT("This will print from EL1");
void switch_to_el(uint32_t target_el);

// get_exception_counter
//
// Returns the number of synchronous exceptions that have occurred since the
// last time the Scapula OS exception counter was cleared.
//
// @return The number of exceptions that have occurred
uint64_t get_exception_counter(void);

// reset_exception_counter
//
// Reset the Scapula OS exception counter to 0
void reset_exception_counter(void);

// increment_exception_counter
//
// Adds an exception occurrence to the exception counter
void increment_exception_counter(void);

#endif
