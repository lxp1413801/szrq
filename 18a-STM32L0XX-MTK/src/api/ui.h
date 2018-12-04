#ifndef __UI_H__
#define __UI_H__
#ifdef __cplusplus
extern "C"{
#endif
	//extern uint8_t menu;
	#include "stdint.h"
	#define MENU_MAIN_HOME 0x00
	#define subMENU_MAIN_HOME_NB_ATTACH 0x01
	#define subMENU_MAIN_HOME_NB_SOCKET 0x02
	#define subMENU_MAIN_HOME_NB_RECEIVED 0x03
	#define subMENU_MAIN_HOME_NB_SEND 0x04
	#define subMENU_MAIN_VAVLE_OFF 	0x05
	#define subMENU_MAIN_VAVLE_ON	0x06
	#define subMENU_MAIN_SEND_TIMES	0x07
	#define subMENU_MAIN_HOME_NB_PSM 0x08
	
	//extern uint8_t lcdLongStringHead;
	extern const uint8_t hexTable[];
	extern volatile uint8_t menu; 
	extern volatile uint8_t subMenu;
	
	extern bool fi_id_writed_in_dlcs_a(void);
	
	extern void ui_disp_comm_success(void);
	extern void ui_disp_comm_error(uint8_t err);
	extern void ui_disp_comm_rssi(void);
	extern void tick_dly(uint8_t dly);
	extern void tick_dly_dspy(uint8_t dly);
	extern void ui_hard_default_disp(void);
	extern void ui_disp_myID(uint16_t x);
	extern void ui_start_dspy_id(uint8_t dly);
	extern void ui_disp_key_index(void);
	extern void ui_disp_device_ver(void);
	extern void ui_disp_qn(void);
	extern void ui_disp_dlc(void);
	extern void ui_dly(uint8_t *str,uint8_t loc,uint8_t dly);
	extern void ui_dly_ex(uint8_t *str,uint8_t loc,uint8_t* d);
	extern void ui_disp_totale_volume(void);
	extern void ui_disp_main(uint32_t tm);
	extern void ui_disp_menu(uint32_t tm);
    extern void ui_disp_menu_v(void);
	extern void ui_show_hms(void);	
	
	extern void ui_disp_myid_ex(uint16_t x);
	
#ifdef __cplusplus
}
#endif
#endif
