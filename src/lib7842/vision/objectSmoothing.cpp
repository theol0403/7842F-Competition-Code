#include "objectSmoothing.hpp"

namespace lib7842
{

  ObjectSmoothing::ObjectSmoothing
  (
    ObjectContainer& sourceContainer, ObjectContainer& destContainer,
    std::initializer_list<sigMerge_t> sigReroutes,
    int lifeMax, int lifeThreshold, double lifeIncrement,
    double emaAlpha, double emaAlphaVel,
    std::initializer_list<compareZones_t> compareZones,
    int debugMode
  )
  :
  m_sourceContainer(&sourceContainer),
  m_destContainer(&destContainer),

  m_sigReroutes(sigReroutes),

  m_lifeMax(lifeMax),
  m_lifeThreshold(lifeThreshold),
  m_lifeIncrement(lifeIncrement),
  m_emaAlpha(emaAlpha),
  m_emaAlphaVel(emaAlphaVel),
  m_compareZones(compareZones),

  m_debugMode(debugMode),

  m_masterLength(m_sourceContainer->arrayLength * 8) //Should be enough
  {
    m_sourceObjects.resize(m_sourceContainer->arrayLength);
    clearArray(m_sourceObjects, 0, m_sourceContainer->arrayLength-1);

    m_masterObjects.resize(m_masterLength);
    clearArray(m_masterObjects, 0, m_masterLength-1);
    m_masterCount = 0;
  }


  void ObjectSmoothing::smoothObjects()
  {
    importSource();
    mergeObjects();
    sortMaster();
    exportObjects(m_destContainer, m_lifeMax - m_lifeThreshold, m_lifeMax);
  }


  /***
  *    ___  ___                                             _
  *    |  \/  |                                            | |
  *    | .  . | __ _ _ __   __ _  __ _ _ __ ___   ___ _ __ | |_
  *    | |\/| |/ _` | '_ \ / _` |/ _` | '_ ` _ \ / _ \ '_ \| __|
  *    | |  | | (_| | | | | (_| | (_| | | | | | |  __/ | | | |_
  *    \_|  |_/\__,_|_| |_|\__,_|\__, |_| |_| |_|\___|_| |_|\__|
  *                               __/ |
  *                              |___/
  * Functions that manage the arrays */

  void ObjectSmoothing::clearArray(std::vector<sortedObjects_t>& clearArray, int startIndex, int endIndex)
  {
    for (int objectNum = startIndex; objectNum <= endIndex; objectNum++)
    {
      clearArray.at(objectNum).objSig = VISION_OBJECT_ERR_SIG;
      clearArray.at(objectNum).objY = 0;
      clearArray.at(objectNum).objX = 0;
      clearArray.at(objectNum).objWidth = 0;
      clearArray.at(objectNum).objHeight = 0;
      clearArray.at(objectNum).objCenterX = 0;
      clearArray.at(objectNum).objCenterY = 0;
      clearArray.at(objectNum).objXVel = 0;
      clearArray.at(objectNum).objYVel = 0;

      clearArray.at(objectNum).matchFound = false;
      clearArray.at(objectNum).lifeCounter = 0;
    }
  }

  void ObjectSmoothing::swapObjects(std::vector<sortedObjects_t>& swapArray, int firstIndex, int secondIndex)
  {
    std::swap(swapArray.at(firstIndex), swapArray.at(secondIndex));
  }

  void ObjectSmoothing::sortArrayY(std::vector<sortedObjects_t>& sortArray, int firstIndex, int lastIndex)
  {
    // Loop through each object looking to swap the largest object to the right
    // except the last one, which will already be sorted by the time we get there
    for (int startIndex = firstIndex; startIndex < lastIndex; startIndex++)
    {
      int greatestIndex = startIndex; //Assume current posision to swap
      bool swapNeeded = false;
      // Loop between current and end looking for greatest object
      for (int currentIndex = startIndex + 1; currentIndex <= lastIndex; currentIndex++)
      {
        //If current object is greater than greatest object
        if (sortArray.at(currentIndex).objCenterY > sortArray.at(greatestIndex).objCenterY)
        {
          greatestIndex = currentIndex;
          swapNeeded = true;
        }
      }
      if(swapNeeded) swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
    }
  }

  void ObjectSmoothing::sortArrayLife(std::vector<sortedObjects_t>& sortArray, int firstIndex, int lastIndex)
  {
    // Loop through each object looking to swap the largest object to the right
    // except the last one, which will already be sorted by the time we get there
    int swapCount = 0;
    for (int startIndex = firstIndex; startIndex < lastIndex; startIndex++)
    {
      int greatestIndex = startIndex; //Assume current posision to swap
      bool swapNeeded = false;
      // Loop between current and end looking for greatest object
      if(sortArray.at(startIndex).lifeCounter < m_lifeMax) //Only swap if nessesary
      {
        for (int currentIndex = startIndex + 1; currentIndex <= lastIndex; currentIndex++)
        {
          //If current object is greater than greatest object
          if (sortArray.at(currentIndex).lifeCounter > sortArray.at(greatestIndex).lifeCounter)
          {
            greatestIndex = currentIndex;
            swapNeeded = true;
          }
        }
        if(swapNeeded)
        {
          swapCount++;
          swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
        }
      }
    }
    if(m_debugMode > 0)
    {
      std::cout << " | " << "L Swap: " << swapCount;
    }
  }



  /*** Step 1:
  *     _____                           _
  *    |_   _|                         | |
  *      | | _ __ ___  _ __   ___  _ __| |_
  *      | || '_ ` _ \| '_ \ / _ \| '__| __|
  *     _| || | | | | | |_) | (_) | |  | |_
  *     \___/_| |_| |_| .__/ \___/|_|   \__|
  *                   | |
  *                   |_|
  * Imports the source array and sorts it by Y into sourceObjects*/
  void ObjectSmoothing::importSource()
  {
    //Copies source into m_source array
    for (int objectNum = 0; objectNum < m_sourceContainer->currentCount; objectNum++)
    {
      if(m_sourceContainer->objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG)
      {
        m_sourceObjects.at(objectNum).objSig = m_sourceContainer->objectArray.at(objectNum).objSig;
        m_sourceObjects.at(objectNum).objX = m_sourceContainer->objectArray.at(objectNum).objX;
        m_sourceObjects.at(objectNum).objY = m_sourceContainer->objectArray.at(objectNum).objY;
        m_sourceObjects.at(objectNum).objWidth = m_sourceContainer->objectArray.at(objectNum).objWidth;
        m_sourceObjects.at(objectNum).objHeight = m_sourceContainer->objectArray.at(objectNum).objHeight;
        m_sourceObjects.at(objectNum).objCenterX = m_sourceContainer->objectArray.at(objectNum).objCenterX;
        m_sourceObjects.at(objectNum).objCenterY = m_sourceContainer->objectArray.at(objectNum).objCenterY;
        m_sourceObjects.at(objectNum).matchFound = false;
        m_sourceObjects.at(objectNum).lifeCounter = 0;
      }
      else std::cout << "Invalid Source: Err" << objectNum << "\n";
    }
    //Resets the remaining slots in the m_sourceObjects
    clearArray(m_sourceObjects, m_sourceContainer->currentCount, m_sourceContainer->arrayLength-1);
    sortArrayY(m_sourceObjects, 0, m_sourceContainer->currentCount-1); //Sorts m_sourceObjects by Y
  }



  /***
  *    ___  ___               _              ______                _   _
  *    |  \/  |              (_)             |  ___|              | | (_)
  *    | .  . | ___ _ __ __ _ _ _ __   __ _  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___
  *    | |\/| |/ _ \ '__/ _` | | '_ \ / _` | |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
  *    | |  | |  __/ | | (_| | | | | | (_| | | | | |_| | | | | (__| |_| | (_) | | | \__ \
  *    \_|  |_/\___|_|  \__, |_|_| |_|\__, | \_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
  *                      __/ |         __/ |
  *                     |___/         |___/
  * Functions that compare and merge source objects into master*/

  bool ObjectSmoothing::compareObjects(sortedObjects_t& sourceObject, sortedObjects_t& wantedObject, int wantedSig)
  {
    for(compareZones_t& compareZone : m_compareZones)
    {
      //compare sig, then Pos, then size
      bool cascadingTrue = true;
      cascadingTrue = cascadingTrue && wantedSig == wantedObject.objSig;
      cascadingTrue = cascadingTrue && (sourceObject.objCenterX >= wantedObject.objCenterX - compareZone.posThresh) && (sourceObject.objCenterX <= wantedObject.objCenterX + compareZone.posThresh);
      cascadingTrue = cascadingTrue && (sourceObject.objCenterY >= wantedObject.objCenterY - compareZone.posThresh) && (sourceObject.objCenterY <= wantedObject.objCenterY + compareZone.posThresh);
      cascadingTrue = cascadingTrue && (sourceObject.objWidth >= wantedObject.objWidth - compareZone.dimThresh) && (sourceObject.objWidth <= wantedObject.objWidth + compareZone.dimThresh);
      cascadingTrue = cascadingTrue && (sourceObject.objHeight >= wantedObject.objHeight - compareZone.dimThresh) && (sourceObject.objHeight <= wantedObject.objHeight + compareZone.dimThresh);
      if(cascadingTrue)
      {
        return true;
      }
    }
    return false;
  }

  double ObjectSmoothing::emaCalculate(double lastValue, double newValue, double emaAlpha)
  {
    return (emaAlpha * newValue + (1.0 - emaAlpha) * lastValue);
  }

  void ObjectSmoothing::mergeObject(sortedObjects_t &destObject, sortedObjects_t &newObject)
  {
    destObject.objWidth = emaCalculate(destObject.objWidth, newObject.objWidth, m_emaAlpha);
    destObject.objHeight = emaCalculate(destObject.objHeight, newObject.objHeight, m_emaAlpha);

    destObject.objX = emaCalculate(destObject.objX, newObject.objX, m_emaAlpha) + destObject.objXVel;
    destObject.objY = emaCalculate(destObject.objY, newObject.objY, m_emaAlpha) + destObject.objYVel;
    destObject.objCenterX = emaCalculate(destObject.objCenterX, newObject.objCenterX, m_emaAlpha) + destObject.objXVel;
    destObject.objCenterY = emaCalculate(destObject.objCenterY, newObject.objCenterY, m_emaAlpha) + destObject.objYVel;

    destObject.objXVel = emaCalculate(destObject.objXVel, (newObject.objCenterX - destObject.objCenterX), m_emaAlphaVel);
    destObject.objYVel = emaCalculate(destObject.objYVel, (newObject.objCenterY - destObject.objCenterY), m_emaAlphaVel);

    destObject.lifeCounter += m_lifeIncrement;
    if(destObject.lifeCounter > m_lifeMax) destObject.lifeCounter = m_lifeMax;
  }

  void ObjectSmoothing::pushObject(int destSig, sortedObjects_t &destObject, sortedObjects_t &newObject)
  {
    destObject.objSig = destSig;
    destObject.objX = newObject.objX;
    destObject.objY = newObject.objY;
    destObject.objWidth = newObject.objWidth;
    destObject.objHeight = newObject.objHeight;
    destObject.objCenterX = newObject.objCenterX;
    destObject.objCenterY = newObject.objCenterY;
    destObject.objXVel = 0;
    destObject.objYVel = 0;
    destObject.lifeCounter = m_lifeIncrement;
  }

  void ObjectSmoothing::trimObject(sortedObjects_t &destObject)
  {
    destObject.objX += destObject.objXVel;
    destObject.objY += destObject.objYVel;
    destObject.objCenterX += destObject.objXVel;
    destObject.objCenterY += destObject.objYVel;
    destObject.lifeCounter -= 1.0;
  }


  /*** Step 2:
  *    ___  ___
  *    |  \/  |
  *    | .  . | ___ _ __ __ _  ___
  *    | |\/| |/ _ \ '__/ _` |/ _ \
  *    | |  | |  __/ | | (_| |  __/
  *    \_|  |_/\___|_|  \__, |\___|
  *                      __/ |
  *                     |___/
  * Compares, Pushes, Merges, and Trims the source objects into Master*/
  void ObjectSmoothing::mergeObjects()
  {
    //Reset Flags Indicating Master Objects Merged
    for(int masterNum = 0; masterNum < m_masterLength; masterNum++)
    {
      m_masterObjects.at(masterNum).matchFound = false;
      for(bool &matchFound : m_masterObjects.at(masterNum).sigFound)
      {
        matchFound = false;
      }
    }


    if(m_debugMode > 2)
    {
      for(int masterNum = 0; masterNum < m_masterCount; masterNum++)
      {
        std::cout << std::endl << "Master:" << masterNum << ", Sig:" << m_masterObjects.at(masterNum).objSig;
      }
    }

    //Variables keeping track of different actions for debugging
    int mergeCount = 0;
    int pushCount = 0;
    int trimCount = 0;

    bool sourceMatchFound = false; // Indicates whether source has found a match

    //Loop through every source object
    for(int sourceObjectNum = 0; sourceObjectNum < m_sourceContainer->currentCount; sourceObjectNum++)
    {

      //Find out whether to merge into different sig
      int sourceSig = m_sourceObjects.at(sourceObjectNum).objSig;
      int destSig = sourceSig;
      for (sigMerge_t &sigReroute : m_sigReroutes) //Loop through sigReroutes and find if there is a swap
      {
        if(sourceSig == sigReroute.sourceSig)
        {
          destSig = sigReroute.destSig;
        }
      }

      //loop through all current master objects
      sourceMatchFound = false; //Exit looping through master when match found
      for(int masterNum = 0; masterNum < m_masterCount && !sourceMatchFound; masterNum++)
      {
        if(!m_masterObjects.at(masterNum).sigFound.at(sourceSig)) //If current signature not already merged
        {
          if(compareObjects(m_sourceObjects.at(sourceObjectNum), m_masterObjects.at(masterNum), destSig))
          {
            if(m_debugMode > 1) std::cout << std::endl << "Master:" << masterNum << ", Sig:" << m_masterObjects.at(masterNum).objSig << " <- Source:" << sourceObjectNum << ", Sig:" << sourceSig;
            mergeCount++;
            mergeObject(m_masterObjects.at(masterNum), m_sourceObjects.at(sourceObjectNum));
            m_masterObjects.at(masterNum).sigFound.at(sourceSig) = true;
            m_masterObjects.at(masterNum).matchFound = true;
            sourceMatchFound = true;
          }
        }
      }
      //If no match was found, if all matches have already been made, or if there are more sources than master
      if(!sourceMatchFound)
      {
        pushCount++;
        //Add extra object to the end of master
        pushObject(destSig, m_masterObjects.at(m_masterCount), m_sourceObjects.at(sourceObjectNum));
        m_masterObjects.at(m_masterCount).matchFound = true;
        m_masterObjects.at(m_masterCount).sigFound.at(sourceSig) = true;
        m_masterCount++; //Let merge know there are extra objects
      }
    }

    //Scan through master looking for objects with no match
    for(int masterNum = 0; masterNum < m_masterCount; masterNum++)
    {
      if(!m_masterObjects.at(masterNum).matchFound)
      {
        trimCount++;
        trimObject(m_masterObjects.at(masterNum));
      }
    }

    if(m_debugMode > 0) std::cout << std::endl << "Count:" << m_masterCount << " | " << "M:" << mergeCount << " " << "P:" << pushCount << " " << "T:" << trimCount;
  }


  /*** Step 3:
  *     _____ _                      _______            _
  *    /  __ \ |                    / /  ___|          | |
  *    | /  \/ | ___  __ _ _ __    / /\ `--.  ___  _ __| |_
  *    | |   | |/ _ \/ _` | '_ \  / /  `--. \/ _ \| '__| __|
  *    | \__/\ |  __/ (_| | | | |/ /  /\__/ / (_) | |  | |_
  *     \____/_|\___|\__,_|_| |_/_/   \____/ \___/|_|   \__|
  * Cleans and Sorts Master and Updates Counts*/
  void ObjectSmoothing::sortMaster()
  {
    //Reset master match found bools
    //delete all elements with a life of 0 and move array down
    //update masterCount to reflect the deleted objects
    //Sort array by life
    //sort array by y

    int simTrimCount = 0;
    for(int masterNum = 0; masterNum < m_masterCount; masterNum++)
    {
      for(int masterNum2 = masterNum+1; masterNum2 < m_masterCount; masterNum2++)
      {
        //  std::cout << std::endl << "Comparing:" << masterNum2 << " with:" << masterNum;
        if(compareObjects(m_masterObjects.at(masterNum), m_masterObjects.at(masterNum2), m_masterObjects.at(masterNum).objSig))
        {
          simTrimCount++;
          trimObject(m_masterObjects.at(masterNum2));
        }
      }
    }
    if(m_debugMode > 0) std::cout << " + " << simTrimCount;

    int purgeCount = 0;

    int newMasterCount = m_masterCount;
    for(int masterNum = 0; masterNum < m_masterCount; masterNum++)
    {
      if(m_masterObjects.at(masterNum).lifeCounter <= 0)
      {
        purgeCount++;
        newMasterCount--;
      }
    }

    if(m_debugMode > 0) std::cout << " | " << "R:" << purgeCount;

    sortArrayLife(m_masterObjects, 0, m_masterCount-1); //Moves all 0 objects to the right
    m_masterCount = newMasterCount;

    int startIndex = 0;
    for(int lifeCount = m_lifeMax; (lifeCount > m_lifeMax - m_lifeThreshold) && (startIndex < m_masterCount); lifeCount--)
    {
      int searchBoundary = startIndex;
      bool sortNeeded = false;
      while(m_masterObjects.at(searchBoundary).lifeCounter >= lifeCount && searchBoundary < m_masterCount)
      {
        sortNeeded = true;
        searchBoundary++;
      }
      if(sortNeeded && startIndex != searchBoundary-1)
      {
        if(m_debugMode > 0) std::cout << " | Y Sort: " << startIndex << "-" << searchBoundary-1;
        sortArrayY(m_masterObjects, startIndex, searchBoundary-1);
      }
      startIndex = searchBoundary;
    }

    clearArray(m_masterObjects, m_masterCount, m_masterLength-1);
  }


  /*** Step 4:
  *     _____                      _
  *    |  ___|                    | |
  *    | |____  ___ __   ___  _ __| |_
  *    |  __\ \/ / '_ \ / _ \| '__| __|
  *    | |___>  <| |_) | (_) | |  | |_
  *    \____/_/\_\ .__/ \___/|_|   \__|
  *              | |
  *              |_|
  * Copy objects into destination container*/
  void ObjectSmoothing::exportObjects(lib7842::ObjectContainer* destContainer, int minLife, int maxLife)
  {
    int exportCount = 0;
    //for each object in master down to x life
    for(int objectNum = 0; objectNum < destContainer->arrayLength && objectNum < m_masterLength; objectNum++)
    {
      if(m_masterObjects.at(objectNum).lifeCounter <= maxLife && m_masterObjects.at(objectNum).lifeCounter >= minLife)
      {
        destContainer->objectArray.at(objectNum).objSig = m_masterObjects.at(objectNum).objSig;
        destContainer->objectArray.at(objectNum).objX = m_masterObjects.at(objectNum).objX;
        destContainer->objectArray.at(objectNum).objY = m_masterObjects.at(objectNum).objY;
        destContainer->objectArray.at(objectNum).objWidth = m_masterObjects.at(objectNum).objWidth;
        destContainer->objectArray.at(objectNum).objHeight = m_masterObjects.at(objectNum).objHeight;
        destContainer->objectArray.at(objectNum).objArea = m_masterObjects.at(objectNum).objWidth * m_masterObjects.at(objectNum).objHeight;
        destContainer->objectArray.at(objectNum).objCenterX = m_masterObjects.at(objectNum).objCenterX;
        destContainer->objectArray.at(objectNum).objCenterY = m_masterObjects.at(objectNum).objCenterY;
        destContainer->objectArray.at(objectNum).discardObject = false;
        exportCount = objectNum + 1;
      }
      else
      {
        // destContainer->objectArray.at(objectNum).objSig = VISION_OBJECT_ERR_SIG;
        // destContainer->objectArray.at(objectNum).objX = 0;
        // destContainer->objectArray.at(objectNum).objY = 0;
        // destContainer->objectArray.at(objectNum).objWidth = 0;
        // destContainer->objectArray.at(objectNum).objHeight = 0;
        // destContainer->objectArray.at(objectNum).objSize = 0;
        // destContainer->objectArray.at(objectNum).objCenterX = 0;
        // destContainer->objectArray.at(objectNum).objCenterY = 0;
        // destContainer->objectArray.at(objectNum).discardObject = false;
      }
    }
    destContainer->currentCount = exportCount;
  }


}
