# Joystick_ESP32C3_ADC
Joystick読み取り ESP32-C3のADC利用

## 概要
ESP32-C3のADCを利用してジョイスティックの傾き（可変抵抗の値）を読み取るプログラムです。

## 特徴
ジョイスティックの傾きに比例した対称性のある変換値を取得することができます。

## 説明
- ESP32-C3のADCは電圧3.3V, 減衰比11dB（デフォルト値:ADC_ATTEN_DB_11）に設定した場合、変換値の下限は0V程度ですが、上限は2.5V程度に制限されます。
- そうするとジョイスティックの中間点では3.3/2=1.65V、左傾倒限界で0V、右傾倒限界で（3.3Vではなく）2.5V、のようになります。
- 解決策として抵抗分圧を利用しました。
```
3.3V - [1.5kR] - Joystick(5kR) - GND
```
このように配線し、analogReadMilliVolts()で電圧を読み取ると、上限2538mV - 中間1269mV - 下限0mV となります。
求めたい値は電圧そのものではなく傾きに比例した変換値なので、この方法で問題ありません。

## 動作確認環境
- PlatformIO + ArduinoFramework
- ESP32-C3
- アルプスアルパイン製 RKJXV1224005（相当品）
- SPI-LCD (ILI9225)

## サンプル画像
![サンプル](https://github.com/jsdiy/Joystick_ESP32C3_ADC/blob/main/sample/IMG_20260218_120035.jpg)
![サンプル](https://github.com/jsdiy/Joystick_ESP32C3_ADC/blob/main/sample/IMG_20260218_120042.jpg)
