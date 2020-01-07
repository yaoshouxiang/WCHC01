/***********************************************************************************************************************
* File Name    : idt_negotiation_task.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file is wpc task function.
* Creation Date: 2019/9/4
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
unsigned char IDT_WPC_Negotiation_Process(void);
/***********************************************************************************************************************
* Function Name: IDT_WPC_Negotiation_Task
* Description  : This function is IDT_WPC_Negotiation_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_WPC_Negotiation_Task(void)
{
	if(IDT_WPC_State.Negotiation_Task_Flag==TRUE)
	{
		IDT_WPC_State.Negotiation_Task_Flag = FALSE;
		WPC_Function_Status.Temperature_Detect_Start_Flag = FALSE;
		if((CHARGE_LED!=LEVEL_LOW)||(ERROR_LED!=LEVEL_LOW))
		{
			CHARGE_STATE_LED_OFF;						// charger led off //
		}
		IDT_WPC_TASK = IDT_WPC_NEGOTIATION_TASK;
	}
	IDT_WPC_TASK = IDT_WPC_Negotiation_Process();
}
/***********************************************************************************************************************
* Function Name: IDT_WPC_Id_Confirmation_Process
* Description  : This function is IDT_WPC_Id_Confirmation_Process function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char IDT_WPC_Negotiation_Process(void)				// --- In Tx state 13(0x0D) --- //
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				if(P9261_RegMessage.Open_Fod_Alarm_Status==TRUE)		// Tx Open FOD alarm status 25(0x19) //	
				{
					P9261_RegMessage.Open_Fod_Alarm_Status = FALSE;
					IDT_WPC_State.Fod_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_FOD_ALARM_TASK;
				}
				else if(P9261_RegMessage.Power_Loss_Fod_Alarm_Status==TRUE)	// Tx Power loss FOD alarm status 24(0x18) //
				{
					P9261_RegMessage.Power_Loss_Fod_Alarm_Status = FALSE;
					IDT_WPC_State.Fod_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_FOD_ALARM_TASK;
				}
				else if(P9261_RegMessage.Other_End_Of_Power_Rx_Alarm_Status==TRUE) // Tx Status Other end-of-power rx alarm 09 //
				{
					P9261_RegMessage.Other_End_Of_Power_Rx_Alarm_Status = FALSE;
					Other_Alarm.Other_Eof_Pwr_Rx_Alarm_Status_Flag = TRUE; // 20191017 modify //
					IDT_WPC_State.Other_Eof_Pwr_Rx_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_OTHER_EOF_PWR_RX_ALARM_TASK;
				}
				else if(P9261_RegMessage.Rx_Jiggle_Alarm_Status==TRUE)	// Rx Jigle alarm status 27(0x1b) //
				{
					P9261_RegMessage.Rx_Jiggle_Alarm_Status = FALSE;
					Other_Alarm.Rx_Jiggle_Status_Flag = TRUE;	// 20191016 modify //
					IDT_WPC_State.Rx_Jiggle_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_JIGGLE_ALARM_TASK;
				}
				else if(P9261_RegMessage.Rx_Over_Current_Ept_Code_Status==TRUE)	// Rx over_current alarm status 07(0x07) //
				{
					P9261_RegMessage.Rx_Over_Current_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Over_Current_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_OVERCURRENT_TASK;
				}
				else if(P9261_RegMessage.Received_Power_Packet_Timeout_Status==TRUE)	// Rx receiver power packet timeout alarm status 12(0x0c) //
				{
					P9261_RegMessage.Received_Power_Packet_Timeout_Status = FALSE;
					Other_Alarm.Receiver_Timeout_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Received_Power_Packet_Timeout_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RECEIVE_POWER_PACKET_TIMEOUT_TASK;
				}
				else if(P9261_RegMessage.No_Response_Rx_Ept_Code_Status==TRUE)	// Rx No_Response_Rx_Ept_Code_Status status 14(0x04) //
				{
					P9261_RegMessage.No_Response_Rx_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Fail_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_FAIL_EPT_CODE_TASK;
				}
				else if(P9261_RegMessage.Internal_Fault_Rx_Ept_Code_Status==TRUE)	// Rx Internal_Fault_Rx_Ept_Code_Status 5(0x05) //
				{
					P9261_RegMessage.Internal_Fault_Rx_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Fail_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_FAIL_EPT_CODE_TASK;
				}
				/*else if(P9261_RegMessage.Wireless_Charger_Off_Status==TRUE)	// Rx Wireless_Charger_Off_Status 21(0x15) //
				{
					P9261_RegMessage.Wireless_Charger_Off_Status = FALSE;
					Other_Alarm.Wireless_Charge_Off_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Wireless_Charger_Off_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_WIRELESS_CHARGER_OFF_TASK;
				}*/
				else if(P9261_RegMessage.Tx_Under_Voltage_Limit_Status==TRUE)	// Tx Status Tx_Under_Voltage_Limit 0x14 //
				{
					P9261_RegMessage.Tx_Under_Voltage_Limit_Status = FALSE;
					IDT_WPC_State.Tx_Under_Voltage_Limit_Task_Flag = TRUE;	// reset p9261 //
					IDT_WPC_NEXT_TASK = IDT_WPC_TX_UNDER_VOLTAGE_LIMIT_TASK;
				}
				else if(P9261_RegMessage.Idle_Status==TRUE)				// Tx State 01 //
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
				else if(P9261_RegMessage.Calibration_Status==TRUE)	   // Tx State 0x10 //
				{
					P9261_RegMessage.Calibration_Status = FALSE;
					IDT_WPC_State.Calibration_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_CALIBRATION_TASK;
				}
				else if(P9261_RegMessage.Power_Transfer_Status==TRUE)	// Tx State 09 //
				{
					P9261_RegMessage.Power_Transfer_Status = FALSE;
					if(P9261_CombinedMessage.In_Charging==TRUE)
					{
						IDT_WPC_State.Power_Transfer_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WCP_POWER_TRANSFER_TASK;
					}
					else
					{
						IDT_WPC_NEXT_TASK = IDT_WPC_NEGOTIATION_TASK;
					}
				}
				else
				{
					IDT_WPC_NEXT_TASK = IDT_WPC_NEGOTIATION_TASK;
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
			IDT_WPC_State.Power_Sw_Off_Task_Flag = TRUE;
			IDT_WPC_NEXT_TASK = IDT_WPC_POWER_SW_OFF_TASK;
		}
	}
	else
	{
		Phone_Placement_Detect();	// phone forget detect //
		WPC_Function_Status.ACC_Off_Dly3_5T_Flag = TRUE;
		IDT_WPC_State.Cellphone_Left_Alert_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_CELLPHONE_LEFT_ALERT_TASK;
	}
	return IDT_WPC_NEXT_TASK;
}
