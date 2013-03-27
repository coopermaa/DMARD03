/*
 * DMT_G_Sensor.pde: 讀取利順 DMARD03 加速度計
 */

#include <DMARD03.h>                // 引用 DMARD03 函式庫

                                    // 建立一個 DMARD03 的 gSensor 物件
DMARD03 gSensor(A4, A5);            // dataPin = 2, clockPin = 3

void setup()
{  
  Serial.begin(9600);                // 開啟 Serial port, 通訊速率為 9600 bps
}

void loop()
{
  DMARD03_results results;          // 解碼結果將放在 DMARD03_results 結構的 result 變數裏
  
  gSensor.readXYZ(results);         // 讀取三軸加速度讀值(Accerleration) 
  
  // 把 X, Y,Z  三軸加速度讀值(Accerleration) 列印到 Serial Port
  Serial.print("X = "); Serial.print(results.X); Serial.print(", ");
  Serial.print("Y = "); Serial.print(results.Y); Serial.print(", ");
  Serial.print("Z = "); Serial.println(results.Z);  

  delay(200);  
}
