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