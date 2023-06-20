/**********************************************************************

  Audacity: A Digital Audio Editor

  FileConfig.cpp

  Leland Lucius

**********************************************************************/

#include "FileConfig.h"

FileConfig::FileConfig() = default;

FileConfig::~FileConfig()
{
   wxASSERT(mDirty == false);
}

void FileConfig::SetImpl(std::unique_ptr<wxConfigBase> impl)
{
   mImpl = std::move(impl);
   SetAppName(mImpl->GetAppName());
   SetVendorName(mImpl->GetVendorName());
   SetStyle(mImpl->GetStyle());
}

void FileConfig::SetPath(const wxString& strPath)
{
   mImpl->SetPath(strPath);
}

const wxString& FileConfig::GetPath() const
{
   return mImpl->GetPath();
}

bool FileConfig::GetFirstGroup(wxString& str, long& lIndex) const
{
   return mImpl->GetFirstGroup(str, lIndex);
}

bool FileConfig::GetNextGroup(wxString& str, long& lIndex) const
{
   return mImpl->GetNextGroup(str, lIndex);
}

bool FileConfig::GetFirstEntry(wxString& str, long& lIndex) const
{
   return mImpl->GetFirstEntry(str, lIndex);
}

bool FileConfig::GetNextEntry(wxString& str, long& lIndex) const
{
   return mImpl->GetNextEntry(str, lIndex);
}

size_t FileConfig::GetNumberOfEntries(bool bRecursive) const
{
   return mImpl->GetNumberOfEntries(bRecursive);
}

size_t FileConfig::GetNumberOfGroups(bool bRecursive) const
{
   return mImpl->GetNumberOfGroups(bRecursive);
}

bool FileConfig::HasGroup(const wxString& strName) const
{
   return mImpl->HasGroup(strName);
}

bool FileConfig::HasEntry(const wxString& strName) const
{
   return mImpl->HasEntry(strName);
}

bool FileConfig::Flush(bool WXUNUSED(bCurrentOnly))
{
   if (!mDirty)
   {
      return true;
   }

   if(mImpl->Flush())
   {
      mDirty = false;
      return true;
   }
   return false;
}

bool FileConfig::RenameEntry(const wxString& oldName, const wxString& newName)
{
   auto res = mImpl->RenameEntry(oldName, newName);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::RenameGroup(const wxString& oldName, const wxString& newName)
{
   auto res = mImpl->RenameGroup(oldName, newName);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::DeleteEntry(const wxString& key, bool bDeleteGroupIfEmpty)
{
   auto res = mImpl->DeleteEntry(key, bDeleteGroupIfEmpty);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::DeleteGroup(const wxString& key)
{
   auto res = mImpl->DeleteGroup(key);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::DeleteAll()
{
   auto res = mImpl->DeleteAll();
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::DoReadString(const wxString& key, wxString *pStr) const
{
   return mImpl->Read(key, pStr);
}

bool FileConfig::DoReadLong(const wxString& key, long *pl) const
{
   return mImpl->Read(key, pl);
}

#if wxUSE_BASE64
bool FileConfig::DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const
{
   return mImpl->Read(key, buf);
}
#endif // wxUSE_BASE64

bool FileConfig::DoWriteString(const wxString& key, const wxString& szValue)
{
   bool res = mImpl->Write(key, szValue);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

bool FileConfig::DoWriteLong(const wxString& key, long lValue)
{
   bool res = mImpl->Write(key, lValue);
   if (res)
   {
      mDirty = true;
   }
   return res;
}

#if wxUSE_BASE64
bool FileConfig::DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf)
{
   bool res = mImpl->Write(key, buf);
   if (res)
   {
      mDirty = true;
   }
   return res;
}
#endif // wxUSE_BASE64
