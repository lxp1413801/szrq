/*
	保存数据的时候，每次从实时全局变量读入值到sysData中；
	而执行上位机指令时，实际很可能直接修改的系统数据sysData结构体；
	这样会很多地方的指令执行是数据错误。
	评语：搞JB！
	处理办法在通讯处理单元模块中加入一个sysData数据的保存函数规避这个问题
	通讯处理单元中的各个函数只能调用文件内的数据保存函数！！！
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
	{0},						//uint8_t		DeviceName[16];			//设备名称或者型号	

	//	//配置信息，与设备本身参数有关
	//	//针对煤气表，qmax，qmin，qst(始动流量)等具有一一对应关系，见GB/T 6968
	0x00,						//vendorCode
	0,							//deviceType
	0,							//deViceClass;
	(emQN_t)default_QN,			//uint16_t	QN;						//标称流量，
	0,							//QMIN
	(emQN_t)default_QMAX,		//uint16_t	QMAX;					//最大流量，
	#if default_QN <= QNDEF_QN60
	QS0_01m3,
	#else
	QS0_1m3,
	#endif						//uint16_t	QS;						//脉冲当量，
	__VAVLE_ON_TIME,			//uint16_t	VavleOnTime;			//开阀门需要的时间,s·			
	__VAVLE_OFF_TIME,			//uint16_t	VavleOffTime;			//关阀门需要的时间,s·	
	PS0_0001,					//uint16_t	rverse02;
	0x00L,						//initVolume
	
	default_HIGHT_FLOW_PROTECT_EN,		//uint8_t		HightFlowProtectEn;
	//	//mode configure descriptor,工作模式配置信息
	#if(NB20E11_SUB_VER>=NB20F11 && NB20E11_SUB_VER<=NB20F11FQ)
		DWM_COMMON_MODE,
	#else
		DWM_STEP_MONEY_MODE,	//
	#endif						//uint8_t		DWM;					//工作模式，普通抄表模式，气量模式等
	0x00,						//uint8_t		t8_reverse00;
	4,							//uint8_t		qcNoTryTimes;
	
	default_CONST_NO_FLOW_TIME_OUT,//uint16_t	qcNoFlowTimeOut;				
	default_CONST_NO_FLOW_TIME_OUT_EX,//uint16_t	qcNoFlowTimeOutEx;	

	300000,						//int32_t		maxOverageVM;			//最大屯气量
	
	default_WARNING_VOLUMING,	//int32_t		warnSetOverageVM_L0;
	default_WARNING_VOLUMING/2,	//int32_t		warnSetOverageVM_L1;
	0L,							//int32_t		offSetOverageVM;
	-30000,						//int32_t		imitOffsetVM;//int32_t     xreverse0x[4];
	 	
	0,							//uint8_t 	superPayNums;		//超级充值次数
	20,							//uint8_t		resetPayNums;		//复位充值次数

	default_DEVICE_ID,			//uint32_t	ID;	//设备id
	0UL,						//uint32_t	userID;				//用户id
	0,							//uint16_t	companyCode;		
	0,							//uint16_t	reverse02;
	//	
	{CTCC_TEST_IOT_IP_BIN,0x00},	//severInf_t	severTest;
	{CTCC_REALEAS_IOT_IP_BIN,0x00},	//severInf_t	severRelease;
	{HW_TEST_IOT_IP_BIN,0x00},		//severInf_t	severUser;
	1,							//uint8_t		severIndex;
	0,							//uint8_t		reverse00[3];
	43200,						//unNetTimeOut
	
	degault_DLC_STATUS,			//uint8_t		DLCS;					//设备状态生命周期,可兼容S=ABCDE
	0,							//uint8_t		keyIndex;			//秘钥等级，密匙索引
	0,							//uint8_t		reverse081600;//uint8_t		priceAdjTimes;
	0,							//uint8_t		reverse081601;//uint8_t		curPriceStep;        
	//	

	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	uint8_t		aesKey[16];
	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	uint8_t		aesKey[16];
	//{{0x00,0x00,1440,0x01},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff}},
	//{{0xff,0xff,1440,0x01},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff},{0xff,0xff,0xffff,0xffff}},		
	//<--for 团标阶梯气价
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

	default_OVERAGE_VM_A,		//int32_t		OVerageVM;						//余量
	0,							//int32_t		OVerageVMRemainder;	

	0,							//int32_t		totalVolume;					//总量		
	0,							//int32_t		totalPayMoney;					//总金额	

	1,							//uint8_t		curStep;
	0,							//uint8_t		offReason;//uint8_t     xreverse0;	
	0,//{0},						//stDevStatus_t	devStatus;				//设备状态
	0,							//stlockReasonBits_t	lockReason;				//锁定记录
	0,							//xreverse0
	
	//<--for szrq
	0x00L,						//int32_t		szrqBalance;
	0x00L,						//int32_t		szrqBalanceVol;
	0x00L,						//int32_t		szrqSeverVolume;
	0x00L,						//int32_t		szrqSeverSetpStartVol;
	{0},						//uint8_t		szrqSeverSettleTime[6];
	{0},						//uint8_t		szrqSeverSetpStartDt[6];
	
	5,							//uint8_t		szrqRoportPeriodType;			//上报周期
	2,						//uint8_t		szrqRoportDataInterval;			//数据间隔
	5,						//uint8_t		szrqtPeriodValue;				//周期值
	0x00,						//__szrq_valveCtrlByte_t		szrqValveCtrlByte;
		
	{0x18,0x25},				//uint8_t		szrqtPeriodTime[2];				//上报时间
	0x28,						//uint8_t		szrqtFreezeDay;					//冻结日
	0x12,						//uint8_t		szrqtFreezeHour;				//冻结时	
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
	
	//清除公司编号
	sysData.companyCode=0x00;
	//清除超流
	sysData.HightFlowProtectEn=default_HIGHT_FLOW_PROTECT_EN;	
	//清除余量
	//sysData.OVerageVM=default_OVERAGE_VM_B;
	sysData.OVerageVM=0L;
	//报警值默认
	sysData.warnSetOverageVM=default_WARNING_VOLUMING;
	sysData.offSetOverageVM=0;
	sysData.limitOffsetVM=-1000;
	//清除单价
	sysData.curPrice=100;
	//清除阶梯

	sysData.totalPayMoney=0x00ul;
	//清除阶梯周期内用量
	sysData.periodVolume=sysData.totalVolume;
	//清除调价

	//清除历史记录
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.VolumePerMon[i]=0x00ul;
	}

	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.MoneyPerMon[i]=0x00l;
	}
	//sysData.LastBSNVM=0;
	//阶段转为B
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
	//清除超流
	sysData.HightFlowProtectEn=default_HIGHT_FLOW_PROTECT_EN;
	//清除余量
	sysData.OVerageVM=default_OVERAGE_VM_A;
	//报警值默认
	sysData.warnSetOverageVM=default_WARNING_VOLUMING;
	sysData.offSetOverageVM=0;
	sysData.limitOffsetVM=-1000;
	//清除单价
	sysData.curPrice=100;
	//清除阶梯
	//sysData.SetpPrice[0]=100;
	//sysData.SetpVolume[0]=INT32_MAX;
	//
	//sysData.priceAdjTimes=0x00;
	//sysData.consumeGasType=0x00;
	//sysData.curPriceStep=0x00;
	//
	//清除总金额，总量
	sysData.totalPayMoney=0x00ul;
	//清除阶梯周期内用量
	sysData.periodVolume=sysData.totalVolume;
	//清除调价
	//m_mem_set((uint8_t*)&(sysData.calcDateTime),0xff,4);
	//m_mem_set((uint8_t*)&(sysData.adjDateTime),0xff,4);
	//sysData.__xdayinMonth=0xff;
	//sysData.__xmonth=0xff;	
	//清除历史记录
	
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.VolumePerMon[i]=0x00ul;
	}
	
	//清除充值记录 
//	for(i=0;i<PAY_RECORD_NUM;i++){
//		sysData.PaymentVM[i]=0x00ul;
//	}
	
	for(i=0;i<VM_RECORD_NUM;i++){
		//sysData.MoneyPerMon[i]=0x00l;
	}	
	//sysData.LastBSNVM=0;
	//清除密码
	
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
	//阶段转为B
	sysData.DLCS=DLC_STATUS_A;	
}

//<<--for czsn
//潮州申能冻结数据
//qc_data_write_to_media(SYS_DATA_MAIN_ADDR,(uint8_t*)(&sysData),sizeof(stDeviceInf_t));
//qc_data_read_from_media((uint8_t*)(&sysData),SYS_DATA_MAIN_ADDR,sizeof(stDeviceInf_t));

//uint16_t czsnSampleTimeTabe[24]={0x00};
//uint16_t czsnSendTimeTable[]
//sampSendTimeTab_t sampleTimeTab={0};
//sampSendTimeTab_t sendTimeTab={0};
/*
uint16_t czsn_data_load_sample_send_time(sampSendTimeTab_t *timeTab,sampSendParam_t* param)
{
	volatile uint16_t t16,i,j,tm,rep,itv,loc=0;
	m_mem_set((uint8_t*)timeTab,0xff,sizeof(sampSendTimeTab_t));
	//tm=0;
	for(i=0;i<4;i++){
		tm=param[i].sampHour;
		tm*=60;
		tm+=param[i].sampMin;
		if(tm>=60*24)continue;
		
		rep=param[i].repeat;
		if(rep==0)continue;
		
		itv=param[i].intervalMin;
		for(j=0;j<rep;j++){
			t16=tm+itv*j;
			if(t16>=60*24)continue;
			
			if(loc>0){
				if(t16<=timeTab->tmieTable[loc-1])continue;
			}
			
			timeTab->tmieTable[loc]=t16;
			loc++;
			if(loc>=MAX_CZSN_SS_TIMES_DAY)break;
		}
	}
	timeTab->pointNum=loc;
	t16=get_minute_in_day();
	for(i=0;i<loc;i++){
		if(t16>=timeTab->tmieTable[i])continue;
		else{
			break;
		}
	}
	if(i<loc){
		timeTab->nextTimeLoc=i;//timeTab->tmieTable[i];
		timeTab->tadayOver=0;
	}else{
		timeTab->nextTimeLoc=MAX_CZSN_SS_TIMES_DAY;//0xffff;
		timeTab->tadayOver=1;
	}
	return loc;
}
*/
uint16_t freeze_part_format(void)
{

	//uint16_t t16;
	uint8_t buf[16];
	volLogHeader_t* sth;
	sth=(volLogHeader_t*)buf;

	sth->writeLoc=0x00;
	sth->unreadNum=0x00;
	sth->itemNum=0x00;
	crc_append(buf,sizeof(volLogHeader_t)-2);
	qc_data_write_to_media(FREEZE_VOLLOG_HEADER_START_ADDR,buf,sizeof(volLogHeader_t));
	return 0;

}
uint16_t freeze_part_init(void)
{
	//uint16_t len;
	uint16_t t16;
	uint8_t buf[16];
	volLogHeader_t* sth;
	sth=(volLogHeader_t*)buf;
	qc_data_read_from_media(buf,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));
	t16=crc_verify(buf,sizeof(volLogHeader_t));
	if(!t16){
		sth->writeLoc=0x00;
		sth->unreadNum=0x00;
		sth->itemNum=0x00;
		crc_append(buf,sizeof(volLogHeader_t)-2);
		qc_data_write_to_media(FREEZE_VOLLOG_HEADER_START_ADDR,buf,sizeof(volLogHeader_t));
	}
	return t16;
}

uint16_t freeze_item_save(void)
{
	uint16_t t16;
	uint32_t t32,itemAddr;
	//itemAddr=SYS_DATA_VOLLOG_ADDR+sizeof(volLogHeader_t);
	volLogHeader_t partHeader;
	volItem_t	item;
	sysDataTime_t dt;
	
	if(!szrqRtcSync)return 0;
	
	qc_data_read_from_media((uint8_t*)&partHeader,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));
	
	t32=sizeof(volItem_t)*partHeader.writeLoc;
	itemAddr=FREEZE_VOLLOG_BODY_START_ADDR+t32;

	m_rtc_get(&dt);
	t32=system_dt_to_time_stamp(&dt);
	item.ts=t32;
	item.vol=totalVolume;
	//t16=__czsn_meter_status_cross_map();
	item.sta=t16;
	crc_append((uint8_t*)&item,sizeof(volItem_t)-2);
	qc_data_write_to_media(itemAddr,(uint8_t*)&item,sizeof(volItem_t));
	//
	if(partHeader.itemNum<FREEZE_VOLLOG_ITEM_NUM_LIMITS)partHeader.itemNum++;
	if(partHeader.unreadNum<FREEZE_VOLLOG_ITEM_NUM_LIMITS)partHeader.unreadNum++;
	t16=partHeader.writeLoc;
	t16++;t16%=FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	partHeader.writeLoc=t16;
	crc_append((uint8_t*)&partHeader,sizeof(volLogHeader_t)-2);
	qc_data_write_to_media(FREEZE_VOLLOG_HEADER_START_ADDR,(uint8_t*)&partHeader,sizeof(volLogHeader_t));
	return partHeader.unreadNum;
}

uint16_t freeze_item_read(uint8_t* buf,uint16_t ssize,uint16_t itemOffset)
{
	uint32_t t32,itemAddr;
	//uint16_t len;
	uint16_t t16;
	volLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));	
	if(partHeader.itemNum==0)return 0;
	if(ssize<sizeof(volItem_t))return 0;
	//找到读起始位置
	t16=partHeader.writeLoc;
	t16=t16+FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	uint16_t temp=partHeader.unreadNum;
	if(temp==0)temp=1;
	t16=t16-temp;
	//t16=t16-partHeader.unreadNum==0?1:partHeader.unreadNum;
	t16%=FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	//加上偏移量
	if(partHeader.unreadNum==0){
		itemOffset=0;
	}else if(itemOffset>=partHeader.unreadNum){
		itemOffset=partHeader.unreadNum-1;
	}
	t16+=itemOffset;
	t16%=FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	itemOffset=t16;	//保存偏移量
	//计算item地址
	t32=sizeof(volItem_t)*itemOffset;
	itemAddr=FREEZE_VOLLOG_BODY_START_ADDR+t32;
	
	qc_data_read_from_media(buf,itemAddr,sizeof(volItem_t));
	return sizeof(volItem_t);
}

uint16_t freeze_item_unread_num_offset(uint16_t itemOffset)
{
	uint16_t t16;
	volLogHeader_t partHeader;
	if(itemOffset==0)return 0;
	qc_data_read_from_media((uint8_t*)&partHeader,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));	
	if(partHeader.itemNum==0)return 0;
	
	if(itemOffset>partHeader.unreadNum)itemOffset=partHeader.unreadNum;
	t16=partHeader.unreadNum;
	t16+=FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	t16-=itemOffset;
	t16%=FREEZE_VOLLOG_ITEM_NUM_LIMITS;
	
	partHeader.unreadNum=t16;
	
	crc_append((uint8_t*)&partHeader,sizeof(volLogHeader_t)-2);
	qc_data_write_to_media(FREEZE_VOLLOG_HEADER_START_ADDR,(uint8_t*)&partHeader,sizeof(volLogHeader_t));
	return partHeader.unreadNum;	
}

uint16_t freeze_item_get_unread_num(void)
{
	volLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));	
	if(partHeader.itemNum==0)return 0;
	return partHeader.unreadNum==0?1:partHeader.unreadNum;
}

uint16_t freeze_item_get_num(void)
{
	volLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,FREEZE_VOLLOG_HEADER_START_ADDR,sizeof(volLogHeader_t));	
	if(partHeader.itemNum==0)return 0;
	//return partHeader.unreadNum==0?1:partHeader.unreadNum;
	return partHeader.itemNum;
}

void freeze_item_save_test(void)
{
	freeze_item_save();
}


//-->>

//<<--for szrq
//szrqVolLogHeader_t
//volLogHeader_t
uint16_t szrq_part_format(void)
{
	//uint16_t t16;
	uint8_t buf[16];
	szrqVolLogHeader_t* sth;
	sth=(szrqVolLogHeader_t*)buf;

	sth->writeLoc=0x00;
	sth->readLoc=0x00;
	sth->itemNum=0x00;
	crc_append(buf,sizeof(szrqVolLogHeader_t)-2);
	qc_data_write_to_media(SZRQ_VOLLOG_HEADER_START_ADDR,buf,sizeof(szrqVolLogHeader_t));
	return 0;
}

uint16_t szrq_item_save(void)
{
	//FREEZE
	//volItem_t
	uint16_t t16=0x00;
	uint32_t t32,itemAddr;
	szrqVolLogHeader_t partHeader;
	szrqVolLogItem_t	item;
	sysDataTime_t dt;
	qc_data_read_from_media((uint8_t*)&partHeader,SZRQ_VOLLOG_HEADER_START_ADDR,sizeof(szrqVolLogHeader_t));
	t32=partHeader.itemNum;
	if(t32>=SZRQ_VOLLOG_ITEM_NUM_LIMITS)return (uint16_t)t32;
	
	t32=sizeof(szrqVolLogItem_t)*t32;
	itemAddr=SZRQ_VOLLOG_BODY_START_ADDR+t32;

	m_rtc_get(&dt);
	t32=system_dt_to_time_stamp(&dt);
	item.ts=t32;
	item.vol=totalVolume;
	//<<--
	item.szrqRoportPeriodType=sysData.szrqRoportPeriodType;
	item.szrqRoportDataInterval=sysData.szrqRoportDataInterval;
	item.szrqtPeriodValue=sysData.szrqtPeriodValue;
	item.szrqcollectNextTm=szrqcollectNextTm;
	item.flow=hourAvgFlow[0];
	//-->>
	crc_append((uint8_t*)&item,sizeof(szrqVolLogItem_t)-2);
	qc_data_write_to_media(itemAddr,(uint8_t*)&item,sizeof(szrqVolLogItem_t));
	//
	if(partHeader.itemNum<SZRQ_VOLLOG_ITEM_NUM_LIMITS)partHeader.itemNum++;
	crc_append((uint8_t*)&partHeader,sizeof(szrqVolLogHeader_t)-2);
	qc_data_write_to_media(SZRQ_VOLLOG_HEADER_START_ADDR,(uint8_t*)&partHeader,sizeof(szrqVolLogHeader_t));
	return partHeader.itemNum;
}

uint16_t szrq_item_read(uint8_t* buf,uint16_t ssize,uint16_t itemOffset)
{
	//FREEZE
	//volItem_t
	//unreadNum
	uint32_t t32,itemAddr;
	uint16_t t16;
	szrqVolLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,SZRQ_VOLLOG_HEADER_START_ADDR,sizeof(szrqVolLogHeader_t));	
	if(partHeader.itemNum==0)return 0;
	
	if(ssize<sizeof(szrqVolLogItem_t))return 0;
	//找到读起始位置
	if(partHeader.readLoc+itemOffset>=partHeader.itemNum)return 0;
	
	t32=partHeader.readLoc+itemOffset;
	itemAddr=t32*sizeof(szrqVolLogItem_t);
	itemAddr+=SZRQ_VOLLOG_BODY_START_ADDR;
	qc_data_read_from_media(buf,itemAddr,sizeof(szrqVolLogItem_t));
	t16=crc_verify(buf,sizeof(szrqVolLogItem_t));
	if(!t16){
		szrq_part_format();
		return 0;
	}
	return sizeof(szrqVolLogItem_t);
}

uint16_t szrq_item_set_read_loc(uint16_t readNum)
{
	uint16_t t16;
	szrqVolLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,SZRQ_VOLLOG_HEADER_START_ADDR,sizeof(szrqVolLogHeader_t));	
	t16=partHeader.readLoc+readNum;
	if(t16>=partHeader.itemNum){
		szrq_part_format();
		return 1;
	}
	partHeader.readLoc=t16;
	crc_append((uint8_t*)&partHeader,sizeof(szrqVolLogHeader_t)-2);
	qc_data_write_to_media(SZRQ_VOLLOG_HEADER_START_ADDR,(uint8_t*)&partHeader,sizeof(szrqVolLogHeader_t));	
	return partHeader.readLoc;
}

uint16_t szrq_item_get_num(void)
{
	szrqVolLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,SZRQ_VOLLOG_HEADER_START_ADDR,sizeof(szrqVolLogHeader_t));	
	return partHeader.itemNum;
}

uint16_t szrq_item_get_unread_num(void)
{
	uint16_t t16;
	szrqVolLogHeader_t partHeader;
	qc_data_read_from_media((uint8_t*)&partHeader,SZRQ_VOLLOG_HEADER_START_ADDR,sizeof(szrqVolLogHeader_t));	
	//return partHeader.itemNum-partHeader.readLoc;
	//t16=partHeader.itemNum;
	if(partHeader.itemNum>partHeader.readLoc){
		t16=partHeader.itemNum-partHeader.readLoc;
	}else{
		t16=0;
	}
	if(t16==0 && partHeader.itemNum!=0){
		szrq_part_format();
	}
	return t16;
}

//-->
//file end
