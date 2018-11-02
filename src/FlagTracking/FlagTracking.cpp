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

  VisionReading mainVisionReading(20, 30);
  screenObjects_t simpleScreenObjects = mainScreenDrawing.createSimpleObjects(30);
  simpleScreenObjects.blueStyle = mainScreenDrawing.createStyle(LV_COLOR_BLACK);
  simpleScreenObjects.redStyle = mainScreenDrawing.createStyle(LV_COLOR_BLACK);
  simpleScreenObjects.blueStyle.body.opa = LV_OPA_50;
  simpleScreenObjects.redStyle.body.opa = LV_OPA_50;

  FlagSorting mainFlagSorting(20, 6, 0.5, 100);
  screenObjects_t sortedScreenObjects = mainScreenDrawing.createSimpleObjects(20);
  sortedScreenObjects.blueStyle = mainScreenDrawing.createStyle(LV_COLOR_BLUE);
  sortedScreenObjects.redStyle = mainScreenDrawing.createStyle(LV_COLOR_RED);


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
    //
    objectExport = mainFlagSorting.exportObjects();
    objectCount = mainFlagSorting.exportCount();
    //
    mainScreenDrawing.drawSimpleObjects(sortedScreenObjects, objectExport, objectCount);


    pros::delay(100);
  }
}
