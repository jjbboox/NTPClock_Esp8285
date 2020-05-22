#ifndef _ERR_SIGNAL_H_
#define _ERR_SIGNAL_H_
#include <Arduino.h>

#define SIGNAL_SHORT_WAIT   500
#define SIGNAL_LONG_WAIT    1000

typedef enum {
    ERR_SIGNAL_ERROR,
    ERR_SIGNAL_WARNING,
    ERR_SIGNAL_INFO
} SignalType;

typedef struct {
    SignalType type;
    uint32_t signal_code;
    String msg;
} SignalDefine;

class SystemSignal {
    public:
        void begin(uint8_t _pin, bool _on) {
            signal_pin = _pin;
            signal_on = _on;
            pinMode(signal_pin, OUTPUT);
            off();
        };
        void throw_signal(SignalDefine &signal, bool msgOnly=false){
            if(Serial) {
                Serial.print(signal_type_head(signal.type));
                Serial.println(signal.msg);
            }
            if(!msgOnly){
                Serial.println("throw_signal: msgOnly is false");
                do {
                    signal_head(signal.type);
                    code_signal(signal.signal_code);
                } while(signal.type == ERR_SIGNAL_ERROR);
            }
        };
        void off(uint16_t ms=0) {
            digitalWrite(signal_pin, !signal_on);
            if(ms) delay(ms);
        };
        void on(uint16_t ms=0) {
            digitalWrite(signal_pin, signal_on);
            if(ms) delay(ms);
        };
        void toggle(uint16_t ms=0) {
            digitalWrite(signal_pin, !digitalRead(signal_pin));
            if(ms) delay(ms);
        };
        void signal_flash(uint16_t on_ms, uint16_t off_ms=0, int times=1) {
            if(!on_ms) return;
            off_ms = off_ms?off_ms:on_ms;
            while(times--) {
                on(on_ms);
                off(off_ms);
            }
        };
    protected:
        uint8_t signal_pin;
        bool signal_on;
        
        static String signal_type_head(SignalType type) {
            if(type == ERR_SIGNAL_ERROR) 
                return "Error: ";
            else if(type == ERR_SIGNAL_WARNING)
                return "Warning: ";
            else if(type == ERR_SIGNAL_INFO)
                return "Info: ";
            else return "";
        };
        void signal_start() {
            signal_flash(SIGNAL_SHORT_WAIT, 0, 3);
            off();
        };
        void code_signal(uint32_t code) {
            Serial.println("code_signal start");
            Serial.println(code, BIN);
            
            bool start = false;
            for(int i = 0; i < 32; i++) {
                bool signal = code & 0x80000000;
                start = start || signal;
                if(start) {
                    if(signal) on(SIGNAL_SHORT_WAIT);
                    else off(SIGNAL_SHORT_WAIT);
                } 
                code <<= 1;
            }
        };
        void signal_head(SignalType type) {
            Serial.println("signal_head start");
            Serial.println(type);
            switch(type) {
                case    ERR_SIGNAL_WARNING:
                    signal_flash(SIGNAL_LONG_WAIT, 0, 2);
                    break;
                case    ERR_SIGNAL_ERROR:
                    signal_flash(SIGNAL_SHORT_WAIT, 0, 3);
                    break;
                case    ERR_SIGNAL_INFO:
                    signal_flash(SIGNAL_LONG_WAIT, 0, 1);
                default:
                    break;
            }
            delay(SIGNAL_LONG_WAIT*2);
        };
};

extern SystemSignal SysSignal;

#endif
