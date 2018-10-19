#include "main.h"




const int OBJECT_NUM = 30;


enum flagSig_t
{
  blueSig = 1,
  redSig = 2
}

struct colorObjects
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objSize; // Avg of width and height
  bool discardObject;
};

// const int NUM_COLORS = 3;
// enum OBJ_COLOR // English for the flag colors
//   {
//     OBJ_BLUE_COLOR,
//     OBJ_RED_COLOR,
//     OBJ_GREEN_COLOR
//   };


class visionTracking;
