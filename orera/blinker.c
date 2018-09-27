//
// ファイル名 : blinker.c
//
//	概要       : まいまい（明滅）式ライントレース サンプルプログラム
//

/* ITRON(TOPPERS/JSP) API */
#include "kernel.h"
#include "kernel_id.h"

/* Mindstorms NXT API */
#include "ecrobot_interface.h"

/* 倒立振子制御パラメータ */
#include "balancer.h"


/* 下記のポート定義はセンサーとモーターの配線に合わせて変更する必要があります */
#define MOTOR_RIGHT		NXT_PORT_B
#define MOTOR_LEFT		NXT_PORT_C
#define TOUCH_SENS		NXT_PORT_S2
#define LIGHT_SENS		NXT_PORT_S3
#define GYRO_SENS		NXT_PORT_S4


/* 下記のパラメータはセンサ個体/環境に合わせてチューニングする必要があります */
#define GYRO_OFFSET  604 /* ジャイロセンサオフセット値(角速度0[deg/sec]時)	*/
#define THRESHOLD    .7F /* ラインエッジ閾値								*/


static float brightness;	/* コース明度			*/
static U16 lightDowned;		/* 消灯時の光センサー値	*/
static U16 lightUpped;		/* 点灯時の光センサー値	*/


//*****************************************************************************
// FUNCTION		: jsp_systick_low_priority
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: 1msec periodical ISR
//*****************************************************************************
void jsp_systick_low_priority(void)
{
	if (get_OS_flag()) /* check whether JSP already started or not */
	{
		isig_tim();          /* cyclic task dispatcher */
		check_NXT_buttons(); /* this must be called here */
	}
}



//*****************************************************************************
// FUNCTION		: ecrobot_device_initialize
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: ECRobot device init hook function
//*****************************************************************************
void ecrobot_device_initialize(void)
{
	// Bluetooth初期化
	ecrobot_init_bt_slave("1234");	/* パスキー "1234" */

	// モータ停止 （ブレーキモード）
	nxt_motor_set_speed(MOTOR_LEFT,  0, 1);	/* 左モータ */
	nxt_motor_set_speed(MOTOR_RIGHT, 0, 1);	/* 右モータ */
}



//*****************************************************************************
// FUNCTION		: ecrobot_device_terminate
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: ECRobot device term hook function
//*****************************************************************************
void ecrobot_device_terminate(void)
{
	// Bluetooth切断
	ecrobot_term_bt_connection();

	// モータ停止 （ブレーキモード）
	nxt_motor_set_speed(MOTOR_LEFT,  0, 1);	/* 左モータ */
	nxt_motor_set_speed(MOTOR_RIGHT, 0, 1);	/* 右モータ */

	// ブレーキを効かせるための待ち時間
	systick_wait_ms(500);
}



//=============================================================================
// タスク名	: TSK_STA
// 概要		: スタータータスク
//
void tsk_starter(VP_INT exinf)
{
	balance_init();							/* 倒立振子制御初期化			*/
	nxt_motor_set_count(MOTOR_LEFT, 0);		/* 左モータエンコーダリセット	*/
	nxt_motor_set_count(MOTOR_RIGHT, 0);	/* 右モータエンコーダリセット	*/

	/* タッチセンサ押下待機 */
	while (ecrobot_get_touch_sensor(TOUCH_SENS) == 0) {
		;	/* DO NOTHING */
	}

	sta_cyc(CYC);	/* 周期起動ハンドラー起動	*/

	ext_tsk();		/* 自タスクの終了			*/
}



//=============================================================================
// ハンドラ名	: CYC
// 概要		: 周期起動ハンドラ
//

void cyc(VP_INT exinf)
{
	U32 systime;

	systime = systick_get_ms();	// システム時刻取得

	//
	// 4ミリ秒周期処理
	//
	switch (systime % 4) {
		case 0 :
			;					// DO NOTHING
			break;
		case 1 :
			iact_tsk(TSK_BLN);	// 倒立制御TASK起動
			iact_tsk(TSK_LOG);	// ロガーTASK起動
			break;
		case 2 :
		case 3 :
		default:
			;					// DO NOTHING
	}

	//
	// 40ミリ秒周期処理
	//
	switch (systime % 40) {
		case 0 :
			iact_tsk(TSK_LON);	// LED点灯TASK起動
			iact_tsk(TSK_LMN);	// 明度計TASK起動
			break;
		case 20 :
			iact_tsk(TSK_LOF);	// LED消灯TASK起動
			iact_tsk(TSK_LMN);	// 明度計TASK起動
			break;
		default :
			;					// DO NOTHING
	}
}



//=============================================================================
// タスク名	: TSK_BLN
// 概要		: 倒立（バランサー）タスク
//
void tsk_balancer(VP_INT exinf)
{
	signed char forward;		/* 前後進命令: -100(後進)～100(前進)		*/
	signed char turn;			/* 旋回命令: -100(左旋回)～100(右旋回)	*/
	signed char pwm_L, pwm_R;	/* 左右モータPWM出力						*/

	forward = 0;	/* 前進命令(停止) */

	if (brightness < THRESHOLD) {
		turn = 50;  /* 右折 */
	} else {
		turn = -50; /* 左折 */
	}

	/* 倒立振子制御 */
	balance_control(
		(float)forward,                              /* 前後進命令			*/
		(float)turn,                                 /* 旋回命令				*/
		(float)ecrobot_get_gyro_sensor(GYRO_SENS),   /* ジャイロセンサ値			*/
		(float)GYRO_OFFSET,                          /* ジャイロセンサオフセット値	*/
		(float)nxt_motor_get_count(MOTOR_LEFT),      /* 左モータ回転角度[deg]	*/
		(float)nxt_motor_get_count(MOTOR_RIGHT),     /* 右モータ回転角度[deg]	*/
		(float)ecrobot_get_battery_voltage(),        /* バッテリ電圧[mV]		*/
		&pwm_L,  /* 左モータPWM出力値 */
		&pwm_R); /* 右モータPWM出力値 */

	nxt_motor_set_speed(MOTOR_LEFT,  pwm_L, 1); /* 左モータPWM出力セット */
	nxt_motor_set_speed(MOTOR_RIGHT, pwm_R, 1); /* 右モータPWM出力セット */

	/* 自タスクの終了 */
	ext_tsk();
}



//=============================================================================
// タスク名	: TSK_LON
// 概要		: LED点灯タスク
//
void tsk_lightOn(VP_INT exinf)
{
	/*赤色LED消灯時の受光量計測 */
	lightDowned = ecrobot_get_light_sensor(LIGHT_SENS);
	
	/* 赤色LEDを点灯する */
	ecrobot_set_light_sensor_active(LIGHT_SENS);
	
	/* 自タスクの終了 */
	ext_tsk();
}



//=============================================================================
// タスク名	: TSK_LOF
// 概要		: LED消灯タスク
//
void tsk_lightOff(VP_INT exinf)
{
	/* 赤色LED点灯時の受光量計測 */
	lightUpped = ecrobot_get_light_sensor(LIGHT_SENS);
	
	/* 赤色LEDを消灯する */
	ecrobot_set_light_sensor_inactive(LIGHT_SENS);
	
	/* 自タスクの終了 */
	ext_tsk();
}



//=============================================================================
// タスク名	: TSK_LMN
// 概要		: 明度計（ルミノメーター）タスク
//
void tsk_luminometer(VP_INT exinf)
{
	U16 lightDiff;				/* 点灯時と消灯時の変化量	*/
	float k;					/* 光センサー非線形補正値	*/
	
	/* 光センサーの変化量を計算 */
	if (lightDowned - lightUpped > 0) {
		lightDiff = lightDowned - lightUpped;
	} else {
		lightDiff = 0U;
	}
	
	/* 光センサー非線形補正係数を計算 （実験データより） */
	k = (1.0382E-3 * lightDowned - 6.3295E-1) * lightDowned + 1.1024E+2;
	
	/* コース明度を計算 */
	brightness = (float) lightDiff / k;
	
	/* 自タスクの終了 */
	ext_tsk();
}



//=============================================================================
// タスク名	: TSK_LOG
// 概要		: ロガータスク
//
void tsk_logger(VP_INT exinf)
{
	S8 dat1;
	S8 dat2;
	
	dat1 = (S8)(brightness * 100);
	dat2 = 0;
	
	/* Bluetooth data logger */
	ecrobot_bt_data_logger(dat1, dat2);

	/* 自タスクの終了 */
	ext_tsk();
}
