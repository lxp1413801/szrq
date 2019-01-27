#include "../../includes/includes.h"
#include "m_interface.h"
#ifndef cfg_IRDA_EN
#define cfg_IRDA_EN 0
#endif
void m_gpio_set_mode(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t mode)
{
	if(!IS_GPIO_MODE(mode))return;
	GPIO_InitTypeDef GPIO_InitStruct={0};
	switch((uint32_t)GPIOx){
		case GPIOA_BASE:	__HAL_RCC_GPIOA_CLK_ENABLE();	break;
		case GPIOB_BASE:	__HAL_RCC_GPIOB_CLK_ENABLE();	break;
		case GPIOC_BASE:	__HAL_RCC_GPIOC_CLK_ENABLE();	break;
		case GPIOD_BASE:	__HAL_RCC_GPIOD_CLK_ENABLE();	break;
		default:return;
	}

	GPIO_InitStruct.Mode      = mode;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH  ;
	GPIO_InitStruct.Pin	=GPIO_Pin;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
//GPIO_PIN_0
void m_gpio_into_lpw(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH  ;
	GPIO_InitStruct.Pin=GPIO_PIN_All - GPIO_PIN_15 -  GPIO_PIN_14 - GPIO_PIN_2 \
		- GPIO_PIN_3 - GPIO_PIN_4 - GPIO_PIN_5;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
	__HAL_RCC_GPIOC_CLK_DISABLE();

	/*
	GPIO_InitStruct.Pin=GPIO_PIN_2;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
	__HAL_RCC_GPIOD_CLK_DISABLE();	
	*/

	GPIO_InitStruct.Pin=GPIO_PIN_2 + GPIO_PIN_6 + GPIO_PIN_8 + GPIO_PIN_7;
	//GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	__HAL_RCC_GPIOB_CLK_DISABLE();		

	GPIO_InitStruct.Pin=GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_15;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	__HAL_RCC_GPIOA_CLK_DISABLE();	
}

void m_gpio_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t x)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx,GPIO_Pin));
	//assert_param(IS_GPIO_PIN_ACTION(PinState));
	uint16_t t;
	t=GPIOx->ODR;
	t&=~GPIO_Pin;
	t|=(x&GPIO_Pin);
	GPIOx->ODR=t;
}

void m_gpio_config_lcd(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_lcd_port_rcc_enable();
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH  ;
	GPIO_InitStruct.Alternate = GPIO_AF1_LCD;
	GPIO_InitStruct.Pin	=LCD_PINS_AT_PORTA;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//
	GPIO_InitStruct.Pin=LCD_PINS_AT_PORTB;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/*
void m_gpio_config_ain_leak_enable(void)
{
	//pa5
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=GPIO_PIN_5;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	m_gpio_write(GPIOA,GPIO_PIN_5,GPIO_PIN_5);
}


void m_gpio_config_ain_leak_disable(void)
{

//	GPIO_InitTypeDef GPIO_InitStruct={0};
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	
//	GPIO_InitStruct.Pin=GPIO_PIN_5;
//	GPIO_InitStruct.Mode=GPIO_MODE_ANALOG;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	

	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=GPIO_PIN_5;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	m_gpio_write(GPIOA,GPIO_PIN_5,0);	
}
*/

void m_gpio_config_ain_leak_enable(void)
{
	//pa5
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_battary_leak_port_rcc_enable();
	
	GPIO_InitStruct.Pin=BATTARY_VOLTAGE_LEAK_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(BATTARY_VOLTAGE_LEAK_PORT, &GPIO_InitStruct);	
	m_gpio_write(BATTARY_VOLTAGE_LEAK_PORT,BATTARY_VOLTAGE_LEAK_PIN,BATTARY_VOLTAGE_LEAK_PIN);
}


void m_gpio_config_ain_leak_disable(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=GPIO_PIN_9;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	m_gpio_write(GPIOC,GPIO_PIN_9,0);	
}
/*
void m_gpio_config_ainx(void)
{
	//this function must be changed to register direct access mode
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_ainx_rcc_enable();
	//analog
	GPIO_InitStruct.Pin=GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	//dig
	//GPIO_InitStruct.Pin
	GPIO_InitStruct.Pin=AIN0_EN_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	//disable ain0
	HAL_GPIO_WritePin(GPIOC, AIN0_EN_PIN, GPIO_PIN_RESET); 
}

void m_gpio_config_ain_en(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_ainx_rcc_enable();
	GPIO_InitStruct.Pin=AIN0_EN_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	HAL_GPIO_WritePin(GPIOC, AIN0_EN_PIN, GPIO_PIN_RESET); 
}

void m_gpio_config_ain_en_xx(void)
{
	//pc13
	uint32_t tmp;
	tmp=__myReg_RCC_IOPENR;
	tmp|=(0x01ul<<2);
	__myReg_RCC_IOPENR=tmp;
	//
	tmp=__myReg_GPIOC_MODER;
	tmp &= (~(0x03ul<<26));
	tmp	|= (0x01ul<<26);
	__myReg_GPIOC_MODER=tmp;
}


void m_gpio_config_pmcomp(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_pmcomp_rcc_enable();
	GPIO_InitStruct.Pin=COMP2_INP_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(COMP2_INP_PORT, &GPIO_InitStruct);	
}
*/
void m_gpio_config_shell_open(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_shell_open_rcc_enable();
	GPIO_InitStruct.Pin=SHELL_OPNE_DEC_PIN;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(SHELL_OPNE_DEC_PORT,&GPIO_InitStruct);
	//pd2
}

void m_gpio_config_alarm(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_alarm_port_rcc_enable();
	GPIO_InitStruct.Pin=ALARM1_PIN;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(ALARM1_PORT,&GPIO_InitStruct);
	//alarm0:pc5
}
void m_gpio_config_alarm_dect(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_alarm_dect_port_rcc_enable();
	GPIO_InitStruct.Pin=ALARM1_DECT_PIN;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(ALARM1_DECT_PORT,&GPIO_InitStruct);
	//alarm0:pc5
}


void m_gpio_config_alarm_irq_disable(void)
{

	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 &= ~((uint32_t)ALARM1_PIN);
	__myReg_EXTI_IMR=t32;
}

void m_gpio_config_alarm_irq_re_enable(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 |= ((uint32_t)ALARM1_PIN);
	__myReg_EXTI_IMR=t32;
}


void m_gpio_config_key0_irq(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_key0_port_rcc_enable();
	GPIO_InitStruct.Pin=KEY0_PIN;
	//GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(KEY0_PORT,&GPIO_InitStruct);
	//key0:pb7
}

void m_gpio_exti4_15_enable(void)
{
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);		
}

void m_gpio_exti4_15_disable(void)
{
	//HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);		
}

void m_gpio_exti2_3_enable(void)
{
	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);	
}

void m_gpio_exti2_3_disable(void)
{
	HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);	
}

void m_gpio_config_key0_irq_disable(void)
{

	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 &= ~((uint32_t)KEY0_PIN);
	__myReg_EXTI_IMR=t32;
}
void m_gpio_config_key0_irq_re_enable(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 |= ((uint32_t)KEY0_PIN);
	__myReg_EXTI_IMR=t32;
}

/*
void m_gpio_config_pmgpio(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//
	m_gpio_pmgpio_rcc_enable();
	GPIO_InitStruct.Pin=COMP2_INP_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;//GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
		
	HAL_GPIO_Init(COMP2_INP_PORT, &GPIO_InitStruct);
	//pdm0:pb5
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);	
	//pdmPrevious=get_batter_status_io();
	//pdmNow=get_batter_status_io();
}
*/
void m_gpio_config_vavle(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_config_vavle_rcc_enable();
	
	GPIO_InitStruct.Pin=VAVLE_CTRL_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(VAVLE_CTRL_PORT, &GPIO_InitStruct);	
	//out 0;
	HAL_GPIO_WritePin(VAVLE_CTRL_PORT, VAVLE_CTRL_PIN, GPIO_PIN_RESET); 	
}
/*
void m_gpio_config_led(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	m_gpio_config_led_rcc_enable();
	
	GPIO_InitStruct.Pin=LED_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);	
	//out 1,off led;
	//HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); 
	//led_off();	
}
void m_gpio_config_led_xx(void)
{
	//pc6
	uint32_t tmp;
	tmp=__myReg_RCC_IOPENR;
	tmp|=(0x01ul<<2);
	__myReg_RCC_IOPENR=tmp;
	//
	tmp=__myReg_GPIOC_MODER;
	tmp &= (~(0x03ul<<12));
	tmp	|= (0x01ul<<12);
	__myReg_GPIOC_MODER=tmp;	
}

void m_gpio_config_2gctrl(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_config_2gctrlout_rcc_enable();
	m_gpio_config_2gctrlin_rcc_enable();
	//
	//GPIO_InitStruct.Pin=_2G_CTRL_OUT_PINS;
	GPIO_InitStruct.Pin=_2G_HPSW_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//
	HAL_GPIO_Init(_2G_HPSW_PORT, &GPIO_InitStruct);	
	//
	//HAL_GPIO_WritePin(_2G_HPSW_PORT, _2G_HPSW_PIN, GPIO_PIN_RESET); 
	m_gpio_config_2g_pwr_off();

}
*/
/*------------------------------------------------------------------------
* define port and pins for alarm1 input 
* STE:pc3
*-----------------------------------------------------------------------*/	

void m_gpio_config_ste(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_ste_port_rcc_enable();
	GPIO_InitStruct.Pin=STE_PIN;
	//GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(STE_PORT,&GPIO_InitStruct);
	m_gpio_ste_exit_enable();
}

void m_gpio_config_ste_irq_disable(void)
{

	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 &= ~((uint32_t)STE_PIN);
	__myReg_EXTI_IMR=t32;
}

void m_gpio_config_ste_irq_re_enable(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 |= ((uint32_t)STE_PIN);
	__myReg_EXTI_IMR=t32;
}

/*
void m_gpio_config_key1(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_key1_port_rcc_enable();
	GPIO_InitStruct.Pin=KEY1_PIN;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(KEY1_PORT,&GPIO_InitStruct);
	//pc2 
	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);	
}

void m_gpio_config_alarm_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_alarm_port_rcc_enable();
	GPIO_InitStruct.Pin=ALARMx_PINS;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(ALARMx_PORT,&GPIO_InitStruct);



	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);	
}
*/
void m_gpio_config_buzzer(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	m_gpio_config_buzzer_rcc_enable();
	
	GPIO_InitStruct.Pin=BUZZER_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//
	HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);	
	//out 1,off buzzer;
	//HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
	buzzer_off();
}

void m_gpio_config_flow_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};

	m_gpio_flow_port_rcc_enable();
	GPIO_InitStruct.Pin=FLOW_PULSE_PIN;//stFlowPulsePins[0].pins;
	//GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	
	HAL_GPIO_Init(FLOW_PULSE_PORT,&GPIO_InitStruct);
	//m_gpio_flow_exit_enable();
}
void m_gpio_config_flow_irq_disable(void)
{

	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 &= ~((uint32_t)FLOW_PULSE_PIN);
	__myReg_EXTI_IMR=t32;
}
void m_gpio_config_flow_irq_re_enable(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 |= ((uint32_t)FLOW_PULSE_PIN);
	__myReg_EXTI_IMR=t32;
}


/*
if in stop mode ,the gpio ??
*/
//extern TaskHandle_t vTaskMainID;
void m_gpio_exti0_1_irqhandler(void)
{

	if(__HAL_GPIO_EXTI_GET_IT(STE_PIN)!= RESET){ 
		__HAL_GPIO_EXTI_CLEAR_IT(STE_PIN);
		m_gpio_ste_port_rcc_enable();
		if(!(m_gpio_read(STE_PORT,STE_PIN))){
			__nop();
			if((void*)vTheadEvenID){
				osSignalSet( vTheadEvenID, flg_EVENT_STE_FALLING);
			}
		}			
	}	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Clear Wake Up Flag */
  //__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

void m_gpio_exti4_15_irqhandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(IR_SIG_PIN)!= RESET)	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(IR_SIG_PIN);		
		if(!(m_gpio_read(IR_SIG_PORT,IR_SIG_PIN))){
			m_timer_init();
			m_gpio_ir_sig_irq_disanle();
		}
	}
	if(__HAL_GPIO_EXTI_GET_IT(ALARM1_PIN)!= RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(ALARM1_PIN);
		m_gpio_alarm_port_rcc_enable();
		if(!(m_gpio_read(ALARM1_PORT,ALARM1_PIN)))
		{
			__nop();
			if((void*)vTheadEvenID){
				osSignalSet( vTheadEvenID, flg_EVENT_ALARM_FALLING);
			}
		}	
	}
}
//
void m_gpio_decanti_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_decanti_port_rcc_enable();
	
	GPIO_InitStruct.Pin=DECANTI_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(DECANTI_PORT, &GPIO_InitStruct);	
	//out 0;
	HAL_GPIO_WritePin(DECANTI_PORT, DECANTI_PIN, GPIO_PIN_RESET); 		
	
}
//add for id
void m_gpio_ir_pwr_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_ir_pwr_rcc_enable();
	
	GPIO_InitStruct.Pin=IR_PWR_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(IR_PWR_PORT, &GPIO_InitStruct);	
	//out 0;
	HAL_GPIO_WritePin(IR_PWR_PORT, IR_PWR_PIN, GPIO_PIN_RESET); 	
}

void m_gpio_ir_pwr_on(void)
{
	m_gpio_ir_pwr_rcc_enable();
	HAL_GPIO_WritePin(IR_PWR_PORT, IR_PWR_PIN, GPIO_PIN_SET);
}

void m_gpio_ir_pwr_off(void)
{
	m_gpio_ir_pwr_rcc_enable();
	HAL_GPIO_WritePin(IR_PWR_PORT, IR_PWR_PIN, GPIO_PIN_SET);
}

void m_gpio_ir_sig_comfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_gpio_ir_sig_rcc_enable();
	GPIO_InitStruct.Pin=IR_SIG_PIN;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
	
	HAL_GPIO_Init(IR_SIG_PORT,&GPIO_InitStruct);
}

void m_gpio_ir_sig_irq_enanle(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 |= ((uint32_t)IR_SIG_PIN);
	__myReg_EXTI_IMR=t32;
}
void m_gpio_ir_sig_irq_disanle(void)
{
	uint32_t t32;
	t32=__myReg_EXTI_IMR;
	t32 &= ~((uint32_t)IR_SIG_PIN);
	__myReg_EXTI_IMR=t32;	
}

void m_gpio_exti2_3_irqhandler(void)
{
	//ste:pc2
	if(__HAL_GPIO_EXTI_GET_IT(FLOW_PULSE_PIN)!= RESET)	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(FLOW_PULSE_PIN);

		m_gpio_flow_port_rcc_enable();
		if(!(m_gpio_read(FLOW_PULSE_PORT,FLOW_PULSE_PIN))){
			__nop();
			if((void*)vTheadFlowPulsID){
				decanti_strong_pull_up_release();
				//osSignalSet( vTheadEvenID, flg_EVENT_FLW_FALLING);
				osSignalSet(vTheadFlowPulsID,flg_FLW_FALLING);
			}			
		}
	}

	if(__HAL_GPIO_EXTI_GET_IT(KEY0_PIN)!= RESET)
	{ 	
		__HAL_GPIO_EXTI_CLEAR_IT(KEY0_PIN);
		m_gpio_key0_port_rcc_enable();
		//m_gpio_exti4_15_disable();
		if(!(m_gpio_read(KEY0_PORT,KEY0_PIN)))
		{
			__nop();
			if((void*)vTheadEvenID){
				osSignalSet( vTheadEvenID, flg_EVENT_KEY_DOWN);
			}
		}		
	}		
	//#endif
}

/*
void m_pdown_check_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_pdown_check_rcc_enable();
	GPIO_InitStruct.Pin=PD_CHECK_PINS;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	
	HAL_GPIO_Init(PD_CHECK_PORTS,&GPIO_InitStruct);
}
m_nbl620_pwr_config
*/
/*
void m_nbl620_pwr_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	m_nbl620_pwr_rcc_enable();
	GPIO_InitStruct.Pin=NB_L620_PWR_PIN;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(NB_L620_PWR_PORT,&GPIO_InitStruct);
}
*/


//file end
//@
