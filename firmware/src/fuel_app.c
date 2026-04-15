#include "fuel_app.h"

#include "fuel_calibration.h"
#include "fuel_measurement.h"
#include "fuel_protocol.h"

static fuel_measurement_result_t g_last_result;
static fuel_calibration_result_t g_last_calibrated;
static fuel_telem_packet_t g_last_packet;

void fuel_app_init(void)
{
    fuel_measurement_init();
    fuel_calibration_init();
    fuel_protocol_init();
}

void fuel_app_process(void)
{
    if (!fuel_measurement_poll(&g_last_result)) {
        return;
    }

    if (!fuel_calibration_apply(&g_last_result, &g_last_calibrated)) {
        return;
    }

    fuel_protocol_build_packet(&g_last_calibrated, &g_last_packet);
    fuel_protocol_publish(&g_last_packet);
}
