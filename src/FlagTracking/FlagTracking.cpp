#include "main.h"

#include "Include/FlagTracking/FlagTracking.hpp"
#include "Include/FlagTracking/VisionReading.hpp"
#include "Include/FlagTracking/FlagSorting.hpp"
#include "Include/FlagTracking/ScreenDrawing.hpp"




// main task
void mainFlagTrackingTask(void*)
{
  flagSig_t mainFlagSig {1, 2};

  ScreenDrawing mainScreenDrawing(mainFlagSig, 316, 212);


  VisionReading mainVisionReading(1, 30);
  screenObjects_t simpleScreenObjects = mainScreenDrawing.initSimpleObjects(30, LV_COLOR_BLUE, LV_COLOR_RED);

  FlagSorting mainFlagSorting(20, 6, 1, 20);
  screenObjects_t sortedScreenObjects = mainScreenDrawing.initSimpleObjects(20, LV_COLOR_BLUE, LV_COLOR_RED);






  simpleObjects_t* objectExport;
  int objectCount;


  while(true)
  {

    mainVisionReading.getObjects(); //Calculates Objects
    mainVisionReading.filterNoise();
    mainVisionReading.filterProp();
    mainVisionReading.filterSize();
    mainVisionReading.discardObjects();

    mainVisionReading.debugObjects(2);


    objectExport = mainVisionReading.exportObjects();
    objectCount = mainVisionReading.exportCurrentCount();
    //mainScreenDrawing.drawSimpleObjects(simpleScreenObjects, objectExport, objectCount);


    mainFlagSorting.importSource(objectExport, objectCount);
    mainFlagSorting.createAllignList();
    mainFlagSorting.debugAllign();
    mainFlagSorting.mergeMaster();
    mainFlagSorting.sortMaster();

    objectExport = mainFlagSorting.exportObjects();
    objectCount = mainFlagSorting.exportCount();

    mainScreenDrawing.drawSimpleObjects(sortedScreenObjects, objectExport, objectCount);




    pros::delay(100);
  }
}
