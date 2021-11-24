
#include "LatticeExternalProcessor.h"

/*  Called by the host to create set of parameters */
std::vector<ExternalParameter> LatticeProcessor::createParameters()
{
    std::vector<ExternalParameter> parameters;
    ExternalParameter param { "Gain", {0, 1, 0.001, 001, 1}}; // { min, max, increment, defaultValue, skew }
    parameters.push_back(param);
    return parameters;
}

/*  This function is called by the host before playback/performance */
void LatticeProcessor::prepareProcessor(int sr, int block)
{
    ignoreParameters(sr, block);
}

/*  Main processing function called continously by the host on the audio thread.
    paramValues is a list of parameter values passed from the host in order of their creation */
void LatticeProcessor::process(float** buffer, int numChannels, int blockSize, std::vector<std::atomic<float>*> paramValues)
{
    for ( int i = 0 ; i < blockSize ; i++)
    {
        for ( int chan = 0 ; chan < numChannels; chan++)
        {
            float gain = (*paramValues[0]);
            buffer[chan][i] = buffer[chan][i] * 0;//gain;
        }
    }
}

/* wrappers for class members - do not edit */
extern "C" std::vector<ExternalParameter> createParameters(){
    struct LatticeProcessor p;
    p.createParameters();
}

extern "C" void prepareProcessor(int sr, int bs){
    struct LatticeProcessor p;
    p.prepareProcessor(sr, bs);
}

extern "C" void process(float** buffer, int channels, int blockSize, std::vector<std::atomic<float>*> params){
    struct LatticeProcessor p;
    p.process(buffer, channels, blockSize, params);
}
