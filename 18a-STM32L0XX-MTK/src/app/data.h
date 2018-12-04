//file name :data.h
#ifndef __m_DATA_H__
#define __m_DATA_H__
#ifdef __cplusplus
	extern "C"{
#endif
	#include <stdint.h>
	#include <stdbool.h>
	#include <string.h>
	
	#ifdef STM32L073xx
		#include "stm32l073xx.h"
	#elif defined(STM32L152xB)
		#ifndef DATA_EEPROM_BASE
			#define DATA_EEPROM_BASE (0x08080000ul)
		#endif
		#include "stm32l1xx_hal.h"

	#endif	
	
	
	#define SYS_DATA_MAIN_ADDR (DATA_EEPROM_BASE)
	#define SYS_DATA_FREEZE_ADDR (DATA_EEPROM_BANK2_BASE)
	
	extern uint32_t adcValue;
	extern uint32_t voltBat;
	extern uint32_t voltAvdd;	
	
	extern void qc_data_read_from_media(uint8_t* d,uint32_t mediaAddr,uint16_t len);
	extern void qc_data_write_to_media(uint32_t mediaAddr,uint8_t* s,uint16_t len);
	extern void qc_data_set_rtc(uint8_t* dt);
	extern uint16_t qc_data_sysdata_set_default(void);
	extern uint16_t qc_data_sysdata_init(void);
	
	extern uint16_t qc_data_sysdata_write_only(uint8_t* tbuf);
	extern osMutexId osMutexSysData;
	//extern osMutexId osMutexSysDataSave;

	extern void qc_data_mutex_create(void);
	
	//extern const uint32_t defaultID;
#ifdef __cplusplus
	}
#endif
#endif
//file end

