/*
	�������ݵ�ʱ��ÿ�δ�ʵʱȫ�ֱ�������ֵ��sysData�У�
	��ִ����λ��ָ��ʱ��ʵ�ʺܿ���ֱ���޸ĵ�ϵͳ����sysData�ṹ�壻
	������ܶ�ط���ָ��ִ�������ݴ���
	�����JB��
	����취��ͨѶ����Ԫģ���м���һ��sysData���ݵı��溯������������
	ͨѶ����Ԫ�еĸ�������ֻ�ܵ����ļ��ڵ����ݱ��溯��������
*/
#include "./includes/includes.h"

//stDevStatus_t lastDevStatus,nowDevStatus;

//stlockReasonBits_t lockReason;
vavleState_t vavleState;

int8_t rssi;
uint8_t opStatus;
uint8_t commonBuf[512+64];
stDeviceInf_t sysData={0};
//stDeviceInf_t* stpSysData=&sysData;
hardDefault_t  hardDefault={0};

volatile int32_t totalVolume=0x00ul;
//volatile int32_t totalMoney=0x00ul;
volatile int32_t curMonthVolume=0x00ul;
volatile int32_t curPeriodVolume=0x00ul;
//volatile int32_t curMoney=0x00ul;

volatile int32_t rtPrice=0x00ul;
volatile int32_t overageMoney=0x00ul;
//volatile int32_t rtOVerageVMRemainder=0x00l;
volatile int32_t overrageVolume=0x00ul;

volatile int32_t 	rtVolume=0x00;
volatile int32_t	__rtVolume=0x00;

volatile bool rtVolumeRefreshFlag=true;
volatile uint32_t popTimesEveryday=0x00;
overageVMState_t ovmStatus;

//add 20171221
volatile int32_t OVerageVMRemainder=0x00;
//end

volatile int32_t realPayValue;	
bool flgValveErrSend=false;


const emQN_t qnTable[]={QN16,QN25,QN40,QN60,QN100,QN160,QN250,QN400,QN600};

const stDeviceInf_t sysDataDefault={
	hw_VER_NUM,					//uint16_t 	HWVer;
	sw_VER_NUM,					//uint16_t	SWVer;
	pro_VER_NUM,				//uint16_t	ProtocolVer;
	0x00,						//uint16_t	rverse01;
	{0x18,0x08,0x08,0x00},		//uint8_t		PD[4];
	//"CDQC181100000002",			//uint8_t		commSN[16];	
	"         9225002",
	{0},						//uint8_t		DeviceName[16];			//�豸���ƻ����ͺ�	

	//	//������Ϣ�����豸��������й�
	//	//���ú����qmax��qmin��qst(ʼ������)�Ⱦ���һһ��Ӧ��ϵ����GB/T 6968
	0x00,						//vendorCode
	0,							//deviceType
	0,							//deViceClass;
	(emQN_t)default_QN,			//uint16_t	QN;						//���������
	0,							//QMIN
	(emQN_t)default_QMAX,		//uint16_t	QMAX;					//���������
	#if default_QN <= QNDEF_QN60
	QS0_01m3,
	#else
	QS0_1m3,
	#endif						//uint16_t	QS;						//���嵱����
	__VAVLE_ON_TIME,			//uint16_t	VavleOnTime;			//��������Ҫ��ʱ��,s��			
	__VAVLE_OFF_TIME,			//uint16_t	VavleOffTime;			//�ط�����Ҫ��ʱ��,s��	
	PS0_0001,					//uint16_t	rverse02;
	0x00L,						//initVolume
	
	default_HIGHT_FLOW_PROTECT_EN,		//uint8_t		HightFlowProtectEn;
	//	//mode configure descriptor,����ģʽ������Ϣ
	#if(NB20E11_SUB_VER>=NB20F11 && NB20E11_SUB_VER<=NB20F11FQ)
		DWM_COMMON_MODE,
	#else
		DWM_STEP_MONEY_MODE,	//
	#endif						//uint8_t		DWM;					//����ģʽ����ͨ����ģʽ������ģʽ��
	0x00,						//uint8_t		t8_reverse00;
	4,							//uint8_t		qcNoTryTimes;
	
	default_CONST_NO_FLOW_TIME_OUT,//uint16_t	qcNoFlowTimeOut;				
	default_CONST_NO_FLOW_TIME_OUT_EX,//uint16_t	qcNoFlowTimeOutEx;	

	300000,						//int32_t		maxOverageVM;			//���������
	
	default_WARNING_VOLUMING,	//int32_t		warnSetOverageVM_L0;
	default_WARNING_VOLUMING/2,	//int32_t		warnSetOverageVM_L1;
	0L,							//int32_t		offSetOverageVM;
	-30000,						//int32_t		imitOffsetVM;//int32_t     xreverse0x[4];
	 	
	0,							//uint8_t 	superPayNums;		//������ֵ����
	20,							//uint8_t		resetPayNums;		//��λ��ֵ����

	default_DEVICE_ID,			//uint32_t	ID;	//�豸id
	0UL,						//uint32_t	userID;				//�û�id
	0,							//uint16_t	companyCode;		
	0,							//uint16_t	reverse02;
	//	
	{CTCC_TEST_IOT_IP_BIN,0x00},	//severInf_t	severTest;
	{CTCC_REALEAS_IOT_IP_BIN,0x00},	//severInf_t	severRelease;
	{HW_TEST_IOT_IP_BIN,0x00},		//severInf_t	severUser;
	1,							//uint8_t		severIndex;
	0,							//uint8_t		reverse00[3];
	43200,						//unNetTimeOut
	
	degault_DLC_STATUS,			//uint8_t		DLCS;					//�豸״̬��������,�ɼ���S=ABCDE
	0,							//uint8_t		keyIndex;			//��Կ�ȼ����ܳ�����
	0,							//uint8_t		reverse081600;//uint8_t		priceAdjTimes;
	0,							//uint8_t		reverse081601;//uint8_t		curPriceStep;        
	//	

	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	uint8_t		aesKey[16];
	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	uint8_t		aesKey[16];
	//{{0x00,0x00,1440,0x01},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff}},
	//{{0xff,0xff,1440,0x01},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff}},		
	//<--for �ű��������
	0x00UL,						//uint32_t	stepSluStartTm;
	UINT32_MAX,					//uint32_t	stepSluEndTm;
	0x00,						//uint16_t	stepSluSn;
	0x01,						//uint8_t	stepCount;
	0x00,						//uint8_t	stepSluCtrlByte;
	0x00,						//uint16_t	stepPeriod;
	0x00,						//uint16_t	stepReverse;
	{INT32_MAX,0,0,0,0,0},		//int32_t	setpVolume[6]
	{10000,0,0,0,0,0},			//int32_t	stepPrice[6];
	
	UINT32_MAX,					//uint32_t	preStepSluStartTm;
	UINT32_MAX,					//uint32_t	preStepSluEndTm;
	0x00,						//uint16_t	preStepSluSn;
	0x01,						//uint8_t	preStepCount;
	0x00,						//uint8_t	preStepSluCtrlByte;
	0x00,						//uint16_t	preStepPeriod;
	0x00,						//uint16_t	preStepReverse;
	{INT32_MAX,0,0,0,0,0},		//int32_t	preSetpVolume[6]
	{10000,0,0,0,0,0},			//int32_t	preStepPrice[6];	
	//-->
	//	
	10000,						//uint32_t	curPrice;
	0,							//uint32_t	periodVolume;

	default_OVERAGE_VM_A,		//int32_t		OVerageVM;						//����
	0,							//int32_t		OVerageVMRemainder;	

	0,							//int32_t		totalVolume;					//����		
	0,							//int32_t		totalPayMoney;					//�ܽ��	

	1,							//uint8_t		curStep;
	0,							//uint8_t		offReason;//uint8_t     xreverse0;	
	0,//{0},						//stDevStatus_t	devStatus;				//�豸״̬
	0,							//stlockReasonBits_t	lockReason;				//������¼
	0,							//xreverse0
	
	//<--for szrq
	0x00L,						//int32_t		szrqBalance;
	0x00L,						//int32_t		szrqBalanceVol;
	0x00L,						//int32_t		szrqSeverVolume;
	0x00L,						//int32_t		szrqSeverSetpStartVol;
	{0},						//uint8_t		szrqSeverSettleTime[6];
	{0},						//uint8_t		szrqSeverSetpStartDt[6];
	
	5,							//uint8_t		szrqRoportPeriodType;			//�ϱ�����
	2,						//uint8_t		szrqRoportDataInterval;			//���ݼ��
	5,						//uint8_t		szrqtPeriodValue;				//����ֵ
	0x00,						//__szrq_valveCtrlByte_t		szrqValveCtrlByte;
		
	{0x18,0x25},				//uint8_t		szrqtPeriodTime[2];				//�ϱ�ʱ��
	0x28,						//uint8_t		szrqtFreezeDay;					//������
	0x12,						//uint8_t		szrqtFreezeHour;				//����ʱ	
	//-->	
	0,							//uint16_t	crc;
};		


extern void qc_data_read_from_media(uint8_t* d,uint32_t mediaAddr,uint16_t len);
extern void qc_data_write_to_media(uint32_t mediaAddr,uint8_t* s,uint16_t len);

//
uint16_t qc_data_realtime_data_clear(void)
{
	__disable_irq();
	//totalVolume=0x00ul;
	//totalMoney=0x00ul;
	//curMonthVolume=0x00ul;
	//curPeriodVolume=0x00ul;
	//curMoney=0x00ul;
	//price=0x00ul;
	overageMoney=0x00ul;
	overrageVolume=0x00ul;
	rtVolume=0x00;
	__rtVolume=0x00;
	__enable_irq();
	return 1;
}

uint16_t qc_data_device_even_lock_clear(void)
{
	__disable_irq();
	sysData.devStatus.t32=0x00ul;
	sysData.devStatus.bits.bPwrStatus=POWER_STATUS_NORMAL;
	sysData.lockReason.t32=0x00ul;	
	__enable_irq();	
	return 1;
}


extern sysDataTime_t stRtcDataTime;

#if PRICE_ADJUST_SLU==PRICE_ADJUST_SLU_REAL_TIME

#else
uint16_t data_api_calc_price(void)
{
	return sysData.curPrice;
}	
#endif

#if HOLIDAY_VAVLE_OFF_DISABLE==1
uint16_t is_holiday(void)
{

	uint32_t t32;
	uint32_t* p;
	p=(uint32_t*)(APP_DATA_MAIN_ADDR+get_offset_addr_of_str(stDeviceInf_t,HolidayInf));
	p+=(dt.stru.mo-1);
    t32=*p;

    if(t32 & (1ul<<(dt.stru.dm-1)))return 1;
    return 0;
}
#else
    #define is_holiday() 0
#endif


//


void data_api_calc_overage_m_redo(void)
{
	//lockReason.bits.bOverageVM=0;
}

void flow_warning_off_v(overageVMState_t ov,uint16_t x)
{
	(void)x;
	if((sysData.DWM>DWM_COMMON_MODE) || \
		(sysData.DWM==DWM_COMMON_MODE && sysData.DLCS<=DLC_STATUS_B)){
		if(ov==OV_OFF){
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_OFF;
			vavle_off_from_app(OFF_REASON_NO_OV);

		}
		else if(ov==OV_WARNNING){
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_WARNING;
	 
		}
		else{
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_NORMAL;
		}
	}else{
		
	}
}

void flow_warning_off_v_ex(overageVMState_t ov,uint16_t x)
{
	(void)x;
	if((sysData.DWM>DWM_COMMON_MODE) || \
		(sysData.DWM==DWM_COMMON_MODE && sysData.DLCS<=DLC_STATUS_B)){
		if(ov==OV_OFF){
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_OFF;
   
		}
		else if(ov==OV_WARNNING){
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_WARNING;
		}
		else{
			sysData.devStatus.bits.bBalanceSta=CNM_BALANCE_NORMAL;
		}
	}else{
	
	}
}

void data_api_calc_clear_nf_flag(void)
{
	uint8_t tmtry=sysData.qcNoTryTimes;
	if(tmtry==1)tmtry=2;
	if(rtVolume!=0){			
		if(sysData.lockReason.bits.bNoflow<tmtry || tmtry==0){
			sysData.lockReason.bits.bNoflow=0;
			//DeviceEvent.bits.bNoFlowOverTime=0;
			sysData.devStatus.bits.bQcFlowTmout=0;
		}
	}	
}

#if USED_STATIC_D_E
void data_api_calc_clear_d_e(void)
{
	__disable_irq();
	rtVolume=__rtVolume;
	__enable_irq();	
    if(sysData.DLCS==DLC_STATUS_D ){
        if(rtVolume!=0){
            sysData.DLCS=DLC_STATUS_C;   
            api_sysdata_save();
        }
    }	
}
#endif
/*
void data_api_calc_all(void)
{
	//uint32_t t32;
	uint16_t ret=0;
	//uint32_t ovflgPre=0x00ul,ovflgNow=0x00ul;
	//bool saveflg=false;.
	if(!rtVolumeRefreshFlag)return;
    #if USED_STATIC_D_E
	data_api_calc_clear_d_e();
    #endif
	osMutexWait(osMutexSysData,osWaitForever);
	__disable_irq();
	rtVolume=__rtVolume;
	__enable_irq();	
	
	data_api_calc_clear_nf_flag();	
	ret=data_api_calc_consume_vloume();		//ret=
	
	if(sysData.DWM==DWM_VOLUME_MODE){
		ovmStatus=data_api_calc_overage_v();
	}else{
		if(sysData.DLCS<=DLC_STATUS_B){
			ovmStatus=data_api_calc_overage_v();
		}else{
			ovmStatus=OV_NORMAL;
		}
	}

    rtVolumeRefreshFlag=0;  
	
	osMutexRelease(osMutexSysData);
	flow_warning_off_v(ovmStatus,ret);
}
*/
/*
void data_api_calc_all_ex(void)
{
	uint16_t ret=0;
	//uint32_t t32;

	if(!rtVolumeRefreshFlag)return;
	
    #if USED_STATIC_D_E
	data_api_calc_clear_d_e();
    #endif
	data_api_calc_clear_nf_flag();	
	osMutexWait(osMutexSysData,osWaitForever);
	//
	__disable_irq();
	rtVolume=__rtVolume;
	__enable_irq();	
	
	ret=data_api_calc_consume_vloume();		//ret=
	if(sysData.DWM==DWM_VOLUME_MODE){
		ovmStatus=data_api_calc_overage_v();
	}else{
		if(sysData.DLCS<=DLC_STATUS_B){
			ovmStatus=data_api_calc_overage_v();
		}else{
			ovmStatus=OV_NORMAL;
		}
	}

	flow_warning_off_v_ex(ovmStatus,ret);
    rtVolumeRefreshFlag=0;  
	osMutexRelease(osMutexSysData);
}
*/
uint32_t cal_second_in_day(void)
{
	uint32_t t32=0;
	t32=m_bcd_2_hex(stRtcDataTime.hh);
	t32*=3600;
	t32=t32+(uint32_t)(m_bcd_2_hex(stRtcDataTime.mm))*60;
	t32=t32+m_bcd_2_hex(stRtcDataTime.ss);
	return t32;
}

//V104
void qc_data_clear_sysdata_to_dcls_b(void)
{
	uint16_t i=0;
	uint32_t t32;
	t32=sysData.ID;
	//m_mem_cpy_len((uint8_t*)stpSysData,(uint8_t*)&sysData,sizeof(sysData));
	sysData.ID=t32;	
	qc_data_realtime_data_clear();
	qc_data_device_even_lock_clear();	
	
	//�����˾���
	sysData.companyCode=0x00;
	//�������
	sysData.HightFlowProtectEn=default_HIGHT_FLOW_PROTECT_EN;	
	//�������
	//sysData.OVerageVM=default_OVERAGE_VM_B;
	sysData.OVerageVM=0L;
	//����ֵĬ��
	sysData.warnSetOverageVM=default_WARNING_VOLUMING;
	sysData.offSetOverageVM=0;
	sysData.limitOffsetVM=-1000;
	//�������
	sysData.curPrice=100;
	//�������

	sysData.totalPayMoney=0x00ul;
	//�����������������
	sysData.periodVolume=sysData.totalVolume;
	//�������

	//�����ʷ��¼
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.VolumePerMon[i]=0x00ul;
	}

	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.MoneyPerMon[i]=0x00l;
	}
	//sysData.LastBSNVM=0;
	//�׶�תΪB
	sysData.DLCS=DLC_STATUS_C;	
}
void qc_data_clear_sysdata_to_dcls_a(void)
{
	uint16_t i=0;
	uint32_t t32;
	t32=sysData.ID;
	sysData.ID=t32;	
	qc_data_realtime_data_clear();
	qc_data_device_even_lock_clear();
	//�������
	sysData.HightFlowProtectEn=default_HIGHT_FLOW_PROTECT_EN;
	//�������
	sysData.OVerageVM=default_OVERAGE_VM_A;
	//����ֵĬ��
	sysData.warnSetOverageVM=default_WARNING_VOLUMING;
	sysData.offSetOverageVM=0;
	sysData.limitOffsetVM=-1000;
	//�������
	sysData.curPrice=100;
	//�������
	//sysData.SetpPrice[0]=100;
	//sysData.SetpVolume[0]=INT32_MAX;
	//
	//sysData.priceAdjTimes=0x00;
	//sysData.consumeGasType=0x00;
	//sysData.curPriceStep=0x00;
	//
	//����ܽ�����
	sysData.totalPayMoney=0x00ul;
	//�����������������
	sysData.periodVolume=sysData.totalVolume;
	//�������
	//m_mem_set((uint8_t*)&(sysData.calcDateTime),0xff,4);
	//m_mem_set((uint8_t*)&(sysData.adjDateTime),0xff,4);
	//sysData.__xdayinMonth=0xff;
	//sysData.__xmonth=0xff;	
	//�����ʷ��¼
	
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.VolumePerMon[i]=0x00ul;
	}
	
	//�����ֵ��¼ 
//	for(i=0;i<PAY_RECORD_NUM;i++){
//		sysData.PaymentVM[i]=0x00ul;
//	}
	
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.MoneyPerMon[i]=0x00l;
	}	
	//sysData.LastBSNVM=0;
	//�������
	
//	sysData.userTransKeyIndex=0;
//	sysData.userServiceKeyIndex=0;
//	sysData.compServiceKeyIndex=0;
//	sysData.compTransKeyIndex=0;
	//
	/*
	m_mem_cpy_len(sysData.compTransKey,(uint8_t*)compTransKeyDefault,8);
	m_mem_cpy_len(sysData.compSeviceKey,(uint8_t*)compServiceKeyDefault,8);
	m_mem_cpy_len(sysData.userTransKey,(uint8_t*)userTransKeyDefault,8);
	m_mem_cpy_len(sysData.userServiceKey,(uint8_t*)userServiceKeyDefault,8);
	*/
	//�׶�תΪB
	sysData.DLCS=DLC_STATUS_A;	
}

//file end
