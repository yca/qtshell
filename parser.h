#ifndef PARSER_H
#define PARSER_H

#include "compiler.h"

#include <QString>

class Parser : public QObject
{
	Q_OBJECT
public:
	Parser(QObject *parent = 0);
	int parse(QString script, const QStringList arguments);
protected slots:
	void timeout();
	void compileFinished();
protected:
	Compiler c;
	QString libfile;
	QStringList includes;
	QString currentScript;

	void *importLib();
};

#endif // PARSER_H
