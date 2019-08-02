#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "src/pretreatment/method.h"

using namespace std;

int main()
{
  string str = "Metadata_MC";
  char intercept = 'A'; 
  int number = 100; 
  bool connected = true;
  bool sequence = true;
  Bipartite Network = getBipartite(str, intercept, number, connected, sequence);
  return 0;
}