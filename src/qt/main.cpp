/*  SPDX-License-Identifier: GPL-2.0-or-later */

#include <QtWidgets/QApplication>

#include "AudacityQtConfig.h"
#include "AudioIO.h"
#include "Prefs.h"
#include "Project.h"
#include "ProjectQMLEnvironment.h"

#include "uicomponents/ApplicationConfiguration.h"

std::shared_ptr<AudacityProject> CreateProject()
{
   static ApplicationConfiguration appConfig;

   auto project = AudacityProject::Create();

   auto& engine = ProjectQMLEnvironment::Get(*project).GetEngine();
   engine.addImportPath(":/uicomponents");
   engine.setInitialProperties({
      { "appConfig", QVariant::fromValue(&appConfig) },
   });
   engine.load("qrc:/qml/main.qml");

   if (engine.rootObjects().isEmpty())
   {
      qDebug() << "Unable to load main.qml";
      return {};
   }

   return project;
}

int main(int argc, char *argv[])
{
   QGuiApplication app(argc, argv);

   std::vector<std::shared_ptr<AudacityProject>> projects;

   InitPreferences(std::make_unique<AudacityQtConfig>());
   AudioIO::Init();

   if(auto project = CreateProject())
      projects.push_back(project);

   if(!projects.empty())
      return app.exec();
   return -1;
}
