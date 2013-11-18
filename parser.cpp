#include "parser.h"
#include "compiler.h"

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QDebug>
#include <QTimer>
#include <QStringList>

#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>

#define mDebug qWarning

static QStringList importLines(QString filename)
{
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly))
		return QStringList();
	return QString::fromUtf8(f.readAll()).split("\n");
}

Parser::Parser(QObject *parent)
	: QObject(parent), c(this)
{
	connect(&c, SIGNAL(finished()), SLOT(compileFinished()));

	/* create includes */
	includes << "#include <QDebug>";
}

int Parser::parse(QString script)
{
	QStringList scrlines = importLines(script);
	scrlines.removeFirst(); //#! part

	/* rest must be a valid c++ code */
	QDir d("/tmp");
	d.mkpath("qtshell");
	QString fname = QString::fromUtf8(QUuid::createUuid().toByteArray().toBase64().mid(0, 8));
	QFile f(fname.prepend("/tmp/qtshell/").append(".c"));
	if (!f.open(QIODevice::WriteOnly)) {
		mDebug("cannot open tmp file '%s'", qPrintable(fname));
		return -EPERM;
	}

	/* write includes */
	f.write(includes.join("\n").append('\n').toUtf8());

	f.write("extern \"C\" {\n");
	f.write(scrlines.join("\n").toUtf8());
	f.write("}\n");

	int err = c.compile(fname);
	if (err) {
		mDebug() << "error compiling script:" << c.output;
		return err;
	}

	libfile = fname.replace(".c", ".so");
	return 0;
}

void Parser::timeout()
{
}

void Parser::compileFinished()
{
	QString warn = c.output;
	if (!warn.isEmpty()) {
		mDebug() << "*******************************************";
		mDebug() << warn;
		mDebug() << "*******************************************";
	}
	if (c.exitcode)
		return;

	mDebug() << "compile finished, executing";
	typedef int(*scrmain)(void);
	scrmain p = (scrmain)importLib();
	p();
}

void * Parser::importLib()
{
	mDebug() << "opening" << libfile;
	void *handle = dlopen(qPrintable(libfile), RTLD_NOW);
	if (!handle) {
		mDebug() << "error loading compiled library:" << dlerror();
		return NULL;
	}

	void *scrmain = dlsym(handle, "scrmain");
	if (!scrmain) {
		mDebug() << "error resolving script entry point" << dlerror();
		return NULL;
	}

	return scrmain;
}
