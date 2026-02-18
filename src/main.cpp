//	ジョイスティック実験
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/02	初版

/*	ピンアサイン
			ESP32-C3
SPI-SS		GPIO_NUM_10
SPI-MOSI	GPIO_NUM_7
SPI-MISO	GPIO_NUM_2	グラフィックLCDでは未使用
SPI-SCK		GPIO_NUM_6
LCD-DC		GPIO_NUM_4
LCD-RESET	GPIO_NUM_NC
{SW-A		GPIO_NUM_8}	参考
{SW-B		GPIO_NUM_9}	参考

			ESP32-C3
			[Antenna side]
			3V3		IO-0	stick-dir1
			EN		IO-1	stick-dir2
LCD-DC		IO-4	IO-2*	SPI-MISO
			IO-5	IO-3	stick-pushSW
SPI-SCK		IO-6	IO-19	USB_D+
SPI-MOSI	IO-7	IO-18	USB_D-
SW-A		IO-8*	TXD
SW-B		IO-9*	RXD
			GND		IO-10*	SPI-SS
			[USB-conn side]
*/
/*	ジョイスティックの配線
3.3V - [1.5kR] - [axis-A//B] - [1.5kR] - GND
・ジョイスティックのA軸B軸のVRは10kR(RKJXV1224005/ALPS ALPINE)。並列で配線して5kR。
・ESP32-C3のADCは、基準電圧3.3Vのとき変換値上限が2.8V。
	ADCの値はジョイスティックの傾きに比例し、下限方向は0Vまで変化する。
	上限方向は傾きの途中から（2.8Vを超えるところから）値が変化しなくなる。
・ジョイスティック可動範囲全体を0.0～2.8Vに収めるため、前後に抵抗を接続する。
	1.5kRの場合、ADCの値（電圧）は、上限2681mV, 中間1650mV, 下限619mV。
	ジョイスティックの可動域全体に対して傾きに比例した値が取得できる。
*/

#include <Arduino.h>
#include "LcdILI9225.hpp"
#include "HwSwitch.hpp"

//オブジェクト
LcdILI9225	lcd;
HwSwitch	stickSw;

//色
static	Color	foreColor(0xFF, 0xFF, 0xFF);
static	Color	bgColor(0x00, 0x00, 0x00);

//定数
static	constexpr	gpio_num_t	StickAxisA = GPIO_NUM_0;
static	constexpr	gpio_num_t	StickAxisB = GPIO_NUM_1;
static	constexpr	gpio_num_t	StickSw = GPIO_NUM_3;

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	pinMode(StickAxisA, INPUT);
	pinMode(StickAxisB, INPUT);
	stickSw.Initialize(StickSw);

	lcd.Initialize(GPIO_NUM_10, GPIO_NUM_4, 1, 1, 1);
	lcd.RotateFlip(ERotFlip::Rot270);
	lcd.SetTextScale(2, 4);
	lcd.SetTextColor(foreColor);
	lcd.SetTextBgColor(bgColor);
	lcd.ClearScreen(bgColor);
}

void	loop(void)
{
	int16_t x = lcd.XFromCol(0), row = 0, y = lcd.YFromRow(row);
	lcd.DrawString(x, y, "Joystick test");

	x = lcd.XFromCol(0); row++; y = lcd.YFromRow(row);
	uint32_t axisA = analogReadMilliVolts(StickAxisA);
	x = lcd.DrawUInt(x, y, "axisA=", axisA, "mV  ");

	x = lcd.XFromCol(0); row++; y = lcd.YFromRow(row);
	uint32_t axisB = analogReadMilliVolts(StickAxisB);
	x = lcd.DrawUInt(x, y, "axisB=", axisB, "mV  ");

	x = lcd.XFromCol(0); row++; y = lcd.YFromRow(row);
	stickSw.State();
	bool isOn = stickSw.IsOn();
	x = lcd.DrawString(x, y, "pushSw=");	lcd.DrawString(x, y, isOn ? "ON " : "OFF");

	delay(100);
}
