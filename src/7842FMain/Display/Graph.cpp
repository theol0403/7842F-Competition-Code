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


Graph &Graph::withRange(int min, int max) {
  lv_chart_set_range(graph, min, max);
  return *this;
}

Graph &Graph::withRes(int res) {
  lv_chart_set_point_count(graph, res);
  return *this;
}

Graph &Graph::withLines(int hor, int ver) {
  lv_chart_set_div_line_count(graph, hor, ver);
  return *this;
}

Graph &Graph::withRefresh(int irefresh) {
  refresh = irefresh;
  return *this;
}


Graph &Graph::withSeries(double* variable, lv_color_t color) {
  series.push_back(std::make_pair(lv_chart_add_series(graph, color), variable));
  return *this;
}


void Graph::build() {
  if(graphTask == nullptr) graphTask = new pros::Task(task, this);
}


void Graph::run() {
  while(true) {
    for(auto &[ser, variable] : series) {
      lv_chart_set_next(graph, ser, *variable);
    }
    pros::delay(refresh);
  }
}

void Graph::task(void* input)
{
  Graph* that = static_cast<Graph*>(input);
  that->run();
}
