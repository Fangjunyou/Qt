#ifndef JOY_LOG_WRITER_H
#define JOY_LOG_WRITER_H
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QTimer>
#include <QMutex>
#include <QByteArray>

/* ÿclient entity����ռ��һ��JoyLogWriter���� */

class LogItem {
public:
	enum JOY_LOG_TYPE {
		SYSTEM = 0,   //ϵͳ
		TRAFFIC,      //�г�
		SCHEDULE,     //����
		NETWORK,      //����
		WARNNING      //�澯
    };
public:
	LogItem(const QDateTime& dt, const QString& str, JOY_LOG_TYPE type) { this->dt = dt; this->info = str; this->type = type; } //��־��Ŀ��ʽ:����ʱ�� + �ո� + ��־���� + ��־����
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
    //��־�ļ����ơ������������¼������Ĭ��500�����Զ��־û�ʱ������Ĭ��1�졢��־�ļ���С���ƣ�Ĭ��5M����־�ĸ��ڵ�
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
	QByteArray  logBuffer;          //��־������
	QMutex lock_buffer;            //����־��������������
	int count;                     //log_stream�е�ǰ��־��Ŀ��
	int max_count;                 //��count >= max_count���־û���־�ļ�
	int interval_save;             //�Զ��־û���־�ļ�ʱ����, ��λ����
	QTimer save_auto_timer;        //�Զ����涨ʱ��
	QTimer everyday_timer;         //ÿ�ն�ʱ����
	QString current_file_name;     //��ǰ��־�ļ�
	JOY_FILE_SIZE size_limit;      //��־�ļ���С����
};

JoyLogWriter& operator<<(JoyLogWriter&, const LogItem&);

#endif
