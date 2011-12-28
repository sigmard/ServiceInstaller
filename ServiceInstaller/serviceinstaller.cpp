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
	else
		ui.messageLabel->setText("Service settings not found!");

	checkServiceState();
}

void ServiceInstaller::checkServiceState()
{
	if(serviceName.isNull() && serviceExecutable.isNull())
	{
		ui.installButton->setText("Install Custom Service");
		connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installCustomService()));
	}
	else 
	{
		QtServiceController controller(serviceName);
		if(controller.isInstalled()){
			ui.installButton->setText("Uninstall Service");
			ui.messageLabel->setText(QString("\"<b>%1</b>\" named service is installed.").arg(serviceName));
			connect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
		}
		else
		{
			ui.installButton->setText("Install Service");
			ui.messageLabel->setText(QString("Follow instructions to install \"<b>%1</b>\" named service.").arg(serviceName));
			connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
		}
	}
}

void ServiceInstaller::installCustomService()
{
	// Not implemented
}

void ServiceInstaller::installService()
{
	if (QtServiceController::install(serviceExecutable, QString(), QString()))
	{
		QMessageBox::information(this, "Service Installed", QString("\"%1\" named service succesfully installed.").arg(serviceName));
		checkServiceState();
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
	}
	else
		QMessageBox::critical(this, "Install Failed", QString("An error occured while installing \"%1\" named service.").arg(serviceName));
}

void ServiceInstaller::uninstallService()
{
	QtServiceController controller(serviceName);
	if (controller.isRunning())
	{
		QMessageBox::warning(this, "Service Running", QString("\"%1\" named service is already running, you need to stop it first!").arg(serviceName));
		return;
	}

	if (controller.uninstall())
	{
		QMessageBox::information(this, "Service Uninstalled", QString("\"%1\" named service succesfully uninstalled.").arg(serviceName));
		checkServiceState();
		disconnect(ui.installButton, SIGNAL(clicked()), this, SLOT(uninstallService()));
	}
	else
		QMessageBox::critical(this, "Uninstall Failed", QString("An error occured while uninstalling \"%1\" named service.").arg(serviceName));
}

void ServiceInstaller::openWindowsServices()
{
	QDesktopServices::openUrl(QUrl(QString("file:///%1\\System32\\services.msc").arg(getenv("windir")), QUrl::TolerantMode));
}

ServiceInstaller::~ServiceInstaller()
{

}
