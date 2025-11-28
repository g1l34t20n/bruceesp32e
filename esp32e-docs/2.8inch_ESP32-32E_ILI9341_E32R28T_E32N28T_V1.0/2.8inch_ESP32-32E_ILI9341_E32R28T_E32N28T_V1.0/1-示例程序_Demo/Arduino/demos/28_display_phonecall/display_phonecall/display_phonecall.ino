//This program is a demo of display phonecall UI

//pin usage as follow:
//                   CS  DC/RS  RESET    SDI/MOSI  SCK   SDO/MISO  BL   RTP_DOUT   RTP_DIN   RTP_SCK   RTP_CS   RTP_IRQ   VCC    GND    
//ESP32-WROOM-32E:   15    2   ESP32-EN     13      14      12     21      39        32        25        33        36     5V     GND 

/*********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/
#include <TFT_eSPI.h> 
#include <SPI.h>
#include <TFT_Touch.h>

#define RTP_DOUT 39
#define RTP_DIN  32
#define RTP_SCK  25
#define RTP_CS   33
#define RTP_IRQ  36

TFT_eSPI my_lcd = TFT_eSPI();
TFT_Touch my_touch = TFT_Touch(RTP_CS, RTP_SCK, RTP_DIN, RTP_DOUT);

                             /*  r     g    b */
#define BLACK        0x0000  /*   0,   0,   0 */
#define BLUE         0x001F  /*   0,   0, 255 */
#define RED          0xF800  /* 255,   0,   0 */
#define GREEN        0x07E0  /*   0, 255,   0 */
#define CYAN         0x07FF  /*   0, 255, 255 */
#define MAGENTA      0xF81F  /* 255,   0, 255 */
#define YELLOW       0xFFE0  /* 255, 255,   0 */
#define WHITE        0xFFFF  /* 255, 255, 255 */
#define NAVY         0x000F  /*   0,   0, 128 */
#define DARKGREEN    0x03E0  /*   0, 128,   0 */
#define DARKCYAN     0x03EF  /*   0, 128, 128 */
#define MAROON       0x7800  /* 128,   0,   0 */
#define PURPLE       0x780F  /* 128,   0, 128 */
#define OLIVE        0x7BE0  /* 128, 128,   0 */
#define LIGHTGREY    0xC618  /* 192, 192, 192 */
#define DARKGREY     0x7BEF  /* 128, 128, 128 */
#define ORANGE       0xFD20  /* 255, 165,   0 */
#define GREENYELLOW  0xAFE5  /* 173, 255,  47 */
#define PINK         0xF81F  /* 255,   0, 255 */

/******************* UI details */
#define BUTTON_R 25
#define BUTTON_SPACING_X 25
#define BUTTON_SPACING_Y 5
#define EDG_Y 5
#define EDG_X 20

typedef struct _button_info
{
     const char button_name[10];
     uint8_t button_name_size;
     uint16_t button_name_colour;
     uint16_t button_colour;
     int16_t button_x;
     int16_t button_y;     
 }button_info;

button_info phone_button[15] = 
{
  "1",4,BLACK,CYAN,EDG_X+BUTTON_R-1,(int16_t)(my_lcd.height()-EDG_Y-4*BUTTON_SPACING_Y-9*BUTTON_R-1),
  "2",4,BLACK,CYAN,EDG_X+3*BUTTON_R+BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-4*BUTTON_SPACING_Y-9*BUTTON_R-1),
  "3",4,BLACK,CYAN,EDG_X+5*BUTTON_R+2*BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-4*BUTTON_SPACING_Y-9*BUTTON_R-1),
  "4",4,BLACK,CYAN,EDG_X+BUTTON_R-1,(int16_t)(my_lcd.height()-EDG_Y-3*BUTTON_SPACING_Y-7*BUTTON_R-1), 
  "5",4,BLACK,CYAN,EDG_X+3*BUTTON_R+BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-3*BUTTON_SPACING_Y-7*BUTTON_R-1),
  "6",4,BLACK,CYAN,EDG_X+5*BUTTON_R+2*BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-3*BUTTON_SPACING_Y-7*BUTTON_R-1),
  "7",4,BLACK,CYAN,EDG_X+BUTTON_R-1,(int16_t)(my_lcd.height()-EDG_Y-2*BUTTON_SPACING_Y-5*BUTTON_R-1),
  "8",4,BLACK,CYAN,EDG_X+3*BUTTON_R+BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-2*BUTTON_SPACING_Y-5*BUTTON_R-1),
  "9",4,BLACK,CYAN,EDG_X+5*BUTTON_R+2*BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-2*BUTTON_SPACING_Y-5*BUTTON_R-1),
  "*",4,BLACK,PINK,EDG_X+BUTTON_R-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_SPACING_Y-3*BUTTON_R-1),
  "0",4,BLACK,CYAN,EDG_X+3*BUTTON_R+BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_SPACING_Y-3*BUTTON_R-1),
  "#",4,BLACK,PINK,EDG_X+5*BUTTON_R+2*BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_SPACING_Y-3*BUTTON_R-1),
  "end",2,BLACK,RED,EDG_X+BUTTON_R-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_R-1),
  "call",2,BLACK,GREEN,EDG_X+3*BUTTON_R+BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_R-1),
  "dele",2,BLACK,LIGHTGREY,EDG_X+5*BUTTON_R+2*BUTTON_SPACING_X-1,(int16_t)(my_lcd.height()-EDG_Y-BUTTON_R-1),
};

uint16_t px, py;
uint16_t text_x=10,text_y=5,text_x_add = 4*phone_button[0].button_name_size+2,text_y_add = 6*phone_button[0].button_name_size;
uint16_t n=0;

boolean is_pressed(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t px,int16_t py)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
 }

void show_menu(void)
{
    uint16_t i;
   for(i = 0;i < sizeof(phone_button)/sizeof(button_info);i++)
   {
      my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, BUTTON_R,phone_button[i].button_colour);
      my_lcd.setTextColor(phone_button[i].button_name_colour);
      my_lcd.drawString(phone_button[i].button_name,phone_button[i].button_x-strlen(phone_button[i].button_name)*phone_button[i].button_name_size*4/2+phone_button[i].button_name_size/2+1,phone_button[i].button_y-phone_button[i].button_name_size*6/2+phone_button[i].button_name_size/2+1,phone_button[i].button_name_size);
   }
   my_lcd.fillRect(1, 1, my_lcd.width()-2, 2,BLACK);
   my_lcd.fillRect(1, 29, my_lcd.width()-2, 2,BLACK);
   my_lcd.fillRect(1, 1, 2, 30,BLACK);
   my_lcd.fillRect(my_lcd.width()-3, 1, 2, 30,BLACK);
}
  
void setup(void) 
{
  pinMode(RTP_IRQ, INPUT);
  my_lcd.init();
  my_lcd.setRotation(0);  
  my_touch.setCal(495, 3398, 721, 3448, 320, 240, 1);
  my_touch.setRotation(0);
  my_lcd.fillScreen(BLUE); 
   show_menu();
}

void loop(void)
{
     uint8_t i;
    if(my_touch.Pressed()&&!digitalRead(RTP_IRQ))  
    {   
        px = my_touch.X();
        py = my_touch.Y();
      for(i=0;i<sizeof(phone_button)/sizeof(button_info);i++)
      {
           if(is_pressed(phone_button[i].button_x-BUTTON_R,phone_button[i].button_y-BUTTON_R,phone_button[i].button_x+BUTTON_R,phone_button[i].button_y+BUTTON_R,px,py))
           {
                my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, BUTTON_R,DARKGREY);
                my_lcd.setTextColor(WHITE);
                      my_lcd.drawString(phone_button[i].button_name,phone_button[i].button_x-strlen(phone_button[i].button_name)*phone_button[i].button_name_size*4/2+phone_button[i].button_name_size/2+1,phone_button[i].button_y-phone_button[i].button_name_size*6/2+phone_button[i].button_name_size/2+1,phone_button[i].button_name_size);
                //delay(100);
                my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, BUTTON_R, phone_button[i].button_colour);
                my_lcd.setTextColor(phone_button[i].button_name_colour);
                      my_lcd.drawString(phone_button[i].button_name,phone_button[i].button_x-strlen(phone_button[i].button_name)*phone_button[i].button_name_size*4/2+phone_button[i].button_name_size/2+1,phone_button[i].button_y-phone_button[i].button_name_size*6/2+phone_button[i].button_name_size/2+1,phone_button[i].button_name_size);  
                if(i < 12)
                {
                    if(n < 13)
                    {
                      my_lcd.setTextColor(GREENYELLOW);
                      my_lcd.drawString(phone_button[i].button_name,text_x,text_y,phone_button[i].button_name_size);
                      text_x += text_x_add-1;
                      n++;
                    }
                }
                else if(12 == i)
                {
                          my_lcd.fillRect(0, 33, my_lcd.width(), 10,BLUE);
                    my_lcd.setTextColor(RED);
                          my_lcd.drawString("Calling ended",my_lcd.width()/2-52,33,1);  
                } 
                else if(13 == i)
                {
                          my_lcd.fillRect(0, 33, my_lcd.width(), 10,BLUE);
                    my_lcd.setTextColor(GREEN);
                          my_lcd.drawString("Calling...",my_lcd.width()/2-52,33,1);  
                }
                else if(14 == i)
                {
                    if(n > 0)
                    {
                        text_x -= (text_x_add-1);  
                        my_lcd.fillRect(text_x, text_y, text_x_add, text_y_add-1,BLUE);
                        n--; 
                    }
                }
                while(!digitalRead(RTP_IRQ));
           }      
        }
    }
}
