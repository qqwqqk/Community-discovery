#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class Node{
  protected:
    int _id;
  public:
    Node(int id = -1);
    ~Node(){};
    int getID();
};

class Edge{
  protected:
    int _node_a;
    int _node_b;
  public:
    Edge(int a = -1, int b = -2);
    ~Edge(){};
    int getNodeA();
    int getNodeB();
};

class Unipartite{
  protected:
    string _name;
    map<int,Node> _nodes;
    vector<Edge> _edges;
  public:
    Unipartite(string name = "");
    ~Unipartite(){};
    string getName();
    map<int,Node> getNodes();
    vector<Edge> getEdges();
    void addNode(Node item = Node());
    void addEdge(Edge item = Edge());
};

class Bipartite{
  protected:
    string _name;
    map<int,Node> _nodes_a;
    map<int,Node> _nodes_b;
    vector<Edge> _edges;
  public:
    Bipartite(string name = "");
    ~Bipartite(){};
    string getName();
    map<int,Node> getNodesA();
    map<int,Node> getNodesB();
    vector<Edge> getEdges();
    void addNodeA(Node item = Node());
    void addNodeB(Node item = Node());
    void addEdge(Edge item = Edge());
};
