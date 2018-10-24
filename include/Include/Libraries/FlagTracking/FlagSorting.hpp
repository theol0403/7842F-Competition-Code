#include "main.h"
#pragma once



struct sortedObjects_t
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objCenterX;
  int objCenterY;

  int destroyCounter;
  bool matchFound;
};




class FlagSorting
{

private:


const int m_sourceCount;

const int m_masterCount;
const int m_decayTime;


sortedObjects_t* m_masterObjects = nullptr;


public:


  FlagSorting(int, int);
  ~FlagSorting();

  void sortMaster();



};
