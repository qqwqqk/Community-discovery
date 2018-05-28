#pragma once
#include"adosql.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

class Node{
protected:
	int ID;
	int Dgree;
public:
	vector<int> EdgeID;
	Node *next;
	Node(int id = -1, int dgree = 0);

	int GetNodeID();
	int GetNodeDgree();

	void AddEdgeID(int eid = -1);

	void PrintNode();
	friend class Network;
};

class Edge{
protected:
	int ID;
	int Dgree;
public:
	vector<int> NodeID;
	Edge *next;
	Edge(int id = -1, int dgree = 0);

	int GetEdgeID();
	int GetEdgeDgree();

	void AddNodeID(int nid = -1);

	void PrintEdge();
	friend class Network;
};

class Link{
protected:
	int ID;
	int NodeID;
	int EdgeID;
	double Weight;
public:
	Link *next;

	int GetLinkID();
	int GetNodeID();
	int GetEdgeID();

	void PrintLink();
	Link(int id = -1, int node = -1, int edge = -1, double weight = 1.0);
	friend class Network;
};

class Network{
private:
	int ID;
	string Name;
	int NodeNumber;
	int EdgeNumber;
	int LinkNumber;
public:
	Node *Nodes;
	Edge *Edges;
	Link *Links;
	Network *next;

	void T_initialize(string netname = "Test");
	void S_initialize(string netname = "Test");

	void PrintNodes();
	void PrintEdges();
	void PrintLinks();
	void PrintNet();

	void TransformToWeightGraphs();
	void TransformToSuperGraphs();

	Network(int id = -1, string name = "");
	~Network();
};