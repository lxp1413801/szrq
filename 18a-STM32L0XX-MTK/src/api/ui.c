#include "./includes/includes.h"
#ifndef cfg_IRDA_EN
#define cfg_IRDA_EN 1
#endif
volatile uint8_t menu=0x00; 
volatile uint8_t subMenu=0x00; 
const uint8_t hexTable[]="0123456789abcdef";
uint8_t lcdBuf[8]={0};
//uint8_t menu=0x00


bool fi_id_writed_in_dlcs_a(void)
{
	return (sysData.ID>10);
}

void tick_dly_dspy(uint8_t dly)
{
    lcd_clear_all(); 
    m_lcd_disp_str(__xT("dly--"));   
	do{
		//osDelay(1000);
        m_lcd_disp_dig(5,dly/10);
        m_lcd_disp_dig(6,dly%10);		
		m_lcd_refresh();
	}while(dly--);
}
void ui_start_dspy_id(uint8_t dly)
{

	//lcdLongStringHead=0;
	while(dly)
	{
		ui_disp_myID(dly>>2);
		osDelay(1000);
		dly--;
	}
}

void ui_dly(uint8_t *str,uint8_t loc,uint8_t dly)
{
	uint8_t i=0;
    lcd_clear_all(); 
	if(loc>5)loc=5;
	while(*str!='\0' && i<loc)
	{
		lcdBuf[i++] = *str++;
		//i++;
	}
	lcdBuf[loc+2]='\0';
	if(dly>20)dly=20;
	while(dly)
	{
		lcdBuf[loc]=hexTable[(dly/10)];
		lcdBuf[loc+1]=hexTable[(dly%10)];	
		//delay_1s();
		osDelay(1000);
		dly--;
		m_lcd_disp_str(lcdBuf);
		//m_lcd_refresh();	
	}
}
void ui_dly_ex(uint8_t *str,uint8_t loc,uint8_t* d)
{
	uint8_t i=0;
	if(loc>5)loc=5;
	while(*str!='\0' && i<loc)
	{
		lcdBuf[i++] = *str++;
		//i++;
	}
	lcdBuf[loc+2]='\0';
	if(*d>20)*d=20;
	while(*d)
	{	
		lcdBuf[loc]=hexTable[((*d)/10)];
		lcdBuf[loc+1]=hexTable[((*d)%10)];	
		//delay_1s();
		osDelay(1000);
		m_lcd_disp_str(lcdBuf);
		//m_lcd_refresh();
		*d--;
	}
}
void ui_hard_default_disp(void)
{
    if(hardDefault.t32==0)return;
    //lcd_enable();
    lcd_clear_all();    
    do{
        if(hardDefault.bits.ClockErr){
            m_lcd_disp_str(__xT("fau-01"));
            break;
        }
        if(hardDefault.bits.FlashErr){
            m_lcd_disp_str(__xT("fau-02"));
            break;
        }
        if(hardDefault.bits.EepromErr){
            m_lcd_disp_str(__xT("fau-03"));
            break;
        }
        if(hardDefault.bits.RtcErr){
            m_lcd_disp_str(__xT("fau-04"));
            break;
        }
        if(hardDefault.bits.ProxErr){
            m_lcd_disp_str(__xT("fau-05"));
            break;
        }

    }while(0);
}

void ui_disp_dp_v(void)
{
	/*
	uint16_t qs;
	qs=sysData.QS;
	switch(qs){
		case QS1_m3:		break;
		case QS0_1m3:		m_lcd_disp_seg_dp6();break;
		case QS0_01m3:		m_lcd_disp_seg_dp5();break;
		case QS0_001m3:		m_lcd_disp_seg_dp4();break;
		default:			m_lcd_disp_seg_dp5();break;
		
	}
	*/
	m_lcd_disp_seg_dp5();
}
void ui_disp_dp_v_ex(uint8_t x)
{
/*
	uint8_t loc=4;
	uint16_t qs;
	qs=sysData.QS;
	switch(qs){
		case QS1_m3:		break;
		case QS0_1m3:		loc=6;break;
		case QS0_01m3:		loc=5;break;
		case QS0_001m3:		loc=4;break;
		default:			loc=5;break;
		
	}
	m_lcd_sisp_dp(loc+x);
	*/
	m_lcd_sisp_dp(5+x);
}
void ui_disp_dp_m(void)
{
	uint16_t ps;
	ps=sysData.PS;
	switch(ps){
		case PS1:		break;
		case PS0_1:		m_lcd_disp_seg_dp6();break;
		case PS0_01:	m_lcd_disp_seg_dp5();break;
		case PS0_001:	m_lcd_disp_seg_dp4();break;
		default:		m_lcd_disp_seg_dp5();break;
	}	
}

void ui_disp_dp_m_ex(uint8_t x)
{
	uint8_t loc=4;
	
	uint16_t ps;
	ps=sysData.PS;
	switch(ps){
		case PS1:		break;
		case PS0_1:		loc=6;break;
		case PS0_01:	loc=5;;break;
		case PS0_001:	loc=4;;break;
		default:		loc=5;;break;
	}	
	m_lcd_sisp_dp(loc+x);
}
void ui_uint32_2_str(uint8_t* buf,int32_t x,uint16_t len)
{
	//uint8_t *p=(uint8_t*)(lcdBuf+(sizeof(lcdBuf)-1));
	int32_t t;
	if(!len)return;
	t=m_math_pow(10,len);
	if(x>=t-1)x=t-1;
	while(len--){
		t=x%10;
		*buf--=hexTable[t];
		x/=10;
	}
}
void ui_uint8_2_str(uint8_t* buf,int8_t x,uint16_t len)
{
	//uint8_t *p=(uint8_t*)(lcdBuf+(sizeof(lcdBuf)-1));
	int16_t t;
	if(!len)return;
    if(len==1)t=10;
    else if(len==2)t=100;
    else if(len==3)t=256;
	if(x>=t)x=(uint8_t)(t-1);
	while(len--){
		t=x%10;
		*buf--=hexTable[t];
		x/=10;
	}
}
uint8_t ui_disp_get_loc(uint16_t pqs)
{
	uint8_t loc=4;
	switch(pqs){
		case 1000:loc=3;break;
		case 10:loc=5;break;
		case 1:loc=6;break;
		case 100:
		default:loc=4;break;
	}	
	return loc;
}

void ui_head_hide(uint8_t* str,uint16_t loc)
{
	while(loc--){
		if(*str=='0'){
			*str++=' ';			
		}
		else{
			break;
		}
	}
}

void ui_disp_comm_success(void)
{
	lcd_clear_all();
	m_lcd_disp_str(__xT("success"));	
	m_lcd_refresh();
}

void ui_disp_comm_ok(void)
{
	lcd_clear_all();
	m_lcd_disp_str(__xT("upstate"));	
	m_lcd_refresh();
}

void ui_disp_comm_error(uint8_t err)
{
	lcd_clear_all();
	m_mem_set(lcdBuf,0,sizeof(lcdBuf));
	m_mem_cpy_len(lcdBuf,__xT("err-"),4);
	lcdBuf[4]='0'+err/10;
	lcdBuf[5]='0'+err%10;
	lcdBuf[6]='\0';
	m_lcd_disp_str(lcdBuf);
	m_lcd_refresh();
}

void ui_disp_comm_rssi(void)
{
	int8_t t8;

	uint32_t t32;
	t32=userTickerSec;
	t32>>=1;
	t32 &= 0x01ul;	
	lcd_clear_all();
	if(t32==0){
		t8=rssi;	
		if(t8>0)t8=-1;
		t8=0-t8;
		
		m_mem_set(lcdBuf,0,sizeof(lcdBuf));
		if(t8>99){
			m_mem_cpy_len(lcdBuf,__xT("co:-"),4);
			lcdBuf[4]='0'+t8/100;
			t8%=100;
			lcdBuf[5]='0'+t8/10;
			lcdBuf[6]='0'+t8%10;
			lcdBuf[7]='\0';
		}else{
			m_mem_cpy_len(lcdBuf,__xT("co:-"),4);
			lcdBuf[4]='0'+t8/10;
			lcdBuf[5]='0'+t8%10;
			lcdBuf[6]='\0';
		}
	}else{
		if(EARFCN[0]=='\0'){
			m_mem_cpy(lcdBuf,(uint8_t*)"unread");
		}else{
			m_mem_cpy_len(lcdBuf,(uint8_t*)EARFCN,7);
			lcdBuf[7]='\0';
		}
	}
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
}
/*
#define m_ui_buy_gas_prompt() if( \
	even.bits.bOMnotEnoughOFF || \
		even.bits.bOMnotEnoughWarning || \
		even.bits.bOVnotEnoughOFF || \
		even.bits.bOVnotEnoughWarning) \
	{m_lcd_disp_seg_QGQ_I();}	
*/
void ui_disp_alarm(void)
{
	#if(config_USED_ALARM_LCD)
	if(DeviceEvent.bits.bAlarmConn){
		m_lcd_disp_seg_alarm_conn();
	}
	if(DeviceEvent.bits.bLeakage){
		m_lcd_disp_seg_alarm_leak();
	}
	#endif
}
#define ui_disp_strong_magnetic() \
	{if(sysData.devStatus.bits.bStrongMagnetic)m_lcd_disp_seg_QCHB_G();}
	
#define ui_disp_nf_prompt() \
	{if(sysData.devStatus.bits.bQcFlowTmout)m_lcd_disp_seg_CSBH_F();}

void ui_disp_buy_gas_prompt(void)
{
	if(sysData.devStatus.bits.bBalanceSta!=CNM_BALANCE_NORMAL){
	//if(nowDevStatus.bits.bBalanceSta!=CNM_BALANCE_NORMAL){
		m_lcd_disp_seg_QGQ_I();
	}
}

void ui_disp_totale_volume(void)
{
	uint32_t tov;
	uint8_t loc;
	tov=totalVolume;
	m_mem_set(lcdBuf,0,sizeof(lcdBuf));	
	lcdBuf[7]='\0';
	lcd_clear_all(); 
	loc=ui_disp_get_loc(sysData.QS);
	if(loc<6){
		if(tov<=9999999){
			ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),tov,7);
			ui_head_hide(lcdBuf,4);
			//m_lcd_sisp_dp(loc);
			ui_disp_dp_v_ex(0);
		}else{
			tov/=10;
			ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),tov,7);
			//ui_head_hide(lcdBuf,loc+1);	
			//m_lcd_sisp_dp(loc+1);
			ui_disp_dp_v_ex(1);
		}
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),tov,7);
		ui_head_hide(lcdBuf,loc);		
		ui_disp_dp_v();
	}
	//ui_disp_dp_v();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
}

void ui_disp_totale_money(void)
{
	uint32_t tom;
	//tom=totalMoney;
	tom=sysData.totalPayMoney-sysData.OVerageVM;
	m_mem_set(lcdBuf,0,sizeof(lcdBuf));
	lcdBuf[7]='\0';
    //if(tom>999999)tom=999999;
    
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),tom,7);
	uint8_t loc=ui_disp_get_loc(sysData.PS);
	ui_head_hide(lcdBuf,loc);

	lcd_clear_all(); 
	ui_disp_dp_m();
	//ui_disp_buy_gas_prompt();
    lcdBuf[0]='t';
	m_lcd_disp_str(lcdBuf);
	m_lcd_refresh();
}


void ui_disp_current_v(void)
{
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);	
	uint32_t cv;
	cv=totalVolume-sysData.periodVolume;
	m_mem_set(lcdBuf,'0',sizeof(lcdBuf));
	lcdBuf[7]='\0';
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),cv,6);
	uint8_t loc=ui_disp_get_loc(st->QS);
	ui_head_hide(lcdBuf,loc);
	lcdBuf[0]='c';
	//lcdBuf[1]=' ';

	lcd_clear_all(); 
	ui_disp_dp_v();
	//ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
}
//金额表工作模式显示
/*
void ui_disp_current_m(void)
{
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);	
 	uint32_t cm;
	//cm=curMoney;
	cm=sysData.totalPayMoney-sysData.OVerageVM-sysData.MoneyPerMon[0];
	m_mem_set(lcdBuf,'0',sizeof(lcdBuf));
	lcdBuf[7]='\0';
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),cm,6);
	uint8_t loc=ui_disp_get_loc(st->PS);
	ui_head_hide(lcdBuf,loc);
	lcdBuf[0]='n';
	//lcdBuf[1]=' ';
	lcd_clear_all();
    ui_disp_dp_m();
	//ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();   
}
*/
void ui_disp_price(void)
{
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);	
	uint32_t pc=sysData.curPrice;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),pc,7);
	uint8_t loc=ui_disp_get_loc(st->PS);
	ui_head_hide(lcdBuf,loc);

	lcd_clear_all(); 
	ui_disp_dp_m();
	m_lcd_disp_seg_DJ_B();
	//ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
}

void ui_disp_pay_value(void)
{
	uint32_t tm;
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);	
	uint8_t dpLoc=0;
	uint8_t loc=0;
	int32_t t32;//=realPayValue;
	if(sysData.DLCS>=DLC_STATUS_C){
		t32=realPayValue;
	}else{
		t32=sysData.OVerageVM;
	}
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	if(t32>9999999){
		t32/=10;
		if(t32>9999999)t32=9999999;
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32,7);
		loc=ui_disp_get_loc(st->PS);
		ui_head_hide(lcdBuf,loc+1);	
		dpLoc=1;
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32,7);
		loc=ui_disp_get_loc(st->PS);
		ui_head_hide(lcdBuf,loc);	
		//dpLoc=1;	
	}
	lcd_clear_all(); 
	tm=userTickerSec;
	if(tm & 0x01ul){
		ui_disp_dp_m_ex(dpLoc);
		m_lcd_disp_str(lcdBuf);
	}else{
		
	}
}

void ui_szrq_disp_overage_vol(void)
{
	//stDeviceInf_t* st;
	//st=(stDeviceInf_t*)(&sysData);	
	int32_t ov=sysData.szrqBalanceVol;
	uint16_t i;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	//uint8_t loc=ui_disp_get_loc(st->QS);
	uint8_t loc=5;
	if(ov>9999999){
		ov/=10;
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),ov,7);
		ui_head_hide(lcdBuf,loc);		
		//ui_disp_dp_v_ex(1);
		m_lcd_sisp_dp(loc+1);
	}else if(ov>=0){
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),ov,7);
		ui_head_hide(lcdBuf,loc-1);
		//ui_disp_dp_v_ex(0);
		m_lcd_sisp_dp(loc+0);
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),0-ov,7);
		ui_head_hide(lcdBuf,loc-1);
		for(i=0;i<loc;i++){
			if(lcdBuf[i]!=' '){
				break;
			}
			continue;
		}
		if(i==0){
			m_mem_cpy(lcdBuf,(uint8_t*)"-999999");
		}else{
			lcdBuf[i-1]='-';
		}
		//ui_disp_dp_v_ex(0);
		m_lcd_sisp_dp(loc+0);
	}
	lcd_clear_all(); 
	ui_disp_dp_v();
	m_lcd_disp_seg_YQL_A1();
	ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);	
}

void ui_disp_overage_v(void)
{
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);	
	int32_t ov=overrageVolume;
	uint16_t i;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	uint8_t loc=ui_disp_get_loc(st->QS);
	if(ov>9999999){
		ov/=10;
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),ov,7);
		ui_head_hide(lcdBuf,loc);		
		ui_disp_dp_v_ex(1);
	}else if(ov>=0){
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),ov,7);
		ui_head_hide(lcdBuf,loc);
		ui_disp_dp_v_ex(0);
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),0-ov,7);
		ui_head_hide(lcdBuf,loc);
		for(i=0;i<loc;i++){
			if(lcdBuf[i]!=' '){
				break;
			}
			continue;
		}
		if(i==0){
			m_mem_cpy(lcdBuf,(uint8_t*)"-999999");
		}else{
			lcdBuf[i-1]='-';
		}
		ui_disp_dp_v_ex(0);
	}
	lcd_clear_all(); 
	ui_disp_dp_v();
	m_lcd_disp_seg_YQL_A1();
	ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();	
}

void ui_szrq_disp_balance(void)
{
	uint16_t i;
	int32_t om,t32;
	//t32= OVerageVMRemainder+sysData.OVerageVMRemainder;
	//t32=t32 / 10000;
	//om=overageMoney - t32;
	om=sysData.szrqBalance;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	//uint8_t loc=ui_disp_get_loc(sysData.PS);
	uint8_t loc=5;
	lcd_clear_all(); 
	if(om>9999999){
		om/=10;
		if(om>9999999)om=9999999;
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),om,7);
		ui_head_hide(lcdBuf,loc);
		//ui_disp_dp_m_ex(1);
		m_lcd_sisp_dp(loc+1);
	}else if(om>=0){
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),om,7);
		ui_head_hide(lcdBuf,loc-1);
		//ui_disp_dp_m_ex(0);	
		m_lcd_sisp_dp(loc+0);
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),0-om,7);
		ui_head_hide(lcdBuf,loc-1);
		for(i=0;i<loc;i++){
			if(lcdBuf[i]!=' '){
				break;
			}
			continue;
		}
		if(i==0){
			m_mem_cpy(lcdBuf,(uint8_t*)"-999999");
		}else{
			lcdBuf[i-1]='-';
		}
		//ui_disp_dp_m();	
		m_lcd_sisp_dp(loc+0);
	}
	m_lcd_disp_seg_YUE_A0();
	ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);	
}

void ui_disp_overage_m_ex(void)
{

	uint16_t i;
	int32_t om,t32;
	t32= OVerageVMRemainder+sysData.OVerageVMRemainder;
	t32=t32 / 10000;
	om=overageMoney - t32;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	uint8_t loc=ui_disp_get_loc(sysData.PS);
	
	lcd_clear_all(); 
	if(om>9999999){
		om/=10;
		if(om>9999999)om=9999999;
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),om,7);
		ui_head_hide(lcdBuf,loc);
		ui_disp_dp_m_ex(1);
	}else if(om>=0){
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),om,7);
		ui_head_hide(lcdBuf,loc);
		ui_disp_dp_m_ex(0);	
	}else{
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),0-om,7);
		ui_head_hide(lcdBuf,loc);
		for(i=0;i<loc;i++){
			if(lcdBuf[i]!=' '){
				break;
			}
			continue;
		}
		if(i==0){
			m_mem_cpy(lcdBuf,(uint8_t*)"-999999");
		}else{
			lcdBuf[i-1]='-';
		}
		ui_disp_dp_m();	
	}
	m_lcd_disp_seg_YUE_A0();
	ui_disp_buy_gas_prompt();
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();	
}
/*
void ui_disp_on(void)
{
	m_lcd_mem_clear_all();
	
}
*/
/*
1:
nf-off:no flow 
sf-off:small flow
cf-off:continuous gas time out
hf-off:hight flow
2:
hb-off:hight battary voltage
lb-off:low battary voltage
3:
lg-off:low gas volume
ng-off:no gas volime
4:
ln-off:low morney
nn-off:no morney
5:
st-off:
ex-off:extern input off
6:
se-off:remote sever off
8:
er-off:device error off
*/
void ui_disp_off_reason(void)
{
	uint8_t str[10];
	uint8_t t8=VavleOffReason;
	switch(t8){
		//case OFF_REASON_NONE:
		case OFF_REASON_HARD_DEFAULT:	m_lcd_disp_str((uint8_t*)"er-Off");break;
		case OFF_REASON_LO_VOLETAGE:	m_lcd_disp_str((uint8_t*)"lb-Off");break;
		case OFF_REASON_HI_VOLETAGE:	m_lcd_disp_str((uint8_t*)"hb-Off");break;
		case OFF_REASON_NO_GAS_FLOW:	m_lcd_disp_str((uint8_t*)"nf-Off");break;
		case OFF_REASON_SAMLL_GAS_FLOW:	m_lcd_disp_str((uint8_t*)"sf-Off");break;
		case OFF_REASON_HIGHT_GAS_FLOW:	m_lcd_disp_str((uint8_t*)"hf-Off");break;
		case OFF_REASON_CONTINUOUS_FLOW:m_lcd_disp_str((uint8_t*)"cf-Off");break;		
		case OFF_REASON_LOW_OV:		m_lcd_disp_str((uint8_t*)"lg-Off");break;
		case OFF_REASON_NO_OV:		m_lcd_disp_str((uint8_t*)"ng-Off");break;		
		case OFF_REASON_STRONG_MAGTIC:	
			m_str_cpy(str,(uint8_t*)" st-Off");
			str[0]=(uint8_t)(sysData.lockReason.bits.bStrongMagnetic)+'0';
			m_lcd_disp_str(str);
			break;	
		case OFF_REASON_EXTERN_SIG:		m_lcd_disp_str((uint8_t*)"et-Off");break;		
		case OFF_REASON_SEVERS:			m_lcd_disp_str((uint8_t*)"se-Off");break;
		case OFF_REASON_MANUAL:			m_lcd_disp_str((uint8_t*)"tstOff");break;
		case OFF_REASON_LEAKAGE:		m_lcd_disp_str((uint8_t*)"Gas-ala");break;
		case OFF_REASON_CZSN_NONET:		m_lcd_disp_str((uint8_t*)"nc-Off");break;
		case OFF_REASON_CZSN_WARNING:	m_lcd_disp_str((uint8_t*)"pr-Off");break;
		case OFF_REASON_SHELL_OPEN:		m_lcd_disp_str((uint8_t*)"sh-Off");break;
		default	:						m_lcd_disp_str((uint8_t*)" press");break;
		
		
		
	}
    
    if(t8==OFF_REASON_HIGHT_GAS_FLOW)
        m_lcd_disp_seg_GLBH_E();
    
	//m_lcd_refresh();
}

void ui_disp_id_lo(void)
{
	uint32_t t32;
	lcd_clear_all(); 
	lcdBuf[7]='\0';
	//x>>=2;
	//t32=sysData.ID;
	//ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32%10000000,7);	
	m_str_cpy(lcdBuf,sysData.commSN+9);
	m_lcd_disp_str(lcdBuf);
}
#if SEND_TIMES_TEST
void ui_disp_send_times(void)
{
	uint32_t t32;
	lcd_clear_all(); 
	lcdBuf[7]='\0';
	//x>>=2;
	t32=nbPopTimes;
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32,7);	
	m_lcd_disp_str(lcdBuf);
}
#endif
void ui_disp_id_hi(void)
{
	uint32_t t32;
	lcd_clear_all(); 
	lcdBuf[7]='\0';
	//x>>=2;
	t32=sysData.ID;
	m_mem_cpy_len(lcdBuf,(uint8_t*)"idh:",4);
	ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32/10000000,3);	
	m_lcd_disp_str(lcdBuf);
}

void ui_disp_myID(uint16_t x)
{
	uint32_t t32;
	lcd_clear_all(); 
	lcdBuf[7]='\0';
	//x>>=2;
	t32=sysData.ID;
	if(x&0x01ul){
		lcdBuf[0]='h';
		lcdBuf[1]=':';
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32/100000,5);
	}else{
		lcdBuf[0]='l';
		lcdBuf[1]=':';
		ui_uint32_2_str((uint8_t*)(lcdBuf+sizeof(lcdBuf)-2),t32%100000,5);	
	}
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
}

//lcdLongStringHead:滚动显示用
/*
void ui_disp_myid_ex(uint16_t x)
{
	uint8_t buf[16]={0};
	uint32_t t32;
	stDeviceInf_t* st;
	st=(stDeviceInf_t*)(&sysData);
	
	t32=sysData.ID;
	
	lcd_clear_all(); 
	m_mem_cpy_len(buf,__xT("id:"),3);
	m_str_b2h(buf+3,(uint8_t*)(&t32),4);
	buf[11]='.';
	buf[12]='\0';
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	m_mem_cpy_len(lcdBuf,buf+lcdLongStringHead,7);
	lcdLongStringHead++;
	if(lcdLongStringHead>6)lcdLongStringHead=0;
	m_lcd_disp_str(lcdBuf);

}
*/
/*
void ui_disp_key_index(void)
{
	uint8_t t;
	t=sysData.compTransKeyIndex;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	m_mem_cpy_len(lcdBuf,__xT("key:"),4);
    if(t<10){
		lcdBuf[4]='0'+t;
	}else if(t<100){
		lcdBuf[4]='0'+t/10;
		t%=10;
		lcdBuf[5]='0'+t;
	}else{
		t&=0xff;
		lcdBuf[4]='0'+t/100;
		t%=100;
		lcdBuf[5]='0'+t/10;
		t%=10;
		lcdBuf[6]='0'+t;
	}
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);	
	//m_lcd_refresh();
}
*/
/*
void ui_disp_key_index_comp(void)
{
	uint8_t t;
	t=sysData.compTransKeyIndex;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	m_mem_cpy_len(lcdBuf,__xT("  l:"),4);
    if(t<10){
		lcdBuf[4]='0'+t;
	}else if(t<100){
		lcdBuf[4]='0'+t/10;
		t%=10;
		lcdBuf[5]='0'+t;
	}else{
		t&=0xff;
		lcdBuf[4]='0'+t/100;
		t%=100;
		lcdBuf[5]='0'+t/10;
		t%=10;
		lcdBuf[6]='0'+t;
	}
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);		
}
*/
/*
void ui_disp_key_index_user(void)
{
	uint8_t t;
	t=sysData.userTransKeyIndex;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	m_mem_cpy_len(lcdBuf,__xT("  r:"),4);
    if(t<10){
		lcdBuf[4]='0'+t;
	}else if(t<100){
		lcdBuf[4]='0'+t/10;
		t%=10;
		lcdBuf[5]='0'+t;
	}else{
		t&=0xff;
		lcdBuf[4]='0'+t/100;
		t%=100;
		lcdBuf[5]='0'+t/10;
		t%=10;
		lcdBuf[6]='0'+t;
	}
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);		
}
*/
void ui_disp_device_ver(void)
{

	uint8_t* p;
	p=(uint8_t*)(&sysData);
	p+=get_offset_addr_of_str(stDeviceInf_t,DeviceName);
	m_mem_cpy(lcdBuf,p);
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);	
	//m_lcd_refresh();
}

void ui_disp_soft_ver(void)
{
	uint32_t t32;
	uint8_t* p;
	uint16_t t16;
	t32=userTickerSec;//>>2;
	t32>>=2;
	t32%=3;
	switch(t32){
		case 0x00:

			lcdBuf[0]='n';
			lcdBuf[1]='b';
			
			t16=NB20E11_SUB_VER;
			t16%=100;
			lcdBuf[2]=(uint8_t)((t16/10)+'0');
			lcdBuf[3]=(uint8_t)((t16%10)+'0');
			
			lcdBuf[4]=(uint8_t)(config_NB_PLAT+'1');
			
			t16=sw_VER_NUM;
			t16%=100;
			lcdBuf[5]=(uint8_t)((t16/10)+'0');
			lcdBuf[6]=(uint8_t)((t16%10)+'0');			
			
			lcdBuf[7]='\0';
			
			break;
		case 0x01:
			p=SPVER;
			if(*p=='\0'){
				m_mem_cpy(lcdBuf,(uint8_t*)"unread");
			}else{
				m_mem_cpy_len(lcdBuf,p,6);
				lcdBuf[6]='\0';
			}
			break;
		case 0x02:
			p=CGSN+8;
			if(*p=='\0'){
				m_mem_cpy(lcdBuf,(uint8_t*)"unread");
			}else{
				m_mem_cpy_len(lcdBuf,p,7);
				lcdBuf[7]='\0';
			}			
			break;
	}
	
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);	
	if(t32==0x00ul){
		//m_lcd_disp_seg_dp2();
		m_lcd_disp_seg_dp4();
		m_lcd_disp_seg_dp5();
	}
	//m_lcd_refresh();
}

void ui_disp_qn(void)
{
	//m_mem_set(lcdBuf,'0',sizeof(lcdBuf));
	//stDeviceInf_t* st;
	//st=(stDeviceInf_t*)(&sysData);	
	uint16_t _qn;
	_qn=sysData.QN;
	lcd_clear_all(); 
	switch(_qn){
        case QN10: 		m_lcd_disp_str(__xT("  qn:10"));m_lcd_disp_seg_dp6();break;
        case QN16: 		m_lcd_disp_str(__xT("  qn:16"));m_lcd_disp_seg_dp6();break;           
        case QN25: 		m_lcd_disp_str(__xT("  qn:25"));m_lcd_disp_seg_dp6();break;
        case QN40: 		m_lcd_disp_str(__xT("  qn:4 "));break;            
        case QN60: 		m_lcd_disp_str(__xT("  qn:6 "));break;
        case QN100: 	m_lcd_disp_str(__xT(" qn:10 "));break;            
        case QN160: 	m_lcd_disp_str(__xT(" qn:16 "));break;
        case QN250: 	m_lcd_disp_str(__xT(" qn:25 "));break;            
        case QN400: 	m_lcd_disp_str(__xT(" qn:40 "));break;
//        case QN600: 	m_lcd_disp_str(__xT(" qn:60 "));break;           
//        case QN1000: 	m_lcd_disp_str(__xT("qn:100 "));break;
		default :		m_lcd_disp_str(__xT(" qn:err"));break;
	}
	//m_lcd_disp_seg_dp6();
	//m_lcd_refresh();
}

void ui_disp_dlc(void)
{
	//#if(APP_VERSION==_17_A)
	#if 1
//	stDeviceInf_t* st;
//	st=(stDeviceInf_t*)(&sysData);

    if(sysData.DLCS<DLC_STATUS_A || sysData.DLCS > DLC_STATUS_E){
        hardDefault.bits.FlashErr=1;
        return;
    }
    lcd_clear_all(); 
    switch(sysData.DLCS){
        case DLC_STATUS_A:m_lcd_disp_str(__xT("  s:a"));break;
        case DLC_STATUS_B:m_lcd_disp_str(__xT("  s:b"));break;
        case DLC_STATUS_C:m_lcd_disp_str(__xT("  s:c"));break;
        case DLC_STATUS_D:m_lcd_disp_str(__xT("  s:d"));break;
        case DLC_STATUS_E:m_lcd_disp_str(__xT("  s:e"));break;
    }
	#else
		return;
	#endif
	//m_lcd_refresh();
}
//vavleState=VALEE_ON
//普通工作模式显示
void ui_disp_voltage_promit(void)
{
	if(sysData.devStatus.bits.bPwrStatus ==POWER_STATUS_LOW || sysData.devStatus.bits.bPwrStatus==POWER_STATUS_DOWN)m_lcd_disp_seg_DYBH_D(); 
	if(sysData.devStatus.bits.bPwrStatus ==POWER_STATUS_OVER)m_lcd_disp_seg_GYBH_C(); 
}

void ui_disp_off(void)
{
    lcd_clear_all(); 
	uint8_t tmtry=sysData.qcNoTryTimes;
	if(tmtry==1)tmtry=2;
	if((sysData.lockReason.bits.bNoflow>=tmtry && tmtry!=0) || (sysData.lockReason.bits.bStrongMagnetic>=MAX_STE_OFF_TIMES) \
	|| sysData.lockReason.bits.bSeverOff || sysData.lockReason.bits.bNoNetwork || sysData.lockReason.bits.bBalance \
	|| sysData.lockReason.bits.bShellOpen ){
		//m_lcd_disp_str((uint8_t*)"   hold");
		if(sysData.lockReason.bits.bNoflow>=tmtry && tmtry!=0){m_lcd_disp_str((uint8_t*)"nf-loc ");}
		else if(sysData.lockReason.bits.bStrongMagnetic>=MAX_STE_OFF_TIMES){
			m_lcd_disp_str((uint8_t*)"st-loc ");
		}
        else if(sysData.lockReason.bits.bSeverOff){m_lcd_disp_str((uint8_t*)"se-loc ");}
		else if(sysData.lockReason.bits.bNoNetwork){m_lcd_disp_str((uint8_t*)"nc-loc ");}
		else if(sysData.lockReason.bits.bBalance){m_lcd_disp_str((uint8_t*)"ng-loc ");}
		else if(sysData.lockReason.bits.bShellOpen){m_lcd_disp_str((uint8_t*)"sh-loc ");}
	}else{
		ui_disp_off_reason();
	}	
	//m_lcd_refresh();	
}

void ui_disp_vavle_state(void)
{
	if(vavleState==VALVE_OFF){
		ui_disp_off();
	}
	else
	{
		lcd_clear_all(); 
		m_lcd_disp_str(__xT(" -ON-  "));
	}
	//m_lcd_refresh();
}

void ui_show_hms(void)
{
	uint8_t t8;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	lcd_clear_all(); 
	t8=stRtcDataTime.hh;

	lcdBuf[0]=hexTable[t8/16];
	lcdBuf[1]=hexTable[t8%16];
	lcdBuf[2]='-';
	m_lcd_disp_seg_dp5();
	t8=stRtcDataTime.mm;
	lcdBuf[3]=hexTable[t8/16];
	lcdBuf[4]=hexTable[t8%16];	
	//lcdBuf[4]='-';
	t8=stRtcDataTime.ss;
	lcdBuf[5]=hexTable[t8/16];
	lcdBuf[6]=hexTable[t8%16];	
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
    //ui_disp_voltage_promit();
}

void ui_show_ymd(void)
{
	uint8_t t8;
	m_mem_set(lcdBuf,'\0',sizeof(lcdBuf));
	lcd_clear_all(); 
	t8=stRtcDataTime.YY;

	lcdBuf[0]=hexTable[t8/16];
	lcdBuf[1]=hexTable[t8%16];
	lcdBuf[2]='-';
	m_lcd_disp_seg_dp5();
	t8=stRtcDataTime.MM;
	lcdBuf[3]=hexTable[t8/16];
	lcdBuf[4]=hexTable[t8%16];	
	//lcdBuf[4]='-';
	t8=stRtcDataTime.DD;
	lcdBuf[5]=hexTable[t8/16];
	lcdBuf[6]=hexTable[t8%16];	
	m_lcd_disp_str(lcdBuf);
	//m_lcd_refresh();
    //ui_disp_voltage_promit();
}

void ui_disp_main_c_mode(uint32_t tm)
{
	//uint16_t t16=tm>>1;
	if(vavleState==VALVE_ON)
	{
        /*
		switch(t16%2)
		{
			case 2:
				ui_disp_totale_volume();
				break;
			case 1:
				ui_disp_current_volume();		
				break;
		}
        */
        ui_disp_totale_volume();
	}
	else if(vavleState==VALVE_OFF)
	{
		ui_disp_off();
	}		
}

void ui_disp_main_v_mode(uint32_t tm)
{
	uint32_t t32;
	if(sysData.DLCS==DLC_STATUS_A){
	
		t32=userTickerSec;//>>1;
		t32>>=1;
		t32%=3;
		switch(t32){
			case 0x00:ui_disp_id_lo();break;
			//case 0x01:ui_disp_id_hi();break;
			case 0x01:ui_disp_overage_v();break;
			case 0x02:ui_disp_comm_rssi();break;
		}
	}else{
		if(vavleState==VALVE_ON)
		{
			ui_disp_overage_v();
		}
		else if(vavleState==VALVE_OFF)
		{
			ui_disp_off();
		}	
	}	
}

void ui_disp_main_m_mode_15E(void)
{
	uint32_t t32;
	if(sysData.DLCS==DLC_STATUS_A){
	
		t32=userTickerSec;//>>1;
		t32>>=1;
		t32%=3;
		switch(t32){
			case 0x00:ui_disp_id_lo();break;
			//case 0x01:ui_disp_id_hi();break;
			case 0x01:ui_disp_overage_m_ex();break;
			case 0x02:ui_disp_comm_rssi();break;
		}
	}else{
		if(vavleState==VALVE_ON)
		{
			ui_disp_overage_m_ex();
		}
		else if(vavleState==VALVE_OFF)
		{
			ui_disp_off();
		}	
	}
}

void ui_disp_main_m_mode(uint32_t tm)
{
	if(vavleState==VALVE_ON)
	{
        ui_disp_overage_m_ex();
	}
	else if(vavleState==VALVE_OFF)
	{
		ui_disp_off();
	}
}

void ui_disp_main_nb_attack(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));
	
	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"CONN-----",4+t16);
	buf[t16+4]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   	
}

void ui_disp_main_nb_socket_create(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"sock-----",4+t16);
	buf[t16+4]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   
}

void ui_disp_main_nb_send(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"send-----",4+t16);
	buf[t16+4]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   
}

void ui_disp_main_nb_received(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"rec-----",3+t16);
	buf[t16+3]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   

}

void ui_disp_main_nb_enter_psm(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"end-----",3+t16);
	buf[t16+3]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   

}

void ui_disp_main_vavle_on(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)" on-----",3+t16);
	buf[t16+3]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   

}

void ui_disp_main_vavle_off(uint32_t tm)
{
	uint16_t t16;
	uint8_t buf[12];
	m_mem_set(buf,'\0',sizeof(buf));

	lcd_clear_all();
	t16=(uint16_t)(tm & 0x03ul);
	m_mem_cpy_len(buf,(uint8_t*)"off-----",3+t16);
	buf[t16+3]='\0';
	m_lcd_disp_str(buf);
	//m_lcd_refresh();   

}
void ui_disp_home_m(uint32_t tm)
{
	switch(subMenu){
		case 0:ui_disp_main_m_mode_15E();break;
		case subMENU_MAIN_HOME_NB_ATTACH:ui_disp_main_nb_attack(tm);break;
		case subMENU_MAIN_HOME_NB_SOCKET:ui_disp_main_nb_socket_create(tm);break;
		case subMENU_MAIN_HOME_NB_SEND:ui_disp_main_nb_send(tm);break;
		case subMENU_MAIN_HOME_NB_RECEIVED:ui_disp_main_nb_received(tm);break;
		case subMENU_MAIN_VAVLE_OFF:ui_disp_main_vavle_off(tm);break;
		case subMENU_MAIN_VAVLE_ON:ui_disp_main_vavle_on(tm);break;
		case subMENU_MAIN_HOME_NB_PSM:ui_disp_main_nb_enter_psm(tm);break;
		#if SEND_TIMES_TEST
		case subMENU_MAIN_SEND_TIMES:ui_disp_send_times();break;
		#endif
		default :break;
	}
}

void ui_disp_home_v(uint32_t tm)
{
	switch(subMenu){
		case 0:ui_disp_main_v_mode(tm);break;
		case subMENU_MAIN_HOME_NB_ATTACH:ui_disp_main_nb_attack(tm);break;
		case subMENU_MAIN_HOME_NB_SOCKET:ui_disp_main_nb_socket_create(tm);break;
		case subMENU_MAIN_HOME_NB_SEND:ui_disp_main_nb_send(tm);break;
		case subMENU_MAIN_HOME_NB_RECEIVED:ui_disp_main_nb_received(tm);break;
		case subMENU_MAIN_VAVLE_OFF:ui_disp_main_vavle_off(tm);break;
		case subMENU_MAIN_VAVLE_ON:ui_disp_main_vavle_on(tm);break;		
		case subMENU_MAIN_HOME_NB_PSM:ui_disp_main_nb_enter_psm(tm);break;
		default :break;
	}
}
void ui_disp_ir_received(void)
{
	m_mem_cpy(lcdBuf,(uint8_t*)"ir---");
	lcd_clear_all(); 
	m_lcd_disp_str(lcdBuf);		
}
void ui_disp_menu_m_dlcs_cde(void)
{
	
}

void ui_disp_menu_m(void)
{
	if(payPromptTimeOut && menu==0){
		ui_disp_pay_value();
		return ;
	}
	switch(menu)
	{
		case 0:ui_disp_home_m(userTickerSec);	break;
		
		case 1:ui_disp_id_lo();				break;
		case 2:ui_disp_id_hi();				break;		
		
		case 3:ui_disp_price();					break;
		case 4:ui_disp_current_v();				break;
		case 5:ui_disp_totale_volume();			break;
		case 6:ui_disp_overage_m_ex();				break;
		case 7:ui_disp_comm_rssi();				break;
		case 8:ui_disp_dlc();					break;
		case 9:ui_disp_qn();					break;
		case 10:ui_disp_soft_ver();				break;
		case 11:ui_show_ymd();					break;
		case 12:ui_show_hms();					break;
	
		case 13:ui_disp_vavle_state();			break;
		#if cfg_IRDA_EN
		case 16:ui_disp_ir_received();			break;
		#endif 
		default:break;				
	}		
	
}
//气量表工作模式显示
void ui_disp_menu_v(void)
{
	uint16_t m;
	m=menu;
	switch(m)
	{
		case 0:ui_disp_home_v(userTickerSec);	break;
		
		case 1:ui_disp_id_lo();				break;
		case 2:ui_disp_id_hi();				break;		
		case 3:ui_disp_totale_volume();			break;
		case 4:	ui_disp_overage_v();
		case 5:ui_disp_comm_rssi();				break;
		case 6:ui_disp_dlc();					break;
		case 7:ui_disp_qn();					break;
		case 8:ui_disp_soft_ver();				break;
		case 9:ui_show_ymd();					break;
		case 10:ui_show_hms();					break;
		case 11:ui_disp_vavle_state();			break;
		#if cfg_IRDA_EN
		case 16:ui_disp_ir_received();			break;
		#endif 
		default:break;					
	}	
}

void ui_disp_main_common_mode_15E(void)
{
	uint32_t t32;
	if(sysData.DLCS==DLC_STATUS_A){
	
		//t32=globleTickerSec>>1;
		t32=userTickerSec;
		t32>>=1;
		t32%=3;
		switch(t32){
			case 0x00:ui_disp_id_lo();break;
			case 0x01:ui_disp_overage_v();break;
			case 0x02:ui_disp_comm_rssi();break;
		}
	}else{
		if(vavleState==VALVE_ON)
		{
			//ui_disp_overage_m_ex();
			ui_disp_totale_volume();
		}
		else if(vavleState==VALVE_OFF)
		{
			ui_disp_off();
		}	
	}
}
void ui_disp_home_comm_reader(uint32_t tm)
{

	switch(subMenu){
		case 0:ui_disp_main_common_mode_15E();break;
		case subMENU_MAIN_HOME_NB_ATTACH:ui_disp_main_nb_attack(tm);break;
		case subMENU_MAIN_HOME_NB_SOCKET:ui_disp_main_nb_socket_create(tm);break;
		case subMENU_MAIN_HOME_NB_SEND:ui_disp_main_nb_send(tm);break;
		case subMENU_MAIN_HOME_NB_RECEIVED:ui_disp_main_nb_received(tm);break;
		case subMENU_MAIN_VAVLE_OFF:ui_disp_main_vavle_off(tm);break;
		case subMENU_MAIN_VAVLE_ON:ui_disp_main_vavle_on(tm);break;
		case subMENU_MAIN_HOME_NB_PSM:ui_disp_main_nb_enter_psm(tm);break;
		#if SEND_TIMES_TEST
		case subMENU_MAIN_SEND_TIMES:ui_disp_send_times();break;
		#endif
		default :break;
	}
	
}
void ui_disp_menu_comm_reader(void)
{
	if(payPromptTimeOut && menu==0){
		ui_disp_pay_value();
		return ;
	}
	switch(menu){
		case 0:ui_disp_home_comm_reader(userTickerSec);	break;
		case 1:ui_disp_id_lo();					break;
		case 2:ui_disp_comm_rssi();				break;
		case 3:ui_disp_dlc();					break;
		case 4:ui_disp_qn();					break;
		case 5:ui_disp_soft_ver();				break;
		case 6:ui_show_ymd();					break;
		case 7:ui_show_hms();					break;
		//<<--add for szrq
		case 8:ui_disp_totale_volume();			break;
		case 9:ui_szrq_disp_balance();			break;
		case 10:ui_szrq_disp_overage_vol();		break;
		//-->
		case 11:ui_disp_vavle_state();			break;
		
		#if cfg_IRDA_EN
		case 16:ui_disp_ir_received();			break;
		#endif 
		default:break;				
	}
	if(sysData.DLCS<=DLC_STATUS_B){
		ui_disp_buy_gas_prompt();
	}
}
void ui_disp_menu(uint32_t tm)
{
 	switch(sysData.DWM)
	{
		case DWM_STEP_MONEY_MODE:
			ui_disp_menu_m();
			ui_disp_buy_gas_prompt();
			break;
		case DWM_COMMON_MODE:
			ui_disp_menu_comm_reader();
			break;
		case DWM_VOLUME_MODE: 	
		default:
			ui_disp_menu_v();
			ui_disp_buy_gas_prompt();
			break;			
	}
    ui_disp_voltage_promit();
    ui_disp_buy_gas_prompt();
    ui_disp_strong_magnetic();
    ui_disp_nf_prompt();
	ui_disp_alarm();
	m_lcd_refresh();
}

