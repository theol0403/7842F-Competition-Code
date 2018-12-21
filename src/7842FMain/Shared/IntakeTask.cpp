// #include "IntakeTask.hpp"
//
//
// enum class intakeModes
// {
//   loading,
//   collecting,
//   off,
//   shoot
// };
//
// bool runIntake;
//
// void intakeControlTask(void*)
// {
//
//   pros::delay(500);
//   while(s_intakeSensor.get_value_calibrated() > -300)
//   {
//     m_Indexer.moveVelocity(100);
//   }
//   m_Indexer.moveVelocity(0);
//
//   intakeModes intakeMode = intakeModes::loading;
//
//   while(true)
//   {
//     //if runIntake is new mode is loading
//     //if runIntake is off mode is off
//
//     switch(intakeMode)
//     {
//
//       case intakeModes::loading:
//       {
//         //Intake and collect until ball is in sensor
//         //once ball is in sensor index brake
//         //collecting
//         break;
//       }
//
//       case intakeModes::collecting:
//       {
//         //Run intake
//         break;
//       }
//
//     }
//
//     pros::delay(20);
//   }
//
// }
