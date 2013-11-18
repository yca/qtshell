#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QProcess>
#include <QEventLoop>
#include <QStringList>

class Compiler : public QObject
{
	Q_OBJECT
public:
	Compiler(QObject *parent = 0);
	int compile(QString filename);
	int wait();
	int exitcode;
	QString output;
signals:
	void finished();
protected slots:
	void started();
	void finished(int exitcode);
protected:
	QEventLoop el;
	QStringList defines;
	QStringList cflags;
	QStringList cxxflags;
	QStringList incpath;
	QStringList libs;
	QProcess pr;
};

#endif // COMPILER_H
