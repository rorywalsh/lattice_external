#pragma once
#include "LatticeProcessorModule.h"
#include <iterator>
#include "Conv.h"
#include "simple_svg_1.0.0.hpp"

using namespace Aurora;

class TVConvProcessor : public LatticeProcessorModule
{



  
public:
    TVConvProcessor();
    
    ChannelData createChannels() override;
    
    /* This function is called by he host to populate the parameter vector */
    ParameterData createParameters() override;
    
    void hostParameterChanged(const char* parameterID, float newValue) override;
    /*  This function is called by the host before playback/performance */
    void prepareProcessor(int sr, std::size_t block) override;
    
    /*  Main processing function called continuously by the host on the audio thread.
        paramValues is a list of parameter values passed from the host in order of their creation */
    void process(float** buffer, std::size_t blockSize) override;
    
    void createDescription(std::string& description)
    {
        description = "";
    }
 
    int getModuleType() override
    {
        return ModuleType::AudioProcessor::filters;
    }
    
    static float remap(float value, float from1, float to1, float from2, float to2)
    {
        return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
    }
    
    const char* getModuleName() override {    return "Time Varying Convolution";     }
    
private:
    Conv<float> conv;
    bool isPlaying = false;
    int sampleIndex = 0;
    std::vector<float> in1, in2;
    float fs;
    int len;
    bool bypass = false;
};

