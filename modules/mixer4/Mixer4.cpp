
#include "Mixer4.h"
#include <iterator>
#include <sstream>


Mixer4::Mixer4()
	:LatticeProcessorModule()
{
    for (int i = 0; i < 4; i++)
        gainParams.push_back("Gain " + std::to_string(i+1));
    for (int i = 0; i < 4; i++)
        panParams.push_back("Pan " + std::to_string(i + 1));
}

LatticeProcessorModule::ChannelData Mixer4::createChannels()
{
	addChannel({ "input1", ChannelType::input });
	addChannel({ "input2", ChannelType::input });
    addChannel({ "input3", ChannelType::input });
    addChannel({ "input4", ChannelType::input });
	addChannel({ "outputL", ChannelType::output });
    addChannel({ "outputR", ChannelType::output });
	return {getChannels(), getNumberOfChannels()};
}



LatticeProcessorModule::ParameterData Mixer4::createParameters()
{
    addParameter({"Gain 1", LatticeProcessorModule::Parameter::Range(0.f, 1.f, 0.5f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Pan 1", LatticeProcessorModule::Parameter::Range(-1.f, 1.f, 0.0f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Gain 2", LatticeProcessorModule::Parameter::Range(0.f, 1.f, 0.5f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Pan 2", LatticeProcessorModule::Parameter::Range(-1.f, 1.f, 0.0f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Gain 3", LatticeProcessorModule::Parameter::Range(0.f, 1.f, 0.5f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Pan 3", LatticeProcessorModule::Parameter::Range(-1.f, 1.f, 0.0f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Gain 4", LatticeProcessorModule::Parameter::Range(0.f, 1.f, 0.5f, 0.001f, 1.f), Parameter::Type::Slider, true});
    addParameter({"Pan 4", LatticeProcessorModule::Parameter::Range(-1.f, 1.f, 0.0f, 0.001f, 1.f), Parameter::Type::Slider, true});
    return {getParameters(), getNumberOfParameters()};
}

void Mixer4::prepareProcessor(int sr, std::size_t block)
{
  fs = sr;
}  


void Mixer4::process(float** buffer, std::size_t blockSize)
{
    float gain[4];
    float pan[4];

    for (int i = 0; i < 4; i++)
    {
        gain[i] = gainSmooth[i](getParameter(gainParams[i].c_str()), 0.001);
        pan[i] = panSmooth[i]((getParameter(panParams[i].c_str())+1.f / 2.f), 0.001);
    }

    
    for(int i = 0; i < blockSize ; i++)
    {
        float ins[4] = {};
        ins[0] = buffer[0][i];
        ins[1] = buffer[1][i];
        ins[2] = buffer[2][i];
        ins[3] = buffer[3][i];
        
        buffer[0][i] = (1 - pan[0]) * (isInputConnected(0) ? ins[0] * gain[0] : 0);
        buffer[1][i] = (pan[0]) * (isInputConnected(0) ? ins[0] * gain[0] : 0);

        for (int x = 1; x < 4; x++)
        {
            buffer[0][i] += (1 - pan[x]) * (isInputConnected(i) ? ins[x] * gain[x] : 0);
            buffer[1][i] += (pan[x]) * (isInputConnected(i) ? ins[x] * gain[x] : 0);
        }
    }

}


// the class factories
#ifdef WIN32
extern "C" 
{
	__declspec(dllexport) LatticeProcessorModule* create() { return new Mixer4; }
};

extern "C" 
{
	__declspec(dllexport) void destroy(LatticeProcessorModule* p) { delete p; }
};
#else
extern "C" LatticeProcessorModule* create(){             return new Mixer4;         }
extern "C" void destroy(LatticeProcessorModule* p){      delete p;                     }
#endif
