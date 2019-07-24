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
vector<Node> Unipartite::getNodes(){
    return _nodes;
};
vector<Edge> Unipartite::getEdges(){
    return _edges;
};
void Unipartite::addNode(Node item){
    _nodes.push_back(item);
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
vector<Node> Bipartite::getNodesA(){
    return _nodes_a;
};
vector<Node> Bipartite::getNodesB(){
    return _nodes_b;
};
vector<Edge> Bipartite::getEdges(){
    return _edges;
};
void Bipartite::addNodeA(Node item){
    _nodes_a.push_back(item);
};
void Bipartite::addNodeB(Node item){
    _nodes_b.push_back(item);
};
void Bipartite::addEdge(Edge item){
    _edges.push_back(item);
}
