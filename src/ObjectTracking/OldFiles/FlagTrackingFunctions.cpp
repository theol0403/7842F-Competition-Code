// #include "main.h"
// #include "FlagTrackingData.hpp"
// #include "FlagTrackingFunctions.hpp"
//
//
// // Count the amount of objects filled in the array
// //Returns # of objects - not array location
// int countObjects(vision_object objectArray[NUM_VISION_OBJECTS])
// {
//   bool exitCounting = false;
//   int objectCount;
//   for (objectCount = 0; !exitCounting && objectCount < NUM_VISION_OBJECTS; objectCount++)
//   {
//     if(objectArray[objectCount].signature == ERR_SIG)
//     {
//       exitCounting = true;
//     }
//   }
//   return objectCount;
// }
//
//
//
// //Fills flag arrays
// void fillObjectPrinting( vision_object obj, int objectNum, int flagColor )
// {
//   flagObjects[flagColor][FILTER_FIRST].objectX[objectNum] = obj.left_coord; // (obj.left_coord * -1) + VISION_FOV_HEIGHT;
//   flagObjects[flagColor][FILTER_FIRST].objectY[objectNum] = obj.top_coord;
//   flagObjects[flagColor][FILTER_FIRST].objectWidth[objectNum] = obj.width;
//   flagObjects[flagColor][FILTER_FIRST].objectHeight[objectNum] = obj.height;
//   flagObjects[flagColor][FILTER_FIRST].objectSize[objectNum] = (obj.height + obj.width) / 2;
// }
//
//
//
// void passArrayObjectPrinting(int flagColor, int firstFilter, int secondFilter)
// {
//   for (int objectNum = 0; objectNum < flagObjects[flagColor][firstFilter].objectCount; objectNum++)
//   {
//     passObjectPrinting(flagColor, firstFilter, secondFilter, objectNum, objectNum);
//   }
//   flagObjects[flagColor][secondFilter].objectCount = flagObjects[flagColor][firstFilter].objectCount;
// }
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
// #define FLAG_BLUE_Y flagObjects[OBJ_BLUE_COLOR][FILTER_FINAL].objectY[blueObjectNum]
// #define FLAG_BLUE_X flagObjects[OBJ_BLUE_COLOR][FILTER_FINAL].objectX[blueObjectNum]
// #define FLAG_RED_Y flagObjects[OBJ_RED_COLOR][FILTER_FINAL].objectY[redObjectNum]
// #define FLAG_RED_X flagObjects[OBJ_RED_COLOR][FILTER_FINAL].objectX[redObjectNum]
//
// int middle_flag_num = 0;
// bool found_pair;
//
// void group_flags(){
//
//   // Loop through all BLUE objects
//   for(int blueObjectNum = 0; blueObjectNum < flagObjects[OBJ_BLUE_COLOR][FILTER_FINAL].objectCount; blueObjectNum++)
//   {
//     found_pair = 0;
//
//     // Loop through all RED objects
//     for(int redObjectNum = 0; redObjectNum < flagObjects[OBJ_RED_COLOR][FILTER_FINAL].objectCount; redObjectNum++)
//     {
//
//       // if redObjectY -15 is smaller than blueObjectY AND redObjectY is greater than blueObjectY + 15
//       if(FLAG_BLUE_Y - 15 < FLAG_RED_Y && FLAG_RED_Y < FLAG_BLUE_Y + 15)
//       {
//         if(FLAG_BLUE_X < FLAG_RED_X && FLAG_RED_X < FLAG_BLUE_X + 40)
//         {
//           //Draw green flag
//           //clear old flag values
//           FLAG_RED_X = -10;
//           FLAG_RED_Y = -10;
//           //store middle flag
//           middle_flag_num++;
//           //middle_flags_x[middle_flag_num] = screen_origin_x + ((FLAG_BLUE_X+FLAG_RED_X)/2);
//           //middle_flags_x[middle_flag_num] = screen_origin_y + ((FLAG_BLUE_Y+FLAG_RED_Y)/2);
//           found_pair = 1;
//         }
//       }
//     }
//     if(!found_pair)
//     {
//       //Draw blue flag
//     }
//     else
//     {
//       //clear old flag values
//       FLAG_BLUE_X = -10;
//       FLAG_BLUE_Y = -10;
//     }
//   }
//   for(int redObjectNum = 0; redObjectNum < flagObjects[OBJ_RED_COLOR][FILTER_FINAL].objectCount; redObjectNum++)
//   {
//     if(FLAG_RED_X > 0)
//     {
//       //draw red flag
//     }
//   }
//
// }
