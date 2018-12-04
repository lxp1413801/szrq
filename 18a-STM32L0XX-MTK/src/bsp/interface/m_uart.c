/*
#include "stdint.h"
#include "stdbool.h"
#include "stm32l0xx_hal.h"
#include "m_uart.h"
#include "cmsis_os.h"
#include "common.h"

#include "../../api/m_api.h"
*/
#include "./includes/includes.h"

uint32_t uartReceivedIdleTime=0x00ul;

uint8_t uartSendBuf[MAX_UART_TX_BUFFER_LEN];
uint16_t uartSendBufCount=0;


uint8_t uartReceivedBuf[MAX_UART_RX_BUFFER_LEN];
uint16_t uartRceivedBufCount=0;

//uint8_t uartReceivedBufUser[MAX_UART_RX_BUFFER_LEN];
//uint16_t uartRceivedBufCountUser=0;

#define uartRceivedBufCountUser nbAplReceivedCount
#define uartReceivedBufUser nbAplReceivedBuffer

UART_HandleTypeDef UartHandle={0};
uint32_t uart4RxTimeOut=0;
UART_WakeUpTypeDef WakeUpSelection; 
bool uartInited=false;
static void m_uart4_error_handler(void)
{
	mcu_error_reset();
	while(1){
		__nop();
	}
}

void m_gpio_config_usart4_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin       = USARTx_RXTX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);		
}

void m_gpio_config_usart4(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_usart4_rcc_enable();
	GPIO_InitStruct.Pin       = USARTx_RXTX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF6_USART4;	
	HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);
}
void m_gpio_config_usart4_reset(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_usart4_rcc_enable();
	GPIO_InitStruct.Pin       = USARTx_RXTX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	//GPIO_InitStruct.Alternate = GPIO_AF6_USART4;	
	HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: transfer complete */
	//if(ThreadIduart4!=NULL)
	{
		//osSignalSet(ThreadIdUart5,os_SIG_BIT_UART5_TX);
		//osSignalSet( vTheadGprsID, U_EVEN_FLG_TX);
		//__nop();
	}
}
//extern TaskHandle_t vTheadGprsID;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	while(HAL_IS_BIT_SET(huart->Instance->ISR, UART_FLAG_RXNE))
	{
	}
	__HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

	/* Check if a transmit Process is ongoing or not */
	if(huart->State == HAL_UART_STATE_BUSY_TX_RX) 
	{
		huart->State = HAL_UART_STATE_BUSY_TX;
	}
	else
	{
		/* Disable the UART Parity Error Interrupt */
		__HAL_UART_DISABLE_IT(huart, UART_IT_PE);

		/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		__HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

		huart->State = HAL_UART_STATE_READY;
	}
	//osSignalSet( vTheadGprsID, U_EVEN_FLG_RX);
	m_uart4_received_start();
}

uint16_t m_uart4_init(uint32_t brate)
{
	//m_clock_config_enable_lse();
	//if(uartInited)return 0;
#if cfg_SYSTEM_CLOCK_HSI
	__HAL_RCC_USARTx_CONFIG(RCC_USARTxCLKSOURCE_HSI);
#endif
	m_uart4_rcc_enable();
	UartHandle.Instance        = USARTx;
	HAL_UART_DeInit(&UartHandle); 
	#if NB_MODULE_VENDOR==L620
	UartHandle.Init.BaudRate   = 9600;
	#elif NB_MODULE_VENDOR==BC26
	UartHandle.Init.BaudRate   = brate;
	//UartHandle.Init.BaudRate=115200;
	#else
		#error "defined nb vendor error!!"
	#endif
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	//UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if(HAL_UART_Init(&UartHandle) != HAL_OK)m_uart4_error_handler();
	osDelay(10);
	m_uart4_received_start();		
#if cfg_SYSTEM_CLOCK_HSI
	while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
	while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
	WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
	if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)m_uart4_error_handler();
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
	HAL_UARTEx_EnableStopMode(&UartHandle); 
	//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
#endif
	uartInited=true;
	return 1;
}
void m_uart4_disable_wakeup(void)
{
	HAL_UARTEx_DisableStopMode(&UartHandle);
}
/*
void m_uart4_enable_wakeup(void)
{
	//##-2- Wake Up second step  ###############################################
	// make sure that no UART transfer is on-going 
	while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
	// make sure that UART is ready to receive 
	// (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig)  
	while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);

	// set the wake-up event:
	// specify wake-up on start-bit detection 
	WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
	//WakeUpSelection.WakeUpEvent=UART_WAKEUP_ON_READDATA_NONEMPTY;
	if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)m_uart4_error_handler();
	// Enable the UART Wake UP from stop mode Interrupt 
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);

	HAL_UARTEx_EnableStopMode(&UartHandle); 
	// enter stop mode 
	//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	// ... STOP mode ... 

	// at that point, MCU has been awoken: the LED has been turned back on 
	// Wake Up on start bit detection successful 
	HAL_UARTEx_DisableStopMode(&UartHandle);
	//__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
	//m_clock_set_system_clock_hsi();
	//m_uart4_init();
	//m_uart4_received_start();
}
*/

void m_uart4_enable_wakeup(void)
{
	#if cfg_SYSTEM_CLOCK_HSI	==0
	//m_gpio_config_USARTx_input();
	#endif
}

int16_t m_uart4_deinit(void)
{
	USARTx_FORCE_RESET();
	USARTx_RELEASE_RESET();

	HAL_GPIO_DeInit(USARTx_PORT, USARTx_RXTX_PIN);
	HAL_UART_DeInit(&UartHandle);
	return 1;
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
	
	m_gpio_config_usart4();
	m_uart4_rcc_enable();
	HAL_NVIC_SetPriority(USARTx_IRQn, 3, 1);
	HAL_NVIC_EnableIRQ(USARTx_IRQn);
}


void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	/*##-1- Reset peripherals ##################################################*/

	USARTx_FORCE_RESET();
	USARTx_RELEASE_RESET();
	HAL_GPIO_DeInit(USARTx_PORT, USARTx_RXTX_PIN);

	/*##-3- Disable the NVIC for UART ##########################################*/
	HAL_NVIC_DisableIRQ(USARTx_IRQn);
}
#define __myReg_uart4_ICR (*( ( volatile uint32_t *)	((APBPERIPH_BASE + 0x00004C00)+0x0020)))
#define __myReg_uart4_ISR (*( ( volatile uint32_t *)	((APBPERIPH_BASE + 0x00004C00)+0x001C)))
#define __myReg_uart4_TDR (*( ( volatile uint32_t *)	((APBPERIPH_BASE + 0x00004C00)+0x0028)))	
#define __myReg_uart4_RDR (*( ( volatile uint32_t *)((APBPERIPH_BASE + 0x00004C00)+0x0024)))	

void m_uart4_send_buf_poll(uint8_t *pData, uint16_t Size){
	uint16_t i;
	for(i=0;i<Size;i++){
		while(!(__myReg_uart4_ISR & USART_ISR_TXE));
		__myReg_uart4_TDR=*pData++;
		osDelay(2);
	}
}
void m_uart_send_len(uint8_t* buf,uint16_t len)
{
	m_uart4_send_buf_poll(buf,len);
}

void m_uart4_send_byte_poll(uint8_t b){
	while(!(__myReg_uart4_ISR & USART_ISR_TXE));
	__myReg_uart4_TDR=b;
	osDelay(2);
}

void m_uart4_send_str_poll(uint8_t* str){
	while(*str!='\0'){
		while(!(__myReg_uart4_ISR & USART_ISR_TXE));
		__myReg_uart4_TDR=*str++;
		osDelay(2);
	}
}
void m_uart_send_str(uint8_t* str)
{
	while(*str!='\0'){
		while(!(__myReg_uart4_ISR & USART_ISR_TXE));
		__myReg_uart4_TDR=*str++;
		osDelay(2);
	}	
}
void m_uart_received_echo(uint8_t chr)
{
	#if UART_ECHO_EN==0
	#else
	m_uart4_send_byte_poll(chr);
	if(chr=='\r'){
		m_uart4_send_byte_poll('\n');
	}
	#endif
}
void m_uart_received_msg_post(void)
{
	if(uartRceivedBufCount>4){
		m_mem_cpy_len(uartReceivedBufUser,uartReceivedBuf,uartRceivedBufCount);
		uartRceivedBufCountUser=uartRceivedBufCount;
		uartRceivedBufCount=0;
	}else{
		return;
	}
	if((void*)idTheadUdpId){
		//m_uart_received_msg_post();
		//if()
		osSignalSet( idTheadUdpId, flg_NB_MODULE_UART_RECEIVED_LF);
	}
	
}
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  if((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_TX))
  {
    if((pData == NULL ) || (Size == 0)) 
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(huart);

    //huart->pRxBuffPtr = pData;
    //huart->RxXferSize = Size;
    //huart->RxXferCount = Size;

    /* Computation of UART mask to apply to RDR register */
    UART_MASK_COMPUTATION(huart);

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Check if a transmit process is ongoing or not */
    if(huart->State == HAL_UART_STATE_BUSY_TX) 
    {
      huart->State = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
      huart->State = HAL_UART_STATE_BUSY_RX;
    }

    /* Enable the UART Parity Error Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}
void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart)
{
	//uint8_t chr;
	//chr=(uint8_t)(__myReg_uart4_RDR );
	//m_uart_received_echo(chr);
	__nop();
	//HAL_UARTEx_DisableStopMode(&UartHandle);
	//__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}

HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)
{
	uint32_t isr=__myReg_uart4_ISR;
	uint8_t chr;
	if(isr & (0x01ul<<5))
	{
		uart4_idle_time_reset();
		chr=(uint8_t)(__myReg_uart4_RDR );
		m_uart_received_echo(chr);
		if(uartRceivedBufCount<MAX_UART_RX_BUFFER_LEN){
			//if(chr!=0x0a){
				uartReceivedBuf[uartRceivedBufCount]=chr;
				uartRceivedBufCount++;
			//}
		}
		if(chr==0x0a){

			//__nop();
			//m_uart_received_msg_post();
		}
		//__sleep_in_osdelay_disable();
		return HAL_OK;
	}
	else
	{
		return HAL_BUSY; 
	}
}


//extern osStatus osDelay (uint32_t millisec);


void m_uart4_received_start(void){
	
	HAL_UART_Receive_IT(&UartHandle, (uint8_t*)uartReceivedBuf, sizeof(uartReceivedBuf));
	uartRceivedBufCount=0;
	
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{	
	uint32_t i;
	uint32_t t32;
	//void *p =(void*)(&t32);

	/*
	uint16_t uhMask = huart->Mask;
	__nop();
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {

      *((uint16_t*)p)= (uint16_t)(huart->Instance->RDR & uhMask);

    }
    else
    {
      *((uint8_t*)p) = (uint8_t)(huart->Instance->RDR & (uint8_t)uhMask); 
    }
	huart->ErrorCode == HAL_UART_ERROR_NONE	;
	//
	*/

	
	t32=huart->Instance->CR1;
	t32 &= ~0x01ul;
	t32=huart->Instance->CR1=t32;
	i=1000;
	while(i > 0)i--;
	t32 |= 0x01ul;
	t32=huart->Instance->CR1=t32;
	
	if((void*)idTheadUdpId){
		//m_uart_received_msg_post();
		osSignalSet( idTheadUdpId, flg_NB_MODULE_UART_RECEIVED_ERROR);
	}
	__nop();
}
void uart4_idle_time_disable(void)
{
	uartReceivedIdleTime=0x00ul;
}
void uart4_idle_time_reset(void)
{
	uartReceivedIdleTime=1;//(1000/configTICK_RATE_HZ);
}
void uart4_idle_time_hook(void)
{
	if(uartReceivedIdleTime>0){
		uartReceivedIdleTime+=(1000/configTICK_RATE_HZ);
		if(uartReceivedIdleTime>USART4_RECEIVED_TIME_OUT){
			//uartReceivedIdleTime=(1000/configTICK_RATE_HZ);
			uartReceivedIdleTime=0;
			m_uart_received_msg_post();
			
		}
	}
}
//file end
