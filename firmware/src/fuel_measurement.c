#include "fuel_measurement.h"

void fuel_measurement_init(void)
{
}

bool fuel_measurement_poll(fuel_measurement_result_t *result)
{
    if (result == 0) {
        return false;
    }

    result->raw_capacitance = 2048;
    result->fuel_temp_c_x10 = 185;
    result->air_temp_c_x10 = 223;
    result->fuel_level_percent_x10 = 574;
    result->fuel_volume_l_x10 = 3210;
    result->quality_flags = 0x0001;

    return true;
}
