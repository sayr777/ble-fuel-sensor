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

typedef enum
{
    FUEL_VALIDITY_VALID = 0,
    FUEL_VALIDITY_DEGRADED = 1,
    FUEL_VALIDITY_INVALID = 2,
    FUEL_VALIDITY_UNKNOWN = 3
} fuel_validity_status_t;

typedef struct
{
    uint8_t tank_id;
    uint8_t fuel_profile;
    uint8_t calibration_revision;
} fuel_tank_model_t;

typedef struct
{
    uint8_t consumer_id;
    uint8_t source_tank_id;
    uint8_t consumer_type;
} fuel_consumer_model_t;

typedef struct
{
    uint8_t system_model_revision;
    uint8_t tank_count;
    uint8_t consumer_count;
    fuel_tank_model_t tanks[4];
    fuel_consumer_model_t consumers[8];
} fuel_system_model_t;

typedef struct
{
    uint8_t tank_id;
    uint8_t system_model_revision;
    uint16_t fuel_level_percent_x10;
    uint16_t fuel_volume_l_x10;
    int16_t fuel_temp_c_x10;
    int16_t air_temp_c_x10;
    int16_t tilt_pitch_deg_x10;
    int16_t tilt_roll_deg_x10;
    uint16_t quality_flags;
    uint16_t consumer_mask;
    uint8_t confidence_percent;
    uint8_t fuel_profile;
    uint8_t calibration_revision;
    fuel_validity_status_t validity_status;
    fuel_level_state_t level_state;
} fuel_calibration_result_t;

void fuel_calibration_init(void);
const fuel_system_model_t *fuel_calibration_get_system_model(void);
bool fuel_calibration_apply(const fuel_measurement_result_t *measurement, fuel_calibration_result_t *result);

#endif
