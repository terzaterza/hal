#ifndef DRIVERS_SDEV_H
#define DRIVERS_SDEV_H

#include <stdint.h>
#include <stdlib.h>
#include "common/types.h"

// this can also be static inline functions but this is less code (cleaner)
#define sdev_read(device, ...)    (device->read(device, __VA_ARGS__))
#define sdev_write(device, ...)   (device->write(device, __VA_ARGS__))

typedef struct serial_device {
    status_t (*write)(struct serial_device* device, uint8_t* data, size_t len);
    status_t (*read)(struct serial_device* device, uint8_t* data, size_t len);
    status_t (*test)(struct serial_device* device);
    status_t (*init)(struct serial_device* device);
    status_t (*ioctl)(struct serial_device* device, int ctl_type, void* arg);
    status_t (*close)(struct serial_device* device); // use for freeing any possible malloc
    void* params; // can rename to private_data to match linux
    // struct device* parent; // not needed for now, add if needed to enforce "device tree" structure
} sdev_t;

#endif