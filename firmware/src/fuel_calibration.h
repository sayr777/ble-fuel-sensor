#ifndef FUEL_CALIBRATION_H
#define FUEL_CALIBRATION_H

#include <stdbool.h>
#include <stdint.h>

#include "fuel_measurement.h"

typedef enum
{
    FUEL_LEVEL_STATE_STABLE = 0,
    FUEL_LEVEL_STATE_REFILL = 1,
    FUEL_LEVEL_STATE_DRAIN = 2
} fuel_level_state_t;

typedef struct
{
    uint16_t fuel_level_percent_x10;
    uint16_t fuel_volume_l_x10;
    int16_t fuel_temp_c_x10;
    int16_t air_temp_c_x10;
    uint16_t quality_flags;
    uint8_t fuel_profile;
    uint8_t calibration_revision;
    fuel_level_state_t level_state;
} fuel_calibration_result_t;

void fuel_calibration_init(void);
bool fuel_calibration_apply(const fuel_measurement_result_t *measurement, fuel_calibration_result_t *result);

#endif
