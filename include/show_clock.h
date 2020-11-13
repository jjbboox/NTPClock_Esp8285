#ifndef _SHOW_CLOCK_H_
#define _SHOW_CLOCK_H_
#include <TM1650.h>

typedef enum {
    Nomal,              // 通常
    FadeInOut,          // 淡入淡出
    SegClearDraw,       // 段清除
} ShowMotion;

class ShowClock {
    public:
        ShowClock(TM1650& _tm1650, ShowMotion _sm=Nomal):tm1650(_tm1650), showMotion(_sm){
            oldStr = String("    ");
        };
        void setMotion(ShowMotion sm) {showMotion = sm;};
        ShowMotion getMotion() {return showMotion;};
        void Motion_Nomal(String newTimeStr, uint16_t wait_ms=0);
        void Motion_SetClearDraw(String newTimeStr, uint16_t ms=10, uint16_t wait_ms=50);
        void Motion_FadeInOut(String newTimeStr, uint16_t steps=10, uint16_t ms=5, uint16_t wait_ms=50);
        
    private:
        TM1650 &tm1650;
        String oldStr;
        ShowMotion showMotion;
};

#endif // _SHOW_CLOCK_H_
