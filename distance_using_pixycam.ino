/* Copyright Nov 2016 LeRoy Miller
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
 */

/* 
 * based on http://www.pyimagesearch.com/2015/01/19/find-distance-camera-objectmarker-using-python-opencv/    
 * for use with the Pixy Cam (CMUcam 5) 
 * 
 */
 
#include "Pixy.h"

/*Calibrate using a tape measure or ruler, set your pixycam 2 foot from the color coded object
 * In the serial monitor, take a few reading of for the width and height.
 * they should be the same on each reading, once you are sure they are.
 * Put your readings in the variables, set Calibrate back to 0, reupload the sketch.
 * Now the sketch should start to give you distances.
 */
 
#define Calibrate 0 //Change to 1 see above for details.
#define calDistance 24 //in inches 24inches or 2 foot
#define LCD 1 //Display Average Distance to Parallax 1602 serial LCD otherwise Zero will display much more information to serial console and/or LCD if connected


int calWidth = 40; //Calibrated width reading
int calHeight = 29; //Calibrated height reading
int pixelsWidth;   //read by the camera
int pixelsHeight; //read by the camera
float distanceWidth;   //calculated distance based on the width of the object
float distanceHeight;  //calculated distance based on the height of the object 
float widthOfObject = 3.75; //inches (3.75 inches) real size of your object
float heightOfObject = 2.5; //inches (2.5 inches) real size of your object
int focalLengthWidth;  //calculated focal length for width
int focalLengthHeight; //calculated focal length for height
float avg;
int feet;
int inches;

//Formula = FocalLengthWidth = (pixels * knowdistanceininches) / widthOfObject
//Distance = (widthOfObject * FocalLengthWidth) / pixelsWidth
//focal length and distance for height is calculated the same way replacing width with height values

Pixy pixy;

void setup()
{
  Serial.begin(9600); // 9600 baud for the serial *console* (not for the UART connected to Pixy)
  Serial.print("Starting...\n");

  if (LCD) { Serial.write(17); }
  
  pixy.init();
  //calculate focal length
  focalLengthWidth = (calWidth * calDistance) / widthOfObject;
  focalLengthHeight = (calHeight * calDistance) / heightOfObject;
  
}

void loop()
{
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  blocks = pixy.getBlocks();
  
  if (blocks)
    {
    i++;
    
   // do this (print) every 50 frames because printing every
   // frame would bog down the Arduino
   if (i%50==0)
    {

      if (Calibrate) {
          sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf); 
      for (j=0; j<blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
      }
    }
        
       else {

          if (LCD) { } else {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
          }
      for (j=0; j<blocks; j++)
      {
        //sprintf(buf, "  block %d: ", j);
        //Serial.print(buf); 
        pixelsWidth = pixy.blocks[j].width;
        pixelsHeight = pixy.blocks[j].height;
        distanceWidth = (widthOfObject * focalLengthWidth) / pixelsWidth;
        distanceHeight = (heightOfObject * focalLengthHeight) / pixelsHeight;
        avg = (distanceWidth + distanceHeight)/2;
        avg = round(avg);
        feet = avg/12;
        inches = int(avg) % 12;
        
        if (LCD) { 
          Serial.write(128);
          Serial.print("Average: ");
          Serial.print(avg);
          Serial.print("in");
          Serial.write(148);
          Serial.print(feet);
          Serial.print(" ft. ");
          Serial.print(inches);
          Serial.print(" in."); 
              } else {
        Serial.print("Width: ");
        Serial.print(pixelsWidth);
        Serial.print(" Height: ");
        Serial.print(pixelsHeight);
        Serial.print(" Distance W: ");
        Serial.print(distanceWidth);
        Serial.print("in. ");
        Serial.print("Distance H: ");
        Serial.print(distanceHeight);
        Serial.print("in. Angle:");
        Serial.println(pixy.blocks[j].angle);
        Serial.print("Average: ");
        Serial.print(avg);
        Serial.print("in. ");
        Serial.print(feet);
        Serial.print(" ft. ");
        Serial.print(inches);
        Serial.println(" in.");
       }
     }
    }  
  }
}
}
