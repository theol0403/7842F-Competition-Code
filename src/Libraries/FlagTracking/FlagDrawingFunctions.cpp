#include "main.h"
#include "DisplayFunctions.hpp"
#include "FlagTrackingData.hpp"

static lv_obj_t * flagScreenObjects[display_C::NUM_COLOR_OBJECTS][NUM_VISION_OBJECTS]; //Create flagObject arrays



void drawObjectTask(void*)
{
  initFlagObjects();
  while(true)
  {
    drawFlagObjects(OBJ_BLUE_COLOR);
    drawFlagObjects(OBJ_RED_COLOR);
    delay(display_C::DISPLAY_REFRESH);
  }
}





void initFlagObjects() //Init objects of all colors
{
  for(int colorNum = 0; colorNum < display_C::NUM_COLOR_OBJECTS; colorNum++)
  {
    for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
    {
      flagScreenObjects[colorNum][objectNum] = lv_obj_create(objectContainer, NULL); //Make the screen its parent
      if(colorNum == OBJ_BLUE_COLOR)
      {
        lv_obj_set_style(flagScreenObjects[colorNum][objectNum], &blueObjectStyle); //Give it the style for a blue flagObject
      }
      else if(colorNum == OBJ_RED_COLOR)
      {
        lv_obj_set_style(flagScreenObjects[colorNum][objectNum], &redObjectStyle); //Give it the style for a blue flagObject
      }

    }
  }
}





void drawFlagObjects(int objectColor)
{
  int filterNum = FILTER_FIRST;

    for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
    {
      if (objectNum < flagObjects[objectColor][filterNum].objectCount)
      {
        // make visible
        lv_obj_set_hidden(flagScreenObjects[objectColor][objectNum], false);
        //Set posisitons and size
        lv_obj_set_x(flagScreenObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectX[objectNum] * display_C::OBJECT_SCALE_WIDTH);
        lv_obj_set_y(flagScreenObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectY[objectNum] * display_C::OBJECT_SCALE_HEIGHT);

        lv_obj_set_width(flagScreenObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectWidth[objectNum] * display_C::OBJECT_SCALE_WIDTH);
        lv_obj_set_height(flagScreenObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectHeight[objectNum] * display_C::OBJECT_SCALE_HEIGHT);
      }
      else
      {
        lv_obj_set_hidden(flagScreenObjects[objectColor][objectNum], true);
      }
    }
  }
