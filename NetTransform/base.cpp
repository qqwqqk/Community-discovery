#include "base.h"

Node::Node(int id, int dgree){
	ID = id; Dgree = dgree; 
	EdgeID.clear();
	next = NULL;
}
int Node::GetNodeID(){
	return ID;
}
int Node::GetNodeDgree(){
	return Dgree;
}
void Node::AddEdgeID(int eid){
	Dgree++;
	EdgeID.push_back(eid);
}
void Node::PrintNode(){
	//cout <<"NodeID:" <<ID << "\tNodeDgree" << Dgree << "\tEdgeInformations:";
	for (int i = 0; i < Dgree; i++){
		cout << '\t' << EdgeID[i];
	}
	cout << endl;
}

Edge::Edge(int id, int dgree){
	ID = id; Dgree = dgree;
	NodeID.clear();
	next = NULL;
}
int Edge::GetEdgeID(){
	return ID;
}
int Edge::GetEdgeDgree(){
	return Dgree;
}
void Edge::AddNodeID(int nid){
	Dgree++;
	NodeID.push_back(nid);
}
void Edge::PrintEdge(){
	//cout << "EdgeID:" << ID << "\tEdgeDgree" << Dgree << "\tNodeInformations:";
	for (int i = 0; i < Dgree; i++){
		cout << '\t' << NodeID[i];
	}
	cout << endl;
}

Link::Link(int id, int node, int edge,double weight){
	ID = id; NodeID = node; EdgeID = edge;
	Weight = weight;
	next = NULL;
}
int Link::GetLinkID(){
	return ID;
}
int Link::GetNodeID(){
	return NodeID;
}
int Link::GetEdgeID(){
	return EdgeID;
}
void Link::PrintLink(){
	cout << ID << '\t' << NodeID << '\t' << EdgeID << endl;
}

Network::Network(int id, string name){
	ID = id; Name = name; 
	NodeNumber = 0; EdgeNumber = 0; LinkNumber = 0;
	Nodes = new Node;
	Edges = new Edge;
	Links = new Link;
	next = NULL;
}
Network::~Network(){
	delete Nodes;
	delete Edges;
	delete Links;
}

void Network::T_initialize(string netname){
	Name = netname;

	Node *pn = Nodes;
	Edge *pe = Edges;
	Link *pl = Links;

	// 以读模式打开文件
	ifstream ifile;
	string str = Name + ".txt";

	ifile.open(str);
	if (!ifile.is_open()){
		cout << "error!" << endl;
	}

	NodeNumber = 0;
	EdgeNumber = 0;
	LinkNumber = 0;

	string L_cache;
	string temp;

	int lid, nid, eid;
	int begin, end, tag;

	while (!ifile.eof()){
		getline(ifile, L_cache);

		if (L_cache.size() == 0){break;}
		begin = 0, end = 0, tag = 0;

		for (int i = 0; i <= (int)L_cache.size(); i++) {
			if (L_cache[i] == '\t' || i == (int)L_cache.size()) {
				switch (tag){
				case 0:
					tag++;
					begin = 0;
					end = i;
					temp = L_cache.substr(begin, end - begin);
					lid = atoi(temp.c_str());
					break;
				case 1:
					tag++;
					begin = end + 1;
					end = i;
					temp = L_cache.substr(begin, end - begin);
					eid = atoi(temp.c_str());
					break;
				case 2:
					tag++;
					begin = end + 1;
					end = i;
					temp = L_cache.substr(begin, end - begin);
					nid = atoi(temp.c_str());
					break;
				default:
					cout << "error" << endl;
					break;
				}
			}
		}

		//建立链接表
		pl->next = new Link(lid, nid, eid);
		LinkNumber++;
		pl = pl->next;

		//建立边表
		pe = Edges;
		while (1){
			if (pe->next == NULL){
				pe->next = new Edge(eid, 0);
				EdgeNumber++;
				pe->next->AddNodeID(nid);
				break;
			}
			if (pe->next->ID == eid){
				pe->next->AddNodeID(nid);
				break;
			}
			pe = pe->next;
		}

		//建立节点表
		pn = Nodes;
		while (1){
			if (pn->next == NULL){
				pn->next = new Node(nid, 0);
				NodeNumber++;
				pn->next->AddEdgeID(eid);
				break;
			}
			if (pn->next->ID == nid){
				pn->next->AddEdgeID(eid);
				break;
			}
			pn = pn->next;
		}

	}
	
	// 关闭打开的文件
	ifile.close();

	Nodes = Nodes->next;
	Edges = Edges->next;
	Links = Links->next;
}
void Network::S_initialize(string netname){
	Node *pn = Nodes;
	Edge *pe = Edges;
	Link *pl = Links;
	string sql;
	_bstr_t str;
	ADOLINK record;

	_variant_t vid, vnodeid, vedgeid;
	int id, nodeid, edgeid;

	Name = netname;

	record.Connect();
	_RecordsetPtr m_pRecordset;

	sql = "select * from [" + Name + "]";				//初始化链接信息
	str = sql.c_str();
	m_pRecordset = record.GetRecordset(str);

	if (!m_pRecordset->BOF)
		m_pRecordset->MoveFirst();
	else{
		cout << "链接表空！" << endl;
	}
	while (!m_pRecordset->adoEOF){
		try{
			vid = m_pRecordset->GetCollect("ID");
			id = vid;
			vnodeid = m_pRecordset->GetCollect("womans");
			nodeid = vnodeid;
			nodeid = nodeid<0 ? -1 : nodeid;
			vedgeid = m_pRecordset->GetCollect("events");
			edgeid = vedgeid;
			edgeid = edgeid<0 ? -1 : edgeid;
		}
		catch (_com_error e){
			cout << e.Description() << endl;
		}

		//建立链接表
		m_pRecordset->MoveNext();
		pl->next = new Link(id, nodeid, edgeid);
		LinkNumber++;
		pl = pl->next;

		//建立边表
		pe = Edges;
		while (1){
			if (pe->next == NULL){
				pe->next = new Edge(edgeid, 0);
				EdgeNumber++;
				pe->next->AddNodeID(nodeid);
				break;
			}
			if (pe->next->ID == edgeid){
				pe->next->AddNodeID(nodeid);
				break;
			}
			pe = pe->next;
		}

		//建立节点表
		pn = Nodes;
		while (1){
			if (pn->next == NULL){
				pn->next = new Node(nodeid, 0);
				NodeNumber++;
				pn->next->AddEdgeID(edgeid);
				break;
			}
			if (pn->next->ID == nodeid){
				pn->next->AddEdgeID(edgeid);
				break;
			}
			pn = pn->next;
		}


	}

	record.ExitConnect();

	Nodes = Nodes->next;
	Edges = Edges->next;
	Links = Links->next;
}

void Network::TransformToSuperGraphs(){
	string str = Name + "_SuperGraphs.txt";

	ofstream ofile;
	ofile.open(str);
	if (!ofile){
		abort();//打开失败，结束程序
	}

	Edge *pe = Edges;

	while (pe != NULL){
		for (int i = 0; i < (int)pe->NodeID.size(); i++){
			ofile << pe->NodeID[i] << '\t';
		}
		ofile << '\n';
		pe = pe->next;
	}

	ofile.close();

}
void Network::TransformToWeightGraphs(){
	string str = Name + "_WeightGraphs.txt";

	Link Net, *pl;
	Edge *pe = Edges;
	int node1, node2, count = 0;
	int EdgeNumber = 0;
	double weight;

	while (pe != NULL){
		weight = 1.0 / pe->Dgree;
		count = (int)pe->NodeID.size();
		for (int i = 0; i < count; i++){
			for (int j = i + 1; j < count; j++){
				node1 = pe->NodeID[i];
				node2 = pe->NodeID[j];
				pl = &Net;
				while (1){
					if (node1 == pl->EdgeID && node2 == pl->NodeID){
						pl->Weight = pl->Weight + weight;
						break;
					}
					if (node1 == pl->NodeID && node2 == pl->EdgeID){
						pl->Weight = pl->Weight + weight;
						break;
					}
					if(pl->next==NULL){
						pl->next = new Link(EdgeNumber++, node1, node2, weight);
						break;
					}
					pl = pl->next;
				}
			}
		}
		pe = pe->next;
	}

	ofstream ofile;
	ofile.open(str);
	if (!ofile){
		abort();//打开失败，结束程序
	}

	pl = Net.next;
	while (pl != NULL){
		//cout << pl->ID << '\t';
		ofile << pl->NodeID << '\t' << pl->EdgeID << '\t' << pl->Weight << '\n';
		pl = pl->next;
	}

	ofile.close();
}

void Network::PrintNodes(){
	cout << "网络中共有" << NodeNumber << "个节点，详细信息如下：" << endl;
	Node *pn = Nodes;
	while (pn){
		pn->PrintNode();
		pn = pn->next;
	}
}
void Network::PrintEdges(){
	cout << "网络中共有" << EdgeNumber << "条边，详细信息如下：" << endl;
	Edge *pe = Edges;
	while (pe){
		pe->PrintEdge();
		pe = pe->next;
	}
}
void Network::PrintLinks(){
	cout << "网络中共有" << LinkNumber << "条连接，详细信息如下：" << endl;
	Link *pl = Links;
	while (pl){
		pl->PrintLink();
		pl = pl->next;
	}
}
void Network::PrintNet(){
	this->PrintNodes();
	this->PrintEdges();
	this->PrintLinks();
}