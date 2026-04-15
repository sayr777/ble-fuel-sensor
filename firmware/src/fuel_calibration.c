#include "fuel_calibration.h"

static uint16_t g_previous_volume_l_x10;

void fuel_calibration_init(void)
{
    g_previous_volume_l_x10 = 0;
}

bool fuel_calibration_apply(const fuel_measurement_result_t *measurement, fuel_calibration_result_t *result)
{
    uint16_t current_volume_l_x10;

    if ((measurement == 0) || (result == 0)) {
        return false;
    }

    current_volume_l_x10 = (uint16_t)(measurement->raw_capacitance + 1162U);

    result->fuel_level_percent_x10 = (uint16_t)(current_volume_l_x10 / 6U);
    result->fuel_volume_l_x10 = current_volume_l_x10;
    result->fuel_temp_c_x10 = measurement->fuel_temp_c_x10;
    result->air_temp_c_x10 = measurement->air_temp_c_x10;
    result->quality_flags = 0x0001;
    result->fuel_profile = 2;
    result->calibration_revision = 3;

    if (current_volume_l_x10 > (uint16_t)(g_previous_volume_l_x10 + 30U)) {
        result->level_state = FUEL_LEVEL_STATE_REFILL;
        result->quality_flags |= 0x0002;
    } else if (g_previous_volume_l_x10 > (uint16_t)(current_volume_l_x10 + 30U)) {
        result->level_state = FUEL_LEVEL_STATE_DRAIN;
        result->quality_flags |= 0x0004;
    } else {
        result->level_state = FUEL_LEVEL_STATE_STABLE;
    }

    g_previous_volume_l_x10 = current_volume_l_x10;
    return true;
}
