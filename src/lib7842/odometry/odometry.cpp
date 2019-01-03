#include "odometry.hpp"

namespace lib7842
{

  Odometry::Odometry(
    okapi::ADIEncoder *lEncoder,
    okapi::ADIEncoder *rEncoder,
    okapi::ADIEncoder *mEncoder,
    double distanceLeft, double distanceRight, double distanceMiddle,
    double mainWheelDiam, double middleWheelDiam
  ):
    m_lEncoder(lEncoder),
    m_rEncoder(rEncoder),
    m_mEncoder(mEncoder),

    m_distanceLeft(distanceLeft),
    m_distanceRight(distanceRight),
    m_distanceMiddle(distanceMiddle),

    m_straight(mainWheelDiam * PI),
    m_middle(middleWheelDiam * PI)
    {

    };


    void Odometry::debug()
    {
      printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
      -xPos,
      -yPos,
      toDEG(-aPos));
    }

    void Odometry::step() {
      double newL = m_lEncoder->get();
      double newR = m_rEncoder->get();
      double newC = m_mEncoder->get();

      double dL = (m_lLastVal - newL) / m_straight;
      double dR = (m_rLastVal - newR) / m_straight;
      double dC = (m_mLastVal - newC) / m_middle;

      m_lLastVal = newL;
      m_rLastVal = newR;
      m_mLastVal = newC;

      double newA = aPos + ((dL - dR) / (m_distanceLeft + m_distanceRight));
      double dA = newA - aPos;

      // std::cout << "dA: " << dA << std::endl;

      double localOffX, localOffY;

      if(dA != 0) {
        localOffX = 2 * sin(dA / 2) * ((dC / dA) + m_distanceMiddle);
        localOffY = 2 * sin(dA / 2) * ((dR / dA) + m_distanceRight);
      } else {
        localOffX = dC;
        localOffY = dR;
      }

      double avgA = aPos + (dA / 2);

      double polarR = sqrt((localOffX * localOffX) + (localOffY * localOffY));
      double polarA = atan2(localOffY, localOffX) - avgA;

      double dX = cos(polarA) * polarR;
      double dY = sin(polarA) * polarR;

      xPos += dX;
      yPos += dY;
      aPos = newA;
    }

    void Odometry::setPos(double x, double y, double a) {
      xPos = x;
      yPos = y;
      aPos = a;
    }

    void Odometry::resetSensors()
    {
      m_lEncoder->reset();
      m_rEncoder->reset();
      m_mEncoder->reset();
    }

  }
