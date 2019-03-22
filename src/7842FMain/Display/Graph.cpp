#include "Graph.hpp"

Graph::Graph(lv_obj_t* parent) : Graph(parent, lv_obj_get_style(parent)->body.main_color) {}

Graph::Graph(lv_obj_t* parent, lv_color_t mainColor) :
graph(lv_chart_create(parent, NULL))
{
  lv_obj_set_size(graph, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(graph, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_chart_set_type(graph, LV_CHART_TYPE_LINE);
  lv_chart_set_series_opa(graph, LV_OPA_70);
  lv_chart_set_series_width(graph, 2);

  lv_style_t* gStyle = new lv_style_t;
  lv_style_copy(gStyle, &lv_style_pretty);
  gStyle->body.main_color = mainColor;
  gStyle->body.grad_color = mainColor;
  gStyle->line.color = LV_COLOR_WHITE;
  lv_obj_set_style(graph, gStyle);
}

Graph::~Graph() {
  lv_obj_del(graph);
}
