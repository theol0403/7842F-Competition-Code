#include "ChassisTuner.hpp"
#include <sstream>
#include <iomanip>

ChassisTuner::ChassisTuner(lv_obj_t* parent, lib7842::OdomController* icontroller) : ChassisTuner(parent, lv_obj_get_style(parent)->body.main_color, icontroller) {}

ChassisTuner::ChassisTuner(lv_obj_t* parent, lv_color_t mainColor, lib7842::OdomController* icontroller) :
container(parent), controller(icontroller)
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


  lv_style_copy(&containerStyle, &lv_style_plain);
  containerStyle.body.main_color = mainColor;
  containerStyle.body.grad_color = mainColor;
  containerStyle.body.border.color = LV_COLOR_WHITE;
  containerStyle.body.border.width = 3;
  containerStyle.body.radius = 0;


  std::vector<const char*>* btnLabels = new std::vector<const char*>;
  *btnLabels = {"kP+", "kI+", "kD+", "\n", "kP-", "kI-", "kD-", ""};

  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_obj_set_hidden(btnm, true);
  lv_btnm_set_map(btnm, btnLabels->data());

  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, btnm_bg);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, btnm_rel);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, btnm_pr);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, btnm_ina);

  tuners.push_back(buildTuner(btnm, "Distance", controller->distancePid));
  tuners.push_back(buildTuner(btnm, "Angle", controller->anglePid));
  tuners.push_back(buildTuner(btnm, "Turn", controller->turnPid));


  lv_obj_set_hidden(tuners[1], false);

}


ChassisTuner::~ChassisTuner() {
  lv_obj_del(container);
}


lv_obj_t* ChassisTuner::buildTuner(lv_obj_t* copy, std::string name, IterativePosPIDController* pid) {

  lv_obj_t* tuner = lv_obj_create(container, NULL);
  lv_obj_set_size(tuner, lv_obj_get_width(container), lv_obj_get_height(container)/2);
  lv_obj_align(tuner, container, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_obj_set_style(tuner, &containerStyle);
  lv_obj_set_hidden(tuner, true);

  lv_obj_t* btnm = lv_btnm_create(tuner, copy);
  lv_obj_set_hidden(btnm, false);
  lv_obj_set_size(btnm, lv_obj_get_width(tuner), lv_obj_get_height(tuner));
  lv_obj_align(btnm, tuner, LV_ALIGN_CENTER, 0, 0);

  lv_btnm_set_action(btnm, btnAction);

  Passer* passer = new Passer(this, pid);
  lv_obj_set_free_ptr(btnm, passer);

  return tuner;
}


lv_res_t ChassisTuner::btnAction(lv_obj_t* btnm, const char *itxt) {
  Passer* passer = static_cast<Passer*>(lv_obj_get_free_ptr(btnm));
  auto &[that, pid] = *passer;
  std::string label = itxt;
  return LV_RES_OK;
}
