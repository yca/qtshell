#include <QDebug>
#include <QStringList>
#include <QCoreApplication>

#include "parser.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
	case QtDebugMsg:
		fprintf(stderr, "%s\n", msg);
		break;
	case QtWarningMsg:
		//fprintf(stderr, "Warning: %s\n", msg);
		break;
	case QtCriticalMsg:
		//fprintf(stderr, "Critical: %s\n", msg);
		break;
	case QtFatalMsg:
		//fprintf(stderr, "Fatal: %s\n", msg);
		//abort();
		break;
	}
}

int main(int argc, char *argv[])
{
	qInstallMsgHandler(myMessageOutput);
	QCoreApplication a(argc, argv);

	Parser p;
	p.parse(a.arguments()[1]);

	return a.exec();
}
