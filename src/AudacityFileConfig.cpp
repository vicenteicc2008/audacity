/**********************************************************************

Audacity: A Digital Audio Editor

AudacityFileConfig.cpp

Paul Licameli split from Prefs.cpp

**********************************************************************/


#include "AudacityFileConfig.h"

#include "HelpSystem.h"
#include "wxPanelWrapper.h"
#include "ShuttleGui.h"
#include "../images/Help.xpm"

#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/wfstream.h>

namespace
{
void ShowWarn(const wxString& filepath)
{
   wxDialogWrapper dlg(nullptr, wxID_ANY, XO("Audacity Configuration Error"));

   ShuttleGui S(&dlg, eIsCreating);

   wxButton *retryButton;
   wxButton *quitButton;

   S.SetBorder(5);
   S.StartVerticalLay(wxEXPAND, 1);
   {
      S.SetBorder(15);
      S.StartHorizontalLay(wxALIGN_RIGHT, 0);
      {
         S.AddFixedText(
            XO("The following configuration file could not be accessed:\n\n"
               "\t%s\n\n"
               "This could be caused by many reasons, but the most likely are that "
               "the disk is full or you do not have write permissions to the file. "
               "More information can be obtained by clicking the help button below.\n\n"
               "You can attempt to correct the issue and then click \"Retry\" to continue.\n\n"
               "If you choose to \"Quit Audacity\", your project may be left in an unsaved "
               "state which will be recovered the next time you open it.")
            .Format(filepath),
            false,
            500);
      }
      S.EndHorizontalLay();

      S.SetBorder(5);
      S.StartHorizontalLay(wxALIGN_RIGHT, 0);
      {
         // Can't use themed bitmap since the theme manager might not be
         // initialized yet and it requires a configuration file.
         wxButton *b = S.Id(wxID_HELP).AddBitmapButton(wxBitmap(Help_xpm));
         b->SetToolTip( XO("Help").Translation() );
         b->SetLabel(XO("Help").Translation());       // for screen readers

         b = S.Id(wxID_CANCEL).AddButton(XXO("&Quit Audacity"));
         b = S.Id(wxID_OK).AddButton(XXO("&Retry"));
         dlg.SetAffirmativeId(wxID_OK);

         b->SetDefault();
         b->SetFocus();
      }
      S.EndHorizontalLay();
   }
   S.EndVerticalLay();

   dlg.Layout();
   dlg.GetSizer()->Fit(&dlg);
   dlg.SetMinSize(dlg.GetSize());
   dlg.Center();

   auto onButton = [&](wxCommandEvent &e)
   {
      dlg.EndModal(e.GetId());
   };

   dlg.Bind(wxEVT_BUTTON, onButton);

   switch (dlg.ShowModal())
   {
      case wxID_HELP:
         // Can't use the HelpSystem since the theme manager may not
         // yet be initialized and it requires a configuration file.
         OpenInDefaultBrowser("https://" +
                              HelpSystem::HelpHostname +
                              HelpSystem::HelpServerHomeDir +
                              "Error:_Audacity_settings_file_unwritable");
      break;

      case wxID_CANCEL:
         _exit(-1);
      break;
   }

   dlg.Unbind(wxEVT_BUTTON, onButton);
}

}

std::unique_ptr<AudacityFileConfig>
AudacityFileConfig::Create(const wxString& appName,
                           const wxString& vendorName,
                           const wxString& localFilename,
                           const wxString& globalFilename,
                           long style,
                           const wxMBConv& conv)
{
   while (true)
   {
      auto config = std::unique_ptr<AudacityFileConfig>(safenew AudacityFileConfig(
         appName, vendorName, localFilename, globalFilename, style, conv));

      // Prevent wxFileConfig from attempting a Flush() during object deletion. This happens
      // because we don't use the wxFileConfig::Flush() method and so the wxFileConfig dirty
      // flag never gets reset. During deletion, the dirty flag is checked and a Flush()
      // performed. This can (and probably will) create bogus temporary files.
      config->DisableAutoSave();

      bool canRead = false;
      bool canWrite = false;
      int fd;

      fd = wxOpen(localFilename, O_RDONLY, S_IREAD);
      if (fd != -1 || errno == ENOENT)
      {
         canRead = true;
         if (fd != -1)
         {
            wxClose(fd);
         }
      }

      fd = wxOpen(localFilename, O_WRONLY | O_CREAT, S_IWRITE);
      if (fd != -1)
      {
         canWrite = true;
         wxClose(fd);
      }

      if (canRead && canWrite)
         return config;

      ShowWarn(localFilename);
   }
}

AudacityFileConfig::AudacityFileConfig(const wxString& appName,
                                       const wxString& vendorName,
                                       const wxString& localFilename,
                                       const wxString& globalFilename,
                                       long style,
                                       const wxMBConv& conv)
   : wxFileConfig(appName, vendorName, localFilename, globalFilename, style, conv)
   , mLocalFilename(localFilename)
   , mGlobalFilename(globalFilename)
{
   
}

AudacityFileConfig::~AudacityFileConfig() = default;


bool AudacityFileConfig::Flush(bool)
{
   while (true)
   {
      FilePath backup = mLocalFilename + ".bkp";

      if (!wxFileExists(backup) || (wxRemove(backup) == 0))
      {
         if (!wxFileExists(mLocalFilename) || (wxRename(mLocalFilename, backup) == 0))
         {
            wxFileOutputStream stream(mLocalFilename);
            if (stream.IsOk())
            {
               if (Save(stream))
               {
                  stream.Sync();
                  if (stream.IsOk() && stream.Close())
                  {
                     if (!wxFileExists(backup) || (wxRemove(backup) == 0))
                        return true;
                  }
               }
            }

            if (wxFileExists(backup))
            {
               wxRemove(mLocalFilename);
               wxRename(backup, mLocalFilename);
            }
         }
      }

      ShowWarn(mLocalFilename);
   }
}
