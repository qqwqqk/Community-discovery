#include"method.h"
#include "../datastructure/type.h"
#include <fstream>
#include <cstdlib>
using namespace std;

Bipartite getBipartite(string name, char intercept, int number, bool connected, bool sequence){
  const string split = "_";
  const string sIntercept(1,intercept);
  const string sNumber = number > 0 ? to_string(number) : "0";
  const string sConnected = connected ? "C" : "UC";
  const string sSequence = sequence ? "S" : "US";
  const string netpath = "netdata/" + name + split + sIntercept + sNumber + sConnected + sSequence + ".txt";

  ifstream infile;
  string line;
  const char dilem = ',';
  vector<Edge> edgeCache;
  map<int,int> nodeCacheA, nodeCacheB;
  map<int,int>::iterator iter;

  //按行读取TXT文件，并解析
  infile.open(netpath, ios::in);
  if(!infile){ 
    cout<< "read from meta data" <<endl;
    return  pretreatmentBipartite(name, intercept, number, connected, sequence);
  } else {
    cout<< "read from net data" <<endl;
  }

  while(!infile.eof()){
    getline(infile, line);
    int linePos = 0;
    int lineSize = 0;
    string cache;
    vector<int> array;

    for(int i = 0 ; i < line.size(); i++){
      if(line[i] == dilem){
        lineSize = i - linePos;
        cache = line.substr(linePos, lineSize);
        array.push_back(stoi(cache));
        linePos = i+1;
      }
    }
    cache = line.substr(linePos, line.size() - linePos);
    if(cache.size()>0){ array.push_back(stoi(cache)); }

    //记录两人种类型节点数量并将解析的内容写入边对象缓存
    if(array.size() > 2){
      iter = nodeCacheA.find(array[1]);
      if(iter == nodeCacheA.end()){
        pair<int,int> value(array[1],0);
        nodeCacheA.insert(value);
      }
      iter = nodeCacheB.find(array[2]);
      if(iter == nodeCacheB.end()){
        pair<int,int> value(array[2],0);
        nodeCacheB.insert(value);
      }
      edgeCache.push_back(Edge(array[1],array[2]));
    }
  }
  infile.close();

  //验证解析情况
  //cout << "NodeCacheA\t" << nodeCacheA.size() << endl;
  //cout << "NodeCacheB\t" << nodeCacheB.size() << endl;
  //cout << "EdgeCache\t" << edgeCache.size() << endl << endl;

  //写入Bipartite

  Bipartite Network(name);
  for(int i=0; i<edgeCache.size(); i++){
    Network.addEdge(edgeCache[i]);
  }
  for(iter = nodeCacheA.begin(); iter != nodeCacheA.end(); iter++){
    Network.addNodeA(Node(iter->first));
  }
  for(iter = nodeCacheB.begin(); iter != nodeCacheB.end(); iter++){
    Network.addNodeB(Node(iter->first));
  }

  return Network;
}


Bipartite pretreatmentBipartite(string name, char intercept, int number, bool connected, bool sequence){
  const string split = "_";
  const string sIntercept(1,intercept);
  const string sNumber = number > 0 ? to_string(number) : "0";
  const string sConnected = connected ? "C" : "UC";
  const string sSequence = sequence ? "S" : "US";
  const string metapath = "metadata/" + name + ".txt";
  const string netpath = "netdata/" + name + split + sIntercept + sNumber + sConnected + sSequence + ".txt";

  ifstream infile;
  string line;
  const char dilem = ',';
  vector<Edge> edgeCache;
  map<int,int> nodeCacheA, nodeCacheB;
  map<int,int>::iterator iter;

  //按行读取TXT文件，并解析
  infile.open(metapath, ios::in);
  if(!infile){ cout<< "can't find meta file! "<<endl; exit(0); }
  while(!infile.eof()){
    getline(infile, line);
    int linePos = 0;
    int lineSize = 0;
    string cache;
    vector<int> array;

    for(int i = 0 ; i < line.size(); i++){
      if(line[i] == dilem){
        lineSize = i - linePos;
        cache = line.substr(linePos, lineSize);
        array.push_back(stoi(cache));
        linePos = i+1;
      }
    }
    cache = line.substr(linePos, line.size() - linePos);
    if(cache.size()>0){ array.push_back(stoi(cache)); }

    //记录两人种类型节点数量并将解析的内容写入边对象缓存
    if(array.size() > 2){
      iter = nodeCacheA.find(array[1]);
      if(iter == nodeCacheA.end()){
        pair<int,int> value(array[1],0);
        nodeCacheA.insert(value);
      }
      iter = nodeCacheB.find(array[2]);
      if(iter == nodeCacheB.end()){
        pair<int,int> value(array[2],0);
        nodeCacheB.insert(value);
      }
      edgeCache.push_back(Edge(array[1],array[2]));
    }
  }
  infile.close();

  //验证解析情况
  //cout << "NodeCacheA\t" << nodeCacheA.size() << endl;
  //cout << "NodeCacheB\t" << nodeCacheB.size() << endl;
  //cout << "EdgeCache\t" << edgeCache.size() << endl << endl;

  //截取子网络
  if(number >= 0){
    //截取边信息
    switch(intercept){    
      case 'A':
        if(nodeCacheA.size() <= number){
          break;
        } else {
          for(int i=0; i < edgeCache.size();){
            if(edgeCache[i].getNodeA() > number){
              edgeCache.erase(edgeCache.begin()+i);
            } else {
              i++;
            }
          }
          break;
        }
      case 'B':
        if(nodeCacheB.size() <= number){
          break;
        } else {
          for(int i=0; i < edgeCache.size();){
            if(edgeCache[i].getNodeB() > number){
              edgeCache.erase(edgeCache.begin()+i);
            } else {
              i++;
            }
          }
          break;
        }
      case 'E':
        if(edgeCache.size() <= number){
          break;
        } else {
          edgeCache.erase(edgeCache.begin()+number,edgeCache.end());
          break;
        }
      default:
        cout << "Invalid input, use the default E" << endl;
        if(edgeCache.size() <= number){
          break;
        } else {
          edgeCache.erase(edgeCache.begin()+number,edgeCache.end());
          break;
        }
    }
    //截取点信息
    nodeCacheA.clear();
    nodeCacheB.clear();
    for(int i=0;i<edgeCache.size();i++){
      iter = nodeCacheA.find(edgeCache[i].getNodeA());
      if(iter == nodeCacheA.end()){
        pair<int,int> value(edgeCache[i].getNodeA(),0);
        nodeCacheA.insert(value);
      }
      iter = nodeCacheB.find(edgeCache[i].getNodeB());
      if(iter == nodeCacheB.end()){
        pair<int,int> value(edgeCache[i].getNodeB(),0);
        nodeCacheB.insert(value);
      }
    }
  }

  //验证截取情况
  //cout << "NodeCacheA\t" << nodeCacheA.size() << endl;
  //cout << "NodeCacheB\t" << nodeCacheB.size() << endl;
  //cout << "EdgeCache\t" << edgeCache.size() << endl << endl;

  //连通性检测，划分全连通子图
  if(connected){
    struct SubBipartiteCache{
      vector<Edge> subEdges;
      map<int,int> subNodeA, subNodeB;
    };

    vector<SubBipartiteCache> Networks;
    int index = 0, tag=0, maxA=0, maxB=0, maxE=0;

    //将全部子图存入subCache
    while(!nodeCacheA.empty() && !nodeCacheB.empty()){
      SubBipartiteCache subCache;

      iter = nodeCacheA.begin();
      subCache.subNodeA.insert(pair<int,int> (iter->first, 0));
      nodeCacheA.erase(iter);
      int length = 0;
      while(1){
        length = subCache.subNodeB.size();
        //遍历边集合，将与A类型节点有连接的所有B类型节点转移存储到子图B类节点集合，将连接的边转移存储到子图的边集合
        for(int i=0; i<edgeCache.size();){
          iter = subCache.subNodeA.find(edgeCache[i].getNodeA());
          if(iter != subCache.subNodeA.end()){
            //在子图类型B节点集合中添加
            iter = subCache.subNodeB.find(edgeCache[i].getNodeB());
            if(iter == subCache.subNodeB.end()){ subCache.subNodeB.insert(pair<int, int> (edgeCache[i].getNodeB(), 0)); }
            //删除原类型B节点集合中对应数据
            iter = nodeCacheB.find(edgeCache[i].getNodeB());
            if(iter != nodeCacheB.end()){ nodeCacheB.erase(iter); }
            //转移边集合中对应数据到子图边集合
            subCache.subEdges.push_back(edgeCache[i]);
            edgeCache.erase(edgeCache.begin()+i);
          } else {
            i++;
          }
        }

        //如果已经不存在与A类型相连的B类型节点，退出循环
        if(subCache.subNodeB.size() == length ){ break; }

        //遍历边集合，将与B类型节点有连接的所有B类型节点转移存储到子图A类节点集合，将连接的边转移存储到子图的边集合
        for(int i=0; i<edgeCache.size();){
          iter = subCache.subNodeB.find(edgeCache[i].getNodeB());
          if(iter != subCache.subNodeB.end()){
            //在子图类型A节点集合中添加
            iter = subCache.subNodeA.find(edgeCache[i].getNodeA());
            if(iter == subCache.subNodeA.end()){ subCache.subNodeA.insert(pair<int, int> (edgeCache[i].getNodeA(), 0)); }
            //删除原类型A节点集合中对应数据
            iter = nodeCacheA.find(edgeCache[i].getNodeA());
            if(iter != nodeCacheA.end()){ nodeCacheA.erase(iter); }
            //转移边集合中对应数据到子图边集合
            subCache.subEdges.push_back(edgeCache[i]);
            edgeCache.erase(edgeCache.begin()+i);
          } else {
            i++;
          }
        }
      }

      maxA = maxA > subCache.subNodeA.size() ? maxA : subCache.subNodeA.size();
      maxB = maxB > subCache.subNodeB.size() ? maxB : subCache.subNodeB.size();
      maxE = maxE > subCache.subEdges.size() ? maxE : subCache.subEdges.size();

      Networks.push_back(subCache);
    }

    //寻找最大全连通子图的下标
    switch(intercept){
      case 'A':
        for(int i=0; i<Networks.size(); i++){
          if(Networks[i].subNodeA.size() == maxA){
            index = i; break;
          }
        }
        break;
      case 'B':
        for(int i=0; i<Networks.size(); i++){
          if(Networks[i].subNodeB.size() == maxB){
            index = i; break;
          }
        }
        break;
      case 'E':
        for(int i=0; i<Networks.size(); i++){
          if(Networks[i].subEdges.size() == maxE){
            index = i; break;
          }
        }
        break;
      default:
        cout << "Invalid input, use the default E" << endl;
        for(int i=0; i<Networks.size(); i++){
          if(Networks[i].subEdges.size() == maxE){
            index = i; break;
          }
        }
        break;

    }

    //将最大全连通子图写入缓存
    edgeCache.clear();
    nodeCacheA.clear();
    nodeCacheB.clear();
    edgeCache = Networks[index].subEdges;
    iter = Networks[index].subNodeA.begin();
    tag = 1;
    while(iter != Networks[index].subNodeA.end()){
      nodeCacheA.insert(pair<int, int> (iter->first, tag));
      iter++; tag++;         
    }
    iter = Networks[index].subNodeB.begin();
    tag = 1;
    while(iter != Networks[index].subNodeB.end()){
      nodeCacheB.insert(pair<int, int> (iter->first, tag));
      iter++; tag++;         
    }
  }

  //验证连通情况
  //cout << "NodeCacheA\t" << nodeCacheA.size() << endl;
  //cout << "NodeCacheB\t" << nodeCacheB.size() << endl;
  //cout << "EdgeCache\t" << edgeCache.size() << endl << endl;

  //重新排序
  if(sequence){
    int reSortA, reSortB;
    for(int i=0; i<edgeCache.size(); i++){
      reSortA = nodeCacheA[edgeCache[i].getNodeA()];
      reSortB = nodeCacheB[edgeCache[i].getNodeB()];
      edgeCache[i] = Edge(reSortA,reSortB);
    }
  }

  //验证排序情况
  //cout << "NodeCacheA\t" << nodeCacheA.size() << endl;
  //cout << "NodeCacheB\t" << nodeCacheB.size() << endl;
  //cout << "EdgeCache\t" << edgeCache.size() << endl << endl;


  //写入Bipartite，并输出到TXT文件
  ofstream outfile( netpath , ios::out);
  if(!outfile){ cout<<"file open error!"<<endl; exit(1); } 

  Bipartite Network(name);
  for(int i=0; i<edgeCache.size(); i++){
    Network.addEdge(edgeCache[i]);
    outfile << i << ',' << edgeCache[i].getNodeA() << ',' << edgeCache[i].getNodeB() << '\n';
  }
  for(iter = nodeCacheA.begin(); iter != nodeCacheA.end(); iter++){
    Network.addNodeA(Node(iter->second));
  }
  for(iter = nodeCacheB.begin(); iter != nodeCacheB.end(); iter++){
    Network.addNodeB(Node(iter->second));
  }

  outfile.close();

  return Network;
}