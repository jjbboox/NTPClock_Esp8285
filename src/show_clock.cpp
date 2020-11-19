#include <show_clock.h>

bool checkTimeStr(String timeStr) {
    if(timeStr.length() != 4) return false;
    return true;
}

// 普通显示，清除->等待->显示
void ShowClock::Motion_Nomal(String newTimeStr, uint16_t wait_ms) {
    if(!checkTimeStr(newTimeStr)) return;
    char tmpStrNew[5];
    char tmpStrOld[5];
    strcpy(tmpStrNew, newTimeStr.c_str());

    if(wait_ms > 0) {
        strcpy(tmpStrOld, oldStr.c_str());
        for(int i = 0; i < 4; i++) {
            if(tmpStrOld[i] & 0x7f != tmpStrNew[i] & 0x7f) tmpStrOld[i] = ' ' | tmpStrNew[i] & 0x80;
        }
        tm1650.displayString(tmpStrOld);
        delay(wait_ms);
    }
    tm1650.displayString(tmpStrNew);

    oldStr = newTimeStr;
}

byte getCharImg(char c) {
    byte tmpImg = c;
    byte a = c & ~TM1650_DOT;
#ifndef TM1650_USE_PROGMEM
	tmpImg = TM1650_CDigits[a];
#else
	tmpImg = pgm_read_byte_near(TM1650_CDigits + a);
#endif
    // tmpImg |= c & TM1650_DOT;
    return tmpImg; 
}

// 分段清除，分段显示
void ShowClock::Motion_SetClearDraw(String newTimeStr, uint16_t ms, uint16_t wait_ms) {
    if(!checkTimeStr(newTimeStr)) return;
    if(newTimeStr.equals(oldStr)) return;
    char newStrMap[4];
    char oldStrMap[4];
    for(int i = 0; i < 4; i++) {
        newStrMap[i] = getCharImg(newTimeStr[i]) | (newTimeStr[i] & TM1650_DOT);
        oldStrMap[i] = getCharImg(oldStr[i]) | (newTimeStr[i] & TM1650_DOT);
    }
    byte mask = 0xff;
    for(int i = 0; i < 7; i++) {
        mask <<= 1;
        for(int j = 0; j < 4; j++) {
            if(newTimeStr[j] != oldStr[j]) {
                tm1650.setPosition(j, oldStrMap[j] & mask);
            }
        }
        delay(ms);
    }
    delay(wait_ms);
    mask = 0xff;
    for(int i = 0; i < 7; i++) {
        mask <<= 1;
        for(int j = 0; j < 4; j++) {
            if(newTimeStr[j] != oldStr[j]) {
                tm1650.setPosition(j, newStrMap[j] & (~mask | 0x80));
            }
        }
        delay(ms);
    }
    oldStr = newTimeStr;
}

// 淡入淡出
void ShowClock::Motion_FadeInOut(String newTimeStr, uint16_t steps, uint16_t ms, uint16_t wait_ms){
    if(!checkTimeStr(newTimeStr)) return;
    if(newTimeStr.equals(oldStr)) return;
    char tmpStrNew[5];
    char tmpStrOld[5];
    char tmpStrSpace[5];
    strcpy(tmpStrNew, newTimeStr.c_str());
    strcpy(tmpStrOld, oldStr.c_str());
    for(int i = 0; i < 4; i++) {
        tmpStrOld[i] &= ~TM1650_DOT;
        tmpStrSpace[i] = tmpStrOld[i];
        if(tmpStrSpace[i] & ~TM1650_DOT != tmpStrNew[i] & ~TM1650_DOT) tmpStrSpace[i] = ' ';
        tmpStrSpace[i] |= (tmpStrNew[i] & TM1650_DOT);
        tmpStrOld[i] |= (tmpStrNew[i] & TM1650_DOT);
    }
    for(int i = 1; i <= steps; i++) {
        tm1650.displayString(tmpStrSpace);
        delay(i * ms);
        if(i < steps) {
            tm1650.displayString(tmpStrOld);
            delay((steps - i) * ms);
        }
    }
    delay(wait_ms);
    for(int i = 1; i <= steps; i++) {
        tm1650.displayString(tmpStrNew);
        delay(i * ms);
        if(i < steps) {
            tm1650.displayString(tmpStrSpace);
            delay((steps - i) * ms);
        }
    }
    oldStr = newTimeStr;
}

void ShowClock::Motion_SegClear(String newTimeStr, uint16_t ms, uint16_t wait_ms) {
    if(!checkTimeStr(newTimeStr)) return;
    if(newTimeStr.equals(oldStr)) return;

    // char tmpStrSpace[5];
    // strcpy(tmpStrSpace, oldStr.c_str());
    for(int i = 0; i < 4; i++) {
        char c_s = oldStr[i] & ~TM1650_DOT;
        char n_s = newTimeStr[i] & ~TM1650_DOT;

        if(c_s != n_s) {
            char c_o = oldStr[i] & ~TM1650_DOT;
            char img = getCharImg(c_o);
            img |= c_o & TM1650_DOT;
            for(byte mask = 1; mask != 0x80; mask <<= 1) {
                if(c_o & mask) {
                    img &= ~mask;
                    tm1650.setPosition(i, img);
                    delay(ms);
                }
            }
        }
    }
    delay(wait_ms);
    for(int i = 3; i <= 0; i--) {
        char c_s = oldStr[i] & ~TM1650_DOT;
        char n_s = newTimeStr[i] & ~TM1650_DOT;

        if(c_s != n_s) {
            char c_o = newTimeStr[i] & ~TM1650_DOT;
            char img = getCharImg(c_o);
            img |= newTimeStr[i] & TM1650_DOT;
            byte mask = 0x80;
            for(int j = 0; j < 7; j++) {
                mask = mask >> 1 | 0x80;
                if(c_o & mask) {
                    img &= mask;
                    tm1650.setPosition(i, img);
                    delay(ms);
                }
            }
        }
    }
}