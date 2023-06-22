/*  SPDX-License-Identifier: GPL-2.0-or-later */

#include "ProjectQMLEnvironment.h"

#include <QQmlContext>

auto
ProjectQMLEnvironment::Property::RegisterProperty(ProjectQMLEnvironment& env,
                                                  const QString& name,
                                                  const DataFactory& factory)
   -> DataPointer
{
   auto property = factory(env);
   property->setParent(&env.GetEngine());
   env.GetContext().setContextProperty(name, property);
   return property;
}

ProjectQMLEnvironment::Property::Property(const QString& name, DataFactory factory)
   : RegisteredFactory([name, factory = std::move(factory)](auto& env)
      { return RegisterProperty(env, name, factory); })
{
}

ProjectQMLEnvironment::ProjectQMLEnvironment(AudacityProject& project)
   : mProject(project)
{
   BuildAll();//Collect properties
}

AudacityProject& ProjectQMLEnvironment::GetProject() noexcept
{
   return mProject;
}

QQmlContext& ProjectQMLEnvironment::GetContext() noexcept
{
   return *mEngine.rootContext();
}

QQmlApplicationEngine& ProjectQMLEnvironment::GetEngine() noexcept
{
   return mEngine;
}

//Creates a QML environment and attaches it to a new project.
static AudacityProject::AttachedObjects::RegisteredFactory sQMLEnvironment {
   [](AudacityProject& project)
   {
      return std::make_shared<ProjectQMLEnvironment>(project);
   }
};

ProjectQMLEnvironment& ProjectQMLEnvironment::Get(AudacityProject& project)
{
   return project.Get<ProjectQMLEnvironment>(sQMLEnvironment);
}
