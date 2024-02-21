#include "drivers/sdev.h"

typedef struct sdev {
    sdev_ops_t* ops;
    void* context;
} sdev_t;

/**
 * Open handler
*/
status_t sdev_open(sdev_t* sdev, sdev_ops_t* ops, void* context)
{
    /** @todo Here init other sdev struct members if needed */

    sdev->ops = ops;
    sdev->context = context;

    return STATUS_OK;
}

/**
 * Call the implementation specific test handler
 * @todo Maybe can be implemented as `#define` or `static inline`
*/
status_t sdev_test(sdev_t* sdev)
{
    return sdev->ops->test(sdev->context);
}

/**
 * Call the implementation specific write handler
*/
status_t sdev_write(sdev_t* sdev, uint8_t* data, size_t nbyte)
{
    return sdev->ops->write(sdev->context, data, nbyte);
}

/**
 * Call the implementation specific read handler
*/
status_t sdev_read(sdev_t* sdev, uint8_t* data, size_t nbyte)
{
    return sdev->ops->read(sdev->context, data, nbyte);
}

/**
 * Call the implementation specific ioctl handler
*/
status_t sdev_ioctl(sdev_t* sdev, int ctl_type, void* arg)
{
    return sdev->ops->ioctl(sdev->context, ctl_type, arg);
}

/**
 * Call the implementation specific close handler
*/
status_t sdev_close(sdev_t* sdev)
{
    /** @todo Deinit struct members here if needed */

    return sdev->ops->close(sdev->context);
}