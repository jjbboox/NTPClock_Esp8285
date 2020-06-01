#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#ifdef _USER_SSID_
const String ssid = "my_ssid_xxxx";
const String pwd = "my_wifi_password";
#endif

air_kiss_connect(void (*tick_fun)(String))
{
    int cnt = 0;

    WiFi.mode(WIFI_STA);

#ifdef _USER_SSID_
    WiFi.begin(ssid, pwd);
#else
    WiFi.begin();
#endif
    tick_fun("START");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        tick_fun(".");
        if (cnt++ >= 30)
        {
            tick_fun("SMART");
            WiFi.beginSmartConfig();
            while (true)
            {
                delay(1000);
                if (WiFi.smartConfigDone())
                {
                    tick_fun("Success");
                    // Serial.println("SmartConfig Success");
                    while (WiFi.status() != WL_CONNECTED);
                    break;
                }
            }
        }
    }
    return true;
}
