#include "hal_io.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * Replacement for stdio printf
*/
hal_status_t io_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return HAL_STATUS_OK;
}

/**
 * Replacement for stdio scanf
*/
hal_status_t io_scanf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vscanf(format, args);
    va_end(args);
    return HAL_STATUS_OK;
}

/**
 * Replacement for putchar
*/
hal_status_t io_putchar(const char c)
{
    putchar(c);
    return HAL_STATUS_OK;
}

/**
 * Replacement for getchar
*/
hal_status_t io_getchar(char* c)
{
    *c = getchar();
    return HAL_STATUS_OK;
}