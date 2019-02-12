/**
* Trigger Functions
*/
triggerFunc_t makeTrigger(qPoint point, QLength distanceThresh) {
  return [=](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh; };
}

triggerFunc_t makeTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) {
  return [=](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh && that->computeAngleToPoint(point).abs() < angleThresh; };
}

triggerFunc_t makeTrigger(qPoint point, QAngle angleThresh) {
  return [=](OdomController* that){ return that->computeAngleToPoint(point).abs() < angleThresh; };
}

triggerFunc_t makeTrigger(QAngle angle, QAngle angleThresh) {
  return [=](OdomController* that){ return that->tracker->getTheta() > angle - angleThresh && that->tracker->getTheta() < angle + angleThresh; };
}
