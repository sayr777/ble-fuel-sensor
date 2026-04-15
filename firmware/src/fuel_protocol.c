#include "fuel_protocol.h"

static uint8_t g_sequence;

void fuel_protocol_init(void)
{
    g_sequence = 0;
}

void fuel_protocol_build_packet(const fuel_calibration_result_t *result, fuel_telem_packet_t *packet)
{
    if ((result == 0) || (packet == 0)) {
        return;
    }

    packet->protocol_version = 1;
    packet->message_type = 1;
    packet->sequence = g_sequence++;
    packet->sensor_id = 1;
    packet->tank_id = result->tank_id;
    packet->system_model_revision = result->system_model_revision;
    packet->fuel_level_percent_x10 = result->fuel_level_percent_x10;
    packet->fuel_volume_l_x10 = result->fuel_volume_l_x10;
    packet->fuel_temp_c_x10 = result->fuel_temp_c_x10;
    packet->air_temp_c_x10 = result->air_temp_c_x10;
    packet->tilt_pitch_deg_x10 = result->tilt_pitch_deg_x10;
    packet->tilt_roll_deg_x10 = result->tilt_roll_deg_x10;
    packet->level_state = (uint8_t)result->level_state;
    packet->validity_status = (uint8_t)result->validity_status;
    packet->confidence_percent = result->confidence_percent;
    packet->consumer_mask = result->consumer_mask;
    packet->quality_flags = result->quality_flags;
    packet->fuel_profile = result->fuel_profile;
    packet->calibration_revision = result->calibration_revision;
}

void fuel_protocol_publish(const fuel_telem_packet_t *packet)
{
    (void)packet;
}
