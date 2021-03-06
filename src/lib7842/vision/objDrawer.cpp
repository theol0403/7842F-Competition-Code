#include "objDrawer.hpp"

namespace lib7842 {

lv_style_t ObjRenderer::defaultObjStyle = []() {
  lv_style_copy(&defaultObjStyle, &lv_style_pretty_color);
  defaultObjStyle.body.main_color = LV_COLOR_BLACK;
  defaultObjStyle.body.grad_color = LV_COLOR_BLACK;
  defaultObjStyle.body.radius = 5;
  defaultObjStyle.body.border.color = LV_COLOR_WHITE;
  defaultObjStyle.body.border.width = 2;
  defaultObjStyle.body.border.opa = LV_OPA_100;
  return defaultObjStyle;
}();

ObjRenderer::ObjRenderer(lv_obj_t* iparent, ObjContainer* ioContainer) :
  parent(iparent),
  wScale((double)lv_obj_get_width(parent) / VISION_FOV_WIDTH),
  hScale((double)lv_obj_get_height(parent) / VISION_FOV_HEIGHT),
  oContainer(ioContainer),
  objStyle(defaultObjStyle) {}

ObjRenderer::~ObjRenderer() { clear(); }

void ObjRenderer::expandTo(int size) {
  int diff = size - dObjects.size();
  if (diff <= 0) return;
  for (int i = 0; i < diff; i++) {
    lv_obj_t* obj = lv_obj_create(parent, NULL);
    formatObj(obj);
    dObjects.push_back(obj);
  }
}

void ObjRenderer::formatObj(lv_obj_t* obj) {
  lv_obj_set_style(obj, &objStyle);
  lv_obj_set_hidden(obj, true);
}

void ObjRenderer::format() {
  for (lv_obj_t* obj : dObjects)
    formatObj(obj);
}

ObjRenderer& ObjRenderer::setStyle(lv_color_t main, lv_color_t border, lv_opa_t opa) {
  objStyle.body.main_color = main;
  objStyle.body.grad_color = main;
  objStyle.body.border.color = border;
  objStyle.body.opa = opa;
  format();
  return *this;
}

ObjRenderer& ObjRenderer::setStyle(int sig, lv_color_t main, lv_color_t border, lv_opa_t opa) {
  lv_style_t style;
  lv_style_copy(&style, &objStyle);
  style.body.main_color = main;
  style.body.grad_color = main;
  style.body.border.color = border;
  style.body.opa = opa;
  sigStyles.insert_or_assign(sig, style);
  return *this;
}

void ObjRenderer::draw() {
  expandTo(oContainer->objects.size());

  int dIndex = 0;
  for (auto&& obj : oContainer->objects) {
    lv_obj_set_hidden(dObjects.at(dIndex), false); // make visible

    auto search = sigStyles.find(obj.getAttr(objAttr::sig));
    if (search != sigStyles.end()) {
      lv_obj_set_style(dObjects.at(dIndex), &search->second);
    } else {
      lv_obj_set_style(dObjects.at(dIndex), &objStyle);
    }

    lv_obj_set_x(dObjects.at(dIndex), obj.getAttr(objAttr::x) * wScale);
    lv_obj_set_y(dObjects.at(dIndex), obj.getAttr(objAttr::y) * hScale);

    lv_obj_set_width(dObjects.at(dIndex), obj.getAttr(objAttr::width) * wScale);
    lv_obj_set_height(dObjects.at(dIndex), obj.getAttr(objAttr::height) * hScale);

    dIndex++;
  }

  for (; dIndex < dObjects.size(); dIndex++) {
    lv_obj_set_hidden(dObjects.at(dIndex), true);
  }
}

void ObjRenderer::clear() {
  for (lv_obj_t* obj : dObjects) {
    lv_obj_del(obj);
  }
  dObjects.clear();
}

ObjDrawer::ObjDrawer(lv_obj_t* parent) : dContainer(lv_obj_create(parent, NULL)) {
  lv_obj_set_size(dContainer, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(dContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
  lv_style_copy(&cStyle, &lv_style_plain_color);
  cStyle.body.main_color = LV_COLOR_BLACK;
  cStyle.body.grad_color = LV_COLOR_BLACK;
  lv_obj_set_style(dContainer, &cStyle);

  infoLabel = lv_label_create(dContainer, NULL);
  lv_style_t* textStyle = new lv_style_t;
  lv_style_copy(textStyle, &lv_style_plain);
  textStyle->text.color = LV_COLOR_WHITE;
  textStyle->text.opa = LV_OPA_100;
  lv_obj_set_style(infoLabel, textStyle);
  lv_label_set_text(infoLabel, "");
}

ObjDrawer::~ObjDrawer() { lv_obj_del(dContainer); }

ObjDrawer& ObjDrawer::setStyle(lv_color_t main, lv_color_t border, lv_opa_t opa) {
  cStyle.body.main_color = main;
  cStyle.body.grad_color = main;
  cStyle.body.border.color = border;
  cStyle.body.opa = opa;
  lv_obj_set_style(dContainer, &cStyle);
  return *this;
}

ObjRenderer& ObjDrawer::makeLayer(ObjContainer& oContainer) {
  layers.push_back(ObjRenderer(dContainer, &oContainer));
  return layers.back();
}

void ObjDrawer::draw(ObjContainer& oContainer, bool useLabel) {
  auto it = std::find_if(layers.begin(), layers.end(),
                         [&](const ObjRenderer& layer) { return layer.oContainer == &oContainer; });
  if (it != layers.end()) {
    it->draw();
  } else {
    std::cerr << "ObjDrawer::draw: container not found" << std::endl;
  }

  if (useLabel) {
    if (layers.size() > 0) {
      lv_label_set_text(infoLabel,
                        ("Count: " + std::to_string(it->oContainer->getCount())).c_str());
      lv_obj_align(infoLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    } else {
      lv_label_set_text(infoLabel, "No Layer");
      lv_obj_align(infoLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    }
  }
}

void ObjDrawer::drawAll() {
  for (auto&& layer : layers) {
    layer.draw();
  }

  if (layers.size() > 0) {
    lv_label_set_text(infoLabel,
                      ("Count: " + std::to_string(layers.at(0).oContainer->getCount())).c_str());
    lv_obj_align(infoLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  } else {
    lv_label_set_text(infoLabel, "No Layer");
    lv_obj_align(infoLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  }
}

} // namespace lib7842
