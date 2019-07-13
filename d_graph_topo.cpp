#include "d_graph_topo.h"
#include <QDebug>
#include <queue>
#include <sstream>
#include <QFile>

bool DGraphTopology::readTopoFile(const QString& filename, int &vnum, GRAPH_EDGE &edges)
{
	QFile topo_f(filename);

	if (!topo_f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << QString("open file %1 failed.").arg(filename);
		return false;
	}
	QTextStream in(&topo_f);
	while(!in.atEnd())
	{
		QString line = in.readLine();
		QStringList l = line.split(',');
		if (l.count() == 1)
		{
			vnum = QString(l.at(0)).toInt();
		}
		else if (l.count() == 2)
		{
			edges.push_back(EDGE(QString(l.at(0)).toInt(), QString(l.at(1)).toInt()));
		}
	}
	return true;
}

DGraphTopology::DGraphTopology(int vnum, const GRAPH_EDGE &edges)
{
    buildGraph(vnum, edges);
}



void DGraphTopology::buildGraph(int vnum, const GRAPH_EDGE &edges)
{
	int tail, head;
	nodesGraph.adjArray = std::vector<VERTEX_NODE>(vnum);

	for (int i = 0; i < vnum; i++)
	{
		std::stringstream ss;
		ss << "node" << i;
		ss >> nodesGraph.adjArray[i].descr;
	}

    foreach(auto it, edges)
	{
		tail = it.first;
		head = it.second;
        VERTEX_NODE& node = nodesGraph.adjArray[tail];
		EDGE_NODE *e = new EDGE_NODE;
		e->adjvex = head;
		e->data = 0;
		e->next = node.firstEdge;

		node.firstEdge = e;
	}
	nodesGraph.vnum = vnum;
	visited = new bool[vnum];
}

DGraphTopology::DGraphTopology(const QString& filename)
{
	int vnum = 0;
	GRAPH_EDGE edges;

	if (readTopoFile(filename, vnum, edges))
        buildGraph(vnum, edges);
}

void DGraphTopology::setVertexNodeData(int vex, void *data)
{
	VERTEX_NODE& node = nodesGraph.adjArray[vex];
	node.data = data;
}
void DGraphTopology::setEdgeNodeData(int tail, int head,  void *data)
{
	VERTEX_NODE& node = nodesGraph.adjArray[tail];

	for(EDGE_NODE * e = node.firstEdge; e; e = e->next) {
		if (e->adjvex == head) {
			e->data = data;
			break;
		}
	}
}

void DGraphTopology::setVisitFunc(GRAPH_VISIT_FUNC f) {
	visitFunc = f;
}

int DGraphTopology::inDegree(int vex)
{
	int degree = 0;
	for (int i=0; i < nodesGraph.vnum; i++)
	{
		VERTEX_NODE* n = &nodesGraph.adjArray[i];
		for(EDGE_NODE *e = n->firstEdge; e; e = e->next)
			if (e->adjvex == vex) degree++;
	}
	return degree;
}
int DGraphTopology::outDegree(int vex)
{
	int degree = 0;
	VERTEX_NODE* node = &nodesGraph.adjArray[vex];
	for(EDGE_NODE *e = node->firstEdge; e; e = e->next) degree++;
	return degree;
}

void DGraphTopology::printDegrees()
{
	for (int i = 0; i < nodesGraph.vnum; i++)
	{
		qDebug() << "node" << i << " indegree " << inDegree(i) << ", outdegree " << outDegree(i);
	}
}

void DGraphTopology::DFS(int vex)
{
	visited[vex] = true;
	VERTEX_NODE *n = &nodesGraph.adjArray[vex];
	visitFunc(n, nullptr);
	for (EDGE_NODE* e = n->firstEdge; e; e = e->next) if (!visited[e->adjvex]) DFS(e->adjvex);
}

void DGraphTopology::DFSTraverse()
{
	qDebug() << "start DFSTraverse.";
	for(int i=0; i<nodesGraph.vnum; i++) visited[i] = false;

	for (int i=0; i<nodesGraph.vnum; i++) {
		if (!visited[i]) {
			DFS(i);
		}
	}
	qDebug() << "end DFSTraverse.";
}
void DGraphTopology::BFSTraverse()
{
	qDebug() << "start BFSTraverse.";
	for(int i=0; i<nodesGraph.vnum; i++) visited[i] = false;
	std::queue<int> nodeQueue;
	for (int i = 0; i < nodesGraph.vnum; i++)
	{
		if (!visited[i])
		{
			nodeQueue.push(i);
		}
		while(!nodeQueue.empty())
		{
			int vex = nodeQueue.front(); nodeQueue.pop();
			VERTEX_NODE *n = &nodesGraph.adjArray[vex];
			if (!visited[vex])
			{
			    visitFunc(n, nullptr);
			    visited[vex] = true;
			}
			for(EDGE_NODE *e = n->firstEdge; e; e = e->next) if(!visited[e->adjvex]) nodeQueue.push(e->adjvex);
		}
	}
	qDebug() << "end BFSTraverse.";
}
