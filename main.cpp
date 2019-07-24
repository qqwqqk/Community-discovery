#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "src/pretreatment/method.cpp"

using namespace std;

int main()
{
    string str = "test";
    test(str);
    pretreatmentBipartite(str);
    return 0;
}