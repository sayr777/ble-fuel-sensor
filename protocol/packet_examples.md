# Примеры пакетов

## Пример кадра `ДУТ -> БНСО`

Поля:

- `protocol_version = 0x01`
- `message_type = 0x01`
- `sensor_id = 0x0123`
- `tank_id = 0x01`
- `system_model_revision = 0x01`
- `sequence = 0x17`
- `fuel_level_percent_x10 = 574`
- `fuel_volume_l_x10 = 3210`
- `fuel_temp_c_x10 = 185`
- `air_temp_c_x10 = 223`
- `tilt_pitch_deg_x10 = 12`
- `tilt_roll_deg_x10 = -4`
- `level_state = 0x01`
- `validity_status = 0x00`
- `confidence_percent = 95`
- `consumer_mask = 0x0001`
- `quality_flags = 0x0181`
- `fuel_profile = 0x02`
- `calibration_revision = 0x03`

Интерпретация:

- уровень топлива `57.4%`;
- объем `321.0 л`;
- температура топлива `18.5°C`;
- температура воздуха `22.3°C`;
- продольный уклон `1.2°`;
- поперечный уклон `-0.4°`;
- система распознала режим долива;
- достоверность результата высокая;
- измерение валидно;
- применен специальный профиль обработки долива или слива;
- применена температурная коррекция.

## Пример аналитической сводки по смене

- `transport_work_fuel_l = 92.4`
- `technology_cycle_fuel_l = 31.7`
- `idle_fuel_l = 12.9`
- `auxiliary_fuel_l = 4.2`
- `suspected_drain_fuel_l = 8.5`
- `total_shift_consumption_l = 149.7`

Интерпретация:

- основная часть топлива ушла на транспортную работу;
- существенная доля ушла на технологический цикл;
- холостой ход заметен и требует внимания;
- есть подозрительное изъятие топлива, которое нельзя объяснить штатным режимом работы.

## Пример команды `HELLO` от мобильного приложения

Request:

- `protocol_version = 0x01`
- `message_type = 0x10`
- `opcode = 0x01`
- `transaction_id = 0x2A`
- `payload_length = 0`

Response:

- `protocol_version = 0x01`
- `message_type = 0x11`
- `opcode = 0x01`
- `transaction_id = 0x2A`
- `status = 0x00`
- `fw_version = 1.0.3`
- `capability_flags = telemetry | diagnostics | calibration | temperature_compensation`
- `max_cal_points = 64`

## Пример чтения точки тарировочной таблицы

Request:

- `opcode = 0x31`
- `transaction_id = 0x35`
- `point_index = 12`

Response:

- `opcode = 0x31`
- `transaction_id = 0x35`
- `status = 0x00`
- `point_index = 12`
- `raw_code = 2486`
- `level_percent_x10 = 574`
- `volume_l_x10 = 3210`

## Пример записи параметров долива и слива

Request:

- `opcode = 0x37`
- `transaction_id = 0x41`
- `refill_threshold_l_x10 = 50`
- `drain_threshold_l_x10 = 40`
- `refill_confirm_time_s = 8`
- `drain_confirm_time_s = 12`
- `stable_filter_tc = 20`
- `refill_filter_tc = 6`
- `drain_filter_tc = 10`

Response:

- `opcode = 0x37`
- `transaction_id = 0x41`
- `status = 0x00`
