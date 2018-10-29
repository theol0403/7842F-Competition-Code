#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/VisionReading.hpp"
#include "Include/Libraries/FlagTracking/FlagSorting.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawing.hpp"




// main task
void mainFlagTrackingTask(void*)
{
  flagSig_t mainFlagSig {1, 2};

  VisionReading mainVisionReading(1, 30);

  ScreenDrawing mainScreenDrawing(mainFlagSig, 316, 212);
  mainScreenDrawing.initSimpleObjects(30);

  FlagSorting mainFlagSorting(20, 6, 1, 20);



  simpleObjects_t* objectExport;
  int objectCount;


  while(true)
  {

    mainVisionReading.getObjects(); //Calculates Objects
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterNoise();
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterProp();
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterSize();
    mainVisionReading.debugErrorSig();

    mainVisionReading.discardObjects();

    mainVisionReading.debugObjects(2);


    objectExport = mainVisionReading.exportObjects();
    objectCount = mainVisionReading.exportCurrentCount();
    //mainScreenDrawing.drawSimpleObjects(objectExport, objectCount);


    mainFlagSorting.importSource(objectExport, objectCount);
    mainFlagSorting.createAllignList();
    mainFlagSorting.mergeMaster();
    mainFlagSorting.sortMaster();

    objectExport = mainFlagSorting.exportObjects();
    objectCount = mainFlagSorting.exportCount();

    mainScreenDrawing.drawSimpleObjects(objectExport, objectCount);




    pros::delay(50);
  }
}
