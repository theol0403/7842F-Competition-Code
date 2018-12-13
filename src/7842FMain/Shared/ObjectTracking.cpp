#include "ObjectTracking.hpp"

// main task
void ObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;
  const int BLUESIG2 = 3;
  const int REDSIG2 = 3;

  pros::Vision mainVision(20);
  pros::delay(1000);

  // pros::vision_signature_s_t blueSig = mainVision.get_signature(1);
  // mainVision.print_signature(blueSig);
  //pros::vision_signature_s_t redSig = mainVision.get_signature(2);
  //mainVision.print_signature(redSig);

  pros::vision_signature_s_t SIG_1 = {1, {1, 0, 0}, 9.300000, -2979, -2215, -2597, 11377, 13517, 12447, 0, 0};
  mainVision.set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = {2, {1, 0, 0}, 8.00000, 9227, 9651, 9439, -543, -227, -385, 0, 0};
  mainVision.set_signature(2, &SIG_2);

  lib7842::ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  lib7842::ObjectContainer rawObjects(12, mainScreenDrawing);
    rawObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_BLACK, LV_OPA_40);
    rawObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_BLACK, LV_OPA_40);
    rawObjects.setSigStyle(BLUESIG2, LV_COLOR_BLUE, LV_COLOR_WHITE, LV_OPA_40);
    rawObjects.setSigStyle(REDSIG2, LV_COLOR_RED, LV_COLOR_WHITE, LV_OPA_40);
    lib7842::ObjectReading mainObjectReading(mainVision, rawObjects);

    lib7842::ObjectContainer sortedObjects(20, mainScreenDrawing);
    sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_YELLOW, LV_OPA_70);
    sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_YELLOW, LV_OPA_70);
    lib7842::ObjectSmoothing objectSmoothing(
    rawObjects, sortedObjects, // Source, Dest
    {{BLUESIG2, BLUESIG}, {REDSIG2, REDSIG}}, // Sig merges
    40, 20, 3, // Maxlife, LifeZone, lifeIncrement
    0.35, 0.3, // PosEMA, VelEMA
    {{30, 15}}, // PosThresh, DimThresh
    1 // DebugMode
  );

  while(true)
  {

    mainObjectReading.getObjects();
    rawObjects.filterAreaRange(0, 400);

    mainScreenDrawing.drawSimpleObjects(rawObjects);


    objectSmoothing.smoothObjects();
    mainScreenDrawing.drawSimpleObjects(sortedObjects);


    pros::delay(50);
  }
}
