#pragma once
#include "LatticeProcessorModule.h"
#include <iterator>
#include "simple_svg_1.0.0.hpp"

class FilePlayerProcessor : public LatticeProcessorModule
{
public:
    FilePlayerProcessor();
    
    void createChannelLayout(DynamicArray<const char*> &inputs, DynamicArray<const char*> &outputs) override;
    
    /* This function is called by he host to populate the parameter vector */
    void createParameters(std::vector<ModuleParameter> &parameters) override;
    
    /*  This function is called by the host whenever a parameter changes */
    void hostParameterChanged(const std::string& parameterID, float newValue);
    
    void hostParameterChanged(const std::string& parameterID, std::string newValue);
    
    /*  This function is called by the host before playback/performance */
    void prepareProcessor(int sr, std::size_t block) override;
    
    /* Call this method to trigger host callback */
    void triggerParameterUpdate(const std::string& parameterID, float newValue);
    
    /*  Main processing function called continuously by the host on the audio thread.
        paramValues is a list of parameter values passed from the host in order of their creation */
    void process(float** buffer, int numChannels, std::size_t blockSize, const HostData hostInfo) override;
    
    void createDescription(std::string& description)
    {
        description = "";
    }

    
    std::string getModuleName() override {    return "Soundfile Player";     }
    

private:
};

