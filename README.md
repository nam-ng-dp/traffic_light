# Traffic Light Controller (`XNUCLEO-F103RB`)

STM32CubeIDE project for the **STM32F103RB** (`NUCLEO-F103RB`), implementing a 2-direction traffic light controller with cooperative task scheduling, I2C LCD display, DHT20 temperature/humidity sensing, and 4-button mode control.

## Project Modules

- `Core/Inc/scheduler.h`, `Core/Src/scheduler.c`: Cooperative task scheduler (`10 ms` tick via `TIM2`, supports periodic and one-shot tasks).
- `Core/Inc/software_timer.h`, `Core/Src/software_timer.c`: Software timer array (`MAX_TIMER = 10`) with `setTimer`, `clearTimer`, and `timer_run`.
- `Core/Inc/global.h`, `Core/Src/global.c`: Shared FSM states (`STATUS_AUTO_*`, `STATUS_MAN_*`, `STATUS_SET_*`), independent traffic durations (`red_time`, `amber_time`, `green_time`, `temp_time`), and sensor readings (`temperature`, `humidity`).
- `Core/Inc/fsm_automatic.h`, `Core/Src/fsm_automatic.c`: Automatic 4-phase traffic FSM with 1-second non-negative countdown updates and Button 1 transition to Manual mode from any phase.
- `Core/Inc/fsm_manual.h`, `Core/Src/fsm_manual.c`: Manual 4-phase traffic FSM supporting forward/backward phase stepping and direct return to Automatic mode.
- `Core/Inc/fsm_setting.h`, `Core/Src/fsm_setting.c`: Duration-Setting FSM (`STATUS_SET_RED`, `STATUS_SET_AMBER`, `STATUS_SET_GREEN`) with 2 Hz LED blinking and independent `red_time`, `amber_time`, and `green_time` configuration.
- `Core/Inc/button.h`, `Core/Src/button.c`: Debounced 5-input button driver for `BTN_1` (`PA8`/D7), `BTN_2` (`PA9`/D8), `BTN_3` (`PC7`/D9), `BTN_4` (`PB6`/D10), and `BTN_USER` (`PC13`).
- `Core/Inc/traffic_light.h`, `Core/Src/traffic_light.c`: Traffic light GPIO driver controlling 2-bit encoded outputs `TL1_A/B` (`PB3`, `PB5`) and `TL2_A/B` (`PB4`, `PB10`).
- `Core/Inc/dht20.h`, `Core/Src/dht20.c`: I2C1 DHT20 temperature and humidity sensor driver (`dht20_init`, `dht20_read`).
- `Core/Inc/lcd.h`, `Core/Src/lcd.c`: I2C1 LCD driver providing boot banner, Automatic countdown view, Manual phase view, Setting mode view, and periodic refresh (`lcd_update`).
- `Core/Src/main.c`: CubeMX peripheral initialization (`SystemClock_Config`, `MX_GPIO_Init`, `MX_I2C1_Init`, `MX_TIM2_Init`, `MX_USART2_UART_Init`), driver/FSM initialization, scheduler task registration, and `TIM2` interrupt callback.

## 4-Button Operation Guide

| Button | Pin / Header | Automatic Mode | Manual Mode | Setting Mode (`RED` / `AMBER` / `GREEN`) |
| --- | --- | --- | --- | --- |
| **Button 1** (`index 0`) | `PA8` / D7 | Switch to **Manual Mode** (preserving current phase) | Switch to **Setting Mode** (`SET_RED`) | Cycle `SET_RED` -> `SET_AMBER` -> `SET_GREEN` -> **Automatic Mode** |
| **Button 2** (`index 1`) | `PA9` / D8 | — | Next manual phase (`1 -> 2 -> 3 -> 4 -> 1`) | Increment `temp_time` (`1..99`, wraps `99 -> 1`) |
| **Button 3** (`index 2`) | `PC7` / D9 | — | Previous manual phase (`1 -> 4 -> 3 -> 2 -> 1`) | Decrement `temp_time` (`1..99`, wraps `1 -> 99`) |
| **Button 4** (`index 3`) | `PB6` / D10 | — | Return directly to **Automatic Mode** | Save `temp_time` to active color duration (`red_time` / `amber_time` / `green_time`) |
| **USER** (`index 4`) | `PC13` | — | — | Save `temp_time` to active color duration (alias for Button 4) |

Note: `red_time`, `amber_time`, and `green_time` are stored and applied independently without enforcing `red_time == amber_time + green_time`.

## Build and Flash

1. Open or import `traffic_light` in **STM32CubeIDE** (workspace `/home/pam/Documents/school/stm32_workspace/XNUCLEO-F103RB`).
2. Select the `Debug` configuration (`STM32F103RBTx` / `STM32F103xB`, linker script `STM32F103RBTX_FLASH.ld`) and build the project (**Project -> Build Project**, or regenerate/build `Debug/traffic_light.elf` via STM32CubeIDE managed make).
3. Connect the `NUCLEO-F103RB` board over USB (ST-LINK) and run **Run -> Debug** or **Run -> Run** in STM32CubeIDE to flash `Debug/traffic_light.elf` to the microcontroller.
