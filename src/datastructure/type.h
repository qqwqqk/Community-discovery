#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

struct Link{ 
  int community_a, community_b; 
  double cpi; 
};

class Node{
  protected:
    int _id;
    int _degree;
    vector<int> _subjection;
  public:
    Node(int id = -1);
    ~Node(){};
    int getID();
    int getDegree();
    int getCommunityTag(int index = -1);
    void addDegree();
    void setDegree(int degree = -1);
    void addListTag(int tag = -1);
};

class Edge{
  protected:
    int _node_a;
    int _node_b;
    double _weight;
  public:
    Edge(int a = -1, int b = -2);
    ~Edge(){};
    int getNodeA();
    int getNodeB();
    double getWeight();
    void setWeight(double weight);
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
