#ifndef JOY_LOG_WRITER_H
#define JOY_LOG_WRITER_H
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QTimer>
#include <QMutex>
#include <QByteArray>

/* 每client entity独立占有一个JoyLogWriter对象 */

class LogItem {
public:
	enum JOY_LOG_TYPE {
		SYSTEM = 0,   //系统
		TRAFFIC,      //行车
		SCHEDULE,     //调度
		NETWORK,      //网络
		WARNNING      //告警
    };
public:
	LogItem(const QDateTime& dt, const QString& str, JOY_LOG_TYPE type) { this->dt = dt; this->info = str; this->type = type; } //日志条目格式:日期时间 + 空格 + 日志类型 + 日志内容
	LogItem(const LogItem& other) { dt = other.dt; info = other.info; type = other.type; }
	LogItem& operator=(const LogItem& other) { dt = other.dt; info = other.info; type = other.type; return *this; }

	QDateTime dt;
	QString info;
	JOY_LOG_TYPE type;
};

class JoyLogWriter : public QObject {
	Q_OBJECT
public:
	enum JOY_FILE_SIZE{ ONE_M = 1048576, THREE_M = 3145728, FIVE_M = 5242880, EIGHT_M = 8388608 }; //log file size: 1M, 3M, 5M, 8M
public:
	friend JoyLogWriter& operator<<(JoyLogWriter&, const LogItem&);
public:
    //日志文件名称、缓冲区保存记录条数，默认500条、自动持久化时间间隔，默认1天、日志文件大小限制，默认5M，日志文父节点
    JoyLogWriter(const QString& name = ".", int buffsize = 500, int interval = 3600,  JOY_FILE_SIZE limit = FIVE_M, QObject* parent = 0);
	~JoyLogWriter() { flush(); }
	void flush();

protected slots:
	void autoSave();
	void everydayChange();

private:
	void writeLogFile();
	void changeCurrentFileName();
	void changeDayDir();
private:
	QByteArray  logBuffer;          //日志缓冲区
	QMutex lock_buffer;            //对日志缓冲区加锁控制
	int count;                     //log_stream中当前日志条目数
	int max_count;                 //当count >= max_count，持久化日志文件
	int interval_save;             //自动持久化日志文件时间间隔, 单位毫秒
	QTimer save_auto_timer;        //自动保存定时器
	QTimer everyday_timer;         //每日定时触发
	QString current_file_name;     //当前日志文件
	JOY_FILE_SIZE size_limit;      //日志文件大小限制
};

JoyLogWriter& operator<<(JoyLogWriter&, const LogItem&);

#endif
