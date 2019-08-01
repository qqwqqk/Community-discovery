#pragma once
#include "../datastructure/type.h"
#include <string>
#include <iostream>
using namespace std;

Bipartite getBipartite(
  string name, 
  char intercept = 'A', 
  int number = -1, 
  bool connected = false, 
  bool rank = false
);

Bipartite pretreatmentBipartite(
  string name, 
  char intercept = 'A', 
  int number = -1, 
  bool connected = false, 
  bool rank = false
);