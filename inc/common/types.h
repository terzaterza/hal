#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

typedef enum status {
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_NOT_IMPLEMENTED
} status_t;

#define STATUS_FROM_BOOL(x)     ((x) ? STATUS_OK : STATUS_ERROR)
#define STATUS_AS_BOOL(x)       ((x) == STATUS_OK)

#ifndef UNUSED
#define UNUSED(x)   ((void)(x))
#endif

#endif