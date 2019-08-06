#include <iostream>
#include <cstdlib>
#include "src/pretreatment/filehandle.h"
#include "src/algorithm/cpi.h"

using namespace std;

int main()
{
  string name = "Metadata_TD";
  char intercept = 'A'; 
  int number = 500;
  bool connected = true;
  bool sequence = true;
  char nodetype = 'B';

  Bipartite BipartiteNetwork = getBipartite( name, intercept, number, connected, sequence);
  Unipartite UnipartiteNetwork = getUnipartite( name, intercept, number, connected, sequence, nodetype);

  const vector<Edge> bipartiteEdgeCache = BipartiteNetwork.getEdges();        //计算GC用二分网络原始边连接信息
  const vector<Edge> unipartiteEdgeCache = UnipartiteNetwork.getEdges();      //计算模块度用投影网络边连接信息
  
  vector<Edge> edgeCache = UnipartiteNetwork.getEdges();                      //社区合并过程记录边连接信息
  map<int,Node> nodeCache = UnipartiteNetwork.getNodes();                     //社区合并过程记录节点信息

  //初始化 degree of nodeCache 与 gc of edgeCache
  for(int i=0; i<edgeCache.size(); i++){
    int node_a = edgeCache[i].getNodeA();
    int node_b = edgeCache[i].getNodeB();

    nodeCache[node_a].addDegree();
    nodeCache[node_b].addDegree();

    double bpi = calculationBasePolymerization(bipartiteEdgeCache, nodetype, node_a, node_b);
    edgeCache[i].setWeight(bpi);
  }

  int communityNumber = nodeCache.size();                                     //当前社区数目
  double modularity = calculationModularity(nodeCache, unipartiteEdgeCache);  //当前的模块度
  double modularityMax = modularity;                                          //模块度最大值
  vector<double> modularityCache;                                             //模块度缓存
  modularityCache.push_back(modularity);
 
  while(communityNumber > 1){
    map<int, int> mergeList = calculationMergeList(edgeCache);
    map<string, double> edgeUpdate;

    //更新nodeCache
    for(map<int, Node>::iterator iter_node = nodeCache.begin(); iter_node != nodeCache.end(); iter_node++){
      int original_tag = iter_node->second.getCommunityTag();
      map<int,int>::iterator iter_cache = mergeList.find(original_tag);
      int current_tag = iter_cache != mergeList.end() ? iter_cache->second : original_tag;
      iter_node->second.addListTag(current_tag);
    }

    //更新edgeCache
    for(int i=0; i<edgeCache.size(); i++){
      const int node_a = edgeCache[i].getNodeA();
      const int node_b = edgeCache[i].getNodeB();
      map<int,int>::iterator iter_a = mergeList.find(node_a);
      map<int,int>::iterator iter_b = mergeList.find(node_b);

      if(iter_a == mergeList.end() && iter_b == mergeList.end()){ 
        continue; 
      } else {
        const int key_a = iter_a == mergeList.end() ? node_a : iter_a->second;
        const int key_b = iter_b == mergeList.end() ? node_b : iter_b->second;
        const string key_edge = key_a < key_b ? to_string(key_a)+"_"+to_string(key_b) : to_string(key_b)+"_"+to_string(key_a);
        map<string, double>::iterator iter_edge = edgeUpdate.find(key_edge);
        if(iter_edge != edgeUpdate.end() || key_a == key_b){
          edgeCache.erase(edgeCache.begin()+i);
          i--;
        } else {
          double cpi = calculationCommunityPolymerization(nodeCache, bipartiteEdgeCache, nodetype, key_a, key_b);
          edgeCache[i].setWeight(cpi);
          edgeUpdate.insert(pair<string,double>(key_edge, cpi));
        }
      }
    }

    //更新当前模块度相关
    modularity = calculationModularity(nodeCache, unipartiteEdgeCache);
    modularityMax = modularityMax > modularity ? modularityMax : modularity;
    modularityCache.push_back(modularity);

    communityNumber = communityNumber - calculationMergeNumber(mergeList);
  }

  cout << modularityMax << endl;

  return 0;
}