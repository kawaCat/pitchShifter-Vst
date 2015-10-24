//===================================================================
#ifndef __PitchShift_Include_12345__
#define __PitchShift_Include_12345__
//===================================================================

//===================================================================
#define LIMIT_INT_PARAM     1 //semi tone parameter 
//===================================================================
#define ST_PITCH_MAX_PITCH    24 // ( +-1octave )
#define PLUGIN_CODE 'kPCH' //
#define VERSION_NUM 1000
#define EFFECT_NAME "Pitch Shifter"
#define VENDER_NAME "kawa"
#define PRODUCT_NAME "Pitch Shifter"
//===================================================================
#include <public.sdk/source/vst2.x/audioeffectx.h> //vstsdk
#include <SoundTouch.h>
#include "Editor.h"
//===================================================================

//===================================================================
class PitchShift : public AudioEffectX
{
public:
    PitchShift (audioMasterCallback audioMaster);
    virtual ~PitchShift ();
    //===============================================================
    // Processing
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)override;
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)override;
    //===============================================================
    virtual void setProgramName (char* name) override;
    virtual void getProgramName (char* name) override;
    //===============================================================
    // Parameters
    virtual void  setParameter (VstInt32 index, float value)override;
    virtual float getParameter (VstInt32 index) override;
    virtual void  getParameterLabel (VstInt32 index, char* label)override;
    virtual void  getParameterDisplay (VstInt32 index, char* text)override;
    virtual void  getParameterName (VstInt32 index, char* text)override;
    //===============================================================
    
    //===============================================================
    virtual bool getEffectName(char* name) override
    {
        vst_strncpy (name, EFFECT_NAME, kVstMaxEffectNameLen);
        return true;
    };
    //===============================================================
    virtual bool getVendorString (char* text) override
    {
        vst_strncpy (text, VENDER_NAME, kVstMaxVendorStrLen);
        return true;
    };
    //===============================================================
    virtual bool getProductString (char* text) override
    {
        vst_strncpy (text, PRODUCT_NAME, kVstMaxProductStrLen);
        return true;
    };
    //===============================================================
    virtual VstInt32 getVendorVersion () override{ return VERSION_NUM;};
    //===============================================================
    virtual void resume()override;
    virtual void suspend()override;
    //===============================================================
protected:
    float fParam0;
    char programName[kVstMaxProgNameLen + 1];
    soundtouch::SoundTouch *cSoundTouch1;
};
//===================================================================
#endif
