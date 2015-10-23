//===================================================================
#ifndef __EDITOR_INCLUDE_12345__
#define __EDITOR_INCLUDE_12345__
//===================================================================
#include "vstgui/vstgui.h"
#include "vstgui/aeffguieditor.h"
#include "vstgui/vstcontrols.h"
//===================================================================
class PitchShifterEditor : public AEffGUIEditor
                         , public CControlListener
{
public:
    PitchShifterEditor (AudioEffect* effect);
    virtual ~PitchShifterEditor ();
    //===============================================================
    virtual bool open (void* ptr) override;
    virtual void close () override;
    void idle() override;
    virtual void setParameter (VstInt32 index, float value) override;
    virtual void valueChanged(CControl* pControl) override;
    //===============================================================
private:
    CKnob *CKnob1;
    VSTGUI::CParamDisplay  *CParam1;
    CBitmap* hBackground;
};
//===================================================================
#endif
