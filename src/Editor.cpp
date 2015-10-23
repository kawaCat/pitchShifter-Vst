//===================================================================
#include "Editor.h"
#include "PitchShifter.h"
//===================================================================
CColor kBlueAlphaCColor = { 255,   255, 255,255 };
//===================================================================
// resource id's
enum
{
    kBack = 200,
    kKnob = 201,
    kPoint = 202
};
//===================================================================
void ST_Pitch2Text(float value, char *string)
{
#ifdef LIMIT_INT_PARAM
    sprintf(string, "%2.0f", ((value - 0.5)*ST_PITCH_MAX_PITCH));
#else
    sprintf(string, "%2.2f", ((value - 0.5)*ST_PITCH_MAX_PITCH));
#endif
}
//===================================================================

//===================================================================
PitchShifterEditor::PitchShifterEditor(AudioEffect *effect)
    : AEffGUIEditor(effect)
{
    hBackground = new CBitmap(kBack);
    //===============================================================
    rect.left = 0;
    rect.top  = 0;
    rect.right  = hBackground->getWidth();
    rect.bottom = hBackground->getHeight();
}
//===================================================================
PitchShifterEditor::~PitchShifterEditor()
{
    if (hBackground) { hBackground->forget(); };
    hBackground = 0;
}
//===================================================================
bool PitchShifterEditor::open(void *ptr)
{
    //===============================================================
    AEffGUIEditor::open(ptr);
    //===============================================================
    CBitmap* hKnobBack  = new CBitmap(kKnob);
    CBitmap* hKnobPoint = new CBitmap(kPoint);
    //===============================================================
    CRect size(0, 0, hBackground->getWidth(), hBackground->getHeight());
    CFrame* lFrame = new CFrame(size, ptr, this);
    lFrame->setBackground(hBackground);
    lFrame->setMouseEnabled(true);
    lFrame->setMouseableArea(size);
    //===============================================================
    CPoint point(0, 0);
    int x = hBackground->getWidth() / 2 - hKnobBack->getWidth() / 2;
    int y = hBackground->getHeight() / 2 - hKnobBack->getHeight() / 2;
    //===============================================================
    size(x, y, hKnobBack->getWidth() + x, hKnobBack->getHeight() + y);
    //===============================================================
    CKnob1 = new VSTGUI::CKnob(size, this, 0, hKnobBack, hKnobPoint, CPoint(0, 0));
    CKnob1->setDefaultValue(0.5);
    CKnob1->setValue(this->effect->getParameter(0));
    lFrame->addView(CKnob1);
    //===============================================================
    int fontsize = 15;
    int width = fontsize * 3;
    int height = fontsize + 4;
    x = fontsize;
    y = hBackground->getHeight() - fontsize * 2;
    size(x, y, x + width, y + height);
    //===============================================================
    CParam1 = new VSTGUI::CParamDisplay(size, 0, 0);
    CParam1->setDefaultValue(0.5);
    CParam1->setValue(this->effect->getParameter(0));
    CParam1->setTransparency(true);
    CParam1->setBackColor(kTransparentCColor);
    CParam1->setStyle(VSTGUI::kBoldFace);
    CParam1->setStringConvert(ST_Pitch2Text);
    CParam1->setAntialias(true);
    //===============================================================
    lFrame->addView(CParam1);
    hKnobBack->forget();
    hKnobPoint->forget();
    //===============================================================
    return true;
}
//===================================================================

//===================================================================
void PitchShifterEditor::close()
{
    delete frame;
    frame = nullptr;
}
//===================================================================
void PitchShifterEditor::setParameter(VstInt32 index, float value)
{
    if (frame == nullptr) return;
    //===============================================================
    switch (index)
    {
    case 0:
        if (CKnob1)
        {
            CKnob1->setValue(effect->getParameter(index));
        };
        //===========================================================
        if (CParam1)
        {
            CParam1->setValue(this->effect->getParameter(0));
        };
        //===========================================================
        break;
    }
}
//===================================================================
void PitchShifterEditor::valueChanged(CControl* pControl)
{
    //===============================================================
    long tag = pControl->getTag();
    switch (tag)
    {
    case 0:
        effect->setParameterAutomated(tag, pControl->getValue());
        if (CKnob1)
        {
            CKnob1->setValue(pControl->getValue());
        };
        //===========================================================
        if (CParam1)
        {
            CParam1->setValue(this->effect->getParameter(0));
        };
        //===========================================================
        break;
    }
}
//===================================================================
void PitchShifterEditor::idle()
{
    AEffGUIEditor::idle();
    //===============================================================
    CKnob1->setValue(effect->getParameter(0));
    CParam1->setValue(effect->getParameter(0));
    CKnob1->invalid(); // update and redraw
    CParam1->invalid();// update and redraw
}
//===================================================================