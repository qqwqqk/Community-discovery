#pragma once
#include <vector>
#include <string>
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
        vector<Node> _nodes;
        vector<Edge> _edges;
    public:
        Unipartite(string name = "");
        ~Unipartite(){};
        string getName();
        vector<Node> getNodes();
        vector<Edge> getEdges();
        void addNode(Node item = Node());
        void addEdge(Edge item = Edge());
};

class Bipartite{
    protected:
        string _name;
        vector<Node> _nodes_a;
        vector<Node> _nodes_b;
        vector<Edge> _edges;
    public:
        Bipartite(string name = "");
        ~Bipartite(){};
        string getName();
        vector<Node> getNodesA();
        vector<Node> getNodesB();
        vector<Edge> getEdges();
        void addNodeA(Node item = Node());
        void addNodeB(Node item = Node());
        void addEdge(Edge item = Edge());
};
