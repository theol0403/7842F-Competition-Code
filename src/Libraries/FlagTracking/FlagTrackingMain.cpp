#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"


const int OBJECT_NUM = 30;


enum flagSig_t
{
  blueSig = 1,
  redSig = 2
};

struct colorObjects
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objSize; // Avg of width and height
  bool discardObject;
};





colorObjects flagExport[OBJECT_NUM];


class visionTracking
{

private:


  pros::Vision m_thisVision;


public:

  visionTracking(int portNum)
  : m_thisVision(portNum)
  {
  }

  int m_objectCount{0};

  const int m_objectNum{OBJECT_NUM};
  colorObjects m_flagObjects[OBJECT_NUM] = {};



  // Looks at vision for color, counts objects, and fills them in to master array
  int getObjects()
  {
    pros::vision_object visionTempArray[m_objectNum]; //Creates temp array for vision objects
    m_objectCount = m_thisVision.read_by_size(0, m_objectNum, visionTempArray);


    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
    {
      m_flagObjects[objectNum].objSig = visionTempArray[objectNum].signature;
      m_flagObjects[objectNum].objY = visionTempArray[objectNum].top_coord;
      m_flagObjects[objectNum].objX = visionTempArray[objectNum].left_coord;
      m_flagObjects[objectNum].objWidth = visionTempArray[objectNum].width;
      m_flagObjects[objectNum].objHeight = visionTempArray[objectNum].height;
      m_flagObjects[objectNum].objSize = (visionTempArray[objectNum].height + visionTempArray[objectNum].width) / 2;
      m_flagObjects[objectNum].discardObject = false;
    }

    return m_objectCount;
  }



  int filterObjectSize(float sizeThreshold = 0.5)
  {
    int discardCounter = 0;
    int avgSize = 0;

    //Total object sizes
    for(int objectNum = 0; objectNum < m_objectCount; objectNum++)
    {
      avgSize += m_flagObjects[objectNum].objSize;
    }
    avgSize /= m_objectCount;

    // upper and lower ranges for size threshold
    int sizeLow = avgSize - (avgSize * sizeThreshold);
    int sizeHigh = avgSize + (avgSize * sizeThreshold);


    // loop through objects, look for size, and mark to discard
    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
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
  int filterObjectProp(float propThreshold = 0.2)
  {
    int discardCounter = 0;

    int sizeWidth;
    int sizeHeight;
    int objectSize;
    int sizeLow;
    int sizeHigh;


    // loop through objects, look for size, and fill into new array
    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
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





  void exportArray()
  {


    int exportNum = 0;

    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
    {
      if(m_flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
      {
        flagExport[exportNum].objSig = m_flagObjects[objectNum].objSig;
        flagExport[exportNum].objY = m_flagObjects[objectNum].objY;
        flagExport[exportNum].objX = m_flagObjects[objectNum].objX;
        flagExport[exportNum].objWidth = m_flagObjects[objectNum].objWidth;
        flagExport[exportNum].objHeight = m_flagObjects[objectNum].objHeight;
        flagExport[exportNum].objSize = m_flagObjects[objectNum].objSize;
        flagExport[exportNum].discardObject = m_flagObjects[objectNum].discardObject;
        exportNum++;
      }
      else
      {
        flagExport[exportNum].objSig = VISION_OBJECT_ERR_SIG;
        flagExport[exportNum].objY = 0;
        flagExport[exportNum].objX = 0;
        flagExport[exportNum].objWidth = 0;
        flagExport[exportNum].objHeight = 0;
        flagExport[exportNum].objSize = 0;
        flagExport[exportNum].discardObject = 0;
      }

    }

  }






};































class screenDrawing
{
private:

  const int m_objectWidthScale;
  const int m_objectHeightScale;

  lv_obj_t * objectContainer;
  lv_style_t blueObjectStyle;
  lv_style_t redObjectStyle;
  lv_style_t objectStyle;


  lv_obj_t * flagObjects[OBJECT_NUM];

public:






  screenDrawing(int containerWidth, int containerHeight, int objectWidthScale, int objectHeightScale)
  :
  m_objectWidthScale(objectWidthScale),
  m_objectHeightScale(objectHeightScale)
  {

    // Background ----------------------------------------------------------------------
    objectContainer = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(objectContainer, containerWidth, containerHeight);
    lv_obj_align(objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    //lv_obj_set_pos(objectContainer, 0, 0);

    // Style for background of screen
    static lv_style_t backgroundStyle;
    lv_style_copy(&backgroundStyle, &lv_style_plain_color);
    backgroundStyle.body.main_color = LV_COLOR_GRAY;
    lv_obj_set_style(objectContainer, &backgroundStyle);
    // Background ----------------------------------------------------------------------


    // Object Sytles ----------------------------------------------------------------------
    //Flag Object Style
    lv_style_copy(&objectStyle, &lv_style_pretty_color);
    objectStyle.body.main_color = LV_COLOR_GREEN;
    objectStyle.body.grad_color = LV_COLOR_GREEN;
    objectStyle.body.radius = 8;
    objectStyle.body.border.color = LV_COLOR_GREEN;
    objectStyle.body.border.width = 3;
    objectStyle.body.border.opa = LV_OPA_100;

    //Blue object style
    lv_style_copy(&blueObjectStyle, &objectStyle);
    blueObjectStyle.body.main_color = LV_COLOR_BLUE;
    blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
    blueObjectStyle.body.border.color = LV_COLOR_BLACK;

    //Red object style
    lv_style_copy(&redObjectStyle, &objectStyle);
    redObjectStyle.body.main_color = LV_COLOR_RED;
    redObjectStyle.body.grad_color = LV_COLOR_RED;
    redObjectStyle.body.border.color = LV_COLOR_BLACK;
    // Object Sytles ----------------------------------------------------------------------





    for(int objectNum = 0; objectNum < OBJECT_NUM; objectNum++)
    {
      flagObjects[objectNum] = lv_obj_create(objectContainer, NULL); //Make the screen its parent
    }


  }







  void drawFlagObjects()
  {

    for(int objectNum = 0; objectNum < OBJECT_NUM; objectNum++)
    {
      if(flagExport[objectNum].objSig != VISION_OBJECT_ERR_SIG)
      {

        // make visible
        lv_obj_set_hidden(flagObjects[objectNum], false);

        //Set posisitons and size
        lv_obj_set_x(flagObjects[objectNum], flagExport[objectNum].objX * m_objectWidthScale);
        lv_obj_set_y(flagObjects[objectNum], flagExport[objectNum].objY * m_objectHeightScale);

        lv_obj_set_width(flagObjects[objectNum], flagExport[objectNum].objWidth * m_objectWidthScale);
        lv_obj_set_height(flagObjects[objectNum], flagExport[objectNum].objHeight * m_objectHeightScale);


        if(flagExport[objectNum].discardObject)
        {
          lv_obj_set_style(flagObjects[objectNum], &objectStyle);
        }
        else if(flagExport[objectNum].objSig == blueSig)
        {
          lv_obj_set_style(flagObjects[objectNum], &blueObjectStyle); //Give it the style for a blue flagObject
        }
        else if(flagExport[objectNum].objSig == redSig)
        {
          lv_obj_set_style(flagObjects[objectNum], &redObjectStyle); //Give it the style for a blue flagObject
        }


      }
      else
      {
        lv_obj_set_hidden(flagObjects[objectNum], true);
      }
    }

  }





};























const int OBJECT_CONTAINER_WIDTH(316); // 480
const int OBJECT_CONTAINER_HEIGHT(212); // 240

const int OBJECT_SCALE_WIDTH(OBJECT_CONTAINER_WIDTH / VISION_FOV_WIDTH);
const int OBJECT_SCALE_HEIGHT(OBJECT_CONTAINER_HEIGHT / VISION_FOV_HEIGHT);


// main task
void mainFlagTrackingTask(void*ignore)
{
  visionTracking mainVisionTracking(9);
  screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    mainVisionTracking.exportArray();
    mainScreenDrawing.drawFlagObjects();




    pros::delay(100);
  }
}



















//
//
//
//
// //Creates struct array for storing object data
// colorObjects flagObjects[OBJECT_NUM];
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
//   vision_object visionObjectArray[OBJECT_NUM]; //Creates container array for vision objects
//
//     int objCount;
//     int startPos = 0;
//
//     objCount = mainVision.read_by_sig(0, FLAG_BLUE_SIG, OBJECT_NUM/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_BLUE_COLOR);
//     startPos = objCount;
//
//     objCount = mainVision.read_by_sig(0, FLAG_RED_SIG, OBJECT_NUM/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_RED_COLOR);
//
//     return startPos + objCount;
//
// }
