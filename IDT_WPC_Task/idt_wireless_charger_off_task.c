/***********************************************************************************************************************
* File Name    : idt_wireless_charger_off_task.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file is wpc task function.
* Creation Date: 2019/11/14
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_adc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "wpc_application.h"
#include "acc_task.h"
#include "buzzer_task.h"
#include "led_task.h"
#include "power_sw_task.h"
#include "door_task.h"
#include "trunk_task.h"
#include "p9261_read_task.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
/***********************************************************************************************************************
Locate variables and functions
***********************************************************************************************************************/
unsigned char IDT_Wireless_Charger_Off_Process(void);
/***********************************************************************************************************************
* Function Name: IDT_Wireless_Charger_Off_Task
* Description  : This function is IDT_RX_Jiggle_Alarm_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_Wireless_Charger_Off_Task(void)
{
	if(IDT_WPC_State.Wireless_Charger_Off_Task_Flag==TRUE)
	{
		IDT_WPC_State.Wireless_Charger_Off_Task_Flag = FALSE;
		//---------------------------------------------//
		WPC_Function_Status.Temperature_Detect_Start_Flag = FALSE;
		CHARGE_STATE_LED_OFF;						// charger led off //
		LED_Status.Error_LED_Trigger_Flag = TRUE;	//Error_LED_Flash_Out();
		Buzzer_State.Error_Buzzer_Trigger_Flag = TRUE;
		WPC_Function_Status.Alarm_Continuous_Flag = TRUE;
		//---------------------------------------------//
		IDT_WPC_TASK = IDT_WPC_WIRELESS_CHARGER_OFF_TASK;
	}
	IDT_WPC_TASK = IDT_Wireless_Charger_Off_Process();
}
/***********************************************************************************************************************
* Function Name: IDT_Wireless_Charger_Off_Process
* Description  : This function is IDT_RX_Jiggle_Alarm_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char IDT_Wireless_Charger_Off_Process(void)
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				if(Other_Alarm.Wireless_Charge_Off_Flag==FALSE)
				{
					if(WPC_Function_Status.Alarm_Continuous_Flag==TRUE)
					{
						WPC_Function_Status.Alarm_Continuous_Flag = FALSE;
						Buzzer_Disable();
						Alarm_LED_Disable();
					}
					//-----------------------------------------------------------------------//				
					if(P9261_RegMessage.Idle_Status==TRUE)					// Tx State 01 //
					{
						P9261_RegMessage.Idle_Status = FALSE;
						IDT_WPC_State.Idle_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WPC_IDLE_TASK;
					}
					else if(P9261_RegMessage.Analog_Ping_Status==TRUE)		// Tx State 02 //
					{
						P9261_RegMessage.Analog_Ping_Status = FALSE;
						IDT_WPC_State.Analog_Ping_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WPC_ANALOG_PING_TASK;
					}
					else if(P9261_RegMessage.Q_Measurement_Status==TRUE)	// Tx State 03 //
					{
						P9261_RegMessage.Q_Measurement_Status = FALSE;
						IDT_WPC_State.Q_Measurement_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WPC_Q_MEASUREMENT_TASK;
					}
					else if(P9261_RegMessage.Digital_Ping_Status==TRUE)		// Tx State 04 //
					{
						P9261_RegMessage.Digital_Ping_Status = FALSE;
						IDT_WPC_State.Digital_Ping_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WPC_DIGITAL_PING_TASK;
					}
					else
					{
						IDT_WPC_NEXT_TASK = IDT_WPC_WIRELESS_CHARGER_OFF_TASK;
					}
				}
				else
				{
					if((Buzzer_State.Buzzer_Working_End_Flag==TRUE)&&(LED_Status.LED_Working_End_Flag==TRUE))
					{
						Buzzer_State.Buzzer_Working_End_Flag = FALSE;
						LED_Status.LED_Working_End_Flag = FALSE;
						Other_Alarm.Wireless_Charge_Off_Flag = FALSE;
					}
					IDT_WPC_NEXT_TASK = IDT_WPC_WIRELESS_CHARGER_OFF_TASK;
				}
			}
			else
			{
				IDT_WPC_State.EUT_Sleep_Task_Flag = TRUE;
				IDT_WPC_NEXT_TASK = IDT_WPC_EUT_SLEEP_TASK;
			}
		}
		else
		{
			Reset_Buzzer_LED_State();
			IDT_WPC_State.Power_Sw_Off_Task_Flag = TRUE;
			IDT_WPC_NEXT_TASK = IDT_WPC_POWER_SW_OFF_TASK;
		}
	}
	else
	{
		Buzzer_Disable();
		Alarm_LED_Disable();
		Phone_Placement_Detect();	// phone forget detect //
		WPC_Function_Status.ACC_Off_Dly3_5T_Flag = TRUE;
		IDT_WPC_State.Cellphone_Left_Alert_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_CELLPHONE_LEFT_ALERT_TASK;
	}
	return IDT_WPC_NEXT_TASK;
}