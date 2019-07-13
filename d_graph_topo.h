#ifndef JOINT_GRAPH_H
#define JOINT_GRAPH_H
#include <vector>
#include <list>
#include <string>
#include <utility>

#include <QString>

/* 
本文件是有向图数据结构的实现，用于实现区域路口拓扑图。
图作为通用数据结构实现，和具体业务分离。
本实现采用邻接表法
*/

/*邻接边， 表达相邻顶点间的邻接关系 */
typedef struct EdgeNode {
	int adjvex;    //领接点索引，在
    void *data;    //指向边用户数据
	
	EdgeNode *next; //下一领接点
	EdgeNode() {adjvex = -1; data = 0; next = 0; }
}EDGE_NODE;
typedef struct VertexNode {
    void *data;   //指向顶点用户数据
	std::string descr;
	EdgeNode *firstEdge;  //第一个邻接关系，按顺时针形成链表
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
	std::vector<VERTEX_NODE> adjArray; //顶点向量表
	int vnum;     //顶点数
	int edgenum;  //邻接边数

}DGRAPH_ADJ_LIST;

typedef std::pair<int, int> EDGE; //按顺序<尾节点号，首节点号>

typedef std::list<EDGE> GRAPH_EDGE;

typedef void (*GRAPH_VISIT_FUNC)(VERTEX_NODE*, void*);

class DGraphTopology {

public:
    DGraphTopology() {}
	DGraphTopology(int vnum, const GRAPH_EDGE &edges);
	DGraphTopology(const QString&);
	void setVertexNodeData(int vex, void *data);                   //为顶点附着用户数据
	void setEdgeNodeData(int tail, int head, void *data);          //为边附着用户数据
	int inDegree(int vex);                                         //获取顶点入度
	int outDegree(int vex);                                        //获取顶点出度
	void DFSTraverse();                                            //深度优先遍历
	void BFSTraverse();                                            //广度优先遍历

	void printDegrees();

	void setVisitFunc(GRAPH_VISIT_FUNC);                           //设置访问函数
private:
    void buildGraph(int vnum, const GRAPH_EDGE &edges);
protected:
	void DFS(int vex);                                             //从顶点vex深度优先搜索
	DGRAPH_ADJ_LIST nodesGraph;
	bool *visited;                                                 //遍历标记
	GRAPH_VISIT_FUNC visitFunc;

	bool readTopoFile(const QString& filename, int &vnum, GRAPH_EDGE &edges);
};

#endif
