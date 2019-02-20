// #include "ShootController.hpp"
//
// ShootController::ShootController(OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel) :
// tracker(itracker), intake(iintake), flywheel(iflywheel)
// shootTask(task, this)
// {}
//
//   void ShootController::clearQueue() {
//     stateQueue.clear();
//   }
//   void ShootController::addQueue(shootStates);
//   void ShootController::setState(shootStates);
//
//   void ShootController::run()
//   {
//
//     while(true)
//     {
//       double filteredSensor = sensorFilter.filter(lineSensor->get_value_calibrated());
//
//       switch(intakeState)
//       {
//
//         case intakeStates::off:
//         {
//           intake->moveVelocity(0);
//           indexer->moveVelocity(0);
//           break;
//         }
//
//         case intakeStates::loading:
//         {
//           //Intake and collect until ball is in sensor
//           //once ball is in sensor index brake
//           //collecting
//           if(filteredSensor < -200)
//           {
//             intakeState = intakeStates::collecting;
//           }
//           else
//           {
//             intake->moveVelocity(200);
//             indexer->moveVelocity(100);
//           }
//           break;
//         }
//
//         case intakeStates::collecting:
//         {
//           //Run intake
//           intake->moveVelocity(200);
//           indexer->moveVelocity(0);
//           break;
//         }
//
//         case intakeStates::shootIndexer:
//         {
//           intake->moveVelocity(0);
//           indexer->moveVelocity(200);
//           intakeState = intakeStates::loading; // Allows mode to be set to off while waiting
//           pros::delay(200);
//           break;
//         }
//
//         case intakeStates::shootBoth:
//         {
//           intake->moveVelocity(200);
//           indexer->moveVelocity(200);
//           //intakeState = intakeStates::loading; // Allows mode to be set to off while waiting
//           pros::delay(200);
//           break;
//         }
//
//         case intakeStates::outIntake:
//         {
//           intake->moveVelocity(-200);
//           indexer->moveVelocity(0);
//           break;
//         }
//
//         case intakeStates::outBoth:
//         {
//           intake->moveVelocity(-200);
//           indexer->moveVelocity(-100);
//           break;
//         }
//
//         case intakeStates::outSlow:
//         {
//           intake->moveVelocity(-40);
//           indexer->moveVelocity(-40);
//           break;
//         }
//
//         case intakeStates::compress:
//         {
//           intake->moveVelocity(80);
//           indexer->moveVelocity(-40);
//           pros::delay(400);
//           intakeState = intakeStates::loading;
//           break;
//         }
//
//       }
//
//       pros::delay(10);
//     }
//
//   }
//
//
//   void ShootController::task(void* input)
//   {
//     ShootController* that = static_cast<ShootController*>(input);
//     that->run();
//   }
