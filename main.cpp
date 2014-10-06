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
		fprintf(stderr, "%s\n", msg);
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

	Parser *p = new Parser;
	QObject::connect(p, SIGNAL(destroyed()), &a, SLOT(quit()));
	p->parse(a.arguments()[1], a.arguments());

	return a.exec();
}
