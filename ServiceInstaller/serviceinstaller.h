#ifndef SERVICEINSTALLER_H
#define SERVICEINSTALLER_H

#include <QtCore/QUrl>
#include <QtCore/QString>

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>

#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include "qtservice.h"
#include "ui_serviceinstaller.h"

class ServiceInstaller : public QDialog
{
	Q_OBJECT

public:
	ServiceInstaller(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ServiceInstaller();
	QString serviceName;
	QString serviceExecutable;

private:
	Ui::ServiceInstallerClass ui;

	private slots:
		void openWindowsServices();
		void installService();
		void uninstallService();
		void installCustomService();
		void checkServiceState();
};

#endif // SERVICEINSTALLER_H
