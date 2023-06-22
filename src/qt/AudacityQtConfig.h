/*  SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include <wx/confbase.h>

class AudacityQtConfig : public wxConfigBase
{
   wxString mPath;
public:
   void SetPath(const wxString& strPath) override { mPath = strPath; }
   const wxString& GetPath() const override { return mPath; }
   bool GetFirstGroup(wxString& str, long& lIndex) const override { return false; }
   bool GetNextGroup(wxString& str, long& lIndex) const override { return false; }
   bool GetFirstEntry(wxString& str, long& lIndex) const override { return false; }
   bool GetNextEntry(wxString& str, long& lIndex) const override { return false; }
   size_t GetNumberOfEntries(bool bRecursive) const override { return 0; }
   size_t GetNumberOfGroups(bool bRecursive) const override { return 0; }
   bool HasGroup(const wxString& strName) const override { return false; }
   bool HasEntry(const wxString& strName) const override { return false; }
   bool Flush(bool bCurrentOnly) override { return false; }
   bool RenameEntry(const wxString& oldName, const wxString& newName) override { return false; }
   bool RenameGroup(const wxString& oldName, const wxString& newName) override { return false; }
   bool DeleteEntry(const wxString& key, bool bDeleteGroupIfEmpty) override { return false; }
   bool DeleteGroup(const wxString& key) override { return false; }
   bool DeleteAll() override { return false; }

protected:
   bool DoReadString(const wxString& key, wxString* pStr) const override { return false; }
   bool DoReadLong(const wxString& key, long* pl) const override { return false; }
   bool DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const override { return false; }
   bool DoWriteString(const wxString& key, const wxString& value) override { return false; }
   bool DoWriteLong(const wxString& key, long value) override { return false; }
   bool DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf) override { return false; }
};
