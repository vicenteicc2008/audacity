/**********************************************************************

Audacity: A Digital Audio Editor

@file AudacityFileConfig.h
@brief Extend FileConfig with application-specific behavior

Paul Licameli split from Prefs.h

**********************************************************************/

#ifndef __AUDACITY_FILE_CONFIG__
#define __AUDACITY_FILE_CONFIG__

#include <memory>
#include <wx/fileconf.h>

/// \brief Our own specialisation of wxFileConfig.
class AUDACITY_DLL_API AudacityFileConfig final : public wxFileConfig
{
   AudacityFileConfig(const wxString& appName = wxEmptyString,
                      const wxString& vendorName = wxEmptyString,
                      const wxString& localFilename = wxEmptyString,
                      const wxString& globalFilename = wxEmptyString,
                      long style = wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE,
                      const wxMBConv& conv = wxConvAuto());
public:

   static
   std::unique_ptr<AudacityFileConfig> Create(const wxString& appName = wxEmptyString,
                                              const wxString& vendorName = wxEmptyString,
                                              const wxString& localFilename = wxEmptyString,
                                              const wxString& globalFilename = wxEmptyString,
                                              long style = wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE,
                                              const wxMBConv& conv = wxConvAuto());

   ~AudacityFileConfig() override;

   bool Flush(bool bCurrentOnly) override;

private:
   const wxString mLocalFilename;
   const wxString mGlobalFilename;
};

#endif
