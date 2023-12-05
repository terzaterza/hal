cmake_minimum_required(VERSION 3.27)
project(hal C)

set(CMAKE_C_STANDARD 11)

add_executable(hal main.c
        inc/hal_uart.h
        inc/hal_i2c.h
        inc/hal_gpio.h
        inc/hal_core.h)
