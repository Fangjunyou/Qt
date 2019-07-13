#ifndef JOINT_GRAPH_H
#define JOINT_GRAPH_H
#include <vector>
#include <list>
#include <string>
#include <utility>

#include <QString>

/* 
���ļ�������ͼ���ݽṹ��ʵ�֣�����ʵ������·������ͼ��
ͼ��Ϊͨ�����ݽṹʵ�֣��;���ҵ����롣
��ʵ�ֲ����ڽӱ�
*/

/*�ڽӱߣ� ������ڶ������ڽӹ�ϵ */
typedef struct EdgeNode {
	int adjvex;    //��ӵ���������
    void *data;    //ָ����û�����
	
	EdgeNode *next; //��һ��ӵ�
	EdgeNode() {adjvex = -1; data = 0; next = 0; }
}EDGE_NODE;
typedef struct VertexNode {
    void *data;   //ָ�򶥵��û�����
	std::string descr;
	EdgeNode *firstEdge;  //��һ���ڽӹ�ϵ����˳ʱ���γ�����
	VertexNode() { data = 0; descr = std::string(); firstEdge = 0; }
	~VertexNode() { 
		EDGE_NODE *e = firstEdge;
		while(e)
		{
			firstEdge = e->next;
			delete e;
			e = firstEdge;
		}
	}
}VERTEX_NODE;

typedef struct DGraphAdjList{
	DGraphAdjList() { vnum = 0; edgenum = 0; }
	std::vector<VERTEX_NODE> adjArray; //����������
	int vnum;     //������
	int edgenum;  //�ڽӱ���

}DGRAPH_ADJ_LIST;

typedef std::pair<int, int> EDGE; //��˳��<β�ڵ�ţ��׽ڵ��>

typedef std::list<EDGE> GRAPH_EDGE;

typedef void (*GRAPH_VISIT_FUNC)(VERTEX_NODE*, void*);

class DGraphTopology {

public:
    DGraphTopology() {}
	DGraphTopology(int vnum, const GRAPH_EDGE &edges);
	DGraphTopology(const QString&);
	void setVertexNodeData(int vex, void *data);                   //Ϊ���㸽���û�����
	void setEdgeNodeData(int tail, int head, void *data);          //Ϊ�߸����û�����
	int inDegree(int vex);                                         //��ȡ�������
	int outDegree(int vex);                                        //��ȡ�������
	void DFSTraverse();                                            //������ȱ���
	void BFSTraverse();                                            //������ȱ���

	void printDegrees();

	void setVisitFunc(GRAPH_VISIT_FUNC);                           //���÷��ʺ���
private:
    void buildGraph(int vnum, const GRAPH_EDGE &edges);
protected:
	void DFS(int vex);                                             //�Ӷ���vex�����������
	DGRAPH_ADJ_LIST nodesGraph;
	bool *visited;                                                 //�������
	GRAPH_VISIT_FUNC visitFunc;

	bool readTopoFile(const QString& filename, int &vnum, GRAPH_EDGE &edges);
};

#endif
