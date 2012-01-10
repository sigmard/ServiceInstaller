/****************************************************************************
** 
**	Copyright (C) 2011, ServiceInstaller
**	2011 - SigmaRD ~ Gökmen Göksel <gokmen@goksel.me>
**	http://github.com/sigmard/ServiceInstaller
**
**	This program is free software; you can redistribute it and/or modify it
**	under the terms of the GNU General Public License as published by the Free
**	Software Foundation; either version 2 of the License, or (at your option)
**	any later version.
**
****************************************************************************/

#ifndef SERVICEINSTALLER_H
#define SERVICEINSTALLER_H

#include <QtCore/QUrl>
#include <QtCore/QDir>
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
	QString dataFiles;
	QString installPath;

private:
	Ui::ServiceInstallerClass ui;

	private slots:
		void openWindowsServices();
		void installService();
		void uninstallService();
		void checkServiceState();
};

#endif // SERVICEINSTALLER_H
