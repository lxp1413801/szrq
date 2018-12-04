#ifndef __user_configs_h__
#define __user_configs_h__
#ifdef __cplusplus
extern "C"{
#endif
	#define __S(x) #x
	#define __def(x) x
	
	//<<--
	//�������ã������޸�
	
	//ʹ��hsiʱ�ӻ���ʹ��msi
	#ifndef cfg_SYSTEM_CLOCK_HSI
		#define cfg_SYSTEM_CLOCK_HSI 0
	#endif 
	//RTC�Ļ���ʱ����
	#ifndef cfg_RTC_INTERVAL
		#define cfg_RTC_INTERVAL	1
	#endif
	
	//��Ҫ��ѭ���̵߳�ִ�����ڣ����ڼ�ʱ
	#ifndef MAIN_TASK_TIME_INTERVAL
		#define MAIN_TASK_TIME_INTERVAL 1
	#endif

	#ifndef default_CONST_ID
		#define default_CONST_ID (*( ( volatile uint32_t *)(0x0801FFF0)))
	#endif
	//-->>
	//----------------------------------------------------------------------------------
	//ѡ������
	#define BC26 0
	#define L620 1
	#define NB_MODULE_VENDOR	BC26	
	//������������汾��
	#define sw_VER_NUM 	10
	#define hw_VER_NUM	10
	#define pro_VER_NUM	10
	
	//�Ƿ����õ����ĵ�����
	#ifndef PWR_DOWN_IO_STATUS_EN
		#define PWR_DOWN_IO_STATUS_EN 0
	#endif
	
	#ifndef cfg_IRDA_EN
		#define cfg_IRDA_EN 1
	#endif

	//ƽ̨����
	#define UDP 	0
	#define COAP	1	
	#ifndef config_NB_PLAT
		#define config_NB_PLAT COAP
	#endif
	
	//����coap��ʽƽ̨���ǲ���ƽ̨
	//#define USED_NB_TEST_PLAT 0

	#define COAP_PLAT_CTCC_TEST		0
	#define COAP_PLAT_CTCC_RELEASE	1
	#define COAP_PLAT_HW_TEST		2
	#define COAP_PLAT_HW_RELEASE	3
	
	#ifndef COAP_PLAT
		//#define COAP_PLAT	COAP_PLAT_CTCC_TEST
		#define COAP_PLAT	COAP_PLAT_CTCC_RELEASE
		//#define COAP_PLAT	COAP_PLAT_HW_TEST
		//#define COAP_PLAT	COAP_PLAT_HW_RELEASE
	#endif
	//�Ƿ�����psmģʽ
	#ifndef config_NB_PSM_EN
		#define config_NB_PSM_EN 1
	#endif
	
	#if (config_NB_PLAT == UDP &&  config_NB_PSM_EN==1)
		#error "where config_NB_PLAT = UDP,config_NB_PSM_EN must set be 0!!!"
	#endif 

	//���幫������
	//�����ڲ��Ӱ汾
	#include "subversion.h"
	#include "qndef.h"
	
	//���Ե�ط�������
	#ifndef SEND_TIMES_TEST
		#define SEND_TIMES_TEST 0
	#endif

	//���峬ʱ����
	#define NO_FLOW_PROTECT_EN_TEST 0
	//���ӳ�ʱ����
	//#define NO_FLOW_PROTECT_EN_TEST_EX 0
	
	#if(NO_FLOW_PROTECT_EN_TEST ==0 && NO_FLOW_PROTECT_EN_TEST_EX!=0)
		#error "no flow test defined error!!!"
	#endif
	
	//----------------------------------------------------------------------------------
	//һ��Ϊ�û�����
	//��������
	#ifndef HIGHT_FLOW_PROTECT_EN
		#define HIGHT_FLOW_PROTECT_EN 1
	#endif 
	
	//��ʱ������������
	#ifndef NO_FLOW_PROTECT_EN
		#define NO_FLOW_PROTECT_EN 1
	#endif
	
	//�쳣С��������
	#ifndef SMALL_FLOW_PROTECT_EN
		#define SMALL_FLOW_PROTECT_EN 0
	#endif 
	
	//��������ʱ�䳬ʱ
	#ifndef CONST_FLOW_TIMEOUT_PROTECT_EN
		#define CONST_FLOW_TIMEOUT_PROTECT_EN 0
	#endif
	
	//���ӵ�ǿ�ű���
	#ifndef STRONGMAGNETIC_PROTECT_EX_EN
		#define STRONGMAGNETIC_PROTECT_EX_EN 0
	#endif
	
	//�Ƿ��������D��E�׶�
	#if (NB20E11_SUB_VER >= NB20F11 && NB20E11_SUB_VER<=NB20F11FQ)
		#define USED_STATIC_D_E 0
		#define NO_FLOW_PROTECT_LOCK_EN 0
	#else
		#define USED_STATIC_D_E 1
		#define NO_FLOW_PROTECT_LOCK_EN 1
	#endif

	//��ʱʱ��
	#ifndef default_CONST_NO_FLOW_TIME_OUT
	
		#define default_CONST_NO_FLOW_TIME_OUT 24*30
		
		#if NO_FLOW_PROTECT_EN_TEST
			#warning "no flow time out test!!!"
		#endif	
	#endif
	
	#ifndef default_CONST_NO_FLOW_TIME_OUT_EX
	
		#define default_CONST_NO_FLOW_TIME_OUT_EX 24*4
		
		#if NO_FLOW_PROTECT_EN_TEST
			#warning "no flow time out test!!!"
		#endif
		
	#endif	
	
	#ifndef default_CONST_FLOW_TIME_OUT
		#define default_CONST_FLOW_TIME_OUT 0
	#endif
	
	#ifndef default_CONST_SMALL_FLOW_TIME_OUT
		#define default_CONST_SMALL_FLOW_TIME_OUT 0
	#endif
	//��ʼ��ʱ��0.25m3���������B�׶�
	#ifndef DEVICE_DLCS_CONVERTED_USED_OFF
		#define  DEVICE_DLCS_CONVERTED_USED_OFF 1
	#endif
	//������ֵ��������
	#ifndef SUPPER_PAY_TIMES_LIMITS
		#define SUPPER_PAY_TIMES_LIMITS 3
	#endif
	
	//ifndef PAY_prompt
	#ifndef PAY_PROMPT_TIME_LONG 
		#define PAY_PROMPT_TIME_LONG  24*3600
	#endif
	
	//ǿ����������
	#ifndef MAX_STE_OFF_TIMES
		#define MAX_STE_OFF_TIMES 3
	#endif
	//�������Լ�ȼ����������
	//Ĭ�� id
	#ifndef default_DEVICE_ID
		#define default_DEVICE_ID 1
	#endif
	
	//Ĭ�Ͻ׶�
	#ifndef degault_DLC_STATUS
		#define degault_DLC_STATUS DLC_STATUS_A
	#endif
	
	//����Ĭ�Ϲ�������
	#ifndef default_HIGHT_FLOW_PROTECT_EN
		#define default_HIGHT_FLOW_PROTECT_EN 0
	#endif

	#if config_NB_PLAT==UDP
		//����Ĭ�Ϸ���������
		#ifndef default_UDP_SEVER_SETTING_M 
			#define default_UDP_SEVER_SETTING_M "211.149.175.111,28883"
			
			//#define default_UDP_SEVER_SETTING_M "171.221.221.50,1895"
			//#define default_UDP_SEVER_SETTING_M "171.221.221.50,8123"
		#endif
		
		#ifndef default_UDP_SEVER_SETTING_S 
			//#define default_UDP_SEVER_SETTING_S "211.149.185.137,2200"
			#define default_UDP_SEVER_SETTING_M "211.149.175.111,28883"
			//#define default_UDP_SEVER_SETTING_M "171.221.221.50,1895"
		#endif
		#warning "nb iot used udp!!!"
	#elif config_NB_PLAT==COAP
		#ifndef COAP_PLAT
			#error "COAP_PLAT must be defined!"
		#endif
		
		#define CTCC_TEST_IOT_IP_BIN 	{180,101,147,115} 
		#define CTCC_REALEAS_IOT_IP_BIN	{117,60,157,137}
		#define HW_TEST_IOT_IP_BIN		{139,159,140,34}
		#define HW_RELEAS_IOT_IP_BIN	{139,159,140,34}
		

		//���Ų���ƽ̨
		#if(COAP_PLAT == COAP_PLAT_CTCC_TEST)
			#define default_SEVER_INDEX 0
			//#define IOT_IP "180.101.147.115"
			//#define default_UDP_SEVER_SETTING_M "AT+NCDP=180.101.147.115\r\n"
			//#define default_UDP_SEVER_SETTING_S "AT+NCDP=180.101.147.115\r\n"
			#warning "nb iot used coap ctcc test plat!!!"
		//������ʽƽ̨
		#elif(COAP_PLAT == COAP_PLAT_CTCC_RELEASE)
			//#define IOT_IP "117.60.157.137"
			#define default_SEVER_INDEX 1
			//#define default_UDP_SEVER_SETTING_M "AT+NCDP=117.60.157.137\r\n"
			//#define default_UDP_SEVER_SETTING_S "AT+NCDP=117.60.157.137\r\n"
		//��Ϊ����ƽ̨
		#elif(COAP_PLAT == COAP_PLAT_HW_TEST)
			#define default_SEVER_INDEX 2
			//#define IOT_IP "139.159.140.34"
			//#define default_UDP_SEVER_SETTING_M "AT+NCDP=139.159.140.34\r\n"
			//#define default_UDP_SEVER_SETTING_S "AT+NCDP=139.159.140.34\r\n"	
			#warning "nb iot used coap hw test plat!!!"
		//��Ϊ��ʽƽ̨
		#elif(COAP_PLAT == COAP_PLAT_HW_RELEASE)
			#define default_SEVER_INDEX 2
			//#define IOT_IP "139.159.140.34"
			//#define default_UDP_SEVER_SETTING_M "AT+NCDP=139.159.140.34\r\n"
			//#define default_UDP_SEVER_SETTING_S "AT+NCDP=139.159.140.34\r\n"
			#warning "nb iot used coap hw plat!!!"			
		#else
		#error "COAP_PLAT defined error!"
		#endif
	/*
	#if USED_NB_TEST_PLAT
		//����ƽ̨

		#define IOT_IP "180.101.147.115"

		#ifndef default_UDP_SEVER_SETTING_M 
			#define default_UDP_SEVER_SETTING_M "AT+NCDP=180.101.147.115\r\n"
		#endif
		
		#ifndef default_UDP_SEVER_SETTING_S 
			#define default_UDP_SEVER_SETTING_S "AT+NCDP=180.101.147.115\r\n"
		#endif	
		#warning "nb iot used coap test plat!!!"
	#else 
		//��ʽƽ̨
		#define IOT_IP "117.60.157.137"

		#ifndef default_UDP_SEVER_SETTING_M 
			#define default_UDP_SEVER_SETTING_M "AT+NCDP=117.60.157.137\r\n"
		#endif
		
		#ifndef default_UDP_SEVER_SETTING_S 
			#define default_UDP_SEVER_SETTING_S "AT+NCDP=117.60.157.137\r\n"
		#endif

	#endif	
	*/
	#endif

#ifdef __cplusplus
}
#endif
#endif
