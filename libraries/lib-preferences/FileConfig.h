/**********************************************************************

  Audacity: A Digital Audio Editor

  FileConfig.h

  Leland Lucius

**********************************************************************/

#ifndef __AUDACITY_WIDGETS_FILECONFIG__
#define __AUDACITY_WIDGETS_FILECONFIG__

#include <memory>
#include <wx/confbase.h>

///@brief Wraps wxConfigBase adding dirty flag that prevents extensive
///redundant writes when wxConfigBase::Flush() is called. Replicates
///wxConfigBase interface.
///Requires another wxConfigBase instance that is used as an actual
///storage (e.g. all write/reads are delegated to that object)
//TODO: wxFileConfig already has dirty flag
class PREFERENCES_API FileConfig final : public wxConfigBase
{
public:
   
   FileConfig();
   ~FileConfig() override;

   void SetImpl(std::unique_ptr<wxConfigBase> impl);

   void SetPath(const wxString& strPath) override;
   const wxString& GetPath() const override;
   bool GetFirstGroup(wxString& str, long& lIndex) const override;
   bool GetNextGroup(wxString& str, long& lIndex) const override;
   bool GetFirstEntry(wxString& str, long& lIndex) const override;
   bool GetNextEntry(wxString& str, long& lIndex) const override;
   size_t GetNumberOfEntries(bool bRecursive = false) const override;
   size_t GetNumberOfGroups(bool bRecursive = false) const override;
   bool HasGroup(const wxString& strName) const override;
   bool HasEntry(const wxString& strName) const override;
   bool Flush(bool bCurrentOnly = false) override;
   bool RenameEntry(const wxString& oldName, const wxString& newName) override;
   bool RenameGroup(const wxString& oldName, const wxString& newName) override;
   bool DeleteEntry(const wxString& key, bool bDeleteGroupIfEmpty = true) override;
   bool DeleteGroup(const wxString& key) override;
   bool DeleteAll() override;

   // Set and Get values of the version major/minor/micro keys in audacity.cfg when Audacity first opens
   void SetVersionKeysInit( int major, int minor, int micro)
   {
      mVersionMajorKeyInit = major;
      mVersionMinorKeyInit = minor;
      mVersionMicroKeyInit = micro;
   }
   void GetVersionKeysInit( int& major, int& minor, int& micro) const
   {
      major = mVersionMajorKeyInit;
      minor = mVersionMinorKeyInit;
      micro = mVersionMicroKeyInit;
   }

protected:

   bool DoReadString(const wxString& key, wxString *pStr) const override;
   bool DoReadLong(const wxString& key, long *pl) const override;
#if wxUSE_BASE64
   bool DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const override;
#endif // wxUSE_BASE64

   bool DoWriteString(const wxString& key, const wxString& szValue) override;
   bool DoWriteLong(const wxString& key, long lValue) override;
#if wxUSE_BASE64
   bool DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf) override;
#endif // wxUSE_BASE64

private:
   std::unique_ptr<wxConfigBase> mImpl;

   // values of the version major/minor/micro keys in audacity.cfg
   // when Audacity first opens
   int mVersionMajorKeyInit{};
   int mVersionMinorKeyInit{};
   int mVersionMicroKeyInit{};

   bool mDirty{false};
};

#endif

