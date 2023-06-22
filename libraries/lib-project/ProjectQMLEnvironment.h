/*  SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include <QQmlApplicationEngine>

#include "Project.h"
#include "ClientData.h"

/**
 * \brief Gives an access to a QML environment attached to a project
 */
class PROJECT_API ProjectQMLEnvironment final
   : public ClientData::Site<ProjectQMLEnvironment,
                             QObject,
                             ClientData::SkipCopying,
                             ClientData::BarePtr>
   , public ClientData::Base
{
   AudacityProject& mProject;
   QQmlApplicationEngine mEngine;
public:

   ///\brief Binds object returned by a factory to the new variable
   ///in the root qml context of a project.
   ///Returned QObject will be owned by hosting environment.
   class PROJECT_API Property final : public RegisteredFactory
   {
      static DataPointer RegisterProperty(ProjectQMLEnvironment& env,
                                          const QString& name,
                                          const DataFactory& factory);
   public:
      /**
       * \param name Name of the new variable in the root context
       * \param factory Factory that creates a variable
       */
      Property(const QString& name, DataFactory factory);
   };

   ProjectQMLEnvironment(AudacityProject& project);

   AudacityProject& GetProject() noexcept;
   QQmlContext& GetContext() noexcept;
   QQmlApplicationEngine& GetEngine() noexcept;

   static ProjectQMLEnvironment& Get(AudacityProject& project);
};
