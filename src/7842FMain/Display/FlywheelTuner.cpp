#include "FlywheelTuner.hpp"


FlywheelTuner::FlywheelTuner(lv_obj_t* parent) :
container(lv_obj_create(parent, NULL)), mainColor(LV_COLOR_HEX(0xFF7F00))
{
  /**
  * Container
  */
  lv_obj_set_size(container, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_t* cStyle = new lv_style_t;
  lv_style_copy(cStyle, &lv_style_plain_color);
  cStyle->body.main_color = mainColor;
  cStyle->body.grad_color = mainColor;
  lv_obj_set_style(container, cStyle);
  lv_obj_set_style(container, cStyle);
}

FlywheelTuner::~FlywheelTuner() {
  lv_obj_del(container);
}


FlywheelTuner &FlywheelTuner::withButton(std::string name, double* variable, buttonType_t type, double modifier) {
  buttons.push_back(std::make_pair(name, button_t{variable, type, modifier}));
  return *this;
}


void FlywheelTuner::build() {

  std::vector<const char*>* btnLabels = new std::vector<const char*>;

  for(auto &button : buttons) {
    std::string* str = new std::string;
    *str = button.first + "+";
    btnLabels->push_back(str->c_str());
  }
  btnLabels->push_back("\n");
  for(auto &button : buttons) {
    std::string* str = new std::string;
    *str = button.first + "-";
    btnLabels->push_back(str->c_str());
  }

  btnLabels->push_back("");
  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_btnm_set_map(btnm, btnLabels->data());

  lv_obj_set_size(btnm, lv_obj_get_width(container), lv_obj_get_height(container) / 3);
  lv_obj_align(btnm, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_btnm_set_action(btnm, btnAction);
  lv_obj_set_free_ptr(btnm, this);

  lv_style_t* style_bg = new lv_style_t;
  lv_style_copy(style_bg, &lv_style_plain);
  style_bg->body.main_color = mainColor;
  style_bg->body.grad_color = mainColor;
  style_bg->body.border.color = LV_COLOR_WHITE;
  style_bg->body.border.width = 3;
  style_bg->body.radius = 0;
  style_bg->body.padding.hor = 5;
  style_bg->body.padding.ver = 5;
  style_bg->body.padding.inner = 5;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, style_bg);

  lv_style_t* style_rel = new lv_style_t;
  lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
  style_rel->body.main_color = mainColor;
  style_rel->body.grad_color = mainColor;
  style_rel->body.border.color = LV_COLOR_WHITE;
  style_rel->body.border.width = 2;
  style_rel->body.border.opa = LV_OPA_100;
  style_rel->body.radius = 2;
  style_rel->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, style_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, style_pr);

  lv_style_t* style_ina = new lv_style_t;
  lv_style_copy(style_ina, &lv_style_btn_ina);
  style_ina->body.main_color = mainColor;
  style_ina->body.grad_color = mainColor;
  style_ina->body.border.width = 0;
  style_ina->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, style_ina);

}


lv_res_t FlywheelTuner::btnAction(lv_obj_t* btnm, const char *itxt) {
  std::string label = itxt;
  //std::cout << label << std::endl;
  int labelPos = label.find_last_of({'+', '-'});
  bool sign = label.at(labelPos) == '+' ? true : false;
  label.erase(labelPos, std::string::npos);

  FlywheelTuner* that = static_cast<FlywheelTuner*>(lv_obj_get_free_ptr(btnm));

  auto search = std::find_if(that->buttons.begin(), that->buttons.end(), [=](const std::pair<std::string, button_t> &button){ return button.first == label; });
  if (search == that->buttons.end()) {
    std::cerr << "FlywheelTuner::btnAction : no label found" << std::endl;
  }

  std::cout << search->first << std::endl;


  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}
