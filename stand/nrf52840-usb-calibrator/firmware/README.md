# Прошивки стенда ProMicro

В этом разделе лежат минимальные `Arduino`-скетчи для двухплатного прототипа на `ProMicro NRF52840`.

## Состав

- [arduino/promicro_dut_proto/promicro_dut_proto.ino](arduino/promicro_dut_proto/promicro_dut_proto.ino) - прототип `ДУТ`;
- [arduino/promicro_bnso_emulator/promicro_bnso_emulator.ino](arduino/promicro_bnso_emulator/promicro_bnso_emulator.ino) - эмулятор `БНСО`.

## Что требуется

- `Arduino IDE 2.x`;
- `Adafruit nRF52 Boards`;
- `Bluefruit52Lib`.

## Назначение скетчей

### `promicro_dut_proto`

Скетч:

- публикует телеметрию в `BLE advertising manufacturer data`;
- позволяет менять значения через `USB Serial`;
- помогает прогонять сценарии `stable/refill/drain`.

### `promicro_bnso_emulator`

Скетч:

- сканирует BLE advertising;
- ищет кадры `ДУТ`;
- декодирует поля;
- печатает журнал в `Serial`.

## Быстрый старт

1. Открыть `promicro_dut_proto.ino` и прошить первую плату.
2. Открыть `promicro_bnso_emulator.ino` и прошить вторую плату.
3. Открыть два окна `Serial Monitor`.
4. На стороне `ДУТ` выполнить `help`.
5. Убедиться, что `БНСО` печатает принятые кадры.
