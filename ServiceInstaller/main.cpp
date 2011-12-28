#include "serviceinstaller.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ServiceInstaller w;
	w.show();
	return a.exec();
}
