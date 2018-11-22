#include "ObjectData.hpp"


ObjectPrinting::ObjectPrinting(const char* fileName, int arrayLength, int objectCount)
:
m_arrayLength(arrayLength),
m_objectCount(objectCount)
{
  m_arrayNum = 0;
  char filePath[100];
  sprintf(filePath, "Source\\ObjectTracking\\ObjectData\\%s%s", fileName, ".h");
  outputFile = fopen(filePath, "w");

  fprintf(outputFile, "//#include \"ObjectTracking/ObjectData/%s.h\"\n", fileName);
  fprintf(outputFile, "//ObjectReplay testInput(%s, %d, %d, destContainer);\n\n", fileName, arrayLength, objectCount);
  fprintf(outputFile, "simpleObjects_t %s[%d] = \n{", fileName, m_arrayLength*m_objectCount);
}

ObjectPrinting::~ObjectPrinting()
{
  fprintf(outputFile, "};\n");
  fclose(outputFile);
  usleep(20000);
  exit(0);
}

void ObjectPrinting::outputObjects(objectContainer_t* objectContainer)
{
  if(m_arrayNum >= m_arrayLength)
  {
    this->~ObjectPrinting();
  }
  m_arrayNum++;

  fprintf(outputFile, "\n");
  for(int objectNum = 0; objectNum < m_objectCount; objectNum++)
  {
    fprintf(outputFile, " {");
    if(objectNum < objectContainer->objectCount)
    {
      fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objSig);
    }
    else
    {
      fprintf(outputFile, "%d, ", VISION_OBJECT_ERR_SIG);
    }

    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objX);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objY);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objWidth);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objHeight);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objSize);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objCenterX);
    fprintf(outputFile, "%d, ", (int)objectContainer->objectArray[objectNum].objCenterY);
    fprintf(outputFile, "0}");
    if(objectNum < m_objectCount-1 || m_arrayNum < m_arrayLength)
    {
      fprintf(outputFile, ",\n");
    }
    else
    {
      fprintf(outputFile, "\n");
    }
  }

}









ObjectReplay::ObjectReplay(simpleObjects_t* sourceArray, int arrayLength, int objectCount, objectContainer_t* destContainer)
:
m_sourceArray(sourceArray),
m_arrayLength(arrayLength),
m_objectCount(objectCount),
m_destContainer(destContainer)
{
  m_arrayNum = 0;
}

ObjectReplay::~ObjectReplay()
{
  //
}

void ObjectReplay::outputObjects()
{
  if(m_arrayNum >= m_arrayLength) m_arrayNum = 0;

  int objectScan = 0;
  while(m_sourceArray[(m_arrayNum*m_objectCount) + objectScan].objSig != VISION_OBJECT_ERR_SIG && objectScan < m_objectCount)
  {
    objectScan++;
  }

  m_destContainer->objectCount = objectScan;
  m_destContainer->objectArray = &m_sourceArray[m_arrayNum * m_objectCount];

  m_arrayNum++;
}
