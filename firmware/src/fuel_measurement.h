#ifndef FUEL_MEASUREMENT_H
#define FUEL_MEASUREMENT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint16_t raw_capacitance;
    int16_t fuel_temp_c_x10;
    int16_t air_temp_c_x10;
    uint16_t fuel_level_percent_x10;
    uint16_t fuel_volume_l_x10;
    uint16_t quality_flags;
} fuel_measurement_result_t;

void fuel_measurement_init(void);
bool fuel_measurement_poll(fuel_measurement_result_t *result);

#endif
