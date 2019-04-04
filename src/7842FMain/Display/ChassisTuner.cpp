#include "ChassisTuner.hpp"
#include <sstream>
#include <iomanip>

ChassisTuner::ChassisTuner(lv_obj_t* parent) : ChassisTuner(parent, lv_obj_get_style(parent)->body.main_color) {}

ChassisTuner::ChassisTuner(lv_obj_t* parent, lv_color_t mainColor) :
container(parent)
{

  /**
  * ButtonMatrix Styles
  */
  lv_style_t* btnm_bg = new lv_style_t;
  lv_style_copy(btnm_bg, &lv_style_plain);
  btnm_bg->body.main_color = mainColor;
  btnm_bg->body.grad_color = mainColor;
  btnm_bg->body.border.color = LV_COLOR_WHITE;
  btnm_bg->body.border.width = 0;
  btnm_bg->body.radius = 0;
  btnm_bg->body.padding.hor = 5;
  btnm_bg->body.padding.ver = 5;
  btnm_bg->body.padding.inner = 5;

  lv_style_t* btnm_rel = new lv_style_t;
  lv_style_copy(btnm_rel, &lv_style_btn_tgl_rel);
  btnm_rel->body.main_color = mainColor;
  btnm_rel->body.grad_color = mainColor;
  btnm_rel->body.border.color = LV_COLOR_WHITE;
  btnm_rel->body.border.width = 2;
  btnm_rel->body.border.opa = LV_OPA_100;
  btnm_rel->body.radius = 2;
  btnm_rel->text.color = LV_COLOR_WHITE;

  lv_style_t* btnm_pr = new lv_style_t;
  lv_style_copy(btnm_pr, btnm_rel);
  btnm_pr->body.main_color = LV_COLOR_WHITE;
  btnm_pr->body.grad_color = LV_COLOR_WHITE;
  btnm_pr->text.color = mainColor;

  lv_style_t* btnm_ina = new lv_style_t;
  lv_style_copy(btnm_ina, &lv_style_btn_ina);
  btnm_ina->body.main_color = mainColor;
  btnm_ina->body.grad_color = mainColor;
  btnm_ina->body.border.width = 0;
  btnm_ina->text.color = LV_COLOR_WHITE;


    std::vector<const char*>* btnLabels = new std::vector<const char*>;
    *btnLabels = {"Dist+", "\n", "Dist-", "\n", "Top+", "\n", "Top-", "\n", "Mid+", "\n", "Mid-", ""};

    lv_obj_t* btnm = lv_btnm_create(angleContainer, NULL);
    lv_btnm_set_map(btnm, btnLabels->data());

    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, btnm_bg);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, btnm_rel);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, btnm_pr);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, btnm_ina);

    lv_obj_set_size(btnm, lv_obj_get_width(angleContainer)/3*2, lv_obj_get_height(angleContainer));
    lv_btnm_set_action(btnm, angleBtnAction);
    lv_obj_set_free_ptr(btnm, this);




ChassisTuner::~ChassisTuner() {
  lv_obj_del(container);
}

void ChassisTuner::allignLabel(lv_obj_t* label, std::string text, double* offset) {
  int height = lv_obj_get_height(lv_obj_get_parent(label)) - 15;
  lv_label_set_text(label, text.c_str());
  *offset += height/3.0;
  lv_obj_align(label, NULL, LV_ALIGN_OUT_TOP_LEFT, lv_obj_get_width(lv_obj_get_parent(label))/3.0*2.0, *offset - lv_obj_get_height(label) + height/6.0);
}

void ChassisTuner::calcAngleLabels() {
  double offset = -30.0;
  {
    std::stringstream str;
    str << angler->distanceToFlag.convert(foot);
    allignLabel(angleLabels[0], str.str(), &offset);
  }
  {
    std::stringstream str;
    str << angler->getTopFlagAngle();
    allignLabel(angleLabels[1], str.str(), &offset);
  }
  {
    std::stringstream str;
    str << angler->getMiddleFlagAngle();
    allignLabel(angleLabels[2], str.str(), &offset);
  }
}


lv_res_t ChassisTuner::angleBtnAction(lv_obj_t* btnm, const char *itxt) {
  ChassisTuner* that = static_cast<ChassisTuner*>(lv_obj_get_free_ptr(btnm));
  std::string label = itxt;

  int labelPos = label.find_last_of({'+', '-'});
  bool sign = label.at(labelPos) == '+' ? true : false;
  label.erase(labelPos, std::string::npos);

  if(label == "Dist") {
    that->angler->distanceToFlag += 0.5_ft * boolToSign(sign);
  } else if(label == "Top") {
    that->angler->topAngles.at(that->angler->distanceToFlag.convert(foot)) += 0.5 * boolToSign(sign);
  } else if(label == "Mid") {
    that->angler->middleAngles.at(that->angler->distanceToFlag.convert(foot)) += 0.5 * boolToSign(sign);
  } else {
    std::cerr << "No Button Found" << std::endl;
  }

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


lv_res_t ChassisTuner::actionBtnAction(lv_obj_t* btnm, const char *itxt) {
  ChassisTuner* that = static_cast<ChassisTuner*>(lv_obj_get_free_ptr(btnm));
  std::string label = itxt;

  if(label == "Top") {
    that->angler->doJobLoop(ShootController::angleTop);
  } else if(label == "Mid") {
    that->angler->doJobLoop(ShootController::angleMiddle);
  } else if(label == "Cycle") {
    that->angler->doJob(ShootController::cycle);
  } else if(label == "Shoot") {
    that->angler->doJob(ShootController::enableShoot);
  } else {
    std::cerr << "No Button Found" << std::endl;
  }
  //std::cout << label << std::endl;

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


void ChassisTuner::calcDial() {
  const int lineLength = 60;
  QAngle backAngle = -45_deg;

  {
    double theta = (backAngle + (angler->getHoodAngle() * degree)).convert(radian);
    auto &[points, line] = dialLines.at(0);
    double x = lineLength * sin(theta);
    double y = -lineLength * cos(theta);
    points[1] = {(int16_t)(points[0].x + x), (int16_t)(points[0].y + y)};
    lv_line_set_points(line, points.data(), points.size());
    lv_obj_invalidate(line);
  }

  {
    double theta = (backAngle + (angler->getTopFlagAngle() * degree)).convert(radian);
    auto &[points, line] = dialLines.at(1);
    double x = lineLength * sin(theta);
    double y = -lineLength * cos(theta);
    points[1] = {(int16_t)(points[0].x + x), (int16_t)(points[0].y + y)};
    lv_line_set_points(line, points.data(), points.size());
    lv_obj_invalidate(line);
  }

  {
    double theta = (backAngle + (angler->getMiddleFlagAngle() * degree)).convert(radian);
    auto &[points, line] = dialLines[2];
    double x = lineLength * sin(theta);
    double y = -lineLength * cos(theta);
    points[1] = {(int16_t)(points[0].x + x), (int16_t)(points[0].y + y)};
    lv_line_set_points(line, points.data(), points.size());
    lv_obj_invalidate(line);
  }


  std::stringstream str;
  str << "Angle:\n" << std::fixed << std::setprecision(3) << angler->getHoodAngle();
  lv_label_set_text(dialLabel, str.str().c_str());
  lv_obj_align(dialLabel, NULL, LV_ALIGN_CENTER, 0, lv_obj_get_height(lv_obj_get_parent(dialLabel))/3.0);
}


void ChassisTuner::taskFnc(void* input) {
  pros::delay(500);
  ChassisTuner* that = static_cast<ChassisTuner*>(input);
  while(true) {
    that->calcAngleLabels();
    that->calcDial();

    pros::delay(50);
  }
}
