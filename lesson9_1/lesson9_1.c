#include <reg52.h>

void TurnMotor(unsigned long angle);

void main()
{
    TurnMotor(360*25); //360¶È*25£¬¼´25È¦
    while (1);
}
/* Èí¼þÑÓÊ±º¯Êý£¬ÑÓÊ±Ô¼2ms */
void delay()
{
    unsigned int i = 200;
	
    while (i--);
}
/* ²½½øµç»ú×ª¶¯º¯Êý£¬angle-Ðè×ª¹ýµÄ½Ç¶È */
void TurnMotor(unsigned long angle)
{
    unsigned char tmp;  //ÁÙÊ±±äÁ¿
    unsigned char index = 0;  //½ÚÅÄÊä³öË÷Òý
    unsigned long beats = 0;  //ËùÐè½ÚÅÄ×ÜÊý
    unsigned char code BeatCode[8] = {  //²½½øµç»ú½ÚÅÄ¶ÔÓ¦µÄIO¿ØÖÆ´úÂë
        0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6
    };
    
    beats = (angle*4096) / 360; //¼ÆËãÐèÒªµÄ½ÚÅÄ×ÜÊý£¬4096ÅÄ¶ÔÓ¦Ò»È¦
    while (beats--)  //ÅÐ¶Ïbeats²»Îª0Ê±Ö´ÐÐÑ­»·£¬È»ºó×Ô¼õ1
    {
        tmp = P1;                    //ÓÃtmp°ÑP1¿Úµ±Ç°ÖµÔÝ´æ
        tmp = tmp & 0xF0;            //ÓÃ&²Ù×÷ÇåÁãµÍ4Î»
        tmp = tmp | BeatCode[index]; //ÓÃ|²Ù×÷°Ñ½ÚÅÄ´úÂëÐ´µ½µÍ4Î»
        P1  = tmp;                   //°ÑµÍ4Î»µÄ½ÚÅÄ´úÂëºÍ¸ß4Î»µÄÔ­ÖµËÍ»ØP1
        index++;                     //½ÚÅÄÊä³öË÷ÒýµÝÔö
        index = index & 0x07;        //ÓÃ&²Ù×÷ÊµÏÖµ½8¹éÁã
        delay();                     //ÑÓÊ±2ms£¬¼´2msÖ´ÐÐÒ»ÅÄ
    }
    P1 = P1 | 0x0F;  //¹Ø±Õµç»úËùÓÐµÄÏà
}

//x*12/11059200 = 0.002 x = 1843  T0 = y; y+x = 65536 y = 63693; y = b8c4