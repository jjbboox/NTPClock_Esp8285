#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#ifdef _USER_SSID_
const String ssid = "xxxxxx";
const String pwd = "xxxxxxxx";
#endif

void air_kiss_connect()
{
    int cnt = 0;

    WiFi.mode(WIFI_STA);

#ifdef _USER_SSID_
    WiFi.begin(ssid, pwd);
#else
    WiFi.begin();
#endif

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        if (cnt++ >= 30)
        {
            
            WiFi.beginSmartConfig();
            while (true)
            {
                if (WiFi.smartConfigDone())
                {
                    Serial.println("SmartConfig Success");
                    break;
                }
            }
        }
    }
}
