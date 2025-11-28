/*
This is a sketch to calibrate the screem and list the values to use in the
setCal() function.
The calibration report is sent to the Serial port.
*/

// Call up the TFT driver library
#include <TFT_eSPI.h> 
#include <SPI.h>

// Call up touch screen library
#include <TFT_Touch.h>

// Invoke custom TFT driver library
TFT_eSPI my_lcd = TFT_eSPI();

// These are the pins I used to interface between the 2046 touch controller and MCU
// they can be changed to other digital pins

#define RTP_DOUT 39
#define RTP_DIN  32
#define RTP_SCK  25
#define RTP_CS   33
//The touch screen interrupt request pin (T_IRQ) is not used

/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(RTP_CS, RTP_SCK, RTP_DIN, RTP_DOUT);

int X_Raw = 0, Y_Raw = 0;

void setup()
{
  // Messages are sent to the serial port
  Serial.begin(38400);

  // Initialise the TFT
  my_lcd.init();

  // Set the TFT screen to landscape orientation
  my_lcd.setRotation(1);

  my_lcd.setTextDatum(TC_DATUM);  // Set text plotting reference datum to Top Centre (TC)
  my_lcd.setTextColor(TFT_WHITE, TFT_BLACK); // Set text to white on black
}

/* Main program */
void loop()
{
  int x1, y1;
  int x2, y2;
  int x3, y3;
  bool xyswap  = 0, xflip = 0, yflip = 0;

  Serial.println("TFT_Touch Calibration, follow TFT screen prompts..");
    
  // Reset the calibration values
  touch.setCal(0, 4095, 0, 4095, 320, 240, 0);//, 0, 0);
  
  // Set TFT the screen to landscape orientation
  my_lcd.setRotation(1);
  
  // Set Touch the screen to the same landscape orientation
  touch.setRotation(1);

  // Clear the screen
  my_lcd.fillScreen(TFT_BLACK);

  // Show the screen prompt
  drawPrompt();

  drawCross(30, 30, TFT_RED);
  while (!touch.Pressed());
  delay(100);
  
  getCoord(); // This function assigns values to X_Raw and Y_Raw

  drawCross(30, 30, TFT_BLACK);
  Serial.print("First point : Raw x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  x1 = X_Raw;
  y1 = Y_Raw;

  drawCross(my_lcd.width()/2, my_lcd.height()/2, TFT_RED);
  delay(10);
  
  while (getCoord()); // This waits for the centre area to be touched
  
  drawCross(my_lcd.width()/2, my_lcd.height()/2, TFT_BLACK);
  Serial.print("Second point : Raw x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  drawCross(my_lcd.width()-30, my_lcd.height()-30, TFT_RED);
  
  while (!getCoord()); // This waits until the centre area is no longer pressed
  delay(10);           // Wait a little for touch bounces to stop after release
  
  getCoord();
  drawCross(my_lcd.width()-30, my_lcd.height()-30, TFT_BLACK);
  Serial.print("Third point : Raw x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  x2 = X_Raw;
  y2 = Y_Raw;

  drawCross(my_lcd.width()/2, my_lcd.height()/2, TFT_RED);
  delay(10);
  
  while (getCoord()); // This waits for the centre area to be touched
  
  drawCross(my_lcd.width()/2, my_lcd.height()/2, TFT_BLACK);
  Serial.print("Fourth point : Raw x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  drawCross(30, my_lcd.height()-30, TFT_RED);
  
  while (!getCoord()); // This waits until the centre area is no longer pressed
  delay(10);           // Wait a little for touch bounces to stop after release
  
  getCoord();
  drawCross(30, my_lcd.height()-30, TFT_BLACK);
  Serial.print("Fifth point : Raw x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  x3 = X_Raw;
  y3 = Y_Raw;

  int temp;
  if (abs(x1 - x3) > 1000) {
    xyswap = 1;
    temp = x1; x1 = y1; y1 = temp;
    temp = x2; x2 = y2; y2 = temp;
    temp = x3; x3 = y3; y3 = temp;
  }
  else xyswap = 0;
  

  //if (x2 < x1) {
  //  temp = x2; x2 = x1; x1 = temp;
  //  xflip = 1;
  //}
  
  //if (y2 < y1) {
  //  temp = y2; y2 = y1; y1 = temp;
  //  yflip = 1;
  //}

  int hmin = x1;// - (x2 - x1) * 3 / (my_lcd.width()/10 - 6);
  int hmax = x2;// + (x2 - x1) * 3 / (my_lcd.width()/10 - 6);

  int vmin = y1;// - (y2 - y1) * 3 / (my_lcd.height()/10 - 6);
  int vmax = y2;// + (y2 - y1) * 3 / (my_lcd.height()/10 - 6);

 // Serial.println();
 // Serial.println("These are the values you can use in Calibration.h");
 // Serial.print("#define _XMIN ");
 // Serial.println(xmin);
 // Serial.print("#define _YMIN ");
 // Serial.println(ymin);

 // Serial.print("#define _XMAX ");
 // Serial.println(xmax);
 // Serial.print("#define _YMAX ");
 // Serial.println(ymax);

  Serial.println();
  Serial.println("  //This is the calibration line to use in your sketch");
  Serial.println("  //you can copy and paste into your sketch setup()");
  Serial.print("  touch.setCal(");
  Serial.print(hmin); Serial.print(", ");
  Serial.print(hmax); Serial.print(", ");
  Serial.print(vmin); Serial.print(", ");
  Serial.print(vmax); Serial.print(", ");
  Serial.print(my_lcd.width()); Serial.print(", ");
  Serial.print(my_lcd.height()); Serial.print(", ");
  Serial.print(xyswap); //Serial.print(", ");
  //Serial.print(xflip); Serial.print(", ");
  //Serial.print(yflip);
  Serial.println(");");

  Serial.println();
  Serial.println("Test the touch screen, green crosses appear at the touch coordinates!");
  Serial.println("Send any character from the serial monitor window to restart calibration");

  // These are the calibration settings the sketch has calculated to try out!
  touch.setCal(hmin, hmax, vmin, vmax, my_lcd.width(), my_lcd.height(), xyswap);//, xflip, yflip); // Raw xmin, xmax, ymin, ymax, 320, 240, XYswap, xflip, yflip

  // Keep TFT and Touch rotations the same, try values 0 to 3
  // Start with the current orientation
  // Receiving any serial character moves on to next orientation test
  my_lcd.setRotation(1);
  touch.setRotation(1);
  test();

  my_lcd.setRotation(2);
  touch.setRotation(2);
  test();

  my_lcd.setRotation(3);
  touch.setRotation(3);
  test();

  my_lcd.setRotation(0);
  touch.setRotation(0);
  test();

  Serial.println();
}

void test(void)
{
  my_lcd.fillScreen(TFT_BLACK);

  drawCross(30, 30, TFT_WHITE);

  drawCross(my_lcd.width() - 30, my_lcd.height() - 30, TFT_WHITE);

  int centre = my_lcd.width()/2; // Get and work out x coord of screen centre

  String text;
  text+= "Screen rotation = ";
  text+= my_lcd.getRotation();
  char buffer[30];
  text.toCharArray(buffer,30);
  
  my_lcd.drawString(buffer, centre, 50, 2);

  my_lcd.drawString("Touch anywhere on screen", centre, 70, 2);
  my_lcd.drawString("to test settings", centre, 90, 2);

  my_lcd.drawString("Send a character from the", centre, 120, 2);
  my_lcd.drawString("IDE Serial Monitor to", centre, 140, 2);
  my_lcd.drawString("continue!", centre, 160, 2);

  while (Serial.available()) Serial.read(); // Empty the serial buffer before we start
  
  while (!Serial.available()) {
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      int X_Coord = touch.X();
      int Y_Coord = touch.Y();

      drawCross(X_Coord, Y_Coord, TFT_GREEN);

      //delay(20);
      my_lcd.setCursor(centre, 0,  2);
      my_lcd.print("X = ");my_lcd.print(X_Coord);my_lcd.print("   ");
      my_lcd.setCursor(centre, 20,  2);
      my_lcd.print("Y = ");my_lcd.print(Y_Coord);my_lcd.print("   ");

    }
  }
}

void drawPrompt(void)
{
  my_lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  int centre = my_lcd.width()/2; // Get and work out x coord of screen centre

  my_lcd.drawString("CALIBRATION", centre, 20, 2);

  my_lcd.drawString("Touch the red cross accurately", centre, 61, 2);
  my_lcd.drawString("( using a cocktail stick works well! )", centre, 81, 2);
}

void drawCross(int x, int y, unsigned int color)
{
  my_lcd.drawLine(x - 5, y, x + 5, y, color);
  my_lcd.drawLine(x, y - 5, x, y + 5, color);
}

bool getCoord()
{
  bool Xwait = 1, Ywait = 1;
  int X_Temp1 = 9999, Y_Temp1 = 9999;
  int X_Temp2 = -1, Y_Temp2 = -1;
  X_Raw = -1;
  Y_Raw = -1;

  while (Xwait || Ywait) {
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      X_Temp1 = touch.RawX();
      Y_Temp1 = touch.RawY();
    }
    delay(5);
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      X_Temp2 = touch.RawX();
      Y_Temp2 = touch.RawY();
    }

    #define RAW_ERROR 10
    
    if ( (abs(X_Temp1 - X_Temp2) < RAW_ERROR) && Xwait ) {
      X_Raw = (X_Temp1 + X_Temp2) / 2;
      Xwait = 0;
    }
    if ( (abs(Y_Temp1 - Y_Temp2) < RAW_ERROR) && Ywait ) {
      Y_Raw = (Y_Temp1 + Y_Temp2) / 2;
      Ywait = 0;
    }
  }

  // Check if press is near middle third of screen
  if ((X_Raw > 1365) && (X_Raw < 2731) && (Y_Raw > 1365) && (Y_Raw < 2371)) return 0;
  
  // otherwise it is near edge for calibration points
  else return 1;
}
