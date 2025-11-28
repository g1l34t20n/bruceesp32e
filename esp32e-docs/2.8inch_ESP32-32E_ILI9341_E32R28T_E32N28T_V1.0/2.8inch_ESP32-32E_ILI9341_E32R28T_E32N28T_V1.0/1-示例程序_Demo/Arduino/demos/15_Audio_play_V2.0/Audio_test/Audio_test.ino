//This example is reading MP3 files from an SD card for playback

//pin usage as follow:
//                   CS  DC/RS  RESET    SDI/MOSI  SCK   SDO/MISO  BL   RTP_DOUT   RTP_DIN   RTP_SCK   RTP_CS   RTP_IRQ   VCC    GND    
//ESP32-WROOM-32E:   15    2   ESP32-EN     13      14      12     21      39        32        25        33        36     5V     GND 
//                   SD_CS   SD_SCK   SD_MISO  SD_MOSI   AUDIO_EN  AUDIO_DAC
//                     5       18       19       23         4         26
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

#include <Arduino.h>
#include "SPI.h"
#include "SD.h"
#include "FS.h"
#include "Audio.h"
#include <Ticker.h>
#include <TFT_eSPI.h>
#include <TFT_Touch.h>
#include <TJpg_Decoder.h>
#include "demo_music.h"
#include "simsun16.h"
#include "music_mute.h"
#include "music_pause.h"
#include "music_play.h"
#include "music_sound.h"

extern Audio audio;
extern int song_num;
extern char * title_list[99];

char tbuf[100] = {0}, time_buf[100] = {0};
bool play_flag = true, flag1 = true, flag2 = true;
uint32_t music_id = 0;
uint32_t act = 0;
uint32_t afd = 0;

#define RTP_DOUT 39
#define RTP_DIN  32
#define RTP_SCK  25
#define RTP_CS   33
#define RTP_IRQ  36

#define SD_CS   5
#define SD_SCK  18
#define SD_MISO 19
#define SD_MOSI 23

#define AUDIO_EN 4

Ticker ticker;
Audio audio(true, I2S_DAC_CHANNEL_LEFT_EN);

struct audioMessage
{
  uint8_t     cmd;
  const char* txt;
  uint32_t    value;
  uint32_t    ret;
} audioTxMessage, audioRxMessage;

enum : uint8_t { SET_VOLUME, GET_VOLUME, CONNECTTOHOST, CONNECTTOSD };

QueueHandle_t audioSetQueue = NULL;
QueueHandle_t audioGetQueue = NULL;

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
TFT_eSprite clk = TFT_eSprite(&tft);
TFT_Touch my_touch = TFT_Touch(RTP_CS, RTP_SCK, RTP_DIN, RTP_DOUT);
SPIClass MySPI(HSPI);

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if(y >= tft.height()) 
  {
    return 0;
  }
  tft.pushImage(x, y, w, h, bitmap);
  // Return 1 to decode next block
  return 1;
}

void CreateQueues()
{
  audioSetQueue = xQueueCreate(10, sizeof(struct audioMessage));
  audioGetQueue = xQueueCreate(10, sizeof(struct audioMessage));
}

void audioTask(void *parameter)
{
  CreateQueues();
  if (!audioSetQueue || !audioGetQueue) 
  {
    log_e("queues are not initialized");
    while (true) {
      ; // endless loop
    }
  }

  struct audioMessage audioRxTaskMessage;
  struct audioMessage audioTxTaskMessage;


  while (true) 
  {
    if (xQueueReceive(audioSetQueue, &audioRxTaskMessage, 1) == pdPASS) 
    {
      if (audioRxTaskMessage.cmd == SET_VOLUME) 
      {
        audioTxTaskMessage.cmd = SET_VOLUME;
        audio.setVolume(audioRxTaskMessage.value);
        audioTxTaskMessage.ret = 1;
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == CONNECTTOHOST) 
      {
        audioTxTaskMessage.cmd = CONNECTTOHOST;
        audioTxTaskMessage.ret = audio.connecttohost(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == CONNECTTOSD) 
      {
        audioTxTaskMessage.cmd = CONNECTTOSD;
        audioTxTaskMessage.ret = audio.connecttoSD(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == GET_VOLUME) 
      {
        audioTxTaskMessage.cmd = GET_VOLUME;
        audioTxTaskMessage.ret = audio.getVolume();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else 
      {
        Serial.println("ERROR");
      }
    }
    audio.loop();
  }
}

void audioInit() 
{
  xTaskCreatePinnedToCore(
    audioTask,
    "audioplay",
    5000,
    NULL,
    2 | portPRIVILEGE_BIT,
    NULL,
    0
  );
}

audioMessage transmitReceive(audioMessage msg) 
{
  xQueueSend(audioSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(audioGetQueue, &audioRxMessage, portMAX_DELAY) == pdPASS) 
  {
    if (msg.cmd != audioRxMessage.cmd) 
    {
      Serial.println("wrong reply from message queue");
    }
  }
  return audioRxMessage;
}

void audioSetVolume(uint8_t vol)
{
  audioTxMessage.cmd = SET_VOLUME;
  audioTxMessage.value = vol;
  audioMessage RX = transmitReceive(audioTxMessage);
}

uint8_t audioGetVolume() 
{
  audioTxMessage.cmd = GET_VOLUME;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

bool audioConnecttohost(const char* host) 
{
  audioTxMessage.cmd = CONNECTTOHOST;
  audioTxMessage.txt = host;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

bool audioConnecttoSD(const char* filename) 
{
  audioTxMessage.cmd = CONNECTTOSD;
  audioTxMessage.txt = filename;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

void tcr1s() 
{
  act = audio.getAudioCurrentTime();
  //afd = audio.getAudioFileDuration();
  //uint32_t pos = audio.getFilePos();
  Serial.println(act);
  Serial.println(afd);
  if(act)
  {
    tft.setTextColor(TFT_BLUE);
    sprintf(time_buf,"%02d:%02d", (act/60), (act%60));
    tft.fillRect(96, 167, 40,18,TFT_WHITE);
    tft.drawString(time_buf,96,167,2);
  }
 // sprintf(time_buf,"%02d:%02d", (afd/60), (afd%60));
 // tft.fillRect(369, 217, 40,20,TFT_WHITE);
 // tft.drawString(time_buf,369,217,2);
  //tft.fillRect(0, 211, tft.width(),30,TFT_WHITE);
  //tft.drawString(time_buf,56,218,2);
  //vTaskDelay(10);
  //Serial.println("audioTime: %i:%02d - duration: %i:%02d", (act / 60), (act % 60) , (afd / 60), (afd % 60));
}

void setup()
{
  Serial.begin(115200);
  pinMode(AUDIO_EN, OUTPUT);
  digitalWrite(AUDIO_EN, LOW);
  pinMode(SD_CS, OUTPUT);//SD卡CS脚
  digitalWrite(SD_CS, HIGH);
  pinMode(RTP_IRQ, INPUT);
  tft.begin();          /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */
  my_touch.setCal(495, 3398, 721, 3448, 320, 240, 1);
  tft.fillScreen(TFT_WHITE);
  clk.setColorDepth(8);
  clk.loadFont(simsun16);
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  MySPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  while(!SD.begin(SD_CS,MySPI)) 
  {
    Serial.println("SD card does not exist"); 
    clk.createSprite(320, 60); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_RED, TFT_WHITE); 
    clk.drawString("SD card does not exist",160,16);
    clk.drawString("Please insert SD card",160,46);
    clk.pushSprite(0,tft.height()/2-30);
    clk.deleteSprite();
    delay(100); 
  }
  tft.fillScreen(TFT_WHITE);
  delay(200);
  audioInit();
  audio.setVolume(21); // 0...21
  ticker.attach(1, tcr1s);
  if(!demo_music())
  {
    clk.createSprite(320, 30); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_RED, TFT_WHITE); 
    clk.drawString("Not find MP3 file",160,16);
    clk.pushSprite(0,tft.height()/2-15);
    clk.deleteSprite();
    while(1);
  }
  if(song_num>8)  //最大显示8首歌曲
  {
    song_num = 8;  
  }
  for(int i=0; i< song_num; i++)  
  {
    clk.createSprite(310, 20); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_BLACK, TFT_WHITE); 
    sprintf(tbuf, "%d.%s",i+1,demo_music_get_title(i));
    clk.drawString(tbuf,155,10);
    clk.pushSprite(5,5+i*20);
    clk.deleteSprite();
  }
  Serial.println("Setup done");
  delay(500);
  clk.createSprite(310, 20); 
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_RED, TFT_WHITE); 
  sprintf(tbuf, "%d.%s",1,demo_music_get_title(0));
  clk.drawString(tbuf,155,10);
  clk.pushSprite(5,5);
  clk.deleteSprite();
  tft.drawFastHLine(0, 165, tft.width(), TFT_BLACK);
  tft.setTextColor(TFT_BLUE);
  tft.fillRect(0, 166, tft.width(),28,TFT_WHITE);
  tft.drawString("Audio time : ",10,167,2);
  tft.drawString("Total time : ",180,167,2);
  tft.drawString("00:00",266,167,2);
  tft.drawFastHLine(0, 185, tft.width(), TFT_BLACK);
  TJpgDec.drawJpg(40,tft.height()-52,music_play, sizeof(music_play));
  TJpgDec.drawJpg(200,tft.height()-52,music_sound, sizeof(music_sound));
}

uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
void loop()
{
  if(my_touch.Pressed()&&!digitalRead(RTP_IRQ))
  {
     t_x = my_touch.X();
     t_y = my_touch.Y(); 
    if((t_x>=40)&&(t_x<90)&&t_y>=(tft.height()-52)&&t_y < (tft.height()-1))
    {
      flag1 = !flag1;
      if(flag1)
      {
          TJpgDec.drawJpg(40,tft.height()-52,music_pause, sizeof(music_pause));
      }
      else
      {
          TJpgDec.drawJpg(40,tft.height()-52,music_play, sizeof(music_play));
      }
      demo_music_pause();
      while(!digitalRead(RTP_IRQ));
    }
    if((t_x>=200)&&(t_x<250)&&t_y>=(tft.height()-52)&&t_y < (tft.height()-1))
    {
      flag2 = !flag2;
      if(flag2)
      {
          TJpgDec.drawJpg(200,tft.height()-52,music_sound, sizeof(music_sound));
      }
      else
      {
          TJpgDec.drawJpg(200,tft.height()-52,music_mute, sizeof(music_mute));
      }
      digitalWrite(AUDIO_EN, !flag2);
      while(!digitalRead(RTP_IRQ));
    }
  }
  if(play_flag)
  {
    play_flag = false;
    demo_music_play(music_id);
    delay(2000);
    afd = audio.getAudioFileDuration();  
    sprintf(time_buf,"%02d:%02d", (afd/60), (afd%60));
    tft.setTextColor(TFT_BLUE);
    tft.fillRect(266, 167, 40,18,TFT_WHITE);
    tft.drawString(time_buf,266,167,2);

  }
  if(act >= afd)
  {
    play_flag = true;
    clk.createSprite(310, 20); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_BLACK, TFT_WHITE); 
    sprintf(tbuf, "%d.%s",music_id+1,demo_music_get_title(music_id));
    clk.drawString(tbuf,155,10);
    clk.pushSprite(5,5+20*music_id);
    clk.deleteSprite();
    music_id++; 
    if(music_id >= song_num)
    {
        music_id = 0;
    }
    clk.createSprite(310, 20); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_RED, TFT_WHITE); 
    sprintf(tbuf, "%d.%s",music_id+1,demo_music_get_title(music_id));
    clk.drawString(tbuf,155,10);
    clk.pushSprite(5,5+20*music_id);
    clk.deleteSprite();
  }
}
