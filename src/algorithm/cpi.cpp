#include "cpi.h"
#include <cstdlib>
#include <set>
#include <math.h>
using namespace std;

double calculationBasePolymerization(vector<Edge> edges, char nodetype, int item_i, int item_j){
  if(nodetype != 'A' && nodetype !='B'){ cout<<"Invalid node type input"<<endl; exit(1);}
  set<int> set_i, set_j, set_ij;
  for(int i=0; i<edges.size(); i++){
    const int node_id = nodetype == 'A' ? edges[i].getNodeA() : edges[i].getNodeB();
    const int node_tag = nodetype == 'A' ? edges[i].getNodeB() : edges[i].getNodeA();

    if(node_id == item_i){ set_i.insert(node_tag); set_ij.insert(node_tag); }
    if(node_id == item_j){ set_j.insert(node_tag); set_ij.insert(node_tag); }
  }
  double basepolymerization = 2.0 * ( set_i.size() + set_j.size() - set_ij.size()) / ( set_i.size() + set_j.size() );
  return basepolymerization;
}

double calculationCommunityPolymerization(map<int,Node> nodes, vector<Edge> edges, char nodetype, int item_i, int item_j){
  if(nodetype != 'A' && nodetype !='B'){ cout<<"Invalid node type input"<<endl; exit(1);}
  set<int> community_i, community_j;
  set<int> sub_i, sub_j;
  
  //缓存community_i,community_j所属节点
  for(map<int, Node>::iterator iter=nodes.begin(); iter != nodes.end(); iter++){
    if(iter->second.getCommunityTag() == item_i){ community_i.insert(iter->first);}
    if(iter->second.getCommunityTag() == item_j){ community_j.insert(iter->first);}
  }
  const int communitynumber_i = community_i.size();
  const int communitynumber_j = community_j.size();

  //缓存sub_i,sub_j
  for(int i=0; i<edges.size(); i++){
    const int node_id = nodetype == 'A' ? edges[i].getNodeA() : edges[i].getNodeB();
    const int node_tag = nodetype == 'A' ? edges[i].getNodeB() : edges[i].getNodeA();
    if(community_i.find(node_id) != community_i.end()){ 
      sub_i.insert(node_tag);
    }
    if(community_j.find(node_id) != community_j.end()){ 
      sub_j.insert(node_tag); 
    }
  }

  //calculation item of community polymerization
  double dxy = 0, dx_y = 0, d_xy = 0;
  for(set<int>::iterator iter_i = sub_i.begin(); iter_i != sub_i.end(); iter_i++){
    const int item_id = *iter_i;
    if(sub_j.find(item_id) != sub_j.end()){
      dxy = dxy + sqrt( 1.0 * sub_i.count(item_id) / communitynumber_i * sub_j.count(item_id) / communitynumber_j);
    } else {
      dx_y = dx_y + 1.0 * sub_i.count(item_id) / communitynumber_i;
    }
  }
  for(set<int>::iterator iter_j = sub_j.begin(); iter_j != sub_j.end(); iter_j++){
    const int item_id = *iter_j;
    if(sub_i.find(item_id) != sub_i.end()){
      dxy = dxy + sqrt( 1.0 * sub_i.count(item_id) / communitynumber_i * sub_j.count(item_id) / communitynumber_j);
    } else {
      d_xy = d_xy + 1.0 * sub_j.count(item_id) / communitynumber_j;
    }
  }

  //calculation community polymerization
  double communitypolymerization = dxy / ( dxy + dx_y + d_xy);

  return communitypolymerization;
}

double calculationModularity(map<int,Node> nodes, vector<Edge> edges){
  const double edges_number = edges.size();
  double community_edge = 0, item_combination = 0, community_combination = 0;

  for(int i=0; i<edges_number; i++){
    int node_a_tag = nodes.find(edges[i].getNodeA())->second.getCommunityTag();
    int node_b_tag = nodes.find(edges[i].getNodeB())->second.getCommunityTag();
    if(node_a_tag == node_b_tag){ community_edge = community_edge + 1; }
  }

  for(map<int,Node>::iterator node_i = nodes.begin(); node_i != nodes.end(); node_i++ ){
    for(map<int,Node>::iterator node_j = nodes.begin(); node_j != nodes.end(); node_j++ ){
      if(node_i->second.getCommunityTag() == node_j->second.getCommunityTag()){ 
        item_combination = 0.5 / edges_number * node_i->second.getDegree() * node_j->second.getDegree();
        community_combination = community_combination + item_combination;
      }
    }
  }

  double modularity = 0.5 / edges_number * (2 * community_edge - community_combination);

  return modularity;
}

map<int, int> calculationMergeList(vector<Link> links){
  map<int,int> mergeList;
  double gravityMax = 0;

  for(int i=0; i<links.size(); i++){
    gravityMax = gravityMax > links[i].cpi ? gravityMax : links[i].cpi;
  }

  for(int i=0; i<links.size(); i++){
    if(links[i].cpi == gravityMax){
      int node_a = links[i].community_a;
      int node_b = links[i].community_b;

      map<int, int>::iterator iter_a = mergeList.find(node_a);
      map<int, int>::iterator iter_b = mergeList.find(node_b);

      if(iter_a != mergeList.end() || iter_b != mergeList.end()){
        if(iter_a == mergeList.end()){
          mergeList.insert(pair<int, int> (node_a, iter_b->second));
        } else {
          mergeList.insert(pair<int, int> (node_b, iter_a->second));
        }
      } else {
        mergeList.insert(pair<int, int> (node_a, node_a));
        mergeList.insert(pair<int, int> (node_b, node_a));
      }
    }
  }
  return mergeList;
}

int calculationCommunityNumber(map<int,Node> nodes){
  set<int> communityCache;

  for(map<int, Node>::iterator iter = nodes.begin(); iter != nodes.end(); iter++){
    communityCache.insert(iter->second.getCommunityTag());
  }

  return communityCache.size();
}
