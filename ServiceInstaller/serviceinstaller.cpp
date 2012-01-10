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

#include "serviceinstaller.h"

ServiceInstaller::ServiceInstaller(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.windowsServiceLink, SIGNAL(linkActivated(QString)), this, SLOT(openWindowsServices()));
	
	QString serviceSettings = QString("%1\\service.ini").arg(QDir::cleanPath(qApp->applicationDirPath()));

	if(QFile::exists(serviceSettings))
	{
		QSettings settings(serviceSettings, QSettings::IniFormat);
		serviceName = settings.value("ServiceName", QVariant()).toString();
		serviceExecutable = settings.value("ServiceExecutable", QVariant()).toString();
		dataFiles = settings.value("DataFiles", QVariant()).toString();
	}
	else {
		QMessageBox::critical(this, "Install Failed", "Service settings not found!");
		qApp->quit();
	}

	ui.path_widget->hide();
	checkServiceState();
}

void ServiceInstaller::checkServiceState()
{
	QtServiceController controller(serviceName);
	if(controller.isInstalled()){
		ui.installButton->setText("Uninstall Service");
		ui.messageLabel->setText(QString("\"<b>%1</b>\" is installed.").arg(serviceName));
		ui.serviceStart->hide();
		connect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
	}
	else
	{
		ui.destinationPath->setText(QString("%1\\SigmaRD\\%2").arg(getenv("ProgramFiles(x86)"), serviceName));
		ui.installButton->setText("Install Service");
		ui.messageLabel->setText(QString("Follow instructions to install \"<b>%1</b>\" service.").arg(serviceName));
		ui.serviceStart->show();
		connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
	}
}

void ServiceInstaller::installService()
{
	QString servicePath = serviceExecutable;
	
	// Service Registration Phase
	if (QtServiceController::install(servicePath, QString(), QString()))
	{
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
		if (ui.serviceStart->isChecked())
		{
			QtServiceController controller(serviceName);
			controller.start();
		}
		QMessageBox::information(this, "Service Installed", QString("\"%1\" succesfully installed.").arg(serviceName));
		checkServiceState();
	}
	else
	{
		QMessageBox::critical(this, "Install Failed", QString("An error occured while installing \"%1\".").arg(serviceName));
		qApp->exit();
	}
}

void ServiceInstaller::uninstallService()
{
	QtServiceController controller(serviceName);
	if (controller.isRunning())
	{
		int ret = QMessageBox::warning(this, "Service Running", 
								QString("\"%1\" service is already running.\n\nDo you want to continue?").arg(serviceName),
                                QMessageBox::Yes | QMessageBox::No);

		if(ret == QMessageBox::Yes)
			controller.stop();
		else
			return;
	}

	if (controller.uninstall())
	{
		QMessageBox::information(this, "Service Uninstalled", QString("\"%1\" service succesfully uninstalled.").arg(serviceName));
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
		checkServiceState();
	}
	else
	{
		QMessageBox::critical(this, "Uninstall Failed", QString("An error occured while uninstalling \"%1\" service.").arg(serviceName));
		qApp->exit();
	}
}

void ServiceInstaller::openWindowsServices()
{
	QDesktopServices::openUrl(QUrl(QString("file:///%1\\System32\\services.msc").arg(getenv("windir")), QUrl::TolerantMode));
}

ServiceInstaller::~ServiceInstaller()
{

}
