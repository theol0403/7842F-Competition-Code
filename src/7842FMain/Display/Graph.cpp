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


Graph &Graph::withRange(int imin, int imax) {
  lv_chart_set_range(graph, imin, imax);
  auto &[min, max, ver] = lineData;
  min = imin;
  max = imax;
  return *this;
}

Graph &Graph::withRes(int res) {
  lv_chart_set_point_count(graph, res);
  return *this;
}

Graph &Graph::withLines(int hor, int ver) {
  lv_chart_set_div_line_count(graph, hor, ver);
  auto &[min, max, div] = lineData;
  div = hor + 1;
  return *this;
}

Graph &Graph::withRefresh(int irefresh) {
  refresh = irefresh;
  return *this;
}


Graph &Graph::withSeries(std::string name, double* variable, lv_color_t color, double scale) {
  lv_chart_series_t* ser = lv_chart_add_series(graph, color);
  lv_chart_init_points(graph, ser, *variable / scale);

  //Add text to legend
  lv_obj_t* label = lv_label_create(graph, NULL);
  lv_label_set_text(label, (std::string(SYMBOL_MINUS) + " " + name).c_str());
  lv_style_t* style = new lv_style_t;
  lv_style_copy(style, &lv_style_plain);
  style->text.color = color;
  style->text.opa = LV_OPA_100;
  style->text.font = &lv_font_dejavu_20;
  style->text.letter_space = 1;
  lv_obj_set_style(label, style);
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 7, 5 + lv_obj_get_height(label) * series.size());

  series.push_back(std::make_tuple(ser, variable, scale));
  return *this;
}


void Graph::build() {
  if(task == nullptr) task = new pros::Task(taskFnc, this);

  lv_style_t* style = new lv_style_t;
  lv_style_copy(style, &lv_style_plain);
  style->text.color = LV_COLOR_WHITE;
  style->text.opa = LV_OPA_100;
  style->text.font = &lv_font_dejavu_10;
  style->text.letter_space = 1;

  auto &[min, max, div] = lineData;
  double lineHeight = (double)lv_obj_get_height(graph) / div;

  for(int i = 0; i < div; i++){
    lv_obj_t* label = lv_label_create(graph, NULL);
    lv_label_set_text(label, std::to_string((int)remapRange(i, 0, div, min, max)).c_str());
    lv_obj_set_style(label, style);
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -5, -lineHeight * i);
  }

  lv_obj_t* label = lv_label_create(graph, NULL);
  lv_label_set_text(label, std::to_string((int)max).c_str());
  lv_obj_set_style(label, style);
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_RIGHT, -5, 3);
}


void Graph::run() {
  while(true) {
    for(auto &[ser, variable, scale] : series) {
      lv_chart_set_next(graph, ser, *variable / scale);
    }
    pros::delay(refresh);
  }
}

void Graph::taskFnc(void* input)
{
  Graph* that = static_cast<Graph*>(input);
  that->run();
}
