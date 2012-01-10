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
	
	if(QFile::exists("service.ini"))
	{
		QSettings settings("service.ini", QSettings::IniFormat);
		serviceName = settings.value("ServiceName", QVariant()).toString();
		serviceExecutable = settings.value("ServiceExecutable", QVariant()).toString();
	}
	else {
		QMessageBox::critical(this, "Install Failed", "Service settings not found!");
		qApp->quit();
	}

	checkServiceState();
}

void ServiceInstaller::checkServiceState()
{
	QtServiceController controller(serviceName);
	if(controller.isInstalled()){
		ui.installButton->setText("Uninstall Service");
		ui.messageLabel->setText(QString("\"<b>%1</b>\" service is installed.").arg(serviceName));
		ui.path_widget->hide();
		connect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
	}
	else
	{
		ui.installButton->setText("Install Service");
		ui.messageLabel->setText(QString("Follow instructions to install \"<b>%1</b>\" service.").arg(serviceName));
		ui.path_widget->show();
		connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
	}
}

void ServiceInstaller::installService()
{
	if (QtServiceController::install(serviceExecutable, QString(), QString()))
	{
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
		QtServiceController controller(serviceName);
		controller.start();
		QMessageBox::information(this, "Service Installed", QString("\"%1\" service succesfully installed.").arg(serviceName));
		checkServiceState();
	}
	else
		QMessageBox::critical(this, "Install Failed", QString("An error occured while installing \"%1\" named service.").arg(serviceName));
}

void ServiceInstaller::uninstallService()
{
	QtServiceController controller(serviceName);
	if (controller.isRunning())
	{
		QMessageBox::information(this, "Service Running", QString("\"%1\" service is already running, stopping service.").arg(serviceName));
		controller.stop();
	}

	if (controller.uninstall())
	{
		QMessageBox::information(this, "Service Uninstalled", QString("\"%1\" service succesfully uninstalled.").arg(serviceName));
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
		checkServiceState();
	}
	else
		QMessageBox::critical(this, "Uninstall Failed", QString("An error occured while uninstalling \"%1\" service.").arg(serviceName));
}

void ServiceInstaller::openWindowsServices()
{
	QDesktopServices::openUrl(QUrl(QString("file:///%1\\System32\\services.msc").arg(getenv("windir")), QUrl::TolerantMode));
}

ServiceInstaller::~ServiceInstaller()
{

}
