#ifndef __M_UART_H__
#define __M_UART_H__


	#ifdef __cplusplus
	extern "C" {
	#endif

#ifndef UART_ECHO_EN
	#define UART_ECHO_EN 0
#endif

	#define USARTx                           USART4
	#define USARTx_CLK_ENABLE()					__HAL_RCC_USART4_CLK_ENABLE()
	#define USARTx_CLK_DISABLE()				__HAL_RCC_USART4_CLK_DISABLE()
		
	#define m_gpio_usart4_rcc_enable() do{__HAL_RCC_GPIOC_CLK_ENABLE();__HAL_RCC_USART4_CLK_ENABLE();}while(0);
	#define m_gpio_usart4_rcc_disable() do{__HAL_RCC_GPIOC_CLK_DISABLE();__HAL_RCC_USART4_CLK_DISABLE();}while(0);
	
	#define m_uart4_rcc_enable() __HAL_RCC_USART4_CLK_ENABLE()
	#define m_uart4_rcc_disable() __HAL_RCC_USART4_CLK_DISABLE();
		
	#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
	#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

	#define USARTx_FORCE_RESET()             __HAL_RCC_USART4_FORCE_RESET()
	#define USARTx_RELEASE_RESET()           __HAL_RCC_USART4_RELEASE_RESET()
/*
	#define USART4_PORT		GPIOC
	#define USART4_TX_PIN	(uint32_t)(GPIO_PIN_10)
	#define USART4_RX_PIN	(uint32_t)(GPIO_PIN_11)
	#define USART4_RXTX_PIN	(uint32_t)(GPIO_PIN_11 | GPIO_PIN_10)
*/
	/* Definition for USARTx Pins */
	#define USARTx_TX_PIN						GPIO_PIN_10
	#define USARTx_TX_GPIO_PORT					GPIOC
	#define USARTx_TX_AF						GPIO_AF6_USART4
	#define USARTx_RX_PIN						GPIO_PIN_11
	#define USARTx_RX_GPIO_PORT					GPIOC
	#define USARTx_RX_AF						GPIO_AF6_USART4
	#define USARTx_PORT 						GPIOC
	#define USARTx_RXTX_PIN						(GPIO_PIN_11 | GPIO_PIN_10)
	/* Definition for USARTx's NVIC */
	#define USARTx_IRQn USART4_5_IRQn


	/* Size of Trasmission buffer */
	#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
	/* Size of Reception buffer */
	#define RXBUFFERSIZE                      TXBUFFERSIZE
	  
	/* Exported macro ------------------------------------------------------------*/
	#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
	/* Exported functions ------------------------------------------------------- */

	#define MAX_UART_TX_BUFFER_LEN (512)
	#define MAX_UART_RX_BUFFER_LEN (256)
	#define USART4_RECEIVED_TIME_OUT 100
	
	extern uint8_t uartSendBuf[MAX_UART_TX_BUFFER_LEN];
	extern uint8_t uartReceivedBuf[MAX_UART_RX_BUFFER_LEN];
	extern uint16_t uartRceivedBufCount;
	extern uint16_t uartSendBufCount;
	//add by lxp
	//extern uint8_t uartReceivedBufUser[MAX_UART_RX_BUFFER_LEN];
	//extern uint16_t uartRceivedBufCountUser;	
	
	#define uartRceivedBufCountUser nbAplReceivedCount
	#define uartReceivedBufUser nbAplReceivedBuffer
	//extern 
	//extern UART_HandleTypeDef UartHandle;
	
	//#define UART_IDLE_TIME ((100*configTICK_RATE_HZ)/1000)

	extern UART_HandleTypeDef UartHandle;
	extern uint32_t uart4RxTimeOut;
	//
	//extern osThreadId ThreadIdUart5;
	extern int16_t m_uart4_deinit(void);
	extern uint16_t m_uart4_init(uint32_t brate);
	extern void m_uart4_enable_wakeup(void);
	extern void m_uart4_disable_wakeup(void);
	//extern void m_uart4_send_start(void);
	extern void m_uart4_received_start(void);
	extern void m_uart_received_echo(uint8_t chr);
	extern void u_uart_send_str(uint8_t* str);
	
	//
	extern void m_uart4_send_buf_poll(uint8_t *pData, uint16_t Size);
	extern void m_uart4_send_byte_poll(uint8_t b);
	extern void m_uart4_send_str_poll(uint8_t* str);
	extern void m_uart_send_len(uint8_t* buf,uint16_t len);
	
	//
	extern void m_uart_send_str(uint8_t* str);
	//
	extern void uart4_idle_time_disable(void);
	extern void uart4_idle_time_reset(void);
	extern void uart4_idle_time_hook(void);
	#ifdef __cplusplus
	}
	#endif

#endif
//file end
