#include "ObjectTrackingMain.hpp"

// main task
void mainObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;
  pros::Vision mainVision(20);
  // pros::delay(1000);
  // pros::vision_signature_s_t blueSig = mainVision.get_signature(1);
  // mainVision.print_signature(blueSig);
  // pros::vision_signature_s_t redSig = mainVision.get_signature(2);
  // mainVision.print_signature(redSig);

  pros::vision_signature_s_t blueSig = {1, {1, 0, 0}, 11.000000, -2857, -2377, -2617, 13067, 13935, 13501, 0, 0};
  mainVision.set_signature(1, &blueSig);
  pros::vision_signature_s_t redSig = {2, {1, 0, 0}, 5.000000, 9347, 10065, 9706, -541, -113, -327, 0, 0};
  mainVision.set_signature(2, &redSig);

  ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  ObjectContainer rawObjects(6, mainScreenDrawing);
  rawObjects.setContainerStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);
  VisionReading mainVisionReading(mainVision, rawObjects);

  ObjectContainer sortedObjects(20, mainScreenDrawing);
  sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE);
  sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED);
  ObjectSmoothing objectSmoothing(rawObjects, sortedObjects, 30, 0.35, false, 0.3, 40, 20);

  while(true)
  {


    mainVisionReading.getObjects();
    mainVisionReading.filterNoise(20);
    //mainVisionReading.filterProp();
    //mainVisionReading.filterSize();
    std::cout << mainVisionReading.discardObjects();

    mainScreenDrawing.drawSimpleObjects(rawObjects);


    objectSmoothing.smoothObjects();
    mainScreenDrawing.drawSimpleObjects(sortedObjects);


    // testInput.outputObjects();
    // mainScreenDrawing.drawSimpleObjects(rawObjects);
    //
    // rawObjectSmoothing.smoothObjects(rawObjects, sortedObjects);
    // mainScreenDrawing.drawSimpleObjects(sortedObjects);
    // rawObjectSmoothing.exportObjects(ghostObjects, 0, 20);
    // mainScreenDrawing.drawSimpleObjects(ghostObjects);

    pros::delay(50);
  }
}
