/**********************************************************************

  Audacity: A Digital Audio Editor

  VSTEffect.h

  Dominic Mazzoni

**********************************************************************/
#if USE_VST

#ifndef __AUDACITY_VST_EFFECT__
#define __AUDACITY_VST_EFFECT__

#include "VSTEffectBase.h"
#include "../StatelessPerTrackEffect.h"
#include "../EffectEditor.h"

#include "SampleFormat.h"
#include <wx/weakref.h>

#include <optional>
#include <atomic>

class wxSlider;
class wxStaticText;

class NumericTextCtrl;

class VSTControl;
#include "VSTControl.h"

typedef intptr_t (*dispatcherFn)(AEffect * effect,
                                 int opCode,
                                 int index,
                                 intptr_t value,
                                 void *ptr,
                                 float opt);

typedef void (*processFn)(AEffect * effect,
                          float **inputs,
                          float **outputs,
                          int sampleframes);

typedef void (*setParameterFn)(AEffect * effect,
                               int index,
                               float parameter);

typedef float (*getParameterFn)(AEffect * effect,
                                int index);

typedef AEffect *(*vstPluginMain)(audioMasterCallback audioMaster);

class VSTTimer;
class VSTEffect;
class wxDynamicLibrary;

#if defined(__WXMAC__)
struct __CFBundle;
typedef struct __CFBundle *CFBundleRef;
#if __LP64__
typedef int CFBundleRefNum;
#else
typedef signed short                    SInt16;
typedef SInt16 CFBundleRefNum;
#endif
#endif

class VSTInstance;
using VSTInstanceArray = std::vector < std::unique_ptr<VSTInstance> >;

///////////////////////////////////////////////////////////////////////////////
//
// VSTEffect
//
///////////////////////////////////////////////////////////////////////////////


wxDECLARE_EVENT(EVT_SIZEWINDOW, wxCommandEvent);
DECLARE_LOCAL_EVENT_TYPE(EVT_UPDATEDISPLAY, -1);


class VSTEditor;

class VSTEffect final
   : public StatelessEffectUIServices
   , public VSTEffectBase
{
public:
   using VSTEffectBase::VSTEffectBase;
   ~VSTEffect() override;

private:
   int ShowClientInterface(const EffectPlugin &plugin, wxWindow &parent,
      wxDialog &dialog, EffectEditor *pEditor, bool forceModal)
   const override;

   std::unique_ptr<EffectEditor> PopulateUI(const EffectPlugin &plugin,
      ShuttleGui &S, EffectInstance &instance, EffectSettingsAccess &access,
      const EffectOutputs *pOutputs) const override;

   void ExportPresets(
      const EffectPlugin &plugin, const EffectSettings &settings)
   const override;

   OptionalMessage ImportPresets(
      const EffectPlugin &plugin, EffectSettings &settings) const override;

   // Non-const and non-virtual function:
   OptionalMessage ImportPresetsNC(EffectSettings &settings);
   void ShowOptions(const EffectPlugin &plugin) const override;

   //! Will never be called
   virtual std::unique_ptr<EffectEditor> MakeEditor(
      ShuttleGui & S, EffectInstance &instance, EffectSettingsAccess &access,
      const EffectOutputs *pOutputs) const final;
};

class VSTEditor final
   : public wxEvtHandler
   , public EffectEditor
   , public VSTUIWrapper
{
public:

   VSTEditor(VSTInstance&       instance, EffectType type, bool gui,
      const EffectUIServices&  services,
      EffectSettingsAccess&    access,
      wxWindow*                pParent,
      int                      numParams
   );

   ~VSTEditor() override;

   VSTInstance& GetInstance() const;

   bool ValidateUI() override;
   bool UpdateUI() override;

   void OnClose() override;

   void BuildPlain(EffectSettingsAccess& access, EffectType effectType, double projectRate);
   void BuildFancy(EffectInstance& instance);

   void OnTimer();

   std::unique_ptr<VSTTimer> mTimer;

   void RefreshParameters(int skip = -1) const;

   void Automate(int index, float value) override;

   void OnSlider(wxCommandEvent& evt);    

   int ShowDialog(bool nonModal);

   bool IsGraphicalUI() override;

   void Flush() override;

protected:
   void SizeWindow(int w, int h) override;

private:
   void NotifyParameterChanged(int index, float value);
   void OnIdle(wxIdleEvent &evt);

   VSTInstance& mInstance;
   const EffectType mType;
   const bool mGui;

   bool FetchSettingsFromInstance(EffectSettings& settings);
   bool StoreSettingsToInstance(const EffectSettings& settings);
   void NeedEditIdle(bool state);
   void NeedIdle() override;

   void OnSizeWindow(wxCommandEvent& evt);

   int  mTimerGuard{ 0 };

   bool mWantsEditIdle{ false };
   bool mWantsIdle{ false };

   // Remembers last slider movements until idle time
   std::vector<std::pair<int, double>> mLastMovements{};

   ArrayOf<wxStaticText*> mNames;
   ArrayOf<wxSlider*> mSliders;
   ArrayOf<wxStaticText*> mDisplays;
   ArrayOf<wxStaticText*> mLabels;
   NumericTextCtrl* mDuration;

   wxWindow* mParent;
   wxWeakRef<wxDialog> mDialog;
   
   VSTControl* mControl{};

   // Mapping from parameter ID to string
   std::vector<wxString> mParamNames;

   int mNumParams{ 0 };
};

#endif

#endif // USE_VST
