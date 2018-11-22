#pragma once
#include "ObjectTrackingMain.hpp"


class ObjectPrinting
{

private:

const int m_arrayLength;
const int m_objectCount;
int m_arrayNum = 0;

FILE* outputFile = nullptr;

public:

  ObjectPrinting(const char*, int, int);
  ~ObjectPrinting();

  void outputObjects(objectContainer_t*);

};


class ObjectReplay
{

private:

simpleObjects_t* m_sourceArray;
const int m_arrayLength;
const int m_objectCount;
objectContainer_t* m_destContainer = nullptr;

int m_arrayNum = 0;

public:

  ObjectReplay(simpleObjects_t*, int, int, objectContainer_t*);
  ~ObjectReplay();

  void outputObjects();

};
