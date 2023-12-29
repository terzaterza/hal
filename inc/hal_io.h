#ifndef HAL_IO_H
#define HAL_IO_H

#include "hal_core.h"

/**
 * Replacement for stdio printf
*/
hal_status_t io_printf(const char* format, ...);

/**
 * Replacement for stdio scanf
*/
hal_status_t io_scanf(const char* format, ...);

/**
 * Replacement for putchar
*/
hal_status_t io_putchar(const char c);

/**
 * Replacement for getchar
*/
hal_status_t io_getchar(char* c);

#endif