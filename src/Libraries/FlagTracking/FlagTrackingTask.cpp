#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  flagSig_t mainFlagSig {0, 1};

  visionTracking mainVisionTracking(1, 30);
  std::cout << "Vision";
  screenDrawing mainScreenDrawing(mainFlagSig, 316, 212, 30);
  std::cout << "After";

  visionObjects* objectExport;

  while(true)
  {

    mainVisionTracking.getObjects(); //Calculates Objects

    //mainVisionTracking.filterObjectSize();
    //mainVisionTracking.filterObjectProp();

    objectExport = mainVisionTracking.exportObjects();

    std::cout << "Loop\n";

    mainScreenDrawing.drawVisionObjects(objectExport, 20);




    pros::delay(100);
  }
}
