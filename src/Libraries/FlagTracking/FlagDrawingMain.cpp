#include "main.h"

//#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"


//
// screenDrawing::screenDrawing(int containerWidth, int containerHeight, int objectWidthScale, int objectHeightScale)
// :
// m_objectWidthScale(objectWidthScale),
// m_objectHeightScale(objectHeightScale)
// {
//
// // Background ----------------------------------------------------------------------
// objectContainer = lv_obj_create(lv_scr_act(), NULL);
// lv_obj_set_size(objectContainer, containerWidth, containerHeight);
// lv_obj_align(objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
// //lv_obj_set_pos(objectContainer, 0, 0);
//
// // Style for background of screen
// static lv_style_t backgroundStyle;
// lv_style_copy(&backgroundStyle, &lv_style_plain_color);
// backgroundStyle.body.main_color = LV_COLOR_GRAY;
// lv_obj_set_style(objectContainer, &backgroundStyle);
// // Background ----------------------------------------------------------------------
//
//
// // Object Sytles ----------------------------------------------------------------------
// //Flag Object Style
// lv_style_copy(&objectStyle, &lv_style_pretty_color);
// objectStyle.body.main_color = LV_COLOR_GREEN;
// objectStyle.body.grad_color = LV_COLOR_GREEN;
// objectStyle.body.radius = 8;
// objectStyle.body.border.color = LV_COLOR_GREEN;
// objectStyle.body.border.width = 3;
// objectStyle.body.border.opa = LV_OPA_100;
//
// //Blue object style
// lv_style_copy(&blueObjectStyle, &objectStyle);
// blueObjectStyle.body.main_color = LV_COLOR_BLUE;
// blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
// blueObjectStyle.body.border.color = LV_COLOR_BLACK;
//
// //Red object style
// lv_style_copy(&redObjectStyle, &objectStyle);
// redObjectStyle.body.main_color = LV_COLOR_RED;
// redObjectStyle.body.grad_color = LV_COLOR_RED;
// redObjectStyle.body.border.color = LV_COLOR_BLACK;
// // Object Sytles ----------------------------------------------------------------------
//
//
//
//
//
//   for(int objectNum = 0; objectNum < OBJECT_NUM; objectNum++)
//   {
//     flagObjects[objectNum] = lv_obj_create(objectContainer, NULL); //Make the screen its parent
//   }
//
//
// }
//
//
//
//
//
//
//
//
//   void screenDrawing::drawFlagObjects()
//   {
//
//     for(int objectNum = 0; objectNum < OBJECT_NUM; objectNum++)
//     {
//       if(flagExport[objectNum].objSig != VISION_OBJECT_ERR_SIG)
//       {
//
//       // make visible
//       lv_obj_set_hidden(flagObjects[objectNum], false);
//
//       //Set posisitons and size
//       lv_obj_set_x(flagObjects[objectNum], flagExport[objectNum].objX * m_objectWidthScale);
//       lv_obj_set_y(flagObjects[objectNum], flagExport[objectNum].objY * m_objectHeightScale);
//
//       lv_obj_set_width(flagObjects[objectNum], flagExport[objectNum].objWidth * m_objectWidthScale);
//       lv_obj_set_height(flagObjects[objectNum], flagExport[objectNum].objHeight * m_objectHeightScale);
//
//
//         if(flagExport[objectNum].discardObject)
//         {
//           lv_obj_set_style(flagObjects[objectNum], &objectStyle);
//         }
//         else if(flagExport[objectNum].objSig == blueSig)
//         {
//           lv_obj_set_style(flagObjects[objectNum], &blueObjectStyle); //Give it the style for a blue flagObject
//         }
//         else if(flagExport[objectNum].objSig == redSig)
//         {
//           lv_obj_set_style(flagObjects[objectNum], &redObjectStyle); //Give it the style for a blue flagObject
//         }
//
//
//       }
//       else
//       {
//         lv_obj_set_hidden(flagObjects[objectNum], true);
//       }
//     }
//
//   }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//






// static lv_obj_t * flagObjects[display_C::NUM_COLOR_OBJECTS][NUM_VISION_OBJECTS]; //Create flagObject arrays
//
//
//
// void drawObjectTask(void*)
// {
//   initFlagObjects();
//   while(true)
//   {
//     drawFlagObjects(OBJ_BLUE_COLOR);
//     drawFlagObjects(OBJ_RED_COLOR);
//     delay(display_C::DISPLAY_REFRESH);
//   }
// }
//
//
//
//
//
// void initFlagObjects() //Init objects of all colors
// {
//   for(int colorNum = 0; colorNum < display_C::NUM_COLOR_OBJECTS; colorNum++)
//   {
//     for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
//     {
//       flagObjects[colorNum][objectNum] = lv_obj_create(objectContainer, NULL); //Make the screen its parent
//       if(colorNum == OBJ_BLUE_COLOR)
//       {
//         lv_obj_set_style(flagObjects[colorNum][objectNum], &blueObjectStyle); //Give it the style for a blue flagObject
//       }
//       else if(colorNum == OBJ_RED_COLOR)
//       {
//         lv_obj_set_style(flagObjects[colorNum][objectNum], &redObjectStyle); //Give it the style for a blue flagObject
//       }
//
//     }
//   }
// }
//
//
//
//
//
// void drawFlagObjects(int objectColor)
// {
//   int filterNum = FILTER_FIRST;
//
//     for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
//     {
//       if (objectNum < flagObjects[objectColor][filterNum].objectCount)
//       {
//         // make visible
//         lv_obj_set_hidden(flagObjects[objectColor][objectNum], false);
//         //Set posisitons and size
//         lv_obj_set_x(flagObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectX[objectNum] * display_C::OBJECT_SCALE_WIDTH);
//         lv_obj_set_y(flagObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectY[objectNum] * display_C::OBJECT_SCALE_HEIGHT);
//
//         lv_obj_set_width(flagObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectWidth[objectNum] * display_C::OBJECT_SCALE_WIDTH);
//         lv_obj_set_height(flagObjects[objectColor][objectNum], flagObjects[objectColor][filterNum].objectHeight[objectNum] * display_C::OBJECT_SCALE_HEIGHT);
//       }
//       else
//       {
//         lv_obj_set_hidden(flagObjects[objectColor][objectNum], true);
//       }
//     }
//   }
