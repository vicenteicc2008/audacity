/*  SPDX-License-Identifier: GPL-2.0-or-later */
/**********************************************************************

 Audacity: A Digital Audio Editor

 @file NumericConverterFormatter.h

 Dmitry Vedenko

 **********************************************************************/
#pragma once

#include <optional>
#include <vector>

#include <wx/string.h>

struct NUMERIC_FORMATS_API NumericField final
{
public:
   NumericField(size_t _range, bool _zeropad) noexcept;

   NumericField(const NumericField&) = default;
   NumericField& operator=(const NumericField&) = default;
   // NumericField( NumericField && ) = default;
   // NumericField &operator = ( NumericField && ) = default;

   size_t range; // then take modulo this
   size_t digits { 0 };

   wxString label;
   wxString formatStr;

   size_t pos { wxString::npos }; // Index of this field in the ValueString
   bool zeropad;

private:
   void CreateDigitFormatStr();
};

using NumericFields = std::vector<NumericField>;

struct NUMERIC_FORMATS_API DigitInfo final
{
   size_t field; // Which field
   size_t index; // Index of this digit within the field
   size_t pos;   // Position in the ValueString
};

using DigitInfos = std::vector<DigitInfo>;

struct NUMERIC_FORMATS_API NumericConverterFormatter /* not final */
{
   virtual ~NumericConverterFormatter();

   struct NUMERIC_FORMATS_API ConversionResult final
   {
      wxString valueString;
      std::vector<wxString> fieldValueStrings;
   };

   //! @post result: `GetFields().size() == result.fieldValueStrings.size()`
   virtual ConversionResult
   ValueToString(double value, bool nearest) const = 0;

   virtual std::optional<double>
   StringToValue(const wxString& value) const = 0;

   virtual double SingleStep(double value, int digitIndex, bool upwards) const = 0;

   const wxString& GetPrefix() const;
   const NumericFields& GetFields() const;
   const DigitInfos& GetDigitInfos() const;

protected:
   wxString mPrefix;

   NumericFields mFields;
   DigitInfos mDigits;
};
