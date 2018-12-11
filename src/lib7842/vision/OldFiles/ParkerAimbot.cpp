// #include "robot-config.h"
//
// /*---------------------------------------------------------------------------*/
// /*                                                                           */
// /*        Description: Competition template for VCS VEX V5                    */
// /*                                                                           */
// /*---------------------------------------------------------------------------*/
// 
// //Creates a competition object that allows access to Competition methods.
// vex::competition    Competition;
// void wait(int timeinms)
// {
//     vex::task::sleep(timeinms);
// }
//
//
// bool debug = false;
//
// int angletoturn;
// int motortoturn;
// int motorspeed;
// int cerror, perror, staystill;
// void hackz()
// {
//     /*
//     Vision.takeSnapshot(BLUEFLAG);
//     vex::task::sleep(25);
//     if(Vision.largestObject.exists)
//     {
//         angletoturn = (320 - Vision.largestObject.centerX);//this will tell us which way and how far to turn
//         motortoturn = (angletoturn * 0.71);//idk what this number will be please adjust
//         motorspeed = (angletoturn * .100);//P loop
//         LFD.rotateFor(motortoturn,vex::rotationUnits::deg, motorspeed, vex::velocityUnits::rpm, false);
//         RFD.rotateFor(motortoturn,vex::rotationUnits::deg, motorspeed, vex::velocityUnits::rpm);
//     }
//     else
//     {
//         controller1.Screen.setCursor(3, 6);
//         controller1.Screen.print("No Blue Flag");
//     }
//     */
//     Vision.takeSnapshot(BLUEFLAG);
//     if(Vision.objectCount > 0)
//        {
//         perror = angletoturn;
//         angletoturn = (175-Vision.largestObject.centerX);
//         wait(10);
//         perror = angletoturn;
//         staystill = (cerror - perror) * .2;// the D part of PID
//         motorspeed = (angletoturn * .2) + staystill;
//         if(Vision.largestObject.centerX > 175)
//         {
//         LFD.spin(vex::directionType::rev, motorspeed, vex::velocityUnits::pct);
//         RFD.spin(vex::directionType::rev, motorspeed, vex::velocityUnits::pct);
//         }
//         else if(Vision.largestObject.centerX < 175)
//         {
//         LFD.spin(vex::directionType::fwd, -motorspeed, vex::velocityUnits::pct);
//         RFD.spin(vex::directionType::fwd, -motorspeed, vex::velocityUnits::pct);
//         }
//         else
//         {
//             LFD.stop(vex::brakeType::coast);
//             RFD.stop(vex::brakeType::coast);
//         }
//     }
//     /*else
//     {
//         controller1.Screen.setCursor(3, 6);
//         controller1.Screen.print("No Red Flag");
//     }*/}
//
// int angletoturn2;
// int motortoturn2;
// int motorspeed2;
//
// void visionaligncaps()
// {
//     Vision.takeSnapshot(RED_);
//     if(Vision.objectCount>0)
//     {
//         angletoturn2 = (320 - Vision.largestObject.centerX);//this will tell us which way and how far to turn
//         motortoturn2 = (angletoturn2 * 0.71);//idk what this number will be please adjust
//         motorspeed2 = (motortoturn2 * .100);//P loop
//         LFD.rotateFor(motortoturn2,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm, false);
//         RFD.rotateFor(motortoturn2,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm);
//     }
//     else
//     {
//         controller1.Screen.setCursor(3, 6);
//         controller1.Screen.print("No Red Cap");
//     }
//     Vision.takeSnapshot(BLUE_);
//     if(Vision.objectCount>0)
//     {
//         angletoturn2 = (320 - Vision.largestObject.centerX);//this will tell us which way and how far to turn
//         motortoturn2 = (angletoturn2 * 0.71);//idk what this number will be please adjust
//         motorspeed2 = (motortoturn2 * .100);//P loop
//         LFD.rotateFor(motortoturn2,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm, false);
//         RFD.rotateFor(motortoturn2,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm);
//     }
//     else
//     {
//         controller1.Screen.setCursor(3, 6);
//         controller1.Screen.print("No Blue Cap");
//     }
// }
// int angletoturn3;
// int motortoturn3;
// int motorspeed3;
//
// void visionalignball()
// {
//     Vision.takeSnapshot(RED_);
//     if(Vision.objectCount>0)
//     {
//         angletoturn3 = (320 - Vision.largestObject.centerX);//this will tell us which way and how far to turn
//         motortoturn3 = (angletoturn3 * 0.71);//idk what this number will be please adjust
//         motorspeed3 = (motortoturn3 * .100);//P loop
//         LFD.rotateFor(motortoturn3,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm, false);
//         RFD.rotateFor(motortoturn3,vex::rotationUnits::deg, motorspeed2, vex::velocityUnits::rpm);
//     }
//         else
//     {
//         controller1.Screen.setCursor(3, 6);
//         controller1.Screen.print("No Ball");
//     }
// }
//
// void visionalign()
// {
//     while(controller1.ButtonDown.pressing() == true)
//     {
//       controller1.Screen.setCursor(3, 0);
//       controller1.Screen.print("Flag:");
//       hackz();//line up to the flags
//       vex::task::sleep(25);//dont hog the cpu
//     }
//     while(controller1.ButtonLeft.pressing() == true)
//     {
//        controller1.Screen.setCursor(3, 0);
//        controller1.Screen.print("Ball:");
//        visionalignball();
//        vex::task::sleep(25);//dont hog the cpu
//     }
//     while(controller1.ButtonRight.pressing() == true)
//     {
//        controller1.Screen.setCursor(3, 0);
//        controller1.Screen.print("Caps:");
//        visionaligncaps();
//        vex::task::sleep(25);//dont hog the cpu
//     }
// }
//
//
//
// void drivelimit(int maxamount)
// {
//     LFD.setMaxTorque(maxamount, vex::percentUnits::pct);
// 	//LBD.setMaxTorque(maxamount, vex::percentUnits::pct);
// 	//RBD.setMaxTorque(maxamount, vex::percentUnits::pct);
// 	RFD.setMaxTorque(maxamount, vex::percentUnits::pct);
// }
//
// //lift
// int counter = 0;
// int position = 0;
// int lift()
// {
// 	leftlifty.setStopping(vex::brakeType::brake);
//     rightlifty.setStopping(vex::brakeType::brake);
// 	leftlifty.setReversed(false);
// 	leftlifty.setVelocity(100, vex::velocityUnits::pct);
// 	leftlifty.setTimeout(3, vex::timeUnits::sec);
//     rightlifty.setStopping(vex::brakeType::brake);
// 	rightlifty.setReversed(true);
// 	rightlifty.setVelocity(100, vex::velocityUnits::pct);
// 	rightlifty.setTimeout(3, vex::timeUnits::sec);
// 	while(1 == 1)
// 	{
//
// 		if(controller1.ButtonR1.pressing() == true)//65
// 		{
//             counter++;
// 		}
// 		else if(controller1.ButtonR2.pressing() == true)
// 		{
// 			counter--;
// 		}
// 		else
// 		{
//         }
//
//         if(counter == 0)
//         {
//             position = 0;
//         }
//         else if(counter == 1)
//         {
//             position = 65;
//         }
//         else if(counter == 2)
//         {
//             position = 100;
//         }
//         else if(counter == 3)
//         {
//             position = 490;
//         }
//         else if(counter == 4)
//         {
//             position = 700;
//         }
//         else
//         {
//
//         }
//
//         if(controller1.ButtonLeft.pressing() == true)
//         {
//             position = position - 50;
//         }
//         else if(controller1.ButtonRight.pressing() == true)
//         {
//             position = position + 50;
//         }
//
//         rightlifty.rotateTo(position, vex::rotationUnits::deg, false);
//         leftlifty.rotateTo(position, vex::rotationUnits::deg, true);
//
//
// 		vex::task::sleep(20);
// 	}
// 	return(0);
// }
//
// //flippy
// void flippy()
// {
// 	flipper.setStopping(vex::brakeType::hold);
// 	flipper.setVelocity(100, vex::velocityUnits::pct);
// 	if(controller1.ButtonUp.pressing() == true)
// 	{
// 		if(counter == 0 && flipper.rotation(vex::rotationUnits::deg)!=0)
// 		{
// 		    leftlifty.rotateFor(350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(350, vex::rotationUnits::deg, true);
// 			flipper.rotateTo(0, vex::rotationUnits::deg, true);
// 		    leftlifty.rotateFor(-350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(-350, vex::rotationUnits::deg, true);
// 		}
// 		else if(counter == 1 && flipper.rotation(vex::rotationUnits::deg)!=0)
// 		{
// 		    leftlifty.rotateFor(250, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(250, vex::rotationUnits::deg, true);
// 			flipper.rotateTo(0, vex::rotationUnits::deg, true);
// 		    leftlifty.rotateFor(-250, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(-250, vex::rotationUnits::deg, true);
// 		}
// 		else
// 		{
// 			flipper.rotateTo(0, vex::rotationUnits::deg, true);
// 		}
//
//
// 	}
// 	else if(controller1.ButtonDown.pressing() == true)
// 	{
// 		if(counter == 0 && flipper.rotation(vex::rotationUnits::deg)!=180)
// 		{
// 		    leftlifty.rotateFor(350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(350, vex::rotationUnits::deg, true);
// 			flipper.rotateTo(180, vex::rotationUnits::deg, true);
// 		    leftlifty.rotateFor(-350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(-350, vex::rotationUnits::deg, true);
// 		}
// 		else if(counter == 1 && flipper.rotation(vex::rotationUnits::deg)!=180)
// 		{
// 		    leftlifty.rotateFor(350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(350, vex::rotationUnits::deg, true);
// 			flipper.rotateTo(180, vex::rotationUnits::deg, true);
// 		    leftlifty.rotateFor(-350, vex::rotationUnits::deg, false);
//             rightlifty.rotateFor(-350, vex::rotationUnits::deg, true);
// 		}
// 		else
// 		{
// 			flipper.rotateTo(180, vex::rotationUnits::deg, true);
// 		}
// 	}
// }
//
// int currentvel = 0;
// int change = 1;
// int speed = 0;
// int targetspeed = 0;
//
// int flywheelslew()
// {
//     while(1 == 1)
//     {
//
//     }
//     return(0);
// }
//
// //flywheel
// int moved = 0;
// int throwstuff()
// {
// 	LFlywheel.setReversed(false);
// 	LFlywheel.setStopping(vex::brakeType::coast);
// 	RFlywheel.setReversed(false);
// 	RFlywheel.setStopping(vex::brakeType::coast);
// 	while(1 == 1)
// 	{
//        currentvel = abs(RFlywheel.velocity(vex::velocityUnits::pct));
//
//        if(targetspeed == 0)
//        {
//            LFlywheel.stop(vex::brakeType::coast);
//            RFlywheel.stop(vex::brakeType::coast);
//            speed = 0;
//        }
//        if(currentvel < targetspeed)
//        {
//            speed = speed + change;
//            vex::task::sleep(50);
//        }
//        else if(currentvel > targetspeed)
//        {
//            speed = speed - change;
//            vex::task::sleep(50);
//        }
//        else if(currentvel == targetspeed)
//        {
//            speed = speed;
//            vex::task::sleep(50);
//        }
//        else
//        {
//            speed = speed;
//            vex::task::sleep(50);
//        }
//
//         if(controller1.ButtonB.pressing() == true)
//         {
//             //I'll do doubleshot later
//         }
//         else if(controller1.ButtonY.pressing() == true)
//         {
//             targetspeed = 0;
//             vex::task::sleep(250);
//         }
//         else if(controller1.ButtonX.pressing() == true)
//         {
//             targetspeed = 100;
//             vex::task::sleep(250);
//         }
//         else if(controller1.ButtonA.pressing() == true)
//         {
//             targetspeed = 50;
//             vex::task::sleep(250);
//         }
//         else
//         {
//
//         }
//
//         LFlywheel.setVelocity(-speed, vex::velocityUnits::pct);
//         RFlywheel.setVelocity(-speed, vex::velocityUnits::pct);
//         LFlywheel.spin(vex::directionType::fwd);
//         RFlywheel.spin(vex::directionType::fwd);
//
// 	}
// 	return(0);
// }
//
//
// //intake
// void intakecontrol()
// {
// 	intake.setVelocity(100, vex::velocityUnits::pct);
// 	if(controller1.ButtonL1.pressing() == true)
// 	{
// 		intake.setVelocity(100, vex::velocityUnits::pct);
// 		intake.spin(vex::directionType::fwd);
// 	}
// 	else if(controller1.ButtonL2.pressing() == true)
// 	{
// 		intake.setVelocity(50, vex::velocityUnits::pct);
// 		intake.spin(vex::directionType::rev);
// 		if(intake.velocity(vex::velocityUnits::rpm) > 0)
// 		{
// 			intake.stop(vex::brakeType::coast);
// 		}
// 	}
// 	else
// 	{
// 		//probably nothing, not really sure
// 	}
// }
//
//
// //drive
//
// int leftvalue = controller1.Axis3.value() + controller1.Axis1.value();
// int rightvalue = -controller1.Axis3.value() - controller1.Axis1.value();
//
//
// void drivecontrol()
// {
// 	leftvalue = controller1.Axis3.value() + controller1.Axis1.value();
// 	rightvalue = -controller1.Axis3.value() + controller1.Axis1.value();
//
// 	LFD.setVelocity(leftvalue,vex:: velocityUnits::pct);
// //	LBD.setVelocity(leftvalue,vex:: velocityUnits::pct);
// 	//RBD.setVelocity(rightvalue,vex:: velocityUnits::pct);
// 	RFD.setVelocity(rightvalue,vex:: velocityUnits::pct);
// 	LFD.spin(vex::directionType::fwd);
// //	LBD.spin(vex::directionType::fwd);
// 	RFD.spin(vex::directionType::fwd);
// 	//RBD.spin(vex::directionType::fwd);
//
// 	if(controller1.Axis3.value() == 0 && controller1.Axis1.value() == 0)
// 	{
// 		vex::task::sleep(100);
// 		LFD.stop(vex::brakeType::coast);
// 		//LBD.stop(vex::brakeType::coast);
// 		RFD.stop(vex::brakeType::coast);
// 		//RBD.stop(vex::brakeType::coast);
// 	}
//
//
//
// }
//
// //display code
// int xPos;
// int yPos;
//
// typedef struct _button {
// 	int      xpos;
// 	int      ypos;
// 	int      width;
// 	int      height;
// 	bool     state;
// 	vex::color color;
// } button;
//
// button buttons[] = {
// 	{   40,  70, 55, 55,  false, 0xFFFF0000 },
// 	{   40, 140, 55, 55,  false, 0xFFFF0000 },
// 	{  385,  70, 55, 55,  false, 0xFF0000FF },
// 	{  385, 140, 55, 55,  false, 0xFF0000FF },
//     {  215, 105, 55, 55,  false, 0xFFCCCCCC },
// };
//
//
//
// int displaystuff()
// {
// 	Brain.Screen.clearScreen(0xFF606055);
// 	for(int i = 0; i < 5; i++)
// 	{
// 		Brain.Screen.setFillColor(buttons[i].color);
// 		Brain.Screen.setPenColor(buttons[i].color);
// 		Brain.Screen.drawRectangle(buttons[i].xpos,buttons[i].ypos,buttons[i].width,buttons[i].height);
// 		//vex::task::sleep(500);
// 	}
//
// 	Brain.Screen.setPenColor(0xFF0000FF);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(65, 40, 95, 40);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFFFF0000);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(95, 40, 95, 10);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFF505045);
// 	Brain.Screen.setFillColor(0xFF505045);
// 	Brain.Screen.drawCircle(95, 40, 2, 0xFF707065);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFF0000FF);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(230, 20, 245, 40);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFFFF0000);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(245, 40, 260, 20);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFF505045);
// 	Brain.Screen.setFillColor(0xFF505045);
// 	Brain.Screen.drawCircle(245, 40, 2, 0xFF707065);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFF0000FF);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(385, 40, 385, 10);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFFFF0000);
// 	Brain.Screen.setPenWidth(5);
// 	Brain.Screen.drawLine(385, 40, 415, 40);
// 	//vex::task::sleep(100);
// 	Brain.Screen.setPenColor(0xFF505045);
// 	Brain.Screen.setFillColor(0xFF505045);
// 	Brain.Screen.drawCircle(385, 40, 2, 0xFF707065);
//
// 	while(1  == 1)
// 	{
// 		xPos = Brain.Screen.xPosition();
// 		yPos = Brain.Screen.yPosition();
// 		if(Brain.Screen.pressing() == true)
// 		{
// 			//xpos = Brain.Screen.xPosition();
// 			//ypos = Brain.Screen.yPosition();
//
//
// 			for(int i = 0; i < 5; i++)
// 			{
//
// 				if((xPos > buttons[i].xpos && xPos < buttons[i].xpos + buttons[i].width) && (yPos > buttons[i].ypos && yPos < buttons[i].ypos + buttons[i].height))
// 				{
// 					//Brain.Screen.setPenColor(vex::color::white);
// 					//Brain.Screen.setFillColor(vex::color::black);
// 					//Brain.Screen.printAt(180, 88, "it worked!");
// 					if((xPos > buttons[0].xpos && xPos < buttons[0].xpos + buttons[0].width) && (yPos > buttons[0].ypos && yPos < buttons[0].ypos + buttons[0].height))
// 					{
// 						Brain.Screen.clearLine(3, 0xFF606055);
// 						Brain.Screen.setPenColor(vex::color::white);
// 						Brain.Screen.setFillColor(0xFF606055);
// 						Brain.Screen.printAt(155, 60, "Red Front Selected");
// 						buttons[0].state = true;
// 						buttons[1].state = false;
// 						buttons[2].state = false;
// 						buttons[3].state = false;
//                         buttons[4].state = false;
// 					}
//                     if((xPos > buttons[1].xpos && xPos < buttons[1].xpos + buttons[1].width) && (yPos > buttons[1].ypos && yPos < buttons[1].ypos + buttons[1].height))
// 					{
// 						Brain.Screen.clearLine(3, 0xFF606055);
// 						Brain.Screen.setPenColor(vex::color::white);
// 						Brain.Screen.setFillColor(0xFF606055);
// 						Brain.Screen.printAt(155, 60, "Red Back Selected");
// 						buttons[0].state = false;
// 						buttons[1].state = true;
// 						buttons[2].state = false;
// 						buttons[3].state = false;
//                         buttons[4].state = false;
// 					}
// 					if((xPos > buttons[2].xpos && xPos < buttons[2].xpos + buttons[2].width) && (yPos > buttons[2].ypos && yPos < buttons[2].ypos + buttons[2].height))
// 					{
// 						Brain.Screen.clearLine(3, 0xFF606055);
// 						Brain.Screen.setPenColor(vex::color::white);
// 						Brain.Screen.setFillColor(0xFF606055);
// 						Brain.Screen.printAt(155, 60, "Blue Front Selected");
// 						buttons[0].state = false;
// 						buttons[1].state = false;
// 						buttons[2].state = true;
// 						buttons[3].state = false;
//                         buttons[4].state = false;
// 					}
// 					if((xPos > buttons[3].xpos && xPos < buttons[3].xpos + buttons[3].width) && (yPos > buttons[3].ypos && yPos < buttons[3].ypos + buttons[3].height))
// 					{
// 						Brain.Screen.clearLine(3, 0xFF606055);
// 						Brain.Screen.setPenColor(vex::color::white);
// 						Brain.Screen.setFillColor(0xFF606055);
// 						Brain.Screen.printAt(155, 60, "Blue Back Selected");
// 						buttons[0].state = false;
// 						buttons[1].state = false;
// 						buttons[2].state = false;
// 						buttons[3].state = true;
//                         buttons[4].state = false;
// 					}
// 					if((xPos > buttons[4].xpos && xPos < buttons[4].xpos + buttons[4].width) && (yPos > buttons[4].ypos && yPos < buttons[4].ypos + buttons[4].height))
// 					{
// 						Brain.Screen.clearLine(3, 0xFF606055);
// 						Brain.Screen.setPenColor(vex::color::white);
// 						Brain.Screen.setFillColor(0xFF606055);
// 						Brain.Screen.printAt(155, 60, "Auton Skills Selected");
// 						buttons[0].state = false;
// 						buttons[1].state = false;
// 						buttons[2].state = false;
// 						buttons[3].state = false;
//                         buttons[4].state = true;
// 					}
// 				}
// 				else if((xPos < buttons[i].xpos || xPos > buttons[i].xpos + buttons[i].width) || (yPos < buttons[i].ypos || yPos > buttons[i].ypos + buttons[i].height))
// 				{
// 					//Brain.Screen.setPenColor(vex::color::white);
// 					//Brain.Screen.setFillColor(vex::color::black);
// 					//Brain.Screen.printAt(1, 20, "current pos x: %04d, y: %04d", xPos, yPos);
// 				}
// 				vex::task::sleep(10);
// 			}
// 		}
//
// 		//Brain.Screen.printAt(1, 20, "current pos x: %04d, y: %04d", xpos, ypos);
// /*
//             if(buttons[0].state == true)
//             {
//               Brain.Screen.drawRectangle(160, 110, 50, 50);
//
//             }
// */
//
//         controller1.Screen.setCursor(3, 0);
//         controller1.Screen.print("Speed:%d", (currentvel * 200));
//
//         if((currentvel*200) > 165)
//         {
//             controller1.Screen.setCursor(3, 9);
//             controller1.Screen.print("Ready to Fire");
//         }
//         else
//         {
//             controller1.Screen.setCursor(3, 9);
//             controller1.Screen.print("Wait");
//         }
//
//
// 		vex::task::sleep(50);
// 	}
// 	return(0);
// }
//
// void drivestraight(int degrees)//wheel degrees
// {
//     LFD.setTimeout(3, vex::timeUnits::sec);
//     RFD.setTimeout(3, vex::timeUnits::sec);
//     //RBD.setTimeout(3, vex::timeUnits::sec);
// 	LFD.rotateTo(degrees, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
// 	RFD.rotateTo(degrees, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
// 	//RBD.rotateTo(degrees, vex::rotationUnits::deg);
//     vex::task::sleep(100);
// }
//
// void autoturn(int degrees)//not turning degrees, wheel degrees
// {
//     LFD.setTimeout(3, vex::timeUnits::sec);
//     RFD.setTimeout(3, vex::timeUnits::sec);
//     //RBD.setTimeout(3, vex::timeUnits::sec);
// 	LFD.rotateTo(degrees, vex::rotationUnits::deg, false);//not turning degrees, wheel degrees
// 	RFD.rotateTo(-degrees, vex::rotationUnits::deg, true);//not turning degrees, wheel degrees
// 	//RBD.rotateTo(-degrees, vex::rotationUnits::deg);//not turning degrees, wheel degrees
// vex::task::sleep(100);
// }
//
// void autolift(int degrees)
// {
//     leftlifty.setTimeout(3, vex::timeUnits::sec);
//     rightlifty.setTimeout(3, vex::timeUnits::sec);
//     leftlifty.rotateTo(degrees, vex::rotationUnits::deg, false);
//     rightlifty.rotateTo(degrees, vex::rotationUnits::deg, true);
//     vex::task::sleep(100);
//
// }
//
// void autoflip()
// {
//     leftlifty.setTimeout(3, vex::timeUnits::sec);
//     rightlifty.setTimeout(3, vex::timeUnits::sec);
//     flipper.setTimeout(3, vex::timeUnits::sec);
// 	if(leftlifty.rotation(vex::rotationUnits::deg) < 100)
// 	{
// 		leftlifty.rotateFor(350, vex::rotationUnits::deg, false);
//         rightlifty.rotateFor(350, vex::rotationUnits::deg, true);
// 		flipper.rotateTo(180, vex::rotationUnits::deg, true);
// 		leftlifty.rotateFor(-350, vex::rotationUnits::deg, false);
//         rightlifty.rotateFor(-350, vex::rotationUnits::deg, true);
// 	}
//     else
//     {
//         flipper.rotateTo(0, vex::rotationUnits::deg);
//     }
//     vex::task::sleep(100);
// }
//
// void deploy()
// {
//     autolift(50);//wow its short, like ur mom @ chris
//
// }
//
// void firecatapult()
// {
//
// }
// bool align = false;
// int visiontesting()
// {
//     while(1 == 1)
//     {
//         while(align == true)
//         {
//                 Vision.takeSnapshot(1);
//                 if(Vision.largestObject.exists && Vision.largestObject.width > 50)
//                 {
//                     targetspeed = 100;
//                 }
//                 else if(Vision.largestObject.exists && Vision.largestObject.width < 50)
//                 {
//                     targetspeed = 50;
//                 }
//                 else
//                 {
//                     Vision.takeSnapshot(2);
//                     if(Vision.largestObject.exists && Vision.largestObject.width > 50)
//                     {
//                         targetspeed = 100;
//                     }
//                     else if(Vision.largestObject.exists && Vision.largestObject.width < 50)
//                     {
//                         targetspeed = 50;
//                     }
//                     else
//                     {
//                         targetspeed = 0;
//                     }
//                 }
//
//         }
//     }
//     return(0);
// }
//
// int autonselected = 1;
// /*---------------------------------------------------------------------------*/
// /*                          Pre-Autonomous Functions                         */
// /*                                                                           */
// /*  You may want to perform some actions before the competition starts.      */
// /*  Do them in the following function.  You must return from this function   */
// /*  or the autonomous and usercontrol tasks will not be started.  This       */
// /*  function is only called once after the cortex has been powered on and    */
// /*  not every time that the robot is disabled.                               */
// /*---------------------------------------------------------------------------*/
// void pre_auton()
// {
// 	if(buttons[0].state == true)
// 	{
// 		autonselected = 1;
// 	}
// 	else if(buttons[1].state == true)
// 	{
// 		autonselected = 2;
// 	}
// 	else if(buttons[2].state == true)
// 	{
// 		autonselected = 3;
// 	}
// 	else if(buttons[3].state == true)
// 	{
// 		autonselected = 4;
// 	}
// }
//
// /*---------------------------------------------------------------------------*/
// /*                                                                           */
// /*                              Autonomous Task                              */
// /*                                                                           */
// /*  This task is used to control your robot during the autonomous phase of   */
// /*  a VEX Competition.                                                       */
// /*                                                                           */
// /*  You must modify the code to add your own robot specific commands here.   */
// /*---------------------------------------------------------------------------*/
//
// void autonomous() {
//
// 	switch(autonselected)
// 	{
// 	case 0://none selected
//         //nothing
// 		break;
//
// 	case 1: //Front red 1 (closer to the flags)
//         firecatapult();//score the top flag
//         autoturn(-180);//turn to face the cap on the ground with the ball intake
//         intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);//have the intake always run
//         drivestraight(100);//go straight to intake the ball under the cap
//         vex::task::sleep(200);//waituntil the the ball is intook
//         drivestraight(-20);//back off the cap
//         intake.spin(vex::directionType::fwd, -100, vex::velocityUnits::pct);//have the intake always run
//         autoturn(-60);//turn to face the upside down cap
//         drivestraight(100);//go to flip over the cap
//         autoturn(000);//idk turn to face the spot where we will drive to align with the flags
//         intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);//have the intake always run
//         drivestraight(000);//idk like do some stuff
//         firecatapult();//hit the middle flag;
//         autoturn(000);//idk what this will be, hit the low flags on the bottom
//         drivestraight(100);//go to the flags so they will be shootable atable
//         firecatapult();//shoot the things?
//         drivestraight(100);//go touch the middle bottom flag.
//
//         break;
//
// 	case 2://Back red from the oposite side of the flags (farther from the flags)
//
// 		break;
//
// 	case 3://Front blue from the oposite side of the flags (closer to the flags)
//         drivestraight(36);//drive straight a little
//         firecatapult();//fire the catapult
//         deploy();//deploy
//         autolift(0);//go down
//         autoturn(-180);//turn 90deg to the left!
//         drivestraight(450);//several wheel revolutions
//         intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);//have the intake always run
//         autoturn(-60);// aim at the blue cap
//         drivestraight(360);//reach the blue cap
//         autoflip();//flip the blue cap
//         drivestraight(-360);//back off the cap
//         autoturn(60);//turn the other cap rq
//         drivestraight(360);//go to the other cap
//         autoturn(60);//get the ball out from under the cap
//         autoturn(-60);//go back to being straight
//         drivestraight(-360);//go back to starting position
//         autoturn(-180);//turn to look at the flags
//         drivestraight(720);//get to a point where we can flip the middle flag
//         firecatapult();//what it says
//         drivestraight(180);//flip the bottom flag
//         break;
//
// 	case 4://Back blue from the oposite side of the flags (farther from the flags)
// 		 drivestraight(60);//deploy the intake
//          vex::task::sleep(100);//wait for the intake to be out of the way
//          firecatapult();//shoot @ the high flag
//          deploy();//deploy
//          autolift(0);//go all the way down again
//          autoturn(-180);//turn 90deg to face the cap
//          drivestraight(720);//drop at tilted caps
//          autoturn(60);//get the ball out & face the other cap
//          drivestraight(360);//drive into the other cap
//          autoflip();//do the thing
//          drivestraight(-360);//drive off the cap
// 		break;
//     }
// }
//
// /*---------------------------------------------------------------------------*/
// /*                                                                           */
// /*                              User Control Task                            */
// /*                                                                           */
// /*  This task is used to control your robot during the user control phase of */
// /*  a VEX Competition.                                                       */
// /*                                                                           */
// /*  You must modify the code to add your own robot specific commands here.   */
// /*---------------------------------------------------------------------------*/
//
// void usercontrol( void ) {//CHANGE THE BUTTONS FOR VISIONALIGN BEFORE UNCOMMENTING EVERYTHING!!!!
//     Vision.setWifiMode(vex::vision::wifiMode::off);
//     ////vex::task raise(lift);
// 	////vex::task cat(throwstuff);
// 	////vex::task disp(displaystuff);
//     //vex::task see(visiontesting);
// 	while (1) {
//         visionalign();
//         Vision.takeSnapshot(BLUEFLAG);
//         controller1.Screen.setCursor(3,7);
//         controller1.Screen.print("%d", Vision.objectCount);
// 		// This is the main execution loop for the user control program.
// 		// Each time through the loop your program should update motor + servo
// 		// values based on feedback from the joysticks.
//
// 		//throwstuff();
// 		////flippy();
// 		drivecontrol();
// 		////intakecontrol();
// 		vex::task::sleep(10);//Sleep the task for a short amount of time to prevent wasted resources.
// 	}
// }
//
// //
// // Main will set up the competition functions and callbacks.
// //
// int main() {
// 	// vex::task t(lift);
//
// 	//Run the pre-autonomous function.
// 	//pre_auton();
//     //usercontrol();
// 	//Set up callbacks for autonomous and driver control periods.
// 	Competition.autonomous( autonomous );
// 	Competition.drivercontrol( usercontrol );
// 	//controller1.Screen.clearScreen();
// 	//Prevent main from exiting with an infinite loop.
// 	/*while(true)
// 	{
//
//     if(buttons[0].state == true)
// 	{
// 		autonselected = 1;
// 	}
// 	else if(buttons[1].state == true)
// 	{
// 		autonselected = 2;
// 	}
// 	else if(buttons[2].state == true)
// 	{
// 		autonselected = 3;
// 	}
// 	else if(buttons[3].state == true)
// 	{
// 		autonselected = 4;
// 	}
//     else
//     {
//         autonselected = 0;
//     }
//
// 		//throwstuff();
// 		flippy();
// 		drivecontrol();
// 		intakecontrol();
// 		vex::task::sleep(10);//Sleep the task for a short amount of time to prevent wasted resources.
//
//
//         //remove this before the comp tomorrow
//         if(controller1.ButtonLeft.pressing() == true)
//         {
//          	switch(autonselected)
// 	{
// 	case 0://none selected
//         //nothing
//         firecatapult();
// 		break;
//
// 	case 1: //Front red from the oposite side of the flags (closer to the flags)
//         drivestraight(35);//drive straight a little
//         firecatapult();//fire the catapult
//         deploy();//deploy
//         autolift(0);//go down
//         autoturn(180);//turn 90deg to the right!
//         drivestraight(450);//several wheel revolutions
//         intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);//have the intake always run
//         autoturn(60);// aim at the blue cap
//         drivestraight(360);//reach the blue cap
//         autoflip();//flip the blue cap
//         drivestraight(-360);//back off the cap
//         autoturn(-60);//turn the other cap rq
//         drivestraight(1);//go to the other cap
//         autoturn(-60);//get the ball out from under the cap
//         autoturn(60);//go back to being straight
//         drivestraight(-450);//go back to starting position
//         autoturn(180);//turn to look at the flags
//         drivestraight(2);//get to a point where we can flip the middle flag
//         firecatapult();//what it says
//         drivestraight(180);//flip the bottom flag
// 		break;
//
// 	case 2://Back red from the oposite side of the flags (farther from the flags)
//          drivestraight(180);//deploy the intake
//          vex::task::sleep(100);//wait for the intake to be out of the way
//          firecatapult();//shoot @ the high flag
//          deploy();//deploy
//          autolift(0);//go all the way down again
//          autoturn(-180);//turn 90deg to face the cap
//          drivestraight(720);//drop at tilted caps
//          autoturn(60);//get the ball out & face the other cap
//          drivestraight(360);//drive into the other cap
//          autoflip();//do the thing
//          drivestraight(-360);//drive off the cap
// 		break;
//
// 	case 3://Front blue from the oposite side of the flags (closer to the flags)
//         drivestraight(36);//drive straight a little
//         firecatapult();//fire the catapult
//         deploy();//deploy
//         autolift(0);//go down
//         autoturn(-180);//turn 90deg to the left!
//         drivestraight(450);//several wheel revolutions
//         intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);//have the intake always run
//         autoturn(-60);// aim at the blue cap
//         drivestraight(360);//reach the blue cap
//         autoflip();//flip the blue cap
//         drivestraight(-360);//back off the cap
//         autoturn(60);//turn the other cap rq
//         drivestraight(360);//go to the other cap
//         autoturn(60);//get the ball out from under the cap
//         autoturn(-60);//go back to being straight
//         drivestraight(-360);//go back to starting position
//         autoturn(-180);//turn to look at the flags
//         drivestraight(720);//get to a point where we can flip the middle flag
//         firecatapult();//what it says
//         drivestraight(180);//flip the bottom flag
//         break;
//
// 	case 4://Back blue from the oposite side of the flags (farther from the flags)
// 		 drivestraight(60);//deploy the intake
//          vex::task::sleep(100);//wait for the intake to be out of the way
//          firecatapult();//shoot @ the high flag
//          deploy();//deploy
//          autolift(0);//go all the way down again
//          autoturn(-180);//turn 90deg to face the cap
//          drivestraight(720);//drop at tilted caps
//          autoturn(60);//get the ball out & face the other cap
//          drivestraight(360);//drive into the other cap
//          autoflip();//do the thing
//          drivestraight(-360);//drive off the cap
// 		break;
//     }
//         }
//
//
//         //firecatapult();
//
//     */
//     }
//
