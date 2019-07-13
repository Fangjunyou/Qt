# programs I writed based on Qt
----------------------------------------------
joy_log_writer.h/cpp:
A logger based on Qt. The direcotry of simpleLogger contains simple testing project in Qt creator.
Features:
1. interface easy to use
2. rename log file automatically according to format of "log_[hour]_[minute]_[second]_[microsecond].txt", for example, "log_21_11_52_377.txt".
3. switch log directory automatically at 00:00 every day morning, directory format according to [YYYYDDMM], for example, 20190807.
4. switch log file automatically once its size or number of log items exceeds limitation.

----------------------------------------------
d_graph_topo.h/cpp:
采用邻接表方法，实现数据结构有向图。
1. 有向图construtor的参数：顶点数，<尾顶点号，首顶点号>对表示的边表
2. 设置节点访问函数
3. 调用图深度遍历或广度遍历函数

使用示例：见dGraph/main.cpp
----------------------------------------------

