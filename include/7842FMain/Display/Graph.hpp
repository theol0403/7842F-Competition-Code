#pragma once
#include "main.h"

class Graph
{

public:

  lv_obj_t *graph = nullptr;
  int refresh = 100;
  std::vector<std::pair<lv_chart_series_t*, double*>> series = {};

  pros::Task* graphTask = nullptr;

  Graph(lv_obj_t*);
  Graph(lv_obj_t*, lv_color_t);
  ~Graph();

  Graph &withMin(int);
  Graph &withMax(int);
  Graph &withRes(int);
  Graph &withRefresh(int);

  Graph &withSeries(double*, lv_color_t);

  void build();

  void run();
  static void task();



};
