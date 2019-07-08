#include "joy_log_writer.h"
#include <QDir>
#include <QDebug>
#include <QMutexLocker>

/*dirName����־����Ŀ¼����־Ŀ¼�°����ڽ�������Ŀ¼����Ŀ¼�°�ʱ�佨��־�ļ�*/
JoyLogWriter::JoyLogWriter(const QString& dirName, int buffsize, int interval, JOY_FILE_SIZE limit, QObject* parent) 
	: QObject(parent)
{
	QString dir_path = dirName;
	if (dir_path.right(1) != "\\")
	{
		dir_path += "\\";
	}
	QDir log_dir;
	if (!log_dir.cd(dir_path)) 
	{
		log_dir.mkdir(dir_path);
		log_dir.cd(dir_path);
	}
	QString subDir = QDateTime::currentDateTime().toString("yyyyddMM");
	if(!log_dir.cd(subDir))
	{
		log_dir.mkdir(subDir);
		log_dir.cd(subDir);
	}
	dir_path += subDir;
	dir_path += "\\";

	current_file_name = dir_path + "log_";
	current_file_name += QDateTime::currentDateTime().toString("hh_mm_ss_zzz");
	current_file_name += ".txt";

	qDebug() << current_file_name;

	max_count = buffsize;
	interval_save = interval * 1000;
	connect(&save_auto_timer, SIGNAL(timeout()), this, SLOT(autoSave()));
	save_auto_timer.setInterval(interval_save);
	save_auto_timer.start();

	/* ��ǰʱ�̵���һ��0ʱ�̴���һ�� */
	everyday_timer.setSingleShot(1);
	everyday_timer.setTimerType(Qt::PreciseTimer);
	connect(&everyday_timer, SIGNAL(timeout()), this, SLOT(everydayChange()));
	QTime next_time(0, 0, 0, 0);
	int time_diff = QTime::currentTime().secsTo(next_time) + 24 * 60 * 60;
	everyday_timer.start(time_diff * 1000);

	logBuffer = QByteArray();
	size_limit = limit;
	count = 0;
}
void JoyLogWriter::flush()
{
	writeLogFile();
}
void JoyLogWriter::changeCurrentFileName()
{
	current_file_name = current_file_name.left(current_file_name.lastIndexOf('\\') + 1);
	qDebug() << current_file_name;
	current_file_name += "log_";
	current_file_name +=  QDateTime::currentDateTime().toString("hh_mm_ss_zzz");
	current_file_name += ".txt";
}
void JoyLogWriter::changeDayDir()
{
	int right = current_file_name.lastIndexOf('\\');
	int left = current_file_name.lastIndexOf('\\', right - 1);
	current_file_name.replace(left + 1, right - left - 1, QDateTime::currentDateTime().toString("yyyyMMdd"));

	QDir dir(current_file_name.left(right + 1));
	QString dirName =  dir.dirName();
	dir.cdUp();
	dir.mkdir(dirName);

	changeCurrentFileName();
}
void JoyLogWriter::autoSave()
{
	writeLogFile();
}
void JoyLogWriter::writeLogFile()
{
	/* �ļ���С����һ�����ƣ��л���־�ļ��� */
	QFileInfo f_info(current_file_name);
	if (f_info.size() >= size_limit)
	{
		changeCurrentFileName();
	}

	QFile log_file(current_file_name);
	if(!log_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		qWarning() << QString("open log fie %1 failed!").arg(current_file_name);
		return;
	}

	QMutexLocker locker(&lock_buffer);

	qint64 len = log_file.write(logBuffer);
    qDebug() << QString("write file %1 bytes").arg(len);

	logBuffer.clear();
	count = 0;
}

/*ÿ���賿�л���־Ŀ¼����־�ļ���*/
void JoyLogWriter::everydayChange()
{
	changeDayDir();

	//everyday_timer.setSingleShot(true);
	int msec = QTime::currentTime().msec();
	connect(&everyday_timer, SIGNAL(timeout()), this, SLOT(everydayChange()));
	everyday_timer.start(24*60*60*1000 - msec);
}

JoyLogWriter& operator<<(JoyLogWriter& logger, const LogItem& item)
{
	logger.lock_buffer.lock();

	QTextStream logWriter(&logger.logBuffer, QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	logWriter << item.dt.toString("yyyy-dd-MM&hh:mm:ss.zzz") << " " << item.type << " " << item.info << '\r'; //��־�ļ�ÿ����־��¼��ʽ: ����ʱ�� + �ո� + ��־���� + ��־����
	logger.count++;
    //logWriter.flush();

	logger.lock_buffer.unlock();

	if (logger.count >= logger.max_count)
	{
		logger.flush();    //�����ļ�
	}
	return logger;
}


