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
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("Sigma Service Installer");
	a.setApplicationVersion("1.0");
	a.setOrganizationName("Sigma RD");
	a.setOrganizationDomain("com.sigmard.serviceinstaller");
	ServiceInstaller w;
	w.show();
	return a.exec();
}
