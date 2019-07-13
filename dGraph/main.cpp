#include <QCoreApplication>
#include "../d_graph_topo.h"
#include <iostream>

void visit(VERTEX_NODE *n, void *data)
{
    std::cout << n->descr << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //构建图
    GRAPH_EDGE edges;
    edges.push_back(EDGE(0, 2));
    edges.push_back(EDGE(0, 1));
    edges.push_back(EDGE(2, 3));
    edges.push_back(EDGE(3, 0));

    DGraphTopology t(4, edges);
    t.setVisitFunc(visit);

    t.DFSTraverse();

    return a.exec();
}
