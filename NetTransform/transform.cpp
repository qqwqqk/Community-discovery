#pragma once
#include "base.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void main(){

	string netname = "Table-events_womens(89)";

	Network Head;
	Head.S_initialize(netname);					//�����ݿ⵼������
	//Head.T_initialize(netname);				//��TXT�ı���������

	Head.TransformToSuperGraphs();
	Head.TransformToWeightGraphs();

	//Head.PrintLinks();
	//Head.PrintEdges();
	//Head.PrintNodes();
	//Head.PrintNet();
	system("pause");
}