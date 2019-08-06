#pragma once
#include "../datastructure/type.h"
#include <iostream>
using namespace std;

double calculationBasePolymerization(vector<Edge> edges, char nodetype, int item_i = 0, int item_j = 0);

double calculationCommunityPolymerization(map<int,Node> nodes, vector<Edge> edges, char nodetype, int item_i = 0, int item_j = 0);

double calculationModularity(map<int,Node> nodes, vector<Edge> edges);

map<int, int> calculationMergeList(vector<Edge> edges);

int calculationMergeNumber(map<int, int> source);
