//===================================================================
#include "PitchShifter.h"
//===================================================================

//===================================================================   
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new PitchShift(audioMaster);
}
//===================================================================

//===================================================================
PitchShift::PitchShift(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 1, 1)   // 1 program, 1 parameter only
    , fParam0(0.5f)
    , cSoundTouch1 ( NULL )
{
    setNumInputs(2);// stereo in
    setNumOutputs(2);// stereo out
    setUniqueID(PLUGIN_CODE);
    canProcessReplacing();
    canDoubleReplacing();
    //===============================================================
    vst_strncpy(programName, "Default", kVstMaxProgNameLen);// default program name
    editor = new PitchShifterEditor(this);
    //===============================================================
    cSoundTouch1 = new soundtouch::SoundTouch();
    //===============================================================
    cSoundTouch1->setSampleRate((uint)this->sampleRate);
    cSoundTouch1->setPitch(1.0);
    cSoundTouch1->setTempo(1);
    cSoundTouch1->setRate(1);
    cSoundTouch1->setChannels(2);
    //===============================================================
    setInitialDelay(this->getSampleRate()*0.001 * 100);//100ms
    updateDisplay();
}
//===================================================================
PitchShift::~PitchShift()
{
    // called from AudioEffectX.
    // if (editor != nullptr)
    // {
    //   delete editor;
    // }

    if (cSoundTouch1 != NULL)
    {
        delete cSoundTouch1;
        cSoundTouch1 = NULL;
    }
}
//===================================================================
void PitchShift::resume()
{
    cSoundTouch1->setSampleRate((uint)this->sampleRate);
    cSoundTouch1->setPitch(1.0);
    cSoundTouch1->setTempo(1);
    cSoundTouch1->setRate(1);
    cSoundTouch1->setChannels(2); //need call this
    //===============================================================
    setInitialDelay(this->getSampleRate()*0.001 * 100);//100ms
    updateDisplay();
}
//===================================================================
void PitchShift::suspend()
{
    cSoundTouch1->clear();
    //===============================================================
    setInitialDelay(0);//0ms
    updateDisplay();
}
//===================================================================
void PitchShift::setParameter(VstInt32 index, float value)
{
    fParam0 = value;
    //===============================================================
    if (fParam0 >= 0.49f && fParam0 <= 0.51)
    {
        cSoundTouch1->setPitchSemiTones(0);
    }
    else
    {
#ifdef LIMIT_INT_PARAM
        cSoundTouch1->setPitchSemiTones((int)(ST_PITCH_MAX_PITCH*(fParam0 - 0.5)));
#else
        cSoundTouch1->setPitchSemiTones((float)(ST_PITCH_MAX_PITCH*(fParam0 - 0.5)));
#endif
    }
}
//===================================================================
float PitchShift::getParameter(VstInt32 index)
{
    return fParam0;
}
//===================================================================
void PitchShift::getParameterName(VstInt32 index, char* label)
{
    vst_strncpy(label, "Pitch", kVstMaxParamStrLen);
}
//===================================================================
void PitchShift::getParameterDisplay(VstInt32 index, char* text)
{
    float2string(fParam0, text, 16);
}
//===================================================================
void PitchShift::getParameterLabel(VstInt32 index, char* label)
{
    vst_strncpy(label, "semitone", kVstMaxParamStrLen);
}
//===================================================================
void PitchShift::setProgramName(char* name)
{
    vst_strncpy(programName, name, kVstMaxProgNameLen);
}
//===================================================================
void PitchShift::getProgramName(char* name)
{
    vst_strncpy(name, programName, kVstMaxProgNameLen);
}
//===================================================================
void PitchShift::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1 = inputs[0];
    float* in2 = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    //===============================================================
    VstInt32 sample2 = sampleFrames;
    VstInt32 count = 0;
    //===============================================================
    float * pBuffer = new float[sampleFrames * 2];
    while (--sample2 >= 0)
    {
        pBuffer[count] = *in1++;
        pBuffer[count + 1] = *in2++;
        count += 2;
    }
    //===============================================================
    cSoundTouch1->setChannels(2);
    cSoundTouch1->putSamples(pBuffer, sampleFrames);
    cSoundTouch1->receiveSamples(pBuffer, sampleFrames);
    //===============================================================
    count = 0;
    while (--sampleFrames >= 0)
    {
        (*out1++) = pBuffer[count];
        (*out2++) = pBuffer[count + 1];
        count += 2;
    }
    //===============================================================
    delete[] pBuffer;
}
//===================================================================
void PitchShift::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1 = inputs[0];
    double* in2 = inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    //===============================================================
    VstInt32 sample2 = sampleFrames;
    VstInt32 count = 0;
    //===============================================================
    float* pBuffer = new float[sampleFrames * 2];
    while (--sample2 >= 0)
    {
        pBuffer[count] = (float)*in1++;
        pBuffer[count + 1] = (float)*in2++;
        count = count + 2;
    }
    //===============================================================
    cSoundTouch1->setChannels(2);
    cSoundTouch1->putSamples(pBuffer, sampleFrames);
    cSoundTouch1->receiveSamples(pBuffer, sampleFrames);
    //===============================================================
    count = 0;
    while (--sampleFrames >= 0)
    {
        (*out1++) = (double)pBuffer[count];
        (*out2++) = (double)pBuffer[count + 1];
        count = count + 2;
    }
    //===============================================================
    delete[] pBuffer;
}
//===================================================================