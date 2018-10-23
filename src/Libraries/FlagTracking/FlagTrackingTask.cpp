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

    mainVisionReading.filterNoise();
    mainVisionReading.filterObjectSize();
    mainVisionReading.filterObjectProp();
    mainVisionReading.discardObjects();

    mainVisionReading.debugObjects(2);

    objectExport = mainVisionReading.exportObjects();


    mainScreenDrawing.drawSimpleObjects(objectExport, 20);




    pros::delay(50);
  }
}
