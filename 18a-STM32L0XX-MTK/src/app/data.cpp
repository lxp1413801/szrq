#include "./includes/includes.h"
#include <float.h>

extern stDeviceInf_t sysData;
extern stDeviceInf_t* stpSysData;
extern const stDeviceInf_t sysDataDefault;
uint32_t adcValue = 1ul;
uint32_t voltBat=0x01;
uint32_t voltAvdd=0x01;	
osMutexId osMutexSysData=NULL;
#define MANUAL_ID 60051
void qc_data_mutex_create(void)
{
	osMutexDef(osMutexSysData);
	osMutexSysData = osMutexCreate(osMutex(osMutexSysData));	
}

void qc_data_read_from_media(uint8_t* d,uint32_t mediaAddr,uint16_t len)
{
	m_mem_cpy_len(d,(uint8_t*)mediaAddr,len);
}

void qc_data_write_to_media(uint32_t mediaAddr,uint8_t* s,uint16_t len)
{
	m_flash_write_eeprom(mediaAddr,s,len);
}

void qc_data_set_rtc(uint8_t* dt)
{
	uint8_t t8;
	for(t8=0;t8<sizeof(sysDataTime_t);t8++){
		dt[t8]=m_hex_2_bcd(dt[t8]);
	}
	__nop();
	api_rtc_set((sysDataTime_t*)dt);
}

uint16_t qc_data_sysdata_set_default(void)
{
	uint16_t ret=0;
	uint8_t* buf;
	buf=(uint8_t*)(&sysData);	

	qc_data_read_from_media(buf,(uint32_t)(&sysDataDefault),sizeof(stDeviceInf_t));
	//add 
	sysData.ID=default_CONST_ID;
	//sysData.ID=defaultID;
	#if config_NB_PLAT==UDP
	if(sysData.ID== ~0x00ul || sysData.ID== 0x00ul)sysData.ID=MANUAL_ID;
	#else
	if(sysData.ID== ~0x00ul || sysData.ID== 0x00ul)sysData.ID=MANUAL_ID;
	#endif
	crc_append(buf,sizeof(stDeviceInf_t)-2);
	qc_data_write_to_media(SYS_DATA_MAIN_ADDR,buf,sizeof(stDeviceInf_t));
	
	qc_data_read_from_media(buf,SYS_DATA_MAIN_ADDR,sizeof(stDeviceInf_t));
	ret=crc_verify(buf,sizeof(stDeviceInf_t));		

	return ret;
}

uint16_t qc_data_sysdata_write_only(uint8_t* tbuf)
{
	uint16_t ret=0;
	uint8_t* buf;
	buf=(uint8_t*)(&sysData);	
	m_mem_cpy_len(buf,tbuf,sizeof(stDeviceInf_t));
	crc_append(buf,sizeof(stDeviceInf_t)-2);
	qc_data_write_to_media(SYS_DATA_MAIN_ADDR,buf,sizeof(stDeviceInf_t));

	qc_data_read_from_media(buf,SYS_DATA_MAIN_ADDR,sizeof(stDeviceInf_t));
	ret=crc_verify(buf,sizeof(stDeviceInf_t));		
	return ret;	
}

uint16_t qc_data_sysdata_init(void)
{
	float f=1.0;
	uint16_t ret=0;
	uint8_t* buf;
	uint32_t t32;
	buf=(uint8_t*)(&sysData);

	qc_data_read_from_media(buf,SYS_DATA_MAIN_ADDR,sizeof(stDeviceInf_t));
	ret=crc_verify(buf,sizeof(stDeviceInf_t));
	if(!ret){
		qc_data_read_from_media(buf,(uint32_t)(&sysDataDefault),sizeof(stDeviceInf_t));
		
		sysData.ID=default_CONST_ID;
		//sysData.ID=defaultID;
		#if config_NB_PLAT==UDP
		if(sysData.ID== ~0x00ul || sysData.ID== 0x00ul)sysData.ID=MANUAL_ID;
		#else
		if(sysData.ID== ~0x00ul || sysData.ID== 0x00ul)sysData.ID=MANUAL_ID;
		#endif	
		
		crc_append(buf,sizeof(stDeviceInf_t)-2);
		qc_data_write_to_media(SYS_DATA_MAIN_ADDR,buf,sizeof(stDeviceInf_t));
		
		qc_data_read_from_media(buf,SYS_DATA_MAIN_ADDR,sizeof(stDeviceInf_t));
		ret=crc_verify(buf,sizeof(stDeviceInf_t));		
	}
	
	if(sysData.DLCS==DLC_STATUS_A && sysData.resetPayNums < 9 && sysData.OVerageVM<=0x00l){
		sysData.OVerageVM=default_OVERAGE_VM_A;
		sysData.resetPayNums++;
	}
	
	
	//DeviceEvent.data=sysData.deviceEvent.data;
	//nowDevStatus.t32=0x00;
	//lastDevStatus.t32=sysData.devStatus.t32;
	//lockReason.t32=sysData.lockReason.t32;
	rtPrice=sysData.curPrice;
	//
	m_rng_init();
	m_rng_generation(&f);
	m_rng_reinit();
	t32=(22*3600);
	//
	t32=(uint32_t)(f*t32);
	popTimesEveryday=t32+1000;
	//popTimesEveryday+=t32;
	//<<--
	freeze_part_init();
	//czsn_data_load_sample_send_time(&sampleTimeTab,sysData.sampleParam);
	//czsn_data_load_sample_send_time(&sendTimeTab,sysData.sendParam);
	//-->>
	//
	return ret;
}


//file end
