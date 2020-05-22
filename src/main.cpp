#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <Wire.h>
#include <TM1650.h>
#include "air_kiss_connect.h"

const char *ssid     = "XXXXXXXXX";
const char *password = "xxxxxxxxxxxxxx";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.11.20", 8 * 60 * 60);

// 驱动数码管的TM1650的I2C总线端口
#define TM1650_SDA  9
#define TM1650_SCL  10

// 数码管小数点和冒号的数组下标
// 使用4位共阴带冒号的时钟数码管
// 不同型号的数码管点位不同，控制方式也会有所不同
#define TM1650_DOT_POS        0
#define TM1650_SEMICOLON_POS  1

// 数码管对象实例
TM1650  NixieTube;

void I2C_init(uint8_t sda_pin, uint8_t scl_pin) {
  // TM1650 I2C总线设置为上拉
  pinMode(sda_pin, INPUT_PULLUP);
  pinMode(scl_pin, INPUT_PULLUP);
  
  // 初始化并启动I2C总线
  Wire.begin(sda_pin, scl_pin);
}

// 数码管显示剩余时间
void show_timer(NTPClient &ntp_time) {
  // 显示缓存
  char str[5];
  // 打印显示字符串
  sprintf(str, "%02d%02d", ntp_time.getHours(), ntp_time.getMinutes());
  // 根据秒数的奇偶来切换":"显示
  
  str[TM1650_SEMICOLON_POS] |= (ntp_time.getSeconds()%2)?0x80:0x00;
  // 显示
  NixieTube.displayString(str);
}

void setup(){
  Serial.begin(115200);

  // WiFi.begin(ssid, password);

  // while ( WiFi.status() != WL_CONNECTED ) {
  //   delay ( 500 );
  //   Serial.print ( "." );
  // }
  air_kiss_connect();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  // timeClient.setUpdateInterval(60 * 60 * 1000);
  // timeClient.setTimeOffset(8 * 60 * 60);

  // 初始化I2C总线
  I2C_init(TM1650_SDA, TM1650_SCL);
  
  // 初始化数码管
  NixieTube.init();
  // 设置数码管亮度
  NixieTube.setBrightness(3);

}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  show_timer(timeClient);

  delay(1000);
}
