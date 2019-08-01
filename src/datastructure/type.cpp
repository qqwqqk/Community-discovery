#include "type.h"
#include <string>
using std::string;

//节点类型基本的定义
Node::Node(int id){
    _id = id;
}
int Node::getID(){
    return _id;
}

//边类型基本的定义
Edge::Edge(int a, int b){
    _node_a = a; _node_b = b;
}
int Edge::getNodeA(){
    return _node_a;
}
int Edge::getNodeB(){
    return _node_b;
}

//传统网络基本结构的定义
Unipartite::Unipartite(string name){
    _name = name;
};
string Unipartite::getName(){
    return _name;
};
map<int, Node> Unipartite::getNodes(){
    return _nodes;
};
vector<Edge> Unipartite::getEdges(){
    return _edges;
};
void Unipartite::addNode(Node item){
  map<int,Node>::iterator iter = _nodes.find(item.getID());
  if(iter == _nodes.end()){
    pair<int, Node> value(item.getID(), item);
    _nodes.insert(value);
  }
};
void Unipartite::addEdge(Edge item){
    _edges.push_back(item);
}

//二元网络基本结构的定义
Bipartite::Bipartite(string name){
    _name = name;
};
string Bipartite::getName(){
    return _name;
};
map<int,Node> Bipartite::getNodesA(){
    return _nodes_a;
};
map<int,Node> Bipartite::getNodesB(){
    return _nodes_b;
};
vector<Edge> Bipartite::getEdges(){
    return _edges;
};
void Bipartite::addNodeA(Node item){
  map<int,Node>::iterator iter = _nodes_a.find(item.getID());
  if(iter == _nodes_a.end()){
    pair<int, Node> value(item.getID(), item);
    _nodes_a.insert(value);
  }
};
void Bipartite::addNodeB(Node item){
  map<int,Node>::iterator iter = _nodes_b.find(item.getID());
  if(iter == _nodes_b.end()){
    pair<int, Node> value(item.getID(), item);
    _nodes_b.insert(value);
  }
};
void Bipartite::addEdge(Edge item){
    _edges.push_back(item);
}
