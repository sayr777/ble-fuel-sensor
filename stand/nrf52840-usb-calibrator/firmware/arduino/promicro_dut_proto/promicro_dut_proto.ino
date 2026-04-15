#include <bluefruit.h>

static const uint16_t COMPANY_ID = 0xFFFF;
static const uint16_t SENSOR_ID = 0x0123;

enum LevelState : uint8_t {
  LEVEL_STABLE = 0x00,
  LEVEL_REFILL = 0x01,
  LEVEL_DRAIN = 0x02
};

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

TelemetryFrame g_frame = {
  0x01,
  0x01,
  SENSOR_ID,
  0x00,
  574,
  3210,
  185,
  223,
  LEVEL_STABLE,
  0x0181,
  0x02,
  0x03,
  0x0000
};

unsigned long g_lastBurstMs = 0;
bool g_pendingBurst = false;
uint8_t g_burstCount = 0;

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

void print_help() {
  Serial.println("Commands:");
  Serial.println("  help");
  Serial.println("  status");
  Serial.println("  set level <value>");
  Serial.println("  set volume <value>");
  Serial.println("  set fueltemp <value>");
  Serial.println("  set airtemp <value>");
  Serial.println("  set state stable|refill|drain");
  Serial.println("  set flags <value>");
  Serial.println("  burst");
}

void print_status() {
  Serial.print("sensor_id=0x");
  Serial.println(g_frame.sensor_id, HEX);
  Serial.print("sequence=");
  Serial.println(g_frame.sequence);
  Serial.print("level_x10=");
  Serial.println(g_frame.fuel_level_percent_x10);
  Serial.print("volume_x10=");
  Serial.println(g_frame.fuel_volume_l_x10);
  Serial.print("fuel_temp_x10=");
  Serial.println(g_frame.fuel_temp_c_x10);
  Serial.print("air_temp_x10=");
  Serial.println(g_frame.air_temp_c_x10);
  Serial.print("state=");
  Serial.println(g_frame.level_state);
  Serial.print("quality_flags=0x");
  Serial.println(g_frame.quality_flags, HEX);
}

void update_advertising() {
  Bluefruit.Advertising.stop();
  Bluefruit.Advertising.clearData();
  Bluefruit.ScanResponse.clearData();

  g_frame.crc16 = 0;
  g_frame.crc16 = crc16_ccitt((const uint8_t*)&g_frame, sizeof(g_frame) - sizeof(g_frame.crc16));

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addManufacturerData(COMPANY_ID, (const uint8_t*)&g_frame, sizeof(g_frame));
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(160, 160);
  Bluefruit.Advertising.start(0);
}

void publish_frame() {
  g_frame.sequence++;
  update_advertising();
  Serial.print("advertised seq=");
  Serial.println(g_frame.sequence);
}

void handle_set_command(const String& key, const String& value) {
  if (key == "level") {
    g_frame.fuel_level_percent_x10 = (uint16_t)value.toInt();
  } else if (key == "volume") {
    g_frame.fuel_volume_l_x10 = (uint16_t)value.toInt();
  } else if (key == "fueltemp") {
    g_frame.fuel_temp_c_x10 = (int16_t)value.toInt();
  } else if (key == "airtemp") {
    g_frame.air_temp_c_x10 = (int16_t)value.toInt();
  } else if (key == "flags") {
    g_frame.quality_flags = (uint16_t)strtoul(value.c_str(), nullptr, 0);
  } else if (key == "state") {
    if (value == "stable") {
      g_frame.level_state = LEVEL_STABLE;
    } else if (value == "refill") {
      g_frame.level_state = LEVEL_REFILL;
    } else if (value == "drain") {
      g_frame.level_state = LEVEL_DRAIN;
    }
  }

  publish_frame();
}

void handle_serial() {
  if (!Serial.available()) {
    return;
  }

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line == "help") {
    print_help();
    return;
  }

  if (line == "status") {
    print_status();
    return;
  }

  if (line == "burst") {
    g_pendingBurst = true;
    g_burstCount = 3;
    Serial.println("burst armed");
    return;
  }

  if (line.startsWith("set ")) {
    int split = line.indexOf(' ', 4);
    if (split > 0) {
      String key = line.substring(4, split);
      String value = line.substring(split + 1);
      value.trim();
      handle_set_command(key, value);
      return;
    }
  }

  Serial.println("unknown command");
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("BFUEL-DUT");

  update_advertising();
  print_help();
  print_status();
}

void loop() {
  handle_serial();

  if (g_pendingBurst && g_burstCount > 0 && millis() - g_lastBurstMs >= 150) {
    g_lastBurstMs = millis();
    publish_frame();
    g_burstCount--;
    if (g_burstCount == 0) {
      g_pendingBurst = false;
    }
  }
}
