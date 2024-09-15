#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

typedef struct device {
    uint8_t (*read)(void *, uint16_t);
    void (*write)(void *, uint16_t, uint8_t);
    void *data;

} Device;

#endif
