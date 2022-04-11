#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GlobalVar.h"
    
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef EXT1_PortStructure;
extern UART_HandleTypeDef EXT2_PortStructure;
extern TIM_HandleTypeDef timMicroProg;
extern DMA_HandleTypeDef hdma_adc3;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi4_rx;
extern DMA_HandleTypeDef hdma_spi4_tx;
extern void USER_UART_IT(UART_HandleTypeDef *huart);
extern void USER_INIT_UART(UART_HandleTypeDef *huart);
extern DMA_HandleTypeDef hdma_usart5_tx;
extern DMA_HandleTypeDef hdma_usart5_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while(1){       
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/
void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  //HAL_UART_IRQHandler(&huart1);
  USER_UART_IT(&huart1); //
}

void USART2_IRQHandler(void)
{
  USER_UART_IT(&EXT2_PortStructure); //  
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
    USER_UART_IT(&huart3); //
}

void UART5_IRQHandler(void)
{
    USER_UART_IT(&EXT1_PortStructure); //
}

void USART6_IRQHandler(void)
{
  USER_UART_IT(&huart6); //
}

void DMA2_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

//This function handles DMA2 stream7 global interrupt.
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

//This function handles DMA2 stream1 global interrupt.
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}

//This function handles DMA2 stream6 global interrupt.
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

void DMA2_Stream0_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_adc3);
}

// This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
void TIM8_TRG_COM_TIM14_IRQHandler(void){
  HAL_TIM_IRQHandler(&timMicroProg);
}

// This function handles ADC1, ADC2 and ADC3 interrupts.
void ADC_IRQHandler(void){
  HAL_ADC_IRQHandler(&hadc2);

    // Защита от сбоев ADC1  
  if((hadc2.Instance->SR & ADC_SR_OVR) != RESET){
      hadc2.Instance->SR = hadc2.Instance->SR ^ ADC_SR_OVR;
  }
    
  // Защита от сбоев ADC1  
  if((hadc1.Instance->SR & ADC_SR_OVR) != RESET){
      hadc1.Instance->SR = hadc1.Instance->SR ^ ADC_SR_OVR;
  }
  
  // Защита от сбоев ADC3
  if((hadc3.Instance->SR & ADC_SR_OVR) != RESET){
      hadc3.Instance->SR = hadc3.Instance->SR ^ ADC_SR_OVR;
  }
}

//
void DMA2_Stream4_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_adc1);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

// Внешнее прерывание
void EXTI9_5_IRQHandler(void){
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

void DMA1_Stream7_IRQHandler(void){  
  HAL_DMA_IRQHandler(&hdma_usart5_tx);
}

#define ENCO_DATA_READ_CH 4
#define AT45_READ_CH 0
#define WAIT_READ_CMD 0
void DMA1_Stream0_IRQHandler(void){
  u16 channelNum;
  
  // Определяем по какой ветке сейчас будем работать
  switch (GlobalM4.versionCP24) {
    //----------------------------------------------------
    case CP24_v21: // Если блок версии 2.1 - обработчики свои. Происходим деление стрима ДМА между двумя устройстваим
      channelNum = (DMA1_Stream0->CR >> DMA_SxCR_CHSEL_Pos) & 0x0007UL;  
      switch(channelNum){  //Checking the actual channel number of DMA stream
      case ENCO_DATA_READ_CH:
         HAL_DMA_IRQHandler(&hdma_usart5_rx);     
         GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 0;
        break;
      case AT45_READ_CH:
        HAL_DMA_IRQHandler(&hdma_spi3_rx);  //SPI3 Interrupt processing
        break;
      }
      break;
    //----------------------------------------------------  
    case CP24_v22: // Если блок версии 2.2 - ДМА не делим ни с кем
         HAL_DMA_IRQHandler(&hdma_usart5_rx);     
         GlobalM4.BLOCK_DMA.bits.EXT1_DMA = 0;
      break;      
    //----------------------------------------------------      
      
  }
}

void DMA1_Stream5_IRQHandler(void){
  u16 channelNum;
  
  // Определяем по какой ветке сейчас будем работать
  switch (GlobalM4.versionCP24) {
    //----------------------------------------------------
    case CP24_v21: // Если блок версии 2.1 - обработчики свои. Происходим деление стрима ДМА между двумя устройстваим    
      channelNum = (DMA1_Stream5->CR >> DMA_SxCR_CHSEL_Pos) & 0x0007UL;  
      switch(channelNum){
      case ENCO_DATA_READ_CH:
        HAL_DMA_IRQHandler(&hdma_usart2_rx);
        GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 0;    
        break;
      case AT45_READ_CH:
         Flash_DMA_IRQHandler(&hdma_spi3_tx);
        break;
      }
      break; 
    //----------------------------------------------------  
    case CP24_v22: // Если блок версии 2.2 - ДМА не делим ни с кем
        HAL_DMA_IRQHandler(&hdma_usart2_rx);
        GlobalM4.BLOCK_DMA.bits.EXT2_DMA = 0;    
      break;      
    //----------------------------------------------------                
  }
}

void DMA1_Stream6_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

// Обработчик прерывания по SPI (W25Q16)
void DMA2_Stream3_IRQHandler(void){
  Flash_DMA_IRQHandler(&hdma_spi4_rx);
}
void DMA2_Stream1_IRQHandler(void){
  Flash_DMA_IRQHandler(&hdma_spi4_tx);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif