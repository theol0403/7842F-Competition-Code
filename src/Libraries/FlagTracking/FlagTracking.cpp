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

  FlagSorting mainFlagSorting(20, 6);

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
    mainScreenDrawing.drawSimpleObjects(objectExport);


    mainFlagSorting.importSource(objectExport, mainVisionReading.exportCurrentCount());




    pros::delay(50);
  }
}
