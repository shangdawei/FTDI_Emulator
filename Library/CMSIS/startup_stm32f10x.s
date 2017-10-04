;******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
;* File Name          : startup_stm32f1xx.s
;* Author             : MCD Application Team
;* Version            : V4.0.0
;* Date               : 16-December-2014
;* Description        : STM32F1XX devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Configure the clock system
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address.
;*                      After Reset the Cortex-M3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;
       MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
#if defined (STM32F100xB)
        DCD     WWDG_IRQHandler               ; Window Watchdog
        DCD     PVD_IRQHandler                ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler             ; Tamper
        DCD     RTC_IRQHandler                ; RTC
        DCD     FLASH_IRQHandler              ; Flash
        DCD     RCC_IRQHandler                ; RCC
        DCD     EXTI0_IRQHandler              ; EXTI Line 0
        DCD     EXTI1_IRQHandler              ; EXTI Line 1
        DCD     EXTI2_IRQHandler              ; EXTI Line 2
        DCD     EXTI3_IRQHandler              ; EXTI Line 3
        DCD     EXTI4_IRQHandler              ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler      ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler      ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler      ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler      ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler      ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler      ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler      ; DMA1 Channel 7
        DCD     ADC1_IRQHandler               ; ADC1
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     EXTI9_5_IRQHandler            ; EXTI Line 9..5
        DCD     TIM1_BRK_TIM15_IRQHandler     ; TIM1 Break and TIM15
        DCD     TIM1_UP_TIM16_IRQHandler      ; TIM1 Update and TIM16
        DCD     TIM1_TRG_COM_TIM17_IRQHandler ; TIM1 Trigger and Commutation and TIM17
        DCD     TIM1_CC_IRQHandler            ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler               ; TIM2
        DCD     TIM3_IRQHandler               ; TIM3
        DCD     TIM4_IRQHandler               ; TIM4
        DCD     I2C1_EV_IRQHandler            ; I2C1 Event
        DCD     I2C1_ER_IRQHandler            ; I2C1 Error
        DCD     I2C2_EV_IRQHandler            ; I2C2 Event
        DCD     I2C2_ER_IRQHandler            ; I2C2 Error
        DCD     SPI1_IRQHandler               ; SPI1
        DCD     SPI2_IRQHandler               ; SPI2
        DCD     USART1_IRQHandler             ; USART1
        DCD     USART2_IRQHandler             ; USART2
        DCD     USART3_IRQHandler             ; USART3
        DCD     EXTI15_10_IRQHandler          ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler           ; RTC Alarm through EXTI Line
        DCD     CEC_IRQHandler                ; HDMI-CEC
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     TIM6_DAC_IRQHandler           ; TIM6 and DAC underrun
        DCD     TIM7_IRQHandler               ; TIM7
#endif /* #if defined (STM32F100xB) */

#if defined (STM32F100xE)
        DCD     WWDG_IRQHandler               ; Window Watchdog
        DCD     PVD_IRQHandler                ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler             ; Tamper
        DCD     RTC_IRQHandler                ; RTC
        DCD     FLASH_IRQHandler              ; Flash
        DCD     RCC_IRQHandler                ; RCC
        DCD     EXTI0_IRQHandler              ; EXTI Line 0
        DCD     EXTI1_IRQHandler              ; EXTI Line 1
        DCD     EXTI2_IRQHandler              ; EXTI Line 2
        DCD     EXTI3_IRQHandler              ; EXTI Line 3
        DCD     EXTI4_IRQHandler              ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler      ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler      ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler      ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler      ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler      ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler      ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler      ; DMA1 Channel 7
        DCD     ADC1_IRQHandler               ; ADC1
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     EXTI9_5_IRQHandler            ; EXTI Line 9..5
        DCD     TIM1_BRK_TIM15_IRQHandler     ; TIM1 Break and TIM15
        DCD     TIM1_UP_TIM16_IRQHandler      ; TIM1 Update and TIM16
        DCD     TIM1_TRG_COM_TIM17_IRQHandler ; TIM1 Trigger and Commutation and TIM17
        DCD     TIM1_CC_IRQHandler            ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler               ; TIM2
        DCD     TIM3_IRQHandler               ; TIM3
        DCD     TIM4_IRQHandler               ; TIM4
        DCD     I2C1_EV_IRQHandler            ; I2C1 Event
        DCD     I2C1_ER_IRQHandler            ; I2C1 Error
        DCD     I2C2_EV_IRQHandler            ; I2C2 Event
        DCD     I2C2_ER_IRQHandler            ; I2C2 Error
        DCD     SPI1_IRQHandler               ; SPI1
        DCD     SPI2_IRQHandler               ; SPI2
        DCD     USART1_IRQHandler             ; USART1
        DCD     USART2_IRQHandler             ; USART2
        DCD     USART3_IRQHandler             ; USART3
        DCD     EXTI15_10_IRQHandler          ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler          ; RTC Alarm through EXTI Line
        DCD     CEC_IRQHandler                ; HDMI CEC
        DCD     TIM12_IRQHandler              ; TIM12
        DCD     TIM13_IRQHandler              ; TIM13
        DCD     TIM14_IRQHandler              ; TIM14
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     TIM5_IRQHandler               ; TIM5
        DCD     SPI3_IRQHandler               ; SPI3
        DCD     UART4_IRQHandler              ; UART4
        DCD     UART5_IRQHandler              ; UART5
        DCD     TIM6_DAC_IRQHandler           ; TIM6 and DAC underrun
        DCD     TIM7_IRQHandler               ; TIM7
        DCD     DMA2_Channel1_IRQHandler      ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler      ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler      ; DMA2 Channel3
        DCD     DMA2_Channel4_5_IRQHandler    ; DMA2 Channel4 & Channel5 if MISC_REMAP is not set
                                              ; or DMA2 Channel4 if MISC_REMAP is set
        DCD     DMA2_Channel5_IRQHandler      ; DMA2 Channel5 if MISC_REMAP is set
#endif /* #if defined (STM32F100xE) */

#if defined (STM32F101x6)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_IRQHandler         ; ADC1
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     0                         ; Reserved
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     0                         ; Reserved
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     0                         ; Reserved
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     0                         ; Reserved
#endif /* #if defined (STM32F101x6) */

#if defined (STM32F101xB)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_IRQHandler         ; ADC1
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     TIM4_IRQHandler           ; TIM4
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     I2C2_EV_IRQHandler        ; I2C2 Event
        DCD     I2C2_ER_IRQHandler        ; I2C2 Error
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     SPI2_IRQHandler           ; SPI2
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     USART3_IRQHandler         ; USART3
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     0                         ; Reserved
#endif /* #if defined (STM32F101xB) */

#if defined (STM32F101xE)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_IRQHandler         ; ADC1
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     TIM4_IRQHandler           ; TIM4
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     I2C2_EV_IRQHandler        ; I2C2 Event
        DCD     I2C2_ER_IRQHandler        ; I2C2 Error
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     SPI2_IRQHandler           ; SPI2
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     USART3_IRQHandler         ; USART3
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     FSMC_IRQHandler           ; FSMC
        DCD     0                         ; Reserved
        DCD     TIM5_IRQHandler           ; TIM5
        DCD     SPI3_IRQHandler           ; SPI3
        DCD     UART4_IRQHandler          ; UART4
        DCD     UART5_IRQHandler          ; UART5
        DCD     TIM6_IRQHandler           ; TIM6
        DCD     TIM7_IRQHandler           ; TIM7
        DCD     DMA2_Channel1_IRQHandler  ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler  ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler  ; DMA2 Channel3
        DCD     DMA2_Channel4_5_IRQHandler ; DMA2 Channel4 & Channel5
#endif /* #if defined (STM32F101xE) */

#if defined (STM32F101xG)
        DCD     WWDG_IRQHandler               ; Window Watchdog
        DCD     PVD_IRQHandler                ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler             ; Tamper
        DCD     RTC_IRQHandler                ; RTC
        DCD     FLASH_IRQHandler              ; Flash
        DCD     RCC_IRQHandler                ; RCC
        DCD     EXTI0_IRQHandler              ; EXTI Line 0
        DCD     EXTI1_IRQHandler              ; EXTI Line 1
        DCD     EXTI2_IRQHandler              ; EXTI Line 2
        DCD     EXTI3_IRQHandler              ; EXTI Line 3
        DCD     EXTI4_IRQHandler              ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler      ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler      ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler      ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler      ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler      ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler      ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler      ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler             ; ADC1 & ADC2
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler            ; EXTI Line 9..5
        DCD     TIM9_IRQHandler               ; TIM9
        DCD     TIM10_IRQHandler              ; TIM10
        DCD     TIM11_IRQHandler              ; TIM11
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler               ; TIM2
        DCD     TIM3_IRQHandler               ; TIM3
        DCD     TIM4_IRQHandler               ; TIM4
        DCD     I2C1_EV_IRQHandler            ; I2C1 Event
        DCD     I2C1_ER_IRQHandler            ; I2C1 Error
        DCD     I2C2_EV_IRQHandler            ; I2C2 Event
        DCD     I2C2_ER_IRQHandler            ; I2C2 Error
        DCD     SPI1_IRQHandler               ; SPI1
        DCD     SPI2_IRQHandler               ; SPI2
        DCD     USART1_IRQHandler             ; USART1
        DCD     USART2_IRQHandler             ; USART2
        DCD     USART3_IRQHandler             ; USART3
        DCD     EXTI15_10_IRQHandler          ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler           ; RTC Alarm through EXTI Line
        DCD     0                         ; Reserved
        DCD     TIM12_IRQHandler              ; TIM12
        DCD     TIM13_IRQHandler              ; TIM13
        DCD     TIM14_IRQHandler              ; TIM14
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     FSMC_IRQHandler               ; FSMC
        DCD     0                         ; Reserved
        DCD     TIM5_IRQHandler               ; TIM5
        DCD     SPI3_IRQHandler               ; SPI3
        DCD     UART4_IRQHandler              ; UART4
        DCD     UART5_IRQHandler              ; UART5
        DCD     TIM6_IRQHandler               ; TIM6
        DCD     TIM7_IRQHandler               ; TIM7
        DCD     DMA2_Channel1_IRQHandler      ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler      ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler      ; DMA2 Channel3
        DCD     DMA2_Channel4_5_IRQHandler    ; DMA2 Channel4 & Channel5
#endif /* #if defined (STM32F101xG) */

#if defined (STM32F102x6)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_IRQHandler           ; ADC1
        DCD     USB_HP_IRQHandler  	      ; USB High Priority
        DCD     USB_LP_IRQHandler         ; USB Low  Priority
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     0                         ; Reserved
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     0                         ; Reserved
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     0                         ; Reserved
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler      ; USB Wakeup from suspend
#endif /* #if defined (STM32F102x6) */

#if defined (STM32F102xB)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_IRQHandler           ; ADC1
        DCD     USB_HP_IRQHandler  	      ; USB High Priority
        DCD     USB_LP_IRQHandler         ; USB Low  Priority
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     TIM4_IRQHandler           ; TIM4
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     I2C2_EV_IRQHandler        ; I2C2 Event
        DCD     I2C2_ER_IRQHandler        ; I2C2 Error
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     SPI2_IRQHandler           ; SPI2
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     USART3_IRQHandler         ; USART3
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler      ; USB Wakeup from suspend
#endif /* #if defined (STM32F102xB) */

#if defined (STM32F103x6)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler         ; ADC1 & ADC2
        DCD     USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
        DCD     USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
        DCD     CAN1_RX1_IRQHandler       ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler       ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler       ; TIM1 Break
        DCD     TIM1_UP_IRQHandler        ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler   ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler        ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     0                         ; Reserved
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     0                         ; Reserved
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     0                         ; Reserved
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler      ; USB Wakeup from suspend
#endif /* #if defined (STM32F103x6) */

#if defined (STM32F103xB)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler         ; ADC1 & ADC2
        DCD     USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
        DCD     USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
        DCD     CAN1_RX1_IRQHandler       ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler       ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler       ; TIM1 Break
        DCD     TIM1_UP_IRQHandler        ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler   ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler        ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     TIM4_IRQHandler           ; TIM4
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     I2C2_EV_IRQHandler        ; I2C2 Event
        DCD     I2C2_ER_IRQHandler        ; I2C2 Error
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     SPI2_IRQHandler           ; SPI2
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     USART3_IRQHandler         ; USART3
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler      ; USB Wakeup from suspend
#endif /* #if defined (STM32F103xB) */

#if defined (STM32F103xE)
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler         ; Tamper
        DCD     RTC_IRQHandler            ; RTC
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_IRQHandler          ; EXTI Line 0
        DCD     EXTI1_IRQHandler          ; EXTI Line 1
        DCD     EXTI2_IRQHandler          ; EXTI Line 2
        DCD     EXTI3_IRQHandler          ; EXTI Line 3
        DCD     EXTI4_IRQHandler          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler         ; ADC1 & ADC2
        DCD     USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
        DCD     USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
        DCD     CAN1_RX1_IRQHandler       ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler       ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler        ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler       ; TIM1 Break
        DCD     TIM1_UP_IRQHandler        ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler   ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler        ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler           ; TIM2
        DCD     TIM3_IRQHandler           ; TIM3
        DCD     TIM4_IRQHandler           ; TIM4
        DCD     I2C1_EV_IRQHandler        ; I2C1 Event
        DCD     I2C1_ER_IRQHandler        ; I2C1 Error
        DCD     I2C2_EV_IRQHandler        ; I2C2 Event
        DCD     I2C2_ER_IRQHandler        ; I2C2 Error
        DCD     SPI1_IRQHandler           ; SPI1
        DCD     SPI2_IRQHandler           ; SPI2
        DCD     USART1_IRQHandler         ; USART1
        DCD     USART2_IRQHandler         ; USART2
        DCD     USART3_IRQHandler         ; USART3
        DCD     EXTI15_10_IRQHandler      ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler      ; USB Wakeup from suspend
        DCD     TIM8_BRK_IRQHandler       ; TIM8 Break
        DCD     TIM8_UP_IRQHandler        ; TIM8 Update
        DCD     TIM8_TRG_COM_IRQHandler   ; TIM8 Trigger and Commutation
        DCD     TIM8_CC_IRQHandler        ; TIM8 Capture Compare
        DCD     ADC3_IRQHandler           ; ADC3
        DCD     FSMC_IRQHandler           ; FSMC
        DCD     SDIO_IRQHandler           ; SDIO
        DCD     TIM5_IRQHandler           ; TIM5
        DCD     SPI3_IRQHandler           ; SPI3
        DCD     UART4_IRQHandler          ; UART4
        DCD     UART5_IRQHandler          ; UART5
        DCD     TIM6_IRQHandler           ; TIM6
        DCD     TIM7_IRQHandler           ; TIM7
        DCD     DMA2_Channel1_IRQHandler  ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler  ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler  ; DMA2 Channel3
        DCD     DMA2_Channel4_5_IRQHandler ; DMA2 Channel4 & Channel5
#endif /* #if defined (STM32F103xE) */

#if defined (STM32F103xG)
        DCD     WWDG_IRQHandler               ; Window Watchdog
        DCD     PVD_IRQHandler                ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler             ; Tamper
        DCD     RTC_IRQHandler                ; RTC
        DCD     FLASH_IRQHandler              ; Flash
        DCD     RCC_IRQHandler                ; RCC
        DCD     EXTI0_IRQHandler              ; EXTI Line 0
        DCD     EXTI1_IRQHandler              ; EXTI Line 1
        DCD     EXTI2_IRQHandler              ; EXTI Line 2
        DCD     EXTI3_IRQHandler              ; EXTI Line 3
        DCD     EXTI4_IRQHandler              ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler      ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler      ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler      ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler      ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler      ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler      ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler      ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler             ; ADC1 & ADC2
        DCD     USB_HP_CAN1_TX_IRQHandler     ; USB High Priority or CAN1 TX
        DCD     USB_LP_CAN1_RX0_IRQHandler    ; USB Low  Priority or CAN1 RX0
        DCD     CAN1_RX1_IRQHandler           ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler           ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler            ; EXTI Line 9..5
        DCD     TIM1_BRK_TIM9_IRQHandler      ; TIM1 Break and TIM9
        DCD     TIM1_UP_TIM10_IRQHandler      ; TIM1 Update and TIM10
        DCD     TIM1_TRG_COM_TIM11_IRQHandler ; TIM1 Trigger and Commutation and TIM11
        DCD     TIM1_CC_IRQHandler            ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler               ; TIM2
        DCD     TIM3_IRQHandler               ; TIM3
        DCD     TIM4_IRQHandler               ; TIM4
        DCD     I2C1_EV_IRQHandler            ; I2C1 Event
        DCD     I2C1_ER_IRQHandler            ; I2C1 Error
        DCD     I2C2_EV_IRQHandler            ; I2C2 Event
        DCD     I2C2_ER_IRQHandler            ; I2C2 Error
        DCD     SPI1_IRQHandler               ; SPI1
        DCD     SPI2_IRQHandler               ; SPI2
        DCD     USART1_IRQHandler             ; USART1
        DCD     USART2_IRQHandler             ; USART2
        DCD     USART3_IRQHandler             ; USART3
        DCD     EXTI15_10_IRQHandler          ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler           ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQHandler          ; USB Wakeup from suspend
        DCD     TIM8_BRK_TIM12_IRQHandler     ; TIM8 Break and TIM12
        DCD     TIM8_UP_TIM13_IRQHandler      ; TIM8 Update and TIM13
        DCD     TIM8_TRG_COM_TIM14_IRQHandler ; TIM8 Trigger and Commutation and TIM14
        DCD     TIM8_CC_IRQHandler            ; TIM8 Capture Compare
        DCD     ADC3_IRQHandler               ; ADC3
        DCD     FSMC_IRQHandler               ; FSMC
        DCD     SDIO_IRQHandler               ; SDIO
        DCD     TIM5_IRQHandler               ; TIM5
        DCD     SPI3_IRQHandler               ; SPI3
        DCD     UART4_IRQHandler              ; UART4
        DCD     UART5_IRQHandler              ; UART5
        DCD     TIM6_IRQHandler               ; TIM6
        DCD     TIM7_IRQHandler               ; TIM7
        DCD     DMA2_Channel1_IRQHandler      ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler      ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler      ; DMA2 Channel3
        DCD     DMA2_Channel4_5_IRQHandler    ; DMA2 Channel4 & Channel5
#endif /* #if defined (STM32F103xG) */

#if defined (STM32F105xC)
        DCD     WWDG_IRQHandler            ; Window Watchdog
        DCD     PVD_IRQHandler             ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler          ; Tamper
        DCD     RTC_IRQHandler             ; RTC
        DCD     FLASH_IRQHandler           ; Flash
        DCD     RCC_IRQHandler             ; RCC
        DCD     EXTI0_IRQHandler           ; EXTI Line 0
        DCD     EXTI1_IRQHandler           ; EXTI Line 1
        DCD     EXTI2_IRQHandler           ; EXTI Line 2
        DCD     EXTI3_IRQHandler           ; EXTI Line 3
        DCD     EXTI4_IRQHandler           ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler          ; ADC1 and ADC2
        DCD     CAN1_TX_IRQHandler         ; CAN1 TX
        DCD     CAN1_RX0_IRQHandler        ; CAN1 RX0
        DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler         ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler        ; TIM1 Break
        DCD     TIM1_UP_IRQHandler         ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler            ; TIM2
        DCD     TIM3_IRQHandler            ; TIM3
        DCD     TIM4_IRQHandler            ; TIM4
        DCD     I2C1_EV_IRQHandler         ; I2C1 Event
        DCD     I2C1_ER_IRQHandler         ; I2C1 Error
        DCD     I2C2_EV_IRQHandler         ; I2C2 Event
        DCD     I2C2_ER_IRQHandler         ; I2C1 Error
        DCD     SPI1_IRQHandler            ; SPI1
        DCD     SPI2_IRQHandler            ; SPI2
        DCD     USART1_IRQHandler          ; USART1
        DCD     USART2_IRQHandler          ; USART2
        DCD     USART3_IRQHandler          ; USART3
        DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler        ; RTC alarm through EXTI line
        DCD     OTG_FS_WKUP_IRQHandler     ; USB OTG FS Wakeup through EXTI line
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     TIM5_IRQHandler            ; TIM5
        DCD     SPI3_IRQHandler            ; SPI3
        DCD     UART4_IRQHandler           ; UART4
        DCD     UART5_IRQHandler           ; UART5
        DCD     TIM6_IRQHandler            ; TIM6
        DCD     TIM7_IRQHandler            ; TIM7
        DCD     DMA2_Channel1_IRQHandler   ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler   ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler   ; DMA2 Channel3
        DCD     DMA2_Channel4_IRQHandler   ; DMA2 Channel4
        DCD     DMA2_Channel5_IRQHandler   ; DMA2 Channel5
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     CAN2_TX_IRQHandler         ; CAN2 TX
        DCD     CAN2_RX0_IRQHandler        ; CAN2 RX0
        DCD     CAN2_RX1_IRQHandler        ; CAN2 RX1
        DCD     CAN2_SCE_IRQHandler        ; CAN2 SCE
        DCD     OTG_FS_IRQHandler          ; USB OTG FS
#endif /* #if defined (STM32F105xC) */

#if defined (STM32F107xC)
        DCD     WWDG_IRQHandler            ; Window Watchdog
        DCD     PVD_IRQHandler             ; PVD through EXTI Line detect
        DCD     TAMPER_IRQHandler          ; Tamper
        DCD     RTC_IRQHandler             ; RTC
        DCD     FLASH_IRQHandler           ; Flash
        DCD     RCC_IRQHandler             ; RCC
        DCD     EXTI0_IRQHandler           ; EXTI Line 0
        DCD     EXTI1_IRQHandler           ; EXTI Line 1
        DCD     EXTI2_IRQHandler           ; EXTI Line 2
        DCD     EXTI3_IRQHandler           ; EXTI Line 3
        DCD     EXTI4_IRQHandler           ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
        DCD     ADC1_2_IRQHandler          ; ADC1 and ADC2
        DCD     CAN1_TX_IRQHandler         ; CAN1 TX
        DCD     CAN1_RX0_IRQHandler        ; CAN1 RX0
        DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler         ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler        ; TIM1 Break
        DCD     TIM1_UP_IRQHandler         ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler            ; TIM2
        DCD     TIM3_IRQHandler            ; TIM3
        DCD     TIM4_IRQHandler            ; TIM4
        DCD     I2C1_EV_IRQHandler         ; I2C1 Event
        DCD     I2C1_ER_IRQHandler         ; I2C1 Error
        DCD     I2C2_EV_IRQHandler         ; I2C2 Event
        DCD     I2C2_ER_IRQHandler         ; I2C1 Error
        DCD     SPI1_IRQHandler            ; SPI1
        DCD     SPI2_IRQHandler            ; SPI2
        DCD     USART1_IRQHandler          ; USART1
        DCD     USART2_IRQHandler          ; USART2
        DCD     USART3_IRQHandler          ; USART3
        DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
        DCD     RTC_Alarm_IRQHandler        ; RTC alarm through EXTI line
        DCD     OTG_FS_WKUP_IRQHandler     ; USB OTG FS Wakeup through EXTI line
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     TIM5_IRQHandler            ; TIM5
        DCD     SPI3_IRQHandler            ; SPI3
        DCD     UART4_IRQHandler           ; UART4
        DCD     UART5_IRQHandler           ; UART5
        DCD     TIM6_IRQHandler            ; TIM6
        DCD     TIM7_IRQHandler            ; TIM7
        DCD     DMA2_Channel1_IRQHandler   ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQHandler   ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQHandler   ; DMA2 Channel3
        DCD     DMA2_Channel4_IRQHandler   ; DMA2 Channel4
        DCD     DMA2_Channel5_IRQHandler   ; DMA2 Channel5
        DCD     ETH_IRQHandler             ; Ethernet
        DCD     ETH_WKUP_IRQHandler        ; Ethernet Wakeup through EXTI line
        DCD     CAN2_TX_IRQHandler         ; CAN2 TX
        DCD     CAN2_RX0_IRQHandler        ; CAN2 RX0
        DCD     CAN2_RX1_IRQHandler        ; CAN2 RX1
        DCD     CAN2_SCE_IRQHandler        ; CAN2 SCE
        DCD     OTG_FS_IRQHandler          ; USB OTG FS
#endif /* #if defined (STM32F107xC) */

        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK ADC1_2_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC3_IRQHandler
        PUBWEAK BusFault_Handler
        PUBWEAK CAN1_RX0_IRQHandler
        PUBWEAK CAN1_RX1_IRQHandler
        PUBWEAK CAN1_SCE_IRQHandler
        PUBWEAK CAN1_TX_IRQHandler
        PUBWEAK CAN2_RX0_IRQHandler
        PUBWEAK CAN2_RX1_IRQHandler
        PUBWEAK CAN2_SCE_IRQHandler
        PUBWEAK CAN2_TX_IRQHandler
        PUBWEAK CEC_IRQHandler
        PUBWEAK DMA1_Channel1_IRQHandler
        PUBWEAK DMA1_Channel2_IRQHandler
        PUBWEAK DMA1_Channel3_IRQHandler
        PUBWEAK DMA1_Channel4_IRQHandler
        PUBWEAK DMA1_Channel5_IRQHandler
        PUBWEAK DMA1_Channel6_IRQHandler
        PUBWEAK DMA1_Channel7_IRQHandler
        PUBWEAK DMA2_Channel1_IRQHandler
        PUBWEAK DMA2_Channel2_IRQHandler
        PUBWEAK DMA2_Channel3_IRQHandler
        PUBWEAK DMA2_Channel4_5_IRQHandler
        PUBWEAK DMA2_Channel4_IRQHandler
        PUBWEAK DMA2_Channel5_IRQHandler
        PUBWEAK DebugMon_Handler
        PUBWEAK ETH_IRQHandler
        PUBWEAK ETH_WKUP_IRQHandler
        PUBWEAK EXTI0_IRQHandler
        PUBWEAK EXTI15_10_IRQHandler
        PUBWEAK EXTI1_IRQHandler
        PUBWEAK EXTI2_IRQHandler
        PUBWEAK EXTI3_IRQHandler
        PUBWEAK EXTI4_IRQHandler
        PUBWEAK EXTI9_5_IRQHandler
        PUBWEAK FLASH_IRQHandler
        PUBWEAK FSMC_IRQHandler
        PUBWEAK HardFault_Handler
        PUBWEAK I2C1_ER_IRQHandler
        PUBWEAK I2C1_EV_IRQHandler
        PUBWEAK I2C2_ER_IRQHandler
        PUBWEAK I2C2_EV_IRQHandler
        PUBWEAK MemManage_Handler
        PUBWEAK NMI_Handler
        PUBWEAK OTG_FS_IRQHandler
        PUBWEAK OTG_FS_WKUP_IRQHandler
        PUBWEAK PVD_IRQHandler
        PUBWEAK PendSV_Handler
        PUBWEAK RCC_IRQHandler
        PUBWEAK RTC_Alarm_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK SDIO_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI2_IRQHandler
        PUBWEAK SPI3_IRQHandler
        PUBWEAK SVC_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK TAMPER_IRQHandler
        PUBWEAK TIM10_IRQHandler
        PUBWEAK TIM11_IRQHandler
        PUBWEAK TIM12_IRQHandler
        PUBWEAK TIM13_IRQHandler
        PUBWEAK TIM14_IRQHandler
        PUBWEAK TIM1_BRK_IRQHandler
        PUBWEAK TIM1_BRK_TIM15_IRQHandler
        PUBWEAK TIM1_BRK_TIM9_IRQHandler
        PUBWEAK TIM1_CC_IRQHandler
        PUBWEAK TIM1_TRG_COM_IRQHandler
        PUBWEAK TIM1_TRG_COM_TIM11_IRQHandler
        PUBWEAK TIM1_TRG_COM_TIM17_IRQHandler
        PUBWEAK TIM1_UP_IRQHandler
        PUBWEAK TIM1_UP_TIM10_IRQHandler
        PUBWEAK TIM1_UP_TIM16_IRQHandler
        PUBWEAK TIM2_IRQHandler
        PUBWEAK TIM3_IRQHandler
        PUBWEAK TIM4_IRQHandler
        PUBWEAK TIM5_IRQHandler
        PUBWEAK TIM6_DAC_IRQHandler
        PUBWEAK TIM6_IRQHandler
        PUBWEAK TIM7_IRQHandler
        PUBWEAK TIM8_BRK_IRQHandler
        PUBWEAK TIM8_BRK_TIM12_IRQHandler
        PUBWEAK TIM8_CC_IRQHandler
        PUBWEAK TIM8_TRG_COM_IRQHandler
        PUBWEAK TIM8_TRG_COM_TIM14_IRQHandler
        PUBWEAK TIM8_UP_IRQHandler
        PUBWEAK TIM8_UP_TIM13_IRQHandler
        PUBWEAK TIM9_IRQHandler
        PUBWEAK UART4_IRQHandler
        PUBWEAK UART5_IRQHandler
        PUBWEAK USART1_IRQHandler
        PUBWEAK USART2_IRQHandler
        PUBWEAK USART3_IRQHandler
        PUBWEAK USBWakeUp_IRQHandler
        PUBWEAK USB_HP_CAN1_TX_IRQHandler
        PUBWEAK USB_HP_IRQHandler
        PUBWEAK USB_LP_CAN1_RX0_IRQHandler
        PUBWEAK USB_LP_IRQHandler
        PUBWEAK UsageFault_Handler
        PUBWEAK WWDG_IRQHandler

        SECTION .text:CODE:REORDER:NOROOT(1)

// Default interrupt handlers.
ADC1_2_IRQHandler
ADC1_IRQHandler
ADC3_IRQHandler
BusFault_Handler
CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
CAN1_TX_IRQHandler
CAN2_RX0_IRQHandler
CAN2_RX1_IRQHandler
CAN2_SCE_IRQHandler
CAN2_TX_IRQHandler
CEC_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_5_IRQHandler
DMA2_Channel4_IRQHandler
DMA2_Channel5_IRQHandler
DebugMon_Handler
ETH_IRQHandler
ETH_WKUP_IRQHandler
EXTI0_IRQHandler
EXTI15_10_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
EXTI9_5_IRQHandler
FLASH_IRQHandler
FSMC_IRQHandler
HardFault_Handler
I2C1_ER_IRQHandler
I2C1_EV_IRQHandler
I2C2_ER_IRQHandler
I2C2_EV_IRQHandler
MemManage_Handler
NMI_Handler
OTG_FS_IRQHandler
OTG_FS_WKUP_IRQHandler
PVD_IRQHandler
PendSV_Handler
RCC_IRQHandler
RTC_Alarm_IRQHandler
RTC_IRQHandler
SDIO_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
SPI3_IRQHandler
SVC_Handler
SysTick_Handler
TAMPER_IRQHandler
TIM10_IRQHandler
TIM11_IRQHandler
TIM12_IRQHandler
TIM13_IRQHandler
TIM14_IRQHandler
TIM1_BRK_IRQHandler
TIM1_BRK_TIM15_IRQHandler
TIM1_BRK_TIM9_IRQHandler
TIM1_CC_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_TRG_COM_TIM11_IRQHandler
TIM1_TRG_COM_TIM17_IRQHandler
TIM1_UP_IRQHandler
TIM1_UP_TIM10_IRQHandler
TIM1_UP_TIM16_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
TIM5_IRQHandler
TIM6_DAC_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
TIM8_BRK_IRQHandler
TIM8_BRK_TIM12_IRQHandler
TIM8_CC_IRQHandler
TIM8_TRG_COM_IRQHandler
TIM8_TRG_COM_TIM14_IRQHandler
TIM8_UP_IRQHandler
TIM8_UP_TIM13_IRQHandler
TIM9_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
USBWakeUp_IRQHandler
USB_HP_CAN1_TX_IRQHandler
USB_HP_IRQHandler
USB_LP_CAN1_RX0_IRQHandler
USB_LP_IRQHandler
UsageFault_Handler
WWDG_IRQHandler
        B       .

        END
