#include <iostream>
#include <cstdlib>
#include "src/pretreatment/filehandle.h"
#include "src/algorithm/cpi.h"

using namespace std;

int main()
{
  string name = "Metadata_ST";
  char intercept = 'E'; 
  int number = 2500;
  bool connected = true;
  bool sequence = true;
  char nodetype = 'A';

  Bipartite BipartiteNetwork = getBipartite( name, intercept, number, connected, sequence);
  Unipartite UnipartiteNetwork = getUnipartite( name, intercept, number, connected, sequence, nodetype);

  const vector<Edge> bipartiteEdgeCache = BipartiteNetwork.getEdges();        //计算CPI用二分网络原始边连接信息
  const vector<Edge> unipartiteEdgeCache = UnipartiteNetwork.getEdges();      //计算模块度用投影网络边连接信息
  
  map<int,Node> nodeCache = UnipartiteNetwork.getNodes();                     //社区合并过程记录节点信息
  vector<Link> linkCache;                                                     //社区合并过程记录连接信息
  map<string, double> linkUpdate;                                             //单次合并过程更新的连接

  //初始化 degree of nodeCache 与 bpi of linkCache
  vector<Edge> initCache = UnipartiteNetwork.getEdges();  
  for(int i=0; i<initCache.size(); i++){
    int node_a = initCache[i].getNodeA();
    int node_b = initCache[i].getNodeB();

    nodeCache[node_a].addDegree();
    nodeCache[node_b].addDegree();

    double bpi = calculationBasePolymerization(bipartiteEdgeCache, nodetype, node_a, node_b);
    Link item = {community_a: node_a, community_b: node_b, cpi:bpi};
    linkCache.push_back(item);
  }

  int communityNumber = nodeCache.size();                                     //当前社区数目
  double modularity = calculationModularity(nodeCache, unipartiteEdgeCache);  //当前的模块度
  vector<double> modularityCache;                                             //模块度缓存
  modularityCache.push_back(modularity);

  printProgress(modularityCache.size() - 1, communityNumber, modularity);     //输出初始信息

  while(communityNumber > 1){
    linkUpdate.clear();
    map<int, int> mergeList = calculationMergeList(linkCache);
  
    //更新nodeCache
    for(map<int, Node>::iterator iter_node = nodeCache.begin(); iter_node != nodeCache.end(); iter_node++){
      int original_tag = iter_node->second.getCommunityTag();
      map<int,int>::iterator iter_cache = mergeList.find(original_tag);
      int current_tag = iter_cache != mergeList.end() ? iter_cache->second : original_tag;
      iter_node->second.addListTag(current_tag);
    }

    //更新linkCache
    for(int i=0; i<linkCache.size(); i++){
      map<int,int>::iterator iter_a = mergeList.find(linkCache[i].community_a);
      map<int,int>::iterator iter_b = mergeList.find(linkCache[i].community_b);

      if(iter_a == mergeList.end() && iter_b == mergeList.end()){ continue; } 
      
      const int key_a = iter_a == mergeList.end() ? linkCache[i].community_a : iter_a->second;
      const int key_b = iter_b == mergeList.end() ? linkCache[i].community_b : iter_b->second;
      const string key_link = key_a < key_b ? to_string(key_a)+"_"+to_string(key_b) : to_string(key_b)+"_"+to_string(key_a);
      map<string, double>::iterator iter_link = linkUpdate.find(key_link);

      if(iter_link != linkUpdate.end() || key_a == key_b){
        linkCache.erase(linkCache.begin() + i);
        i--;
      } else {
        double cpi = calculationCommunityPolymerization(nodeCache, bipartiteEdgeCache, nodetype, key_a, key_b);
        linkCache[i].community_a = key_a; linkCache[i].community_b = key_b; linkCache[i].cpi = cpi;
        linkUpdate.insert(pair<string,double>(key_link, cpi));
      }
    }

    //更新当前社区数量以及模块度相关
    communityNumber = calculationCommunityNumber(nodeCache);
    modularity = calculationModularity(nodeCache, unipartiteEdgeCache);
    modularityCache.push_back(modularity);

    printProgress(modularityCache.size() - 1, communityNumber, modularity);     //输出合并进度
  }

  printCommunity(modularityCache, nodeCache, name, intercept, number, connected, sequence, nodetype);                                   //输出社区划分结果

  return 0;
}