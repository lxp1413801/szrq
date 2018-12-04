#ifndef __M_LCD_H__
#define __M_LCD_H__


	#ifdef __cplusplus
	extern "C" {
	#endif
	/* Includes ------------------------------------------------------------------*/
	#include "stm32l0xx_hal.h"
	#include "../../configs/user_configs.h"
	#ifndef config_USED_ALARM_LCD
		#error "config_USED_ALARM_LCD must bedefine !!!"
	#endif 
	#define __mySfr_LCD_CR (*( ( volatile uint32_t *)0X40002400))
	#define __mySfr_LCD_SR (*( ( volatile uint32_t *)0X40002400 + 0x0008))
	/* Define for scrolling sentences*/
	#define SCROLL_SPEED  	200
	#define SCROLL_SPEED_L  400
	#define SCROLL_NUM    	10
		
	/* Define for character '.' */
	#define POINT_OFF      0
	#define POINT_ON       1

	/* Define for caracter ":" */
	#define COLUMN_OFF     0
	#define COLUMN_ON      1

	/* Element values correspond to LCD Glass BAR Level. */
	#define BARLEVEL_OFF    0
	#define BARLEVEL_1_4    1
	#define BARLEVEL_1_2    2
	#define BARLEVEL_3_4    3
	#define BARLEVEL_FULL   4		

	/*
	//for msp430f4152
	#define LCD_DSEG_A 0x01
	#define LCD_DSEG_B 0x02
	#define LCD_DSEG_C 0x04
	
	
	#define LCD_DSEG_F 0x10
	#define LCD_DSEG_G 0x20
	#define LCD_DSEG_E 0x40
	#define LCD_DSEG_D 0x80
  
	#define LCD_DSEG_DP 0x08
	*/
	//What the fuck are you doing, dickhead
	// for stm32l073xx
	#define LCD_DSEG_F 0x01
	#define LCD_DSEG_A 0x02
	#define LCD_DSEG_G 0x04
	#define LCD_DSEG_B 0x08
	
	
	#define LCD_DSEG_E 0x10
	#define LCD_DSEG_C 0x20
	#define LCD_DSEG_D 0x40
	#define LCD_DSEG_DP 0x80
  
	//#define LCD_DSEG_DP 0x08	
	#define LCD_SSEG_YUE_A1 0x01
	#define LCD_SSEG_YQJ_A2 0x02
	#define LCD_SSEG_DANJIA_B 0x08
	#define LCD_SSEG_GYBH_C 0x04
	#define LCD_SSEG_DYBH_D 0x08
	#define LCD_SSEG_GLBH_E 0x08
	#define LCD_SSEG_CSBH_F 0x80
	#define LCD_SSEG_QCBH_G 0x40
	#define LCD_SSEG_KYC_H 0x20
	#define LCD_SSEG_QGQ_I 0x10

	#define LCD_CODE_0 (LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_C + LCD_DSEG_B + LCD_DSEG_A)
	#define LCD_CODE_1 (LCD_DSEG_B + LCD_DSEG_C)
	#define LCD_CODE_2 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_G + LCD_DSEG_E + LCD_DSEG_D)      
	#define LCD_CODE_3 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)  
	#define LCD_CODE_4 (LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C) 
	#define LCD_CODE_5 (LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_A)
	#define LCD_CODE_6 (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_C + LCD_DSEG_G)
	#define LCD_CODE_7 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_C)  
	#define LCD_CODE_8 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_E + LCD_DSEG_F + LCD_DSEG_G)
	#define LCD_CODE_9 (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C+ LCD_DSEG_D)

	#define LCD_CODE_A (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_G)
	#define LCD_CODE_B (LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)
	
	#define LCD_CODE_C_U (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D)
	#define LCD_CODE_C (LCD_DSEG_G + LCD_DSEG_E + LCD_DSEG_D)
	
	#define LCD_CODE_D (LCD_DSEG_B + LCD_DSEG_E + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)
	#define LCD_CODE_E (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_G)
	#define LCD_CODE_F (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_G)
	#define LCD_CODE_G (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C+ LCD_DSEG_D)
	#define LCD_CODE_G_U (LCD_DSEG_A + LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_E + LCD_DSEG_F)
	
	#define LCD_CODE_H_U (LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_E + LCD_DSEG_F + LCD_DSEG_G)
	#define LCD_CODE_H (LCD_DSEG_C + LCD_DSEG_E + LCD_DSEG_F + LCD_DSEG_G)
	#define LCD_CODE_I LCD_CODE_1
	#define LCD_CODE_J (LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_D)
	#define LCD_CODE_K (LCD_DSEG_B+LCD_DSEG_D+LCD_DSEG_E+LCD_DSEG_F+LCD_DSEG_G)
	#define LCD_CODE_L (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_D)
	#define LCD_CODE_M 0
	#define LCD_CODE_N_U (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_A+LCD_DSEG_B+LCD_DSEG_C)
	#define LCD_CODE_N (LCD_DSEG_C +LCD_DSEG_E +LCD_DSEG_G)
	
	#define LCD_CODE_O_U LCD_CODE_0
    #define LCD_CODE_O (LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_E + LCD_DSEG_G)
	
	#define LCD_CODE_P (LCD_DSEG_A +LCD_DSEG_B +LCD_DSEG_E+LCD_DSEG_F+LCD_DSEG_G)
	#define LCD_CODE_Q (LCD_DSEG_A +LCD_DSEG_B +LCD_DSEG_C+LCD_DSEG_F+LCD_DSEG_G)

	#define LCD_CODE_R (LCD_DSEG_A +LCD_DSEG_F +LCD_DSEG_E)
    //#define LCD_CODE_R (LCD_DSEG_E + LCD_DSEG_G)
	#define LCD_CODE_S LCD_CODE_5
	#define LCD_CODE_T (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_D+LCD_DSEG_G)
	
	#define LCD_CODE_U (LCD_DSEG_B +LCD_DSEG_C +LCD_DSEG_D+LCD_DSEG_E+LCD_DSEG_F)
	#define LCD_CODE_V 0
	#define LCD_CODE_W 0

	#define LCD_CODE_X 0
	#define LCD_CODE_Y (LCD_DSEG_B +LCD_DSEG_C +LCD_DSEG_D+LCD_DSEG_G+LCD_DSEG_F)
	#define LCD_CODE_Z LCD_CODE_2
	
	#define LCD_CODE_DP LCD_DSEG_DP
	#define LCD_CODE__ (LCD_DSEG_G)
	#define LCD_CODE_DOT	(LCD_DSEG_G)
	#define LCD_CODE_DDOT	(LCD_DSEG_G+LCD_DSEG_D)


	
	#define m_lcd_disp_seg_YUE_A0() m_lcd_disp_seg(0,0);
	#define m_lcd_clear_seg_YUE_A0() m_lcd_clear_seg(0,0);
	 
	
			
	#define m_lcd_disp_seg_YQL_A1() m_lcd_disp_seg(1,0);
	#define m_lcd_clear_seg_YQL_A1() m_lcd_clear_seg(1,0);
			
	#define m_lcd_disp_seg_DJ_B() m_lcd_disp_seg(3,2);
	#define m_lcd_clear_seg_DJ_B() m_lcd_clear_seg(3,2);
			
	#define m_lcd_disp_seg_GYBH_C() m_lcd_disp_seg(2,0);
	#define m_lcd_clear_seg_GYBH_C() m_lcd_clear_seg(2,0);
			
	#define m_lcd_disp_seg_DYBH_D() m_lcd_disp_seg(3,0);
	#define m_lcd_clear_seg_DYBH_D() m_lcd_clear_seg(3,0);

	#define m_lcd_disp_seg_GLBH_E() m_lcd_disp_seg(3,14);
	#define m_lcd_clear_seg_GLBH_E() m_lcd_clear_seg(3,14);
			
	#define m_lcd_disp_seg_CSBH_F() m_lcd_disp_seg(3,15);
	#define m_lcd_clear_seg_CSBH_F() m_lcd_clear_seg(3,15);
			
	#define m_lcd_disp_seg_QCHB_G() m_lcd_disp_seg(2,15);
	#define m_lcd_clear_seg_QCBH_G() m_lcd_clear_seg(2,15);

	#define m_lcd_disp_seg_KYC_H() m_lcd_disp_seg(1,15);
	#define m_lcd_clear_seg_KYC_H() m_lcd_clear_seg(1,15);

	#define m_lcd_disp_seg_QGQ_I() m_lcd_disp_seg(0,15);
	#define m_lcd_clear_seg_QGQ_I() m_lcd_clear_seg(0,15);
	
	#if config_USED_ALARM_LCD == 0
		#define m_lcd_disp_seg_dp2() m_lcd_disp_seg(3,4);
		#define m_lcd_clear_seg_dp2() m_lcd_clear_seg(3,4);
				
		#define m_lcd_disp_seg_dp3() m_lcd_disp_seg(3,6);
		#define m_lcd_clear_seg_dp3() m_lcd_clear_seg(3,6);
		
		#define m_lcd_disp_seg_alarm_conn() __nop();
		#define m_lcd_disp_seg_alarm_leak() __nop();	
	#else 
		#define m_lcd_disp_seg_dp2() __nop();
		#define m_lcd_clear_seg_dp2() __nop();
				
		#define m_lcd_disp_seg_dp3() __nop();
		#define m_lcd_clear_seg_dp3() __nop();
		
		#define m_lcd_disp_seg_alarm_conn() m_lcd_disp_seg(3,4);
		#define m_lcd_disp_seg_alarm_leak() m_lcd_disp_seg(3,6);
	#endif
	#define m_lcd_disp_seg_dp4() m_lcd_disp_seg(3,8);
	#define m_lcd_clear_seg_dp4() m_lcd_clear_seg(3,8);

	#define m_lcd_disp_seg_dp5() m_lcd_disp_seg(3,10);
	#define m_lcd_clear_seg_dp5() m_lcd_clear_seg(3,10);
	

	#define m_lcd_disp_seg_dp6() m_lcd_disp_seg(3,12);
	#define m_lcd_clear_seg_dp6() m_lcd_clear_seg(3,12);
	
	#define m_lcd_off() __HAL_LCD_DISABLE(&LCDHandle);
	#define m_lcd_on() __HAL_LCD_ENABLE(&LCDHandle);
	
	
	#define LCD_TABLE_LEN (sizeof(LCDDigitalIndexTable)/sizeof(uint8_t))
	#define m_lcd_disp_dig_dp(loc,dig) m_lcd_disp_dig(loc,dig+LCD_DSEG_DP);
	
	#define m_lcd_get_status() (__mySfr_LCD_CR & 0x01ul)
	#define m_lcd_enable() do{__mySfr_LCD_CR |= 0x01ul;}while(0);
	#define m_lcd_disable() do{__mySfr_LCD_CR &= ~0x01ul;}while(0);
	/*
	#define m_lcd_refresh() do{ \
		if(__mySfr_LCD_SR & 0x08ul)__mySfr_LCD_SR |= 0x04ul; \
		}while(0);
		
	#define m_lcd_refresh() do{ \
		__mySfr_LCD_SR &= ~LCD_SR_UDD;\
		__mySfr_LCD_SR |= LCD_SR_UDR; \
		}while(0);	
	*/	
	extern const uint8_t LCDDigitalTable[];
	extern const uint8_t  LCDDigitalIndexTable[];
	
	//
	extern LCD_HandleTypeDef LCDHandle;
	extern uint16_t LCDHandleStat;

	extern void m_lcd_disp_dig(uint8_t loc,uint8_t dig);
	extern void m_lcd_disp_chr(uint8_t loc,uint8_t chr);
	extern void m_lcd_disp_str(uint8_t* str);
	extern void m_lcd_disp_str_refresh(uint8_t* str);
	extern void m_lcd_disp_seg(uint8_t c,uint8_t s);
	extern void m_lcd_clear_seg(uint8_t c,uint8_t s);

	extern void m_lcd_ram_set(uint32_t t);
	extern void m_lcd_ram_clear(void);
	extern void m_lcd_all_on(void);
	extern void m_lcd_all_off(void);
	
	extern void m_lcd_sisp_dp(uint8_t loc);
	
	extern void m_lcd_init(void);	
	extern void m_lcd_reinit(void);
	extern void m_lcd_clear(void);
	extern void m_lcd_refresh(void);
	
	extern uint8_t day_of_week(int16_t y, uint16_t m, uint16_t d);
	#define lcd_clear_all() do{m_lcd_ram_set(~0x0000fffful);}while(0);
	#ifdef __cplusplus
	}
	#endif		
	
	
#endif
	
//file end
//@
