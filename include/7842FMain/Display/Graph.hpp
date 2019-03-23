#pragma once
#include "main.h"

class Graph
{

public:

  lv_obj_t* graph = nullptr;
  int refresh = 100;
  std::vector<std::tuple<lv_chart_series_t*, double*, double>> series = {};

  pros::Task* task = nullptr;

  Graph(lv_obj_t*);
  Graph(lv_obj_t*, lv_color_t);
  ~Graph();

  Graph &withRange(int, int);
  Graph &withRes(int);
  Graph &withLines(int, int);
  Graph &withRefresh(int);

  Graph &withSeries(double*, lv_color_t, double = 1);

  void build();

  void run();
  static void taskFnc(void*);

};
