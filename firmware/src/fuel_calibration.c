#include "fuel_calibration.h"

static uint16_t g_previous_volume_l_x10;
static fuel_system_model_t g_system_model = {
    1,
    2,
    2,
    {
        {1, 2, 3},
        {2, 2, 1}
    },
    {
        {1, 1, 1},
        {2, 2, 2}
    }
};

void fuel_calibration_init(void)
{
    g_previous_volume_l_x10 = 0;
}

const fuel_system_model_t *fuel_calibration_get_system_model(void)
{
    return &g_system_model;
}

bool fuel_calibration_apply(const fuel_measurement_result_t *measurement, fuel_calibration_result_t *result)
{
    uint16_t current_volume_l_x10;

    if ((measurement == 0) || (result == 0)) {
        return false;
    }

    current_volume_l_x10 = (uint16_t)(measurement->raw_capacitance + 1162U);

    result->tank_id = g_system_model.tanks[0].tank_id;
    result->system_model_revision = g_system_model.system_model_revision;
    result->fuel_level_percent_x10 = (uint16_t)(current_volume_l_x10 / 6U);
    result->fuel_volume_l_x10 = current_volume_l_x10;
    result->fuel_temp_c_x10 = measurement->fuel_temp_c_x10;
    result->air_temp_c_x10 = measurement->air_temp_c_x10;
    result->tilt_pitch_deg_x10 = 0;
    result->tilt_roll_deg_x10 = 0;
    result->quality_flags = 0x0081;
    result->consumer_mask = 0x0001;
    result->confidence_percent = 95;
    result->fuel_profile = g_system_model.tanks[0].fuel_profile;
    result->calibration_revision = g_system_model.tanks[0].calibration_revision;
    result->validity_status = FUEL_VALIDITY_VALID;

    if (current_volume_l_x10 > (uint16_t)(g_previous_volume_l_x10 + 30U)) {
        result->level_state = FUEL_LEVEL_STATE_REFILL;
        result->quality_flags |= 0x0100;
    } else if (g_previous_volume_l_x10 > (uint16_t)(current_volume_l_x10 + 30U)) {
        result->level_state = FUEL_LEVEL_STATE_DRAIN;
        result->quality_flags |= 0x0100;
    } else {
        result->level_state = FUEL_LEVEL_STATE_STABLE;
    }

    g_previous_volume_l_x10 = current_volume_l_x10;
    return true;
}
