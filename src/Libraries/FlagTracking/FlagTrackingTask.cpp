#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  flagSig_t mainFlagSig {1, 2};

  VisionReading mainVisionReading(1, 30);

  screenDrawing mainScreenDrawing(mainFlagSig, 316, 212);
  mainScreenDrawing.initSimpleObjects(30);

  simpleObjects* objectExport;

  while(true)
  {

    mainVisionReading.getObjects(); //Calculates Objects
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterNoise();
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterSize();
    mainVisionReading.debugErrorSig();

    mainVisionReading.filterProp();
    mainVisionReading.debugErrorSig();

    mainVisionReading.discardObjects();

    mainVisionReading.debugObjects(2);


    objectExport = mainVisionReading.exportObjects();
    mainScreenDrawing.drawSimpleObjects(objectExport);




    pros::delay(50);
  }
}
