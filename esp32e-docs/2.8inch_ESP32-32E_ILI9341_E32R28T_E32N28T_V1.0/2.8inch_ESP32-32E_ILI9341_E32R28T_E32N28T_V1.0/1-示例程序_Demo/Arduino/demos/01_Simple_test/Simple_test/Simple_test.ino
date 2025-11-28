//This application does not rely on any libraries and it is for ST7796

//This program is a demo of clearing screen.

//pin usage as follow:
//                   CS  DC/RS  RESET    SDI/MOSI  SCK   SDO/MISO  BL      VCC    GND    
//ESP32-WROOM-32E:   15    2   ESP32-EN     13      14      12     21      5V     GND                  

/********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <Print.h>
#include "spi_dev.h"

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0
#define GRAY  0X8430 

#define LCD_WIDTH 240
#define LCD_HEIGHT 320

#define WR_RAM_CMD 0x2C
#define RD_RAM_CMD 0x2E
#define SET_X_CMD  0x2A
#define SET_Y_CMD  0x2B
#define MADCTL_CMD 0x36

bool lock_flag = true;

SPIClass spi = SPIClass(SPI_PORT);

void SPI_Start_Write(void)
{
    if(lock_flag)
    {
       lock_flag = false;
       spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE));
       LCD_CS_LOW;
       SET_SPI_WRITE_MODE;
    }
}

void SPI_End_Write(void)
{
    if(!lock_flag)
    {
      lock_flag = true;
      LCD_CS_HIGH;
      SET_SPI_READ_MODE;
      spi.endTransaction();
    }
}

void LCD_Write_Reg(uint8_t val)
{
    SPI_Start_Write(); 
    LCD_DC_LOW;
    spi_write_8bit(val);
    SPI_End_Write();
}

void LCD_Write_Data_8Bit(uint8_t val)
{
    SPI_Start_Write();  
    LCD_DC_HIGH;
    spi_write_8bit(val);
    SPI_End_Write();
}

void LCD_Write_Data_16Bit(uint16_t val)
{
    SPI_Start_Write(); 
    LCD_DC_HIGH;
    spi_write_16bit(val);
    SPI_End_Write();
}

void LCD_Set_Windows(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    LCD_Write_Reg(SET_X_CMD);
    LCD_Write_Data_16Bit(sx);
    LCD_Write_Data_16Bit(ex);
    LCD_Write_Reg(SET_Y_CMD);
    LCD_Write_Data_16Bit(sy);
    LCD_Write_Data_16Bit(ey);
    LCD_Write_Reg(WR_RAM_CMD);
}

void SPI_Init(void)
{
    spi.begin(SPI_SCLK, SPI_MISO, SPI_MOSI, -1);   
}

void Lcd_Init(void)
{
 //   LCD_RST_LOW;
 //   delay(20);
 //   LCD_RST_HIGH;
 //   delay(20);
 
  LCD_Write_Reg(0xCF);  
  LCD_Write_Data_8Bit(0x00); 
  LCD_Write_Data_8Bit(0xC9); //C1 
  LCD_Write_Data_8Bit(0X30); 
  LCD_Write_Reg(0xED);  
  LCD_Write_Data_8Bit(0x64); 
  LCD_Write_Data_8Bit(0x03); 
  LCD_Write_Data_8Bit(0X12); 
  LCD_Write_Data_8Bit(0X81); 
  LCD_Write_Reg(0xE8);  
  LCD_Write_Data_8Bit(0x85); 
  LCD_Write_Data_8Bit(0x10); 
  LCD_Write_Data_8Bit(0x7A); 
  LCD_Write_Reg(0xCB);  
  LCD_Write_Data_8Bit(0x39); 
  LCD_Write_Data_8Bit(0x2C); 
  LCD_Write_Data_8Bit(0x00); 
  LCD_Write_Data_8Bit(0x34); 
  LCD_Write_Data_8Bit(0x02); 
  LCD_Write_Reg(0xF7);  
  LCD_Write_Data_8Bit(0x20); 
  LCD_Write_Reg(0xEA);  
  LCD_Write_Data_8Bit(0x00); 
  LCD_Write_Data_8Bit(0x00); 
  LCD_Write_Reg(0xC0);    //Power control 
  LCD_Write_Data_8Bit(0x1B);   //VRH[5:0]  //1B
  LCD_Write_Reg(0xC1);    //Power control 
  LCD_Write_Data_8Bit(0x00);   //SAP[2:0];BT[3:0] //01 
  LCD_Write_Reg(0xC5);    //VCM control 
  LCD_Write_Data_8Bit(0x44);   //3F
  LCD_Write_Data_8Bit(0x30);   //3C
  LCD_Write_Reg(0xC7);    //VCM control2 
  LCD_Write_Data_8Bit(0XB6);   //B0
  LCD_Write_Reg(0x36);    // Memory Access Control 
  LCD_Write_Data_8Bit(0x08); 
  LCD_Write_Reg(0x3A);   
  LCD_Write_Data_8Bit(0x55); 
  LCD_Write_Reg(0xB1);   
  LCD_Write_Data_8Bit(0x00);   
  LCD_Write_Data_8Bit(0x1A);  //1A
  LCD_Write_Reg(0xB6);    // Display Function Control 
  LCD_Write_Data_8Bit(0x0A); 
  LCD_Write_Data_8Bit(0x82); 
  LCD_Write_Reg(0xF2);    // 3Gamma Function Disable 
  LCD_Write_Data_8Bit(0x00); 
  LCD_Write_Reg(0x26);    //Gamma curve selected 
  LCD_Write_Data_8Bit(0x01);

  LCD_Write_Reg(0xE0); //Set Gamma
  LCD_Write_Data_8Bit(0x0F);
  LCD_Write_Data_8Bit(0x2A);
  LCD_Write_Data_8Bit(0x28);
  LCD_Write_Data_8Bit(0x08);
  LCD_Write_Data_8Bit(0x0E);
  LCD_Write_Data_8Bit(0x08);
  LCD_Write_Data_8Bit(0x54);
  LCD_Write_Data_8Bit(0XA9);
  LCD_Write_Data_8Bit(0x43);
  LCD_Write_Data_8Bit(0x0A);
  LCD_Write_Data_8Bit(0x0F);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Reg(0XE1); //Set Gamma
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x15);
  LCD_Write_Data_8Bit(0x17);
  LCD_Write_Data_8Bit(0x07);
  LCD_Write_Data_8Bit(0x11);
  LCD_Write_Data_8Bit(0x06);
  LCD_Write_Data_8Bit(0x2B);
  LCD_Write_Data_8Bit(0x56);
  LCD_Write_Data_8Bit(0x3C);
  LCD_Write_Data_8Bit(0x05);
  LCD_Write_Data_8Bit(0x10);
  LCD_Write_Data_8Bit(0x0F);
  LCD_Write_Data_8Bit(0x3F);
  LCD_Write_Data_8Bit(0x3F);
  LCD_Write_Data_8Bit(0x0F);
  LCD_Write_Reg(0x2B); 
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x01);
  LCD_Write_Data_8Bit(0x3f);
  LCD_Write_Reg(0x2A); 
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0x00);
  LCD_Write_Data_8Bit(0xef);   
  LCD_Write_Reg(0x11); //Exit Sleep 
  delay(120);
  LCD_Write_Reg(0x29);       //Display on 

}

void Write_color_Block(uint16_t color, uint32_t len)
{
    volatile uint32_t* wr_buf = spi_write_buf;
    uint32_t color32 = ((color << 8 | color >> 8) << 16) | (color << 8 | color >> 8);
    uint8_t num = len%32,i=0;
    if(num)
    {
        while(*spi_cmd & SPI_USR);
        while(i < num)
        {
            *wr_buf++ = color32;
            i += 2; 
        }
        *spi_write_len = num * 16 - 1;
        *spi_cmd = SPI_USR;
        if(len < 32)
        {
            return;
        }
        len -= num;
        for(i /= 2; i < 16; i++)
        {
            *wr_buf++ = color32;
        }
    }
    while(*spi_cmd & SPI_USR);
    if(!num)
    {    
        while(i < 16)
        {
            *wr_buf++ = color32;
            i++; 
        }
    }
    *spi_write_len = 511;
    while(len)
    {
        while(*spi_cmd & SPI_USR);
        *spi_cmd = SPI_USR;
        len -= 32;
    }
    while(*spi_cmd & SPI_USR);
}

void LCD_Clear_Screen(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint16_t color)
{
    if((sx >= LCD_WIDTH) || (sy >= LCD_HEIGHT))
        return;
    if(((sx + w) > LCD_WIDTH) || ((sy + h) > LCD_HEIGHT))
        return;
    if(((w < 1) || (w > LCD_WIDTH)) || ((h < 1) || (h > LCD_HEIGHT)))
        return;
    LCD_Set_Windows(sx, sy, sx + w - 1, sy + h -1);
    LCD_DC_HIGH;
    SPI_Start_Write(); 
    Write_color_Block(color, w*h);
    SPI_End_Write();
}

void setup()
{
  pinMode(LCD_CS, OUTPUT);
  digitalWrite(LCD_CS, HIGH);
//  pinMode(LCD_RST, OUTPUT);
//  digitalWrite(LCD_RST, HIGH);    
  pinMode(LCD_DC, OUTPUT);
  digitalWrite(LCD_DC, HIGH);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  SPI_Init();
  Lcd_Init();
}

void loop()
{  
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, RED);delay(500);
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, GREEN);delay(500);
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, BLUE);delay(500);
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, GRAY);delay(500);
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);delay(500);
   LCD_Clear_Screen(0, 0, LCD_WIDTH, LCD_HEIGHT, BLACK);delay(500);
   for(uint16_t i=0; i< 5000;i++)
   {
      LCD_Clear_Screen(random(LCD_WIDTH-1), random(LCD_HEIGHT-1), random(LCD_WIDTH), random(LCD_HEIGHT), random(0xFFFF));
   }
}
