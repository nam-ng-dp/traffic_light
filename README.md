# Traffic Light Controller (`XNUCLEO-F103RB`)

STM32CubeIDE project for the **STM32F103RB** (`NUCLEO-F103RB`), migrated from `stm32_lab` (`lab4` branch) as the compile-ready baseline for the BTL hardware target.

## Project Structure

- `Core/Inc/scheduler.h`, `Core/Src/scheduler.c`: Cooperative task scheduler (`10 ms` tick via `TIM2`).
- `Core/Inc/software_timer.h`, `Core/Src/software_timer.c`: Software timer array (`MAX_TIMER = 10`).
- `Core/Inc/global.h`, `Core/Src/global.c`: Shared FSM state and traffic duration defaults.
- `Core/Inc/fsm_automatic.h/.c`, `fsm_manual.h/.c`, `fsm_setting.h/.c`: Automatic, Manual, and Duration-Setting FSMs.
- `Core/Inc/button.h`, `Core/Src/button.c`: Debounced 3-button driver mapped to `BTN_1`, `BTN_2`, `BTN_3` in `main.h` (`TODO` left for `BTN_4` and `BTN_USER`).
- `Core/Inc/traffic_light.h`, `Core/Src/traffic_light.c`: Public traffic-light API preserved; old 6-pin Lab 4 GPIO writes gated behind `#ifdef LAB4_6PIN_HARDWARE` with `#else` TODO stubs for `TL1_A/B` and `TL2_A/B`.
- `Core/Inc/led7seg.h`, `Core/Src/led7seg.c`: Public 7-segment API preserved; old `SEG0..6` / `EN0..3` GPIO writes gated behind `#ifdef LAB4_7SEG_HARDWARE` with `#else` TODO stubs for I2C LCD integration.
- `Core/Src/main.c`: CubeMX-generated `NUCLEO-F103RB` peripheral setup (`SystemClock_Config`, `MX_GPIO_Init`, `MX_I2C1_Init`, `MX_TIM2_Init`, `MX_USART2_UART_Init`) with `lab4` scheduler & FSM initialization inside `USER CODE` blocks.

## Building

Open or import `traffic_light` in STM32CubeIDE (workspace `/home/pam/Documents/school/stm32_workspace/XNUCLEO-F103RB`) and build the `Debug` configuration (`STM32F103RBTx` / `STM32F103xB`, linker script `STM32F103RBTX_FLASH.ld`).
