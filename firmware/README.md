# Прошивка

Прошивка должна обеспечивать:

- инициализацию `nRF52840`;
- измерение емкостного канала;
- измерение температуры топлива и воздуха;
- цифровую фильтрацию и осечку шумов;
- применение расширенной тарировочной таблицы;
- применение таблицы тарировки;
- выбор и хранение профиля топлива;
- отдельную логику обработки доливов и сливов;
- BLE-передачу данных в телематический блок;
- GATT-интерфейс для Android;
- сервисный канал через `USB`;
- хранение настроек в `flash/NVM`.

## Стартовая программная база

Для каркаса проекта предлагается опираться на:

- `nRF5 SDK 17.1.x`
- SoftDevice `S140`
- `app_timer`, `fds`, `nrfx_saadc`, `nrfx_usbd`

На первом этапе прошивку стоит держать совместимой с двумя вариантами прототипа:

- `Promicro NRF52840 / nice!nano v2 compatible`
- `Super 52840 / Seeed XIAO BLE compatible`

Это значит, что пин-аут измерительного тракта, USB-сервиса и статуса платы лучше вынести в платформенный слой.

## Каркас исходников

- [src/main.c](src/main.c)
- [src/fuel_app.c](src/fuel_app.c)
- [src/fuel_app.h](src/fuel_app.h)
- [src/fuel_measurement.c](src/fuel_measurement.c)
- [src/fuel_measurement.h](src/fuel_measurement.h)
- [src/fuel_calibration.c](src/fuel_calibration.c)
- [src/fuel_calibration.h](src/fuel_calibration.h)
- [src/fuel_protocol.c](src/fuel_protocol.c)
- [src/fuel_protocol.h](src/fuel_protocol.h)
- [src/fuel_platform.h](src/fuel_platform.h)
- [src/fuel_platform_nrf5_sdk.c](src/fuel_platform_nrf5_sdk.c)

Подробности по архитектуре стартовой реализации вынесены в:

- [nrf5-sdk-fuel-sensor.md](nrf5-sdk-fuel-sensor.md)
