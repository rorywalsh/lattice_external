
#include "SimpleSynthProcessor.h"
#include <iterator>


SimpleSynthProcessor::Synth::Synth(float rt, float sr)
: att(0.1f), dec(0.3f), sus(0.7f), rel(rt),
squareWave(Aurora::SQUARE),
triangleWave(Aurora::TRIANGLE),
sawWave(Aurora::SAW),
env(Aurora::ads_gen(att, dec, sus), rt, Aurora::def_sr),
osc(&sawWave,sr)
{

}

void SimpleSynthProcessor::Synth::setWaveform(int waveForm)
{
    switch(waveForm)
    {
        case 1:
            osc.waveset(&sawWave);
            return;
        case 2:
            osc.waveset(&squareWave);
            return;
        case 3:
            osc.waveset(&triangleWave);
            return;
    }
}

void SimpleSynthProcessor::Synth::setBlockSize(int blockSize)
{
    osc.vsize(blockSize);
    env.vsize(blockSize);
}

//======================================================================================
SimpleSynthProcessor::SimpleSynthProcessor()
:synth(.1, 44100)
{
    
}

void SimpleSynthProcessor::createChannelLayout(std::vector<std::string> &inputs, std::vector<std::string> &outputs)
{
    inputs.push_back("Input 1");
    inputs.push_back("Input 2");
    outputs.push_back("Output 1");
    outputs.push_back("Output 2");
}

void SimpleSynthProcessor::createParameters(std::vector<ModuleParameter> &parameters)
{
    parameters.push_back({"Attack", {0, 1, 0.001, 0.001, 1}});
    parameters.push_back({"Decay", {0, 2, 0.1, 0.001, 1}});
    parameters.push_back({"Sustain", {0, 1, 0.001, 0.001, 1}});
    parameters.push_back({"Release", {0, 3, 0.001, 0.001, 1}});
    parameters.push_back({"Wave", {1, 3, 1, 1, 1}});
}

void SimpleSynthProcessor::hostParameterChanged(const std::string& parameterID, float newValue)
{
    if(parameterID == "Wave")
    {
        synth.setWaveform(int(newValue));
    }
    else if(parameterID == "Attack")
    {
        synth.setAttack(newValue);
    }
    else if(parameterID == "Decay")
    {
        synth.setDecay(newValue);
    }
    else if(parameterID == "Sustain")
    {
        synth.setSustain(newValue);
    }
    else if(parameterID == "Release")
    {
        synth.setRelease(newValue);
    }    
}

void SimpleSynthProcessor::prepareProcessor(int /* sr */, int /* block */)
{

}

void SimpleSynthProcessor::startNote(int midiNoteNumber, float/* velocity */)
{
    setMidiNoteNumber(midiNoteNumber);
    isNoteOn = true;
}

void SimpleSynthProcessor::stopNote (float /* velocity */)
{
    isNoteOn = false;
}

void SimpleSynthProcessor::triggerParameterUpdate(const std::string& parameterID, float newValue)
{
    updateParameter(parameterID, newValue);
}

void SimpleSynthProcessor::process(float** buffer, int numChannels, int blockSize, const HostInfo)
{
    const float freq = getMidiNoteInHertz(getMidiNoteNumber(), 440);
    synth.setBlockSize(blockSize);
    const std::vector<float> &out = synth(1, freq, isNoteOn);
    for(int i = 0; i < blockSize ; i++)
      for(int chan = 0 ;  chan < numChannels; chan++)
          buffer[chan][i] = out[i];
    
    
}

// the class factories
#ifdef WIN32
    extern "C" 
    {
        __declspec(dllexport) LatticeProcessorModule* create() { return new SimpleSynthProcessor; }
    };

    extern "C" 
    {
        __declspec(dllexport) void destroy(LatticeProcessorModule* p) { delete p; }
    };
#else
    extern "C" LatticeProcessorModule* create(){     return new SimpleSynthProcessor;         }
    extern "C" void destroy(LatticeProcessorModule* p){      delete p;                     }
#endif
