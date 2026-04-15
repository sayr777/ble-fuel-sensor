#ifndef FUEL_PROTOCOL_H
#define FUEL_PROTOCOL_H

#include <stdint.h>

#include "fuel_calibration.h"

typedef struct
{
    uint8_t protocol_version;
    uint8_t sequence;
    uint16_t sensor_id;
    uint16_t fuel_level_percent_x10;
    uint16_t fuel_volume_l_x10;
    int16_t fuel_temp_c_x10;
    int16_t air_temp_c_x10;
    uint8_t level_state;
    uint16_t quality_flags;
    uint8_t fuel_profile;
    uint8_t calibration_revision;
} fuel_telem_packet_t;

void fuel_protocol_init(void);
void fuel_protocol_build_packet(const fuel_calibration_result_t *result, fuel_telem_packet_t *packet);
void fuel_protocol_publish(const fuel_telem_packet_t *packet);

#endif
