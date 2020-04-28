
#include "usart.h"
#include "stm32g0xx_hal_uart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

usart_buffer_t  usart1_buffer;

//unsigned char usart1_buffer[BUFF_SIZE];

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
	uint8_t res;
	
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	

	
//		__HAL_UART_CLEAR_FLAG(&huart1, UART_IT_RXNE);
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
	
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //���������ж�
//	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
//	res = huart1.Instance->RDR;
//	HAL_UART_Receive(&huart1, &res, 1, 0xff);
	
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    //���������ж�
//	__HAL_UART_CLEAR_FLAG(&huart1, UART_IT_RXNE);
}

void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

void User_USART_Init(void)
{
    MX_USART1_UART_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    //���������ж�
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //���������ж�
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
//	HAL_UART_Receive_IT(&huart1, &data, 1);
//	HAL_UART_RegisterCallback(&huart1, HAL_UART_RX_COMPLETE_CB_ID,
//                              HAL_UART_RxCpltCallback);
    MX_USART2_UART_Init();
}



//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	UNUSED(huart);
////	rt_kprintf("%c\r\n", data);
////	rt_memset(usart1_buffer, 0, BUFF_SIZE);
//	if(usart1MsgBuffer.len < BUFF_SIZE)
//	{
//		usart1MsgBuffer.buffer[usart1MsgBuffer.len++] = data;
//	}
//	else
//	{
//		usart1MsgBuffer.len = 0;
//		usart1MsgBuffer.buffer[usart1MsgBuffer.len++] = data;
//	}
////	rt_kprintf("%s\r\n", usart1MsgBuffer.buffer);
//	HAL_UART_Receive_IT(&huart1, &data, 1);
//}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**USART1 GPIO Configuration    
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX 
    */
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
    else if (uartHandle->Instance == USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

    if (uartHandle->Instance == USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration    
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX 
    */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4 | GPIO_PIN_5);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */   

        /* USER CODE END USART1_MspDeInit 1 */
    }
    else if (uartHandle->Instance == USART2)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

        /* USART2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
}

void usart_write(UART_HandleTypeDef *usatHandle, uint8_t *buffer, uint16_t size)
{
	HAL_UART_Transmit(usatHandle, buffer, size, 0xfffff);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
