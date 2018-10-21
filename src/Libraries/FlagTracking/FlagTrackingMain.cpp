#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"




visionTracking::visionTracking(int portNum, int objectNum)
:
m_thisVision(portNum),
m_objectNum(objectNum)
{
  m_flagObjects = new visionObjects[objectNum];
}


visionTracking::~visionTracking()
{
  delete[] m_flagObjects;
}


// Looks at vision for color, counts objects, and fills them in to master array
int visionTracking::getObjects()
{
  pros::vision_object visionTempArray[m_objectNum]; //Creates temp array for vision objects
  m_currentCount = m_thisVision.read_by_size(0, m_objectNum, visionTempArray);


  for (int objectNum = 0; objectNum < m_objectNum; objectNum++)
  {
    m_flagObjects[objectNum].objSig = visionTempArray[objectNum].signature;
    m_flagObjects[objectNum].objY = visionTempArray[objectNum].top_coord;
    m_flagObjects[objectNum].objX = visionTempArray[objectNum].left_coord;
    m_flagObjects[objectNum].objWidth = visionTempArray[objectNum].width;
    m_flagObjects[objectNum].objHeight = visionTempArray[objectNum].height;
    m_flagObjects[objectNum].objSize = (visionTempArray[objectNum].height + visionTempArray[objectNum].width) / 2;
    m_flagObjects[objectNum].discardObject = false;
  }

  return m_currentCount;
}



int visionTracking::filterObjectSize(float sizeThreshold)
{
  int discardCounter = 0;
  int avgSize = 0;

  //Total object sizes
  for(int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    avgSize += m_flagObjects[objectNum].objSize;
  }
  avgSize /= m_objectNum;

  // upper and lower ranges for size threshold
  int sizeLow = avgSize - (avgSize * sizeThreshold);
  int sizeHigh = avgSize + (avgSize * sizeThreshold);


  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    int objSize = m_flagObjects[objectNum].objSize;
    if(objSize > sizeHigh || objSize < sizeLow)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }
    else
    {
    }

  }

  return discardCounter;
}







// Filters object proportions
int visionTracking::filterObjectProp(float propThreshold)
{
  int discardCounter = 0;

  int sizeWidth;
  int sizeHeight;
  int objectSize;
  int sizeLow;
  int sizeHigh;


  // loop through objects, look for size, and fill into new array
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {

    objectSize = m_flagObjects[objectNum].objSize;
    sizeLow = objectSize - (objectSize * propThreshold);
    sizeHigh = objectSize + (objectSize * propThreshold);

    sizeWidth = m_flagObjects[objectNum].objWidth;
    sizeHeight = m_flagObjects[objectNum].objHeight;


    if (sizeWidth < sizeLow || sizeWidth > sizeHigh || sizeHeight < sizeLow || sizeWidth > sizeHigh)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }
    else
    {
    }

  }

  return discardCounter;
}







int visionTracking::discardObjects()
{
  int exportNum = 0;

  for (int objectNum = 0; objectNum < m_objectNum; objectNum++)
  {
    if(m_flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG && !m_flagObjects[exportNum].discardObject)
    {
      m_flagObjects[exportNum].objSig = m_flagObjects[objectNum].objSig;
      m_flagObjects[exportNum].objY = m_flagObjects[objectNum].objY;
      m_flagObjects[exportNum].objX = m_flagObjects[objectNum].objX;
      m_flagObjects[exportNum].objWidth = m_flagObjects[objectNum].objWidth;
      m_flagObjects[exportNum].objHeight = m_flagObjects[objectNum].objHeight;
      m_flagObjects[exportNum].objSize = m_flagObjects[objectNum].objSize;
      m_flagObjects[exportNum].discardObject = false;
      exportNum++;
    }
    else
    {
      m_flagObjects[exportNum].objSig = VISION_OBJECT_ERR_SIG;
      m_flagObjects[exportNum].objY = 0;
      m_flagObjects[exportNum].objX = 0;
      m_flagObjects[exportNum].objWidth = 0;
      m_flagObjects[exportNum].objHeight = 0;
      m_flagObjects[exportNum].objSize = 0;
      m_flagObjects[exportNum].discardObject = false;
    }

  }

  m_currentCount = exportNum + 1;
  return exportNum + 1;
}





visionObjects* visionTracking::exportArray()
{
  return m_flagObjects;
}



//
//
//
//
//
//
// class screenDrawing
// {
// private:
//
//   const int m_objectWidthScale;
//   const int m_objectHeightScale;
//
//   lv_obj_t * objectContainer;
//   lv_style_t blueObjectStyle;
//   lv_style_t redObjectStyle;
//   lv_style_t objectStyle;
//
//
//   lv_obj_t * flagObjects[m_objectNum];
//
// public:
//
//
//
//
//
//
//   screenDrawing(int containerWidth, int containerHeight, int objectWidthScale, int objectHeightScale)
//   :
//   m_objectWidthScale(objectWidthScale),
//   m_objectHeightScale(objectHeightScale)
//   {
//
//     // Background ----------------------------------------------------------------------
//     objectContainer = lv_obj_create(lv_scr_act(), NULL);
//     lv_obj_set_size(objectContainer, containerWidth, containerHeight);
//     lv_obj_align(objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
//     //lv_obj_set_pos(objectContainer, 0, 0);
//
//     // Style for background of screen
//     static lv_style_t backgroundStyle;
//     lv_style_copy(&backgroundStyle, &lv_style_plain_color);
//     backgroundStyle.body.main_color = LV_COLOR_GRAY;
//     lv_obj_set_style(objectContainer, &backgroundStyle);
//     // Background ----------------------------------------------------------------------
//
//
//     // Object Sytles ----------------------------------------------------------------------
//     //Flag Object Style
//     lv_style_copy(&objectStyle, &lv_style_pretty_color);
//     objectStyle.body.main_color = LV_COLOR_GREEN;
//     objectStyle.body.grad_color = LV_COLOR_GREEN;
//     objectStyle.body.radius = 8;
//     objectStyle.body.border.color = LV_COLOR_GREEN;
//     objectStyle.body.border.width = 3;
//     objectStyle.body.border.opa = LV_OPA_100;
//
//     //Blue object style
//     lv_style_copy(&blueObjectStyle, &objectStyle);
//     blueObjectStyle.body.main_color = LV_COLOR_BLUE;
//     blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
//     blueObjectStyle.body.border.color = LV_COLOR_BLACK;
//
//     //Red object style
//     lv_style_copy(&redObjectStyle, &objectStyle);
//     redObjectStyle.body.main_color = LV_COLOR_RED;
//     redObjectStyle.body.grad_color = LV_COLOR_RED;
//     redObjectStyle.body.border.color = LV_COLOR_BLACK;
//     // Object Sytles ----------------------------------------------------------------------
//
//
//
//
//
//     for(int objectNum = 0; objectNum < m_objectNum; objectNum++)
//     {
//       flagObjects[objectNum] = lv_obj_create(objectContainer, NULL); //Make the screen its parent
//     }
//
//
//   }
//
//
//
//
//
//
//
//   void drawFlagObjects()
//   {
//
//     for(int objectNum = 0; objectNum < m_objectNum; objectNum++)
//     {
//       if(flagExport[objectNum].objSig != VISION_OBJECT_ERR_SIG)
//       {
//
//         // make visible
//         lv_obj_set_hidden(flagObjects[objectNum], false);
//
//         //Set posisitons and size
//         lv_obj_set_x(flagObjects[objectNum], flagExport[objectNum].objX * m_objectWidthScale);
//         lv_obj_set_y(flagObjects[objectNum], flagExport[objectNum].objY * m_objectHeightScale);
//
//         lv_obj_set_width(flagObjects[objectNum], flagExport[objectNum].objWidth * m_objectWidthScale);
//         lv_obj_set_height(flagObjects[objectNum], flagExport[objectNum].objHeight * m_objectHeightScale);
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
// };
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
//
//
//
//
//
//
//
// const int OBJECT_CONTAINER_WIDTH(316); // 480
// const int OBJECT_CONTAINER_HEIGHT(212); // 240
//
// const int OBJECT_SCALE_WIDTH(OBJECT_CONTAINER_WIDTH / VISION_FOV_WIDTH);
// const int OBJECT_SCALE_HEIGHT(OBJECT_CONTAINER_HEIGHT / VISION_FOV_HEIGHT);
//
//
// // main task
// void mainFlagTrackingTask(void*ignore)
// {
//   visionTracking mainVisionTracking(9);
//   screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);
//
//   while(true)
//   {
//     mainVisionTracking.getObjects(); //Calculates Objects
//
//     mainVisionTracking.filterObjectSize();
//     mainVisionTracking.filterObjectProp();
//
//     mainVisionTracking.exportArray();
//     mainScreenDrawing.drawFlagObjects();
//
//
//
//
//     pros::delay(100);
//   }
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
// //Creates struct array for storing object data
// visionObjects flagObjects[m_objectNum];
//
//
// // main task
// void flagCalculateTask(void*)
// {
//   while (true)
//   {
//     filterObjectArrays.objectcount = getObjects(); //Calculates Objects
//
//     filterObjects();
//
//     delay(VISION_REFRESH);
//   }
// }
//
//
//
//
//
// void fillObjData(vision_object* visionObjectArray, int startPos, int objCount, int objColor)
// {
//   for (int objectNum = startPos; objectNum < objCount; objectNum++)
//   {
//     flagObjects[objectNum].objY = visionObjectArray[objectNum].top_coord;
//     flagObjects[objectNum].objX = visionObjectArray[objectNum].left_coord;
//     flagObjects[objectNum].objWidth = visionObjectArray[objectNum].width;
//     flagObjects[objectNum].objHeight = visionObjectArray[objectNum].height;
//     flagObjects[objectNum].objSize = (visionObjectArray[objectNum].height + visionObjectArray[objectNum].width) / 2;
//     flagObjects[objectNum].objColor = objColor;
//   }
// }
//
//
//
//
//
// // Looks at vision for color, counts objects, and fills them in
// int getObjects()
// {
//   vision_object visionObjectArray[m_objectNum]; //Creates container array for vision objects
//
//     int objCount;
//     int startPos = 0;
//
//     objCount = mainVision.read_by_sig(0, FLAG_BLUE_SIG, m_objectNum/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_BLUE_COLOR);
//     startPos = objCount;
//
//     objCount = mainVision.read_by_sig(0, FLAG_RED_SIG, m_objectNum/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_RED_COLOR);
//
//     return startPos + objCount;
//
// }
