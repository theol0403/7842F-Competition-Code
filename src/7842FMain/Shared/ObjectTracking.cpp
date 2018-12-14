#include "ObjectTracking.hpp"

// main task
void ObjectTrackingTask(void*)
{

  lib7842::ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  const int BLUESIG = 1;
  const int REDSIG = 2;
  const int BLUESIG2 = 3;
  const int REDSIG2 = 4;

  const int GREENSIG = 5;

  pros::Vision mainVision(11);
  pros::delay(1000);
  // pros::vision_signature_s_t blueSig = mainVision.get_signature(1);
  // mainVision.print_signature(blueSig);
  //pros::vision_signature_s_t redSig = mainVision.get_signature(2);
  //mainVision.print_signature(redSig);
  pros::vision_signature_s_t SIG_1 = {1, {1, 0, 0}, 9.300000, -2979, -2215, -2597, 11377, 13517, 12447, 0, 0};
  mainVision.set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = {2, {1, 0, 0}, 8.00000, 9227, 9651, 9439, -543, -227, -385, 0, 0};
  mainVision.set_signature(2, &SIG_2);


  lib7842::ObjectReading mainObjectReading(mainVision);
//  lib7842::codeSig_t BLUECODE = mainObjectReading.createCodeSig(BLUESIG, GREENSIG);
//  lib7842::codeSig_t REDCODE = mainObjectReading.createCodeSig(BLUESIG, GREENSIG);

  lib7842::ObjectContainer rawObjects(12, mainScreenDrawing);
  rawObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_BLACK, LV_OPA_40);
  rawObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_BLACK, LV_OPA_40);
//  rawObjects.setSigStyle(BLUESIG2, LV_COLOR_BLUE, LV_COLOR_WHITE, LV_OPA_40);
//  rawObjects.setSigStyle(REDSIG2, LV_COLOR_RED, LV_COLOR_WHITE, LV_OPA_40);
//  rawObjects.setSigStyle(BLUECODE.destSig, LV_COLOR_BLUE, LV_COLOR_LIME, LV_OPA_40);
//  rawObjects.setSigStyle(REDCODE.destSig, LV_COLOR_RED, LV_COLOR_LIME, LV_OPA_40);


  lib7842::ObjectContainer smoothedObjects(20, mainScreenDrawing);
  smoothedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_YELLOW, LV_OPA_70);
  smoothedObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_YELLOW, LV_OPA_70);
  lib7842::ObjectSmoothing objectSmoothing
  (
    rawObjects, smoothedObjects, // Source, Dest
  {},//  {{BLUESIG2, BLUESIG},  {REDSIG2, REDSIG}},// {BLUECODE.destSig, BLUESIG}, {REDCODE.destSig, REDSIG}}, // Sig merges
    20, 10, 3, // Maxlife, LifeZone, lifeIncrement
    0.5, 0.4, // PosEMA, VelEMA
    {{20, 15}, {15, 30}, {40, 10}, {50, 5}}, // PosThresh, DimThresh
    1 // DebugMode
  );

  while(true)
  {
    rawObjects.shrinkTo(0);
    mainObjectReading.getSigObjects(rawObjects, {BLUESIG, REDSIG});
    rawObjects.removeRange(lib7842::objArea, 0, 400);
   mainScreenDrawing.drawSimpleObjects(rawObjects);

   objectSmoothing.smoothObjects();
   mainScreenDrawing.drawSimpleObjects(smoothedObjects);


    pros::delay(100);
  }
}
