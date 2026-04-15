#include <bluefruit.h>

static const uint16_t COMPANY_ID = 0xFFFF;

struct TelemetryFrame {
  uint8_t protocol_version;
  uint8_t message_type;
  uint16_t sensor_id;
  uint8_t sequence;
  uint16_t fuel_level_percent_x10;
  uint16_t fuel_volume_l_x10;
  int16_t fuel_temp_c_x10;
  int16_t air_temp_c_x10;
  uint8_t level_state;
  uint16_t quality_flags;
  uint8_t fuel_profile;
  uint8_t calibration_revision;
  uint16_t crc16;
};

uint8_t g_lastSequence = 0xFF;
bool g_seenAny = false;

uint16_t crc16_ccitt(const uint8_t* data, size_t len) {
  uint16_t crc = 0xFFFF;

  while (len--) {
    crc ^= ((uint16_t)(*data++)) << 8;
    for (uint8_t i = 0; i < 8; ++i) {
      if (crc & 0x8000) {
        crc = (uint16_t)((crc << 1) ^ 0x1021);
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}

void printFrame(const TelemetryFrame& frame, int8_t rssi, bool duplicate) {
  Serial.print("rssi=");
  Serial.print(rssi);
  Serial.print(" sensor_id=0x");
  Serial.print(frame.sensor_id, HEX);
  Serial.print(" seq=");
  Serial.print(frame.sequence);
  Serial.print(" level=");
  Serial.print(frame.fuel_level_percent_x10);
  Serial.print(" volume=");
  Serial.print(frame.fuel_volume_l_x10);
  Serial.print(" fuelTemp=");
  Serial.print(frame.fuel_temp_c_x10);
  Serial.print(" airTemp=");
  Serial.print(frame.air_temp_c_x10);
  Serial.print(" state=");
  Serial.print(frame.level_state);
  Serial.print(" flags=0x");
  Serial.print(frame.quality_flags, HEX);
  Serial.print(" calRev=");
  Serial.print(frame.calibration_revision);
  Serial.print(" duplicate=");
  Serial.println(duplicate ? "yes" : "no");
}

void scan_callback(ble_gap_evt_adv_report_t* report) {
  uint8_t buffer[64];
  int len = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, buffer, sizeof(buffer));

  if (len < (int)(sizeof(uint16_t) + sizeof(TelemetryFrame))) {
    Bluefruit.Scanner.resume();
    return;
  }

  uint16_t companyId;
  memcpy(&companyId, buffer, sizeof(companyId));
  if (companyId != COMPANY_ID) {
    Bluefruit.Scanner.resume();
    return;
  }

  TelemetryFrame frame;
  memcpy(&frame, buffer + sizeof(companyId), sizeof(frame));

  uint16_t receivedCrc = frame.crc16;
  frame.crc16 = 0;
  uint16_t calculatedCrc = crc16_ccitt((const uint8_t*)&frame, sizeof(frame) - sizeof(frame.crc16));

  if (receivedCrc != calculatedCrc) {
    Serial.print("crc_error expected=0x");
    Serial.print(calculatedCrc, HEX);
    Serial.print(" got=0x");
    Serial.println(receivedCrc, HEX);
    Bluefruit.Scanner.resume();
    return;
  }

  bool duplicate = g_seenAny && (frame.sequence == g_lastSequence);
  g_lastSequence = frame.sequence;
  g_seenAny = true;

  printFrame(frame, report->rssi, duplicate);
  Bluefruit.Scanner.resume();
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Bluefruit.begin(0, 1);
  Bluefruit.setName("BFUEL-BNSO");
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);

  Serial.println("BNSO emulator scanning...");
}

void loop() {
}
