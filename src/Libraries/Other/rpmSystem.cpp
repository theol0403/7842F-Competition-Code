#include "main.h"
#include "rpmSystem.hpp"




namespace lib7842
{


rpmMeasure::rpmMeasure(int topEncoderPort, bool reverseEncoder, double ticksPerRev, double flywheelEncoderRatio)
: m_timer(), m_encoder(topEncoderPort, topEncoderPort+1, reverseEncoder)
{
	m_lastTime = m_timer.elapsed();
	m_lastEncoder = m_encoder.get_value();

	m_ticksPerRev = ticksPerRev;
  m_flywheelEncoderRatio = flywheelEncoderRatio;

}




int rpmMeasure::calculate()
{
	//Calculate the amount of ms since the last time this function was run
	deviceName.timeInterval = nPgmTime - deviceName.lastTime;
	//Calculate amount of ticks since the function was last run
	deviceName.encoderInterval = SensorValue[deviceName.sensorNum] - deviceName.lastEncoder;

	//If it was ran very recently just return the last RPM
	if(deviceName.timeInterval < deviceName.minRefresh) return deviceName.RPM;

	//If either interval was 0, or sensor was 0, return 0
	if(deviceName.timeInterval == 0 || deviceName.encoderInterval == 0)
	{
		deviceName.RPM = 0;
		return 0;
	}

	//Calculate the RPM based off time and tick interval
	deviceName.RPM = (60000 / deviceName.timeInterval) * (deviceName.encoderInterval/deviceName.encoderTicks) * deviceName.flywheelRatio;


	//Timestamp the last time this function was run, and the encoder position
	deviceName.lastTime = nPgmTime;
	deviceName.lastEncoder = SensorValue[deviceName.sensorNum];

	//Return the RPM to whatever is calling this function
	return deviceName.RPM;

}
