/*
#include "main.h"

int   screen_origin_x = 150;
int   screen_origin_y = 20;
int   screen_width    = 316;
int   screen_height   = 212;
float PI = 3.14159265;

int red_blue_toggle = 0;
int flag_counter = 0;
int red_flags_x[10];
int red_flags_y[10];
int blue_flags_x[10];
int blue_flags_y[10];
int middle_flags_x[10];
int middle_flags_y[10];
int temp_flags_x[10];
int temp_flags_y[10];
int all_flags_y[10][10];
int all_flags_x[10][10];
int blue_flag_num;
int middle_flag_num;
int temp_flag_num;
int red_flag_num;
int all_flag_num;

float cam_height = 8.75;
float distance = 0;
float hyp = 0;
float angle = 0;

int temp = 0;
int clear_counter =0;

float TAN(float in){
  return in * PI / 180;
}





//============================================VISION PLOTTING====================================================
void drawObject( pros::vision_object &obj ) {
  int labelOffset = 0;

  Brain.Screen.drawRectangle( screen_origin_x + obj.left_coord-(obj.width/2), screen_origin_y + obj.top_coord-(obj.height/2), obj.width, obj.height );
  Brain.Screen.setFont( mono12 );

  if(red_blue_toggle){
    blue_flags_x[flag_counter] = obj.left_coord;
    blue_flags_y[flag_counter] = obj.top_coord;
  }else{
    red_flags_x[flag_counter] = obj.left_coord;
    red_flags_y[flag_counter] = obj.top_coord;
  }
}

void drawObjects( c::vision_object_s_t &v ) {
  flag_counter = 0;
  for(int i=0;i<v.objectCount;i++){
    drawObject( v.objects[i] );
    flag_counter++;
  }
  if(red_blue_toggle){
    blue_flag_num = v.objectCount;
  }else{
    red_flag_num = v.objectCount;
  }
}
//===============================================================================================================



void clear_lists(){
  for(int i = 0; i < 9; i++){
    red_flags_x[i] = 0;
    red_flags_y[i] = 0;
    blue_flags_x[i] = 0;
    blue_flags_y[i] = 0;
    middle_flags_x[i] = 0;
    middle_flags_y[i] = 0;
  }
}





void assemble_flags(){
  //clear all_flags
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 3; j++){
      all_flags_x[i][j] = 0;
      all_flags_y[i][j] = 0;
    }
  }
  //insert COLOR_BLUE flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][1] = blue_flags_x[i];
      all_flags_y[i][1] = blue_flags_y[i];
    }
  }
  //insert COLOR_RED flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][2] = red_flags_x[i];
      all_flags_y[i][2] = red_flags_y[i];
    }
  }
  //insert middle flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][3] = middle_flags_x[i];
      all_flags_y[i][3] = middle_flags_y[i];
    }
  }
}






//=========================================FLAG LINE DETECTION===================================================

void vertial_sets(){

}





void group_flags(){
  bool found_pair;
  middle_flag_num = 0;
  //start
  for(int i = 0; i < blue_flag_num; i++){ //for all COLOR_BLUE flags
    found_pair = 1;
    for(int j = 0; j < red_flag_num; j++){
      if(blue_flags_y[i] - 15 < red_flags_y[j] && red_flags_y[j] < (blue_flags_y[i] + 15)){
        if(blue_flags_x[i] < red_flags_x[j] && red_flags_x[j] < blue_flags_x[i] + 40){
          Brain.Screen.setCursor( 3, 2 );
          Brain.Screen.setPenColor( COLOR_GREEN );
          Brain.Screen.drawRectangle( screen_origin_x-15 + ((blue_flags_x[i]+red_flags_x[j])/2), screen_origin_y-15 + ((blue_flags_y[i]+red_flags_y[j])/2), 30, 30);
          Brain.Screen.setPenColor( COLOR_WHITE );
          Brain.Screen.drawRectangle( screen_origin_x-1 + ((blue_flags_x[i]+red_flags_x[j])/2), screen_origin_y-1 + ((blue_flags_y[i]+red_flags_y[j])/2), 3, 3);
          //clear old flag values
          red_flags_x[j] = -10;
          red_flags_y[j] = -10;
          //store middle flag
          middle_flag_num++;
          middle_flags_x[middle_flag_num] = screen_origin_x + ((blue_flags_x[i]+red_flags_x[j])/2);
          middle_flags_x[middle_flag_num] = screen_origin_y + ((blue_flags_y[i]+red_flags_y[j])/2);
          found_pair = 0;
        }
      }
    }
    if(found_pair){
      Brain.Screen.setPenColor( COLOR_BLUE);
      Brain.Screen.drawRectangle( screen_origin_x+10 + blue_flags_x[i], screen_origin_y-15 + blue_flags_y[i], 30, 30);
      Brain.Screen.setPenColor( COLOR_WHITE );
      Brain.Screen.drawRectangle( screen_origin_x+24 + blue_flags_x[i], screen_origin_y-1 + blue_flags_y[i], 3, 3);
    }else{
      //clear old flag values
      blue_flags_x[i] = -10;
      blue_flags_y[i] = -10;
    }
  }
  for(int i = 0; i < red_flag_num; i++){
    if(red_flags_x[i] > 0){
      Brain.Screen.setPenColor( COLOR_RED );
      Brain.Screen.drawRectangle( screen_origin_x-30 + red_flags_x[i], screen_origin_y-15 + red_flags_y[i], 30, 30);
      Brain.Screen.setPenColor( COLOR_WHITE );
      Brain.Screen.drawRectangle( screen_origin_x-16 + red_flags_x[i], screen_origin_y-1 + red_flags_y[i], 3, 3);
    }
  }

}


//================================================================================================================


int main()
{
  Brain.Screen.clearScreen( COLOR_BLACK );
  Brain.Screen.setPenColor( COLOR_GREEN );
  Brain.Screen.drawRectangle( screen_origin_x-1, screen_origin_y-1, screen_width+2, screen_height+2 );
  while(true)
  {
    clear_counter++;
    if(clear_counter == 30)
    {
      Brain.Screen.clearScreen();
      clear_counter =0;
      Brain.Screen.setPenColor( COLOR_GREEN );
      Brain.Screen.drawRectangle( screen_origin_x-1, screen_origin_y-1, screen_width+2, screen_height+2 );
    }
    clear_lists();
    //========CLEAR SCREEN==============
    Brain.Screen.setPenColor( COLOR_BLACK );
    Brain.Screen.drawRectangle( screen_origin_x, screen_origin_y, screen_width, screen_height, COLOR_BLACK );
    //========PLOT BLUE================
    red_blue_toggle = 1;
    Brain.Screen.setPenColor( COLOR_BLUE);
    int n = Vision1.takeSnapshot( FLAG_BLUE );
    drawObjects( Vision1 );
    distance = 0;
    for(int i = 0; i < 3; i++){
      distance+=blue_flags_y[i];
    }
    //========PLOT RED================
    red_blue_toggle = 0;
    Brain.Screen.setPenColor( COLOR_RED );
    int R = Vision1.takeSnapshot( FLAG_RED );
    drawObjects( Vision1 );
    group_flags();



    distance = distance/3;
    distance = (((33.5-cam_height)/TAN((distance-250.96)/-5.45)*2)/12);

    Brain.Screen.setCursor( 2, 2 );
    Brain.Screen.print( "Distance? %3f", distance);
    Brain.Screen.setCursor( 3, 2 );
    Brain.Screen.print( "Distance? %3f", distance);



    this_thread::sleep_for(100);
  }
}

*/















//unmodded






/*
#include "robot-config.h"

using namespace vex;
int   screen_origin_x = 150;
int   screen_origin_y = 20;
int   screen_width    = 316;
int   screen_height   = 212;
float PI = 3.14159265;

int red_blue_toggle = 0;
int flag_counter = 0;
int red_flags_x[10];
int red_flags_y[10];
int blue_flags_x[10];
int blue_flags_y[10];
int middle_flags_x[10];
int middle_flags_y[10];
int temp_flags_x[10];
int temp_flags_y[10];
int all_flags_y[10][10];
int all_flags_x[10][10];
int blue_flag_num;
int middle_flag_num;
int temp_flag_num;
int red_flag_num;
int all_flag_num;

float cam_height = 8.75;
float distance = 0;
float hyp = 0;
float angle = 0;

int temp = 0;
int clear_counter =0;




float TAN(float in){
  return in * PI / 180;
}

//============================================VISION PLOTTING====================================================
void drawObject( vision::object &obj ) {
  int labelOffset = 0;

  Brain.Screen.drawRectangle( screen_origin_x + obj.originX-(obj.width/2), screen_origin_y + obj.originY-(obj.height/2), obj.width, obj.height );
  Brain.Screen.setFont( mono12 );

  if(red_blue_toggle){
    blue_flags_x[flag_counter] = obj.originX;
    blue_flags_y[flag_counter] = obj.originY;
  }else{
    red_flags_x[flag_counter] = obj.originX;
    red_flags_y[flag_counter] = obj.originY;
  }
}

void drawObjects( vision &v ) {
  flag_counter = 0;
  for(int i=0;i<v.objectCount;i++){
    drawObject( v.objects[i] );
    flag_counter++;
  }
  if(red_blue_toggle){
    blue_flag_num = v.objectCount;
  }else{
    red_flag_num = v.objectCount;
  }
}
//===============================================================================================================


void clear_lists(){
  for(int i = 0; i < 9; i++){
    red_flags_x[i] = 0;
    red_flags_y[i] = 0;
    blue_flags_x[i] = 0;
    blue_flags_y[i] = 0;
    middle_flags_x[i] = 0;
    middle_flags_y[i] = 0;
  }
}


//=========================================FLAG LINE DETECTION===================================================

void vertial_sets(){

}





void assemble_flags(){
  //clear all_flags
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 3; j++){
      all_flags_x[i][j] = 0;
      all_flags_y[i][j] = 0;
    }
  }
  //insert blue flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][1] = blue_flags_x[i];
      all_flags_y[i][1] = blue_flags_y[i];
    }
  }
  //insert red flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][2] = red_flags_x[i];
      all_flags_y[i][2] = red_flags_y[i];
    }
  }
  //insert middle flags
  for(int i = 0; i < 9; i ++){
    if(blue_flags_x[i] > 0){
      all_flags_x[i][3] = middle_flags_x[i];
      all_flags_y[i][3] = middle_flags_y[i];
    }
  }
}

void group_flags(){
  bool found_pair;
  middle_flag_num = 0;
  //start
  for(int i = 0; i < blue_flag_num; i++){ //for all blue flags
    found_pair = 1;
    for(int j = 0; j < red_flag_num; j++){
      if(blue_flags_y[i] - 15 < red_flags_y[j] && red_flags_y[j] < (blue_flags_y[i] + 15)){
        if(blue_flags_x[i] < red_flags_x[j] && red_flags_x[j] < blue_flags_x[i] + 40){
          Brain.Screen.setCursor( 3, 2 );
          Brain.Screen.setPenColor( green );
          Brain.Screen.drawRectangle( screen_origin_x-15 + ((blue_flags_x[i]+red_flags_x[j])/2), screen_origin_y-15 + ((blue_flags_y[i]+red_flags_y[j])/2), 30, 30);
          Brain.Screen.setPenColor( white );
          Brain.Screen.drawRectangle( screen_origin_x-1 + ((blue_flags_x[i]+red_flags_x[j])/2), screen_origin_y-1 + ((blue_flags_y[i]+red_flags_y[j])/2), 3, 3);
          //clear old flag values
          red_flags_x[j] = -10;
          red_flags_y[j] = -10;
          //store middle flag
          middle_flag_num++;
          middle_flags_x[middle_flag_num] = screen_origin_x + ((blue_flags_x[i]+red_flags_x[j])/2);
          middle_flags_x[middle_flag_num] = screen_origin_y + ((blue_flags_y[i]+red_flags_y[j])/2);
          found_pair = 0;
        }
      }
    }
    if(found_pair){
      Brain.Screen.setPenColor( blue);
      Brain.Screen.drawRectangle( screen_origin_x+10 + blue_flags_x[i], screen_origin_y-15 + blue_flags_y[i], 30, 30);
      Brain.Screen.setPenColor( white );
      Brain.Screen.drawRectangle( screen_origin_x+24 + blue_flags_x[i], screen_origin_y-1 + blue_flags_y[i], 3, 3);
    }else{
      //clear old flag values
      blue_flags_x[i] = -10;
      blue_flags_y[i] = -10;
    }
  }
  for(int i = 0; i < red_flag_num; i++){
    if(red_flags_x[i] > 0){
      Brain.Screen.setPenColor( red );
      Brain.Screen.drawRectangle( screen_origin_x-30 + red_flags_x[i], screen_origin_y-15 + red_flags_y[i], 30, 30);
      Brain.Screen.setPenColor( white );
      Brain.Screen.drawRectangle( screen_origin_x-16 + red_flags_x[i], screen_origin_y-1 + red_flags_y[i], 3, 3);
    }
  }

}


//================================================================================================================


int main()
{
  Brain.Screen.clearScreen( black );
  Brain.Screen.setPenColor( green );
  Brain.Screen.drawRectangle( screen_origin_x-1, screen_origin_y-1, screen_width+2, screen_height+2 );
  while(true)
  {
    clear_counter++;
    if(clear_counter == 30)
    {
      Brain.Screen.clearScreen();
      clear_counter =0;
      Brain.Screen.setPenColor( green );
      Brain.Screen.drawRectangle( screen_origin_x-1, screen_origin_y-1, screen_width+2, screen_height+2 );
    }
    clear_lists();
    //========CLEAR SCREEN==============
    Brain.Screen.setPenColor( black );
    Brain.Screen.drawRectangle( screen_origin_x, screen_origin_y, screen_width, screen_height, black );
    //========PLOT BLUE================
    red_blue_toggle = 1;
    Brain.Screen.setPenColor( blue);
    int n = Vision1.takeSnapshot( FLAG_BLUE );
    drawObjects( Vision1 );
    distance = 0;
    for(int i = 0; i < 3; i++){
      distance+=blue_flags_y[i];
    }
    //========PLOT RED================
    red_blue_toggle = 0;
    Brain.Screen.setPenColor( red );
    int R = Vision1.takeSnapshot( FLAG_RED );
    drawObjects( Vision1 );
    group_flags();



    distance = distance/3;
    distance = (((33.5-cam_height)/TAN((distance-250.96)/-5.45)*2)/12);

    Brain.Screen.setCursor( 2, 2 );
    Brain.Screen.print( "Distance? %3f", distance);
    Brain.Screen.setCursor( 3, 2 );
    Brain.Screen.print( "Distance? %3f", distance);



    this_thread::sleep_for(100);
  }
}
*/
