#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <stdio.h>
#include "main.h"
#include "string.h"

/*extern uint16_t count;
extern uint16_t max_step;
extern uint8_t nagative_flag;
extern uint8_t buffer[10];*/

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim12;

uint16_t count = 0;
uint16_t max_step = 2500;
uint8_t nagative_flag = 0;
uint8_t buffer[10];

uint8_t DPad_Right;
uint8_t DPad_Down;
uint8_t DPad_Up;
uint8_t DPad_Left;
uint8_t DPad_UpRight;
uint8_t DPad_DownRight;
uint8_t DPad_UpLeft;
uint8_t DPad_DownLeft;

uint8_t Square_Button;
uint8_t Cross_Button;
uint8_t Circle_Button;
uint8_t Triangle_Button;
uint8_t L1_Button;
uint8_t R1_Button;
uint8_t L3_Button;
uint8_t R3_Button;

uint8_t Share_Button;
uint8_t Options_Button;
uint8_t PS_Button;
uint8_t TouchPad_Button;
uint8_t ControllerCharging;
uint8_t Headphones_atrached;
uint8_t Mic_attached;
uint8_t Nan;

uint8_t L2_Value;
uint8_t R2_Value;
int16_t LStick_X_Value;
int16_t LStick_Y_Value;
int16_t RStick_X_Value;
int16_t RStick_Y_Value;
uint8_t BatteryLevel;

void ControllerRead();
void right_1st_joint(uint16_t, uint16_t, uint16_t);
void right_2nd_joint(uint16_t, uint16_t, uint16_t);
void right_3rd_joint(uint16_t, uint16_t, uint16_t);
void left_1st_joint(uint16_t, uint16_t, uint16_t);
void left_2nd_joint(uint16_t, uint16_t, uint16_t);
void left_3rd_joint(uint16_t, uint16_t, uint16_t);

LOCAL void task_1(INT stacd, void *exinf);	// task execution function
LOCAL ID tskid_1;			// Task ID number
LOCAL T_CTSK ctsk_1 = {				// Task creation information
		.itskpri = 10, .stksz = 1024, .task = task_1, .tskatr = TA_HLNG
				| TA_RNG3, };

LOCAL void task_2(INT stacd, void *exinf);	// task execution function
LOCAL ID tskid_2;			// Task ID number
LOCAL T_CTSK ctsk_2 = {				// Task creation information
		.itskpri = 10, .stksz = 1024, .task = task_2, .tskatr = TA_HLNG
				| TA_RNG3, };

LOCAL void task_1(INT stacd, void *exinf) {

	while (1) {
		//tm_printf((UB*) "task 1\n");
		//tk_dly_tsk(500);

		if (HAL_UART_Receive(&huart4, buffer, 10, 30) == HAL_OK) {
			ControllerRead();
		}
		tk_dly_tsk(50);
	}
}

LOCAL void task_2(INT stacd, void *exinf) {

	uint16_t delayTime = 400;

	//500~2500 ⇔ 0~180
	left_1st_joint(1500, 1500, 1500);  //左根本
	right_1st_joint(1500, 1500, 1500);  //右根本
	left_2nd_joint(1500, 1500, 1500);  //左中
	right_2nd_joint(1500, 1500, 1500);  //右中
	left_3rd_joint(1000, 1000, 1000);  //左先
	right_3rd_joint(1000, 1000, 1000);  //右先
	tk_dly_tsk(1000);

	while (1) {
		//左脚上げ
		left_2nd_joint(
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value,
				1500,
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value);
		right_2nd_joint(1500,
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value,
				1500);
		tk_dly_tsk(delayTime);

		//左前出し、右後ろ下げ
		left_1st_joint(1500 + 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value,
				1500, 1500 + 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value);
		right_1st_joint(1500,
				1500 - 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value, 1500);
		tk_dly_tsk(delayTime);

		//左脚下げ
		left_2nd_joint(1500, 1500, 1500);
		right_2nd_joint(1500, 1500, 1500);
		tk_dly_tsk(delayTime);

		//右脚上げ
		left_2nd_joint(1500,
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value,
				1500);
		right_2nd_joint(
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value,
				1500,
				1500 + 4 * abs(LStick_Y_Value) + 2 * R2_Value + 2 * L2_Value);
		tk_dly_tsk(delayTime);

		//右前出し、左後ろ下げ
		left_1st_joint(1500,
				1500 + 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value, 1500);
		right_1st_joint(1500 - 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value,
				1500, 1500 - 4 * LStick_Y_Value + 2 * R2_Value - 2 * L2_Value);
		tk_dly_tsk(delayTime);

		//右脚下げ
		left_2nd_joint(1500, 1500, 1500);
		right_2nd_joint(1500, 1500, 1500);

		left_3rd_joint(1000, 1000, 1000);
		right_3rd_joint(1000, 1000, 1000);
		tk_dly_tsk(delayTime);
	}
}

/* usermain関数 */
EXPORT INT usermain(void) {
	tm_putstring((UB*) "Start User-main program.\n");

	/* Create & Start Tasks */
	tskid_1 = tk_cre_tsk(&ctsk_1);
	tk_sta_tsk(tskid_1, 0);

	tskid_2 = tk_cre_tsk(&ctsk_2);
	tk_sta_tsk(tskid_2, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}

void ControllerRead() {
	DPad_Right = buffer[0] & 1;
	DPad_Down = (buffer[0] >> 1) & 1;
	DPad_Up = (buffer[0] >> 2) & 1;
	DPad_Left = (buffer[0] >> 3) & 1;
	DPad_UpRight = (buffer[0] >> 4) & 1;
	DPad_DownRight = (buffer[0] >> 5) & 1;
	DPad_UpLeft = (buffer[0] >> 6) & 1;
	DPad_DownLeft = (buffer[0] >> 7) & 1;
	uint8_t d_pad[50];
	sprintf(d_pad, "p-pad: %d, %d, %d, %d, %d, %d, %d, %d\r\n", DPad_Right,
			DPad_Down, DPad_Up, DPad_Left, DPad_UpRight, DPad_DownRight,
			DPad_UpLeft, DPad_DownLeft);
	HAL_UART_Transmit(&huart3, d_pad, strlen(d_pad), 0xFFFF);

	Square_Button = buffer[1] & 1;
	Cross_Button = (buffer[1] >> 1) & 1;
	Circle_Button = (buffer[1] >> 2) & 1;
	Triangle_Button = (buffer[1] >> 3) & 1;
	L1_Button = (buffer[1] >> 4) & 1;
	R1_Button = (buffer[1] >> 5) & 1;
	L3_Button = (buffer[1] >> 6) & 1;
	R3_Button = (buffer[1] >> 7) & 1;
	uint8_t button[50];
	sprintf(button, "button: %d, %d, %d, %d, %d, %d, %d, %d\r\n", Square_Button,
			Cross_Button, Circle_Button, Triangle_Button, L1_Button, R1_Button,
			L3_Button, R3_Button);
	HAL_UART_Transmit(&huart3, button, strlen(button), 0xFFFF);

	Share_Button = buffer[2] & 1;
	Options_Button = (buffer[2] >> 1) & 1;
	PS_Button = (buffer[2] >> 2) & 1;
	TouchPad_Button = (buffer[2] >> 3) & 1;
	ControllerCharging = (buffer[2] >> 4) & 1;
	Headphones_atrached = (buffer[2] >> 5) & 1;
	Mic_attached = (buffer[2] >> 6) & 1;
	uint8_t option[50];
	sprintf(option, "option: %d, %d, %d, %d, %d, %d, %d\r\n", Share_Button,
			Options_Button, PS_Button, TouchPad_Button, ControllerCharging,
			Headphones_atrached, Mic_attached);
	HAL_UART_Transmit(&huart3, option, strlen(option), 0xFFFF);

	L2_Value = buffer[3];
	uint8_t L2[10];
	sprintf(L2, "L2: %d\r\n", L2_Value, strlen(L2), 0xFFFF);
	HAL_UART_Transmit(&huart3, L2, strlen(L2), 0xFFFF);

	R2_Value = buffer[4];
	uint8_t R2[10];
	sprintf(R2, "R2: %d\r\n", R2_Value, strlen(R2), 0xFFFF);
	HAL_UART_Transmit(&huart3, R2, strlen(R2), 0xFFFF);

	LStick_X_Value = buffer[5] - 128;
	uint8_t L_X[20];
	sprintf(L_X, "LStick_X: %d\r\n", LStick_X_Value, strlen(L_X), 0xFFFF);
	HAL_UART_Transmit(&huart3, L_X, strlen(L_X), 0xFFFF);

	LStick_Y_Value = buffer[6] - 128;
	uint8_t L_Y[20];
	sprintf(L_Y, "LStick_Y: %d\r\n", LStick_Y_Value, strlen(L_Y), 0xFFFF);
	HAL_UART_Transmit(&huart3, L_Y, strlen(L_Y), 0xFFFF);

	RStick_X_Value = buffer[7] - 128;
	uint8_t R_X[20];
	sprintf(R_X, "RStick_X: %d\r\n", RStick_X_Value, strlen(R_X), 0xFFFF);
	HAL_UART_Transmit(&huart3, R_X, strlen(R_X), 0xFFFF);

	RStick_Y_Value = buffer[8] - 128;
	uint8_t R_Y[20];
	sprintf(R_Y, "RStick_Y: %d\r\n", RStick_Y_Value, strlen(R_Y), 0xFFFF);
	HAL_UART_Transmit(&huart3, R_Y, strlen(R_Y), 0xFFFF);

	BatteryLevel = buffer[9];
	uint8_t Batt[20];
	sprintf(Batt, "BatteryLevel: %d\r\n", BatteryLevel, strlen(Batt), 0xFFFF);
	HAL_UART_Transmit(&huart3, Batt, strlen(Batt), 0xFFFF);

	HAL_UART_Transmit(&huart3, "\r\n", 4, 10);
}

void right_1st_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, front);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, middle);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, rear);
}

void right_2nd_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, front);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, middle);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, rear);
}

void right_3rd_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, front);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, middle);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, rear);
}

void left_1st_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, front);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, middle);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, rear);
}

void left_2nd_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, front);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, middle);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, rear);
}

void left_3rd_joint(uint16_t front, uint16_t middle, uint16_t rear) {
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, front);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, middle);
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, rear);
}
