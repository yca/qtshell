#include "compiler.h"

#include <QDebug>
#include <QProcess>

#include <unistd.h>

Compiler::Compiler(QObject *parent)
	: QObject(parent)
{
	defines << "-DQT_WEBKIT -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED";
	cflags << "-m64 -pipe -O2 -Wall -W -D_REENTRANT -fPIC -shared";
	cxxflags << "-m64 -pipe -O2 -Wall -W -D_REENTRANT";
	incpath << "-I/usr/share/qt4/mkspecs/linux-g++-64 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I.";
	libs << "-L/usr/lib/x86_64-linux-gnu -lQtGui -lQtNetwork -lQtCore -lpthread ";

	pr.setEnvironment(QProcess::systemEnvironment());
	pr.setProcessChannelMode(QProcess::MergedChannels);
	connect(&pr, SIGNAL(started()), SLOT(started()));
	connect(&pr, SIGNAL(finished(int)), SLOT(finished(int)));
}

int Compiler::compile(QString filename)
{
#if 0
	QProcess p;
	p.setEnvironment(QProcess::systemEnvironment());
	p.setProcessChannelMode(QProcess::MergedChannels);
	QString cmd = QString("g++ -c %1 %2 %3 %4 -o %5")
			.arg(cflags.join(" "))
			.arg(defines.join(" "))
			.arg(incpath.join(" "))
			.arg(filename)
			.arg(QString(filename).replace(".c", ".o"));
	/*QString scr = QString(filename).replace(".c", ".sh");
	exportLines(QStringList() << cmd, scr, false);
	QProcess::execute("chmod +x " + scr);
	p.start("sh " + scr);*/

	qDebug() << "running" << cmd;
	p.start(cmd);
	qDebug() << sleep(5);
	p.waitForFinished();
	output = QString::fromUtf8(p.readAllStandardOutput());
	return p.exitCode();
#endif
	QString cmd = QString("g++ %1 %2 %3 %4 -o %5")
			.arg(cflags.join(" "))
			.arg(defines.join(" "))
			.arg(incpath.join(" "))
			.arg(filename)
			.arg(QString(filename).replace(".c", ".so"));
	pr.start(cmd);

	return 0;
}

void Compiler::started()
{
}

void Compiler::finished(int exitcode)
{
	this->exitcode = exitcode;
	output = QString::fromUtf8(pr.readAllStandardOutput());
	emit finished();
}
