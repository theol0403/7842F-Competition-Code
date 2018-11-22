#include "ObjectTrackingMain.hpp"

// main task
void mainFlagTrackingTask(void*)
{

  objectSig_t mainFlagSig {1, 2};

  ObjectDrawing mainScreenDrawing(mainFlagSig, LV_HOR_RES, LV_VER_RES);

  objectContainer_t* rawObjects = mainScreenDrawing.createObjectContainer(6, true);
  rawObjects->blueStyle = mainScreenDrawing.createStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);
  rawObjects->redStyle = mainScreenDrawing.createStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);

  objectContainer_t* sortedObjects = mainScreenDrawing.createObjectContainer(20, true);
  sortedObjects->blueStyle = mainScreenDrawing.createStyle(LV_COLOR_BLUE);
  sortedObjects->redStyle = mainScreenDrawing.createStyle(LV_COLOR_RED);
  ObjectSmoothing rawObjectSmoothing(rawObjects->arrayLength, 30, 0.1, true, 0.1, 30, 10, false);

  objectContainer_t* ghostObjects = mainScreenDrawing.createObjectContainer(20, true);
  ghostObjects->blueStyle = mainScreenDrawing.createStyle(LV_COLOR_ORANGE, LV_COLOR_BLACK, LV_OPA_30);
  ghostObjects->redStyle = mainScreenDrawing.createStyle(LV_COLOR_ORANGE, LV_COLOR_BLACK, LV_OPA_30);


  #include "ObjectTracking/ObjectData/testObjects14.h"
  ObjectReplay testInput(testObjects14, 1000, 6, rawObjects);


  while(true)
  {

    // mainVisionReading.getObjects(); //Calculates Objects
    // mainVisionReading.filterNoise();
    // mainVisionReading.filterProp();
    // mainVisionReading.filterSize();
    // mainVisionReading.discardObjects();
    // mainVisionReading.debugObjects(2);
    // objectExport = mainVisionReading.exportObjects();
    // objectCount = mainVisionReading.exportCurrentCount();
    // //mainScreenDrawing.drawSimpleObjects(simpleScreenObjects, objectExport, objectCount);

    testInput.outputObjects();
    mainScreenDrawing.drawSimpleObjects(rawObjects);

    rawObjectSmoothing.smoothObjects(rawObjects, sortedObjects);
    mainScreenDrawing.drawSimpleObjects(sortedObjects);
    rawObjectSmoothing.exportObjects(ghostObjects, 0, 20);
    mainScreenDrawing.drawSimpleObjects(ghostObjects);

    pros::delay(100);
  }
}
