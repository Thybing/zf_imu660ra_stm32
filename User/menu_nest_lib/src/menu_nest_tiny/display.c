#include "menu_nest_tiny/display.h"
#include "stdio.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"

#define TO_STR_IMPL(str) #str
#define TO_STR(str) TO_STR_IMPL(str)

// 请在这里完成你的显示刷新函数(如果需要的话)
#if DISPLAY_NEED_UPDATE
void display_update(){
    ssd1306_UpdateScreen();
}
#endif

void display_line_text(uint16_t y, const char * str){
    char buff[LINE_MAX_CHAR_NUM + 1];
    snprintf(buff,LINE_MAX_CHAR_NUM + 1,"%-" TO_STR(LINE_MAX_CHAR_NUM) "s",str);

    // 在这里填入你的显示函数，在y行显示buff
    ssd1306_SetCursor(0,y);
    ssd1306_WriteString(buff, Font_7x10,White);
}

void display_clear(){
    // 在这里填入你的清屏函数
    ssd1306_Fill(Black);
}

void display_draw_waterline(uint16_t y, uint8_t color){
    if(color) {
        // 以前景色绘制一条水平线，y为行数，长度为DISPLAY_WIDTH
        ssd1306_Line(0, y, DISPLAY_WIDTH - 1, y, White);
    }else{
        // 以背景色绘制一条水平线，y为行数，长度为DISPLAY_WIDTH
        ssd1306_Line(0, y, DISPLAY_WIDTH - 1, y, Black);
    }
}

void display_draw_verticalline(uint16_t x, uint8_t color){
    if(color) {
        // 以前景色绘制一条垂直线，x为列数，长度为DISPLAY_HEIGHT
        ssd1306_Line(x, 0, x, DISPLAY_HEIGHT - 1, White);
    }else{
        // 以背景色绘制一条垂直线，x为列数，长度为DISPLAY_HEIGHT
        ssd1306_Line(x, 0, x, DISPLAY_HEIGHT - 1, Black);
    }
}