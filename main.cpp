#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "src/pretreatment/filehandle.h"

using namespace std;

int main()
{
  string name = "Metadata_MC";
  char intercept = 'A'; 
  int number = 500; 
  bool connected = true;
  bool sequence = true;
  char nodetype = 'A';

  // Bipartite BipartiteNetwork = getBipartite( name, intercept, number, connected, sequence);
  Unipartite UnipartiteNetwork = getUnipartite( name, intercept, number, connected, sequence, nodetype);

  return 0;
}