#include <QCoreApplication>
#include <../joy_log_writer.h>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    JoyLogWriter logger("log", 3600, JoyLogWriter::THREE_M);
    LogItem i(QDateTime::currentDateTime(), "log info 0.", LogItem::NETWORK);

    while (true)
    {
        logger << i;
        i = LogItem(QDateTime::currentDateTime(), "log info 1.", LogItem::NETWORK);
        logger << i;
        i = LogItem(QDateTime::currentDateTime(), "log info 2.", LogItem::SCHEDULE);
        logger << i;
        i = LogItem(QDateTime::currentDateTime(), "log info 3.", LogItem::SCHEDULE);
        logger << i;
        i= LogItem(QDateTime::currentDateTime(), "log info 4.", LogItem::SCHEDULE);
        logger << i;
    }

    return a.exec();
}
