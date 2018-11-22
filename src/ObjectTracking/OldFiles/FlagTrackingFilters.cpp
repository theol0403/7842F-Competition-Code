// #include "main.h"
// #include "FlagTrackingData.hpp"
// #include "FlagTrackingFunctions.hpp"
//
// const int NUM_VISION_FILTERS(5); //number of filters
// enum FILTER_ENUMS // English for the different filter names
// {
//   FILTER_FIRST = 0,
//   FILTER_SIZE,
//   FILTER_PROP,
//   FILTER_ROLL,
//   FILTER_FINAL
// };
// // List defining filters and their orders
// const int filterLists[NUM_VISION_FILTERS] = {FILTER_PROP, FILTER_SIZE, FILTER_ROLL};
//
//
// const double FILTER_PROP_THRESHOLD(0.2);
// const double FILTER_SIZE_THRESHOLD(0.5);
// const int FILTER_ROLL_NUM(5);
//
//
// filterFunctions filterObjectArrays;
//
//
// class filterFunctions
// {
//   public:
//   simpleObjects_t filterArray[NUM_VISION_FILTERS][NUM_VISION_OBJECTS];
//   int objectCount;
//
//
//   void filterObjects()
//   {
//     populateObjectPrinting(FILTER_FIRST);
//
//     for (int filterNum = 0; filterNum < NUM_VISION_FILTERS; filterNum++)
//     {
//       switch (filterLists[filterNum])
//       {
//         case FILTER_SIZE:
//         filterSize(filterNum);
//         break;
//
//         case FILTER_PROP:
//         filterProp(filterNum);
//         break;
//
//         case FILTER_ROLL:
//         filterObjectRollingAvg(filterNum);
//         break;
//
//
//         default:
//         passArrayObjectPrinting(filterNum, filterNum + 1);
//         break;
//       }
//     }
//   }
//
//
// private:
//
//
//   //Pass data from flag tracking to filtering
//   void populateObjectPrinting(int destFlter)
//   {
//     for (int objectNum = 0; objectNum < objectCount; objectNum++)
//     {
//       filterArray[destFlter][objectNum].objX = flagObjects[objectNum].objX;
//       filterArray[destFlter][objectNum].objY = flagObjects[objectNum].objY;
//       filterArray[destFlter][objectNum].objWidth = flagObjects[objectNum].objWidth;
//       filterArray[destFlter][objectNum].objHeight = flagObjects[objectNum].objHeight;
//       filterArray[destFlter][objectNum].objAvgDim = flagObjects[objectNum].objAvgDim;
//     }
//   }
//
//
//   //Pass object from one filter level to the next
//   void passFilterObject(int sourceFilter, int destFilter, int sourceNum, int destNum)
//   {
//     filterArray[destFilter][destNum].objX = filterArray[destFilter][destNum].objX;
//     filterArray[destFilter][destNum].objY = filterArray[destFilter][destNum].objY;
//     filterArray[destFilter][destNum].objWidth = filterArray[destFilter][destNum].objWidth;
//     filterArray[destFilter][destNum].objHeight = filterArray[destFilter][destNum].objHeight;
//     filterArray[destFilter][destNum].objAvgDim = filterArray[destFilter][destNum].objAvgDim;
//   }
//
//
//
//   //Filters avg object sizes
//   void filterSize(int filterNum)
//   {
//     int avgSize = 0;
//
//     //Total object sizes
//     for(int objectNum = 0; objectNum < objectCount; objectNum++)
//     {
//       avgSize += filterArray[filterNum][objectNum].objAvgDim;
//     }
//     avgSize = avgSize / objectCount;
//
//     // upper and lower ranges for size threshold
//     int sizeLow = avgSize - (avgSize * FILTER_SIZE_THRESHOLD);
//     int sizeHigh = avgSize + (avgSize * FILTER_SIZE_THRESHOLD);
//
//     int destObjNum = 0; //New index counter for object dest
//
//     // loop through objects, look for size, and fill into new array
//     for (int objectNum = 0; objectNum < objectCount; objectNum++)
//     {
//       if(rangeFinder(filterArray[filterNum][objectNum].objAvgDim, sizeLow, sizeHigh))
//       {
//         passFilterObject( filterNum, filterNum + 1, objectNum, destObjNum);
//         destObjNum++;
//       }
//     }
//     objectCount = destObjNum;
//   }
//
//
//
//
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
// /*
// //Filters object proportions
// void filterProp( int flagColor, int filterNum)
// {
//
//   int sizeWidth;
//   int sizeHeight;
//   int objectSize;
//   int sizeLow;
//   int sizeHigh;
//
//   int filteredObjectCount = 0; //New index counter for object dest
//
//   // loop through objects, look for size, and fill into new array
//   for (int objectNum = 0; objectNum < flagObjects[flagColor][filterNum].objectCount; objectNum++)
//   {
//
//     sizeWidth = flagObjects[flagColor][filterNum].objectWidth[objectNum];
//     sizeHeight = flagObjects[flagColor][filterNum].objectHeight[objectNum];
//     objectSize = flagObjects[flagColor][filterNum].objectSize[objectNum];
//
//     sizeLow = objectSize - (objectSize * FILTER_PROP_THRESHOLD);
//     sizeHigh = objectSize + (objectSize * FILTER_PROP_THRESHOLD);
//
//     if (rangeFinder(sizeWidth, sizeLow, sizeHigh) && rangeFinder(sizeHeight, sizeLow, sizeHigh))
//     {
//       passObjectPrinting(flagColor, filterNum, filterNum + 1, objectNum, filteredObjectCount);
//       filteredObjectCount++;
//     }
//
//   }
//   flagObjects[flagColor][filterNum + 1].objectCount = filteredObjectCount;
// }
// */
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
// /*
// simpleObjects_t rollingAvgObjects[OBJ_NUM_COLORS][NUM_VISION_OBJECTS];
//
// // Averages objects through time
// void filterObjectRollingAvg( int flagColor, int filterNum)
// {
//
//   // move all object arrays up the rolling array
//   // Start at the top roll, and fill in from the one before
//   // End at 1, filling 0 into 1
//   for(int rollNum = FILTER_ROLL_NUM; rollNum > 0; rollNum--)
//   {
//     for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
//     {
//       rollingAvgObjects[flagColor][rollNum].objectX[objectNum] = rollingAvgObjects[flagColor][rollNum-1].objectX[objectNum];
//       rollingAvgObjects[flagColor][rollNum].objectY[objectNum] = rollingAvgObjects[flagColor][rollNum-1].objectY[objectNum];
//       rollingAvgObjects[flagColor][rollNum].objectWidth[objectNum] = rollingAvgObjects[flagColor][rollNum-1].objectWidth[objectNum];
//       rollingAvgObjects[flagColor][rollNum].objectHeight[objectNum] = rollingAvgObjects[flagColor][rollNum-1].objectHeight[objectNum];
//       rollingAvgObjects[flagColor][rollNum].objectSize[objectNum] = rollingAvgObjects[flagColor][rollNum-1].objectSize[objectNum];
//     }
//   }
//
//   //Fill in roll 0 with info from the last filter
//   for(int objectNum = 0; objectNum < NUM_VISION_OBJECTS; objectNum++)
//   {
//     rollingAvgObjects[flagColor][0].objectX[objectNum] = flagObjects[flagColor][filterNum].objectX[objectNum];
//     rollingAvgObjects[flagColor][0].objectY[objectNum] = flagObjects[flagColor][filterNum].objectY[objectNum];
//     rollingAvgObjects[flagColor][0].objectWidth[objectNum] = flagObjects[flagColor][filterNum].objectWidth[objectNum];
//     rollingAvgObjects[flagColor][0].objectHeight[objectNum] = flagObjects[flagColor][filterNum].objectHeight[objectNum];
//     rollingAvgObjects[flagColor][0].objectSize[objectNum] = flagObjects[flagColor][filterNum].objectSize[objectNum];
//   }
//
//   int avgX;
//   int avgY;
//   int avgWidth;
//   int avgHeight;
//   int avgSize;
//
//   // Take the averages going up the rolls of a single object
//   for(int objectNum = 0; objectNum < flagObjects[flagColor][filterNum].objectCount; objectNum++)
//   {
//     avgX = 0;
//     avgY = 0;
//     avgWidth = 0;
//     avgHeight = 0;
//     avgSize = 0;
//
//     // Add all the attrs of the objects
//     for(int rollNum = 0; rollNum < FILTER_ROLL_NUM; rollNum++)
//     {
//       avgX += rollingAvgObjects[flagColor][rollNum].objectX[objectNum];
//       avgY += rollingAvgObjects[flagColor][rollNum].objectY[objectNum];
//       avgWidth += rollingAvgObjects[flagColor][rollNum].objectWidth[objectNum];
//       avgHeight += rollingAvgObjects[flagColor][rollNum].objectHeight[objectNum];
//       avgSize += rollingAvgObjects[flagColor][rollNum].objectSize[objectNum];
//     }
//
//     flagObjects[flagColor][filterNum+1].objectX[objectNum] = avgX / FILTER_ROLL_NUM;
//     flagObjects[flagColor][filterNum+1].objectY[objectNum] = avgY / FILTER_ROLL_NUM;
//     flagObjects[flagColor][filterNum+1].objectWidth[objectNum] = avgWidth / FILTER_ROLL_NUM;
//     flagObjects[flagColor][filterNum+1].objectHeight[objectNum] = avgHeight / FILTER_ROLL_NUM;
//     flagObjects[flagColor][filterNum+1].objectSize[objectNum] = avgSize / FILTER_ROLL_NUM;
//   }
//
//
// flagObjects[flagColor][filterNum+1].objectCount = flagObjects[flagColor][filterNum].objectCount;
//
// }
//
// */
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
// // Range finder
// bool rangeFinder(int value, int low, int high)
// {
//   return low <= value && value < high;
// }
