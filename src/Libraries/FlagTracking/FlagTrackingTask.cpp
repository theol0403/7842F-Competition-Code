#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  flagSig_t mainFlagSig {1, 2};

  visionTracking mainVisionTracking(1, 30);
  std::cout << "Vision";
  screenDrawing mainScreenDrawing(mainFlagSig, 316, 212, 30);
  std::cout << "After";

  visionObjects* objectExport;

  while(true)
  {

    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    //mainVisionTracking.filterObjectProp();
    //mainVisionTracking.discardObjects();

    objectExport = mainVisionTracking.exportObjects();

    std::cout << "Object 0 | ";
    std::cout << "Width: " << objectExport[0].objWidth << " | ";
    std::cout << "Height: " << objectExport[0].objHeight << " | ";
    std::cout << "X: " << objectExport[0].objX << " | ";
    std::cout << "Y: " << objectExport[0].objY << " | ";
    std::cout << "Sig: " << objectExport[0].objSig << " | ";
    std::cout << "Discard: " << objectExport[0].discardObject << " | ";
    std::cout << "\n";
    //
    // std::cout << "Object 1 | ";
    // std::cout << "Width: " << objectExport[1].objWidth << " | ";
    // std::cout << "Height: " << objectExport[1].objHeight << " | ";
    // std::cout << "X: " << objectExport[1].objX << " | ";
    // std::cout << "Y: " << objectExport[1].objY << " | ";
    // std::cout << "Sig: " << objectExport[1].objSig << " | ";
    // std::cout << "Discard: " << objectExport[1].discardObject << " | ";
    // std::cout << "\n";
    //
    // std::cout << "Object 2 | ";
    // std::cout << "Width: " << objectExport[2].objWidth << " | ";
    // std::cout << "Height: " << objectExport[2].objHeight << " | ";
    // std::cout << "X: " << objectExport[2].objX << " | ";
    // std::cout << "Y: " << objectExport[2].objY << " | ";
    // std::cout << "Sig: " << objectExport[2].objSig << " | ";
    // std::cout << "Discard: " << objectExport[2].discardObject << " | ";
    // std::cout << "\n";

    mainScreenDrawing.drawVisionObjects(objectExport, 20);




    pros::delay(50);
  }
}
