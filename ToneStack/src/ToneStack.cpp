#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lv2.h>
#include "wdfTonestackTree.hpp"


/**********************************************************************************************************************************************************/

#define PLUGIN_URI "http://aidadsp.cc/plugins/wt-rdf_lv2/ToneStack"
#define TAMANHO_DO_BUFFER 1024
enum {IN, OUT_1, BASS, MID, HIGH, PLUGIN_PORT_COUNT};

/**********************************************************************************************************************************************************/

class ToneStack
{
public:
    ToneStack() {}
    ~ToneStack() {}
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features);
    static void activate(LV2_Handle instance);
    static void deactivate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void *data);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void cleanup(LV2_Handle instance);
    static const void* extension_data(const char* uri);
    float *in;
    float *out_1;
    float *Bass;
    float *Mid;
    float *High;

    float b;
    float m;
    float h;

private:
    wdfTonestackTree *ToneStackTree;
};

/**********************************************************************************************************************************************************/

static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    ToneStack::instantiate,
    ToneStack::connect_port,
    ToneStack::activate,
    ToneStack::run,
    ToneStack::deactivate,
    ToneStack::cleanup,
    ToneStack::extension_data
};

/**********************************************************************************************************************************************************/

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}

/**********************************************************************************************************************************************************/

LV2_Handle ToneStack::instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features)
{
    ToneStack *plugin = new ToneStack();
    plugin->ToneStackTree = new wdfTonestackTree();
    
    plugin->b = 0.5;
    plugin->m = 0.5;
    plugin->h = 0.5;
    
    plugin->ToneStackTree->initTree();
    //plugin->ToneStackTree->setSamplerate(48000);
    plugin->ToneStackTree->adaptTree();
    
    plugin->ToneStackTree->setParam(0, plugin->b); // Bass
    plugin->ToneStackTree->setParam(1, plugin->m); // Mid
    plugin->ToneStackTree->setParam(1, plugin->h); // High

    return (LV2_Handle)plugin;
}

/**********************************************************************************************************************************************************/

void ToneStack::activate(LV2_Handle instance)
{
    // TODO: include the activate function code here
}

/**********************************************************************************************************************************************************/

void ToneStack::deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}

/**********************************************************************************************************************************************************/

void ToneStack::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    ToneStack *plugin;
    plugin = (ToneStack *) instance;

    switch (port)
    {
        case IN:
            plugin->in = (float*) data;
            break;
        case OUT_1:
            plugin->out_1 = (float*) data;
            break;
        case BASS:
            plugin->Bass = (float*) data;
            break;
        case MID:
            plugin->Mid = (float*) data;
            break;
        case HIGH:
            plugin->High = (float*) data;
            break;
    }
}

/**********************************************************************************************************************************************************/

void ToneStack::run(LV2_Handle instance, uint32_t n_samples)
{
    ToneStack *plugin;
    plugin = (ToneStack *) instance;
    
    plugin->b = *plugin->Bass;
    plugin->m = *plugin->Mid;
    plugin->h = *plugin->High;

    plugin->ToneStackTree->setParam(0, plugin->b); // Bass
    plugin->ToneStackTree->setParam(1, plugin->m); // Mid
    plugin->ToneStackTree->setParam(1, plugin->h); // High
    
    // Oversample?
    for (uint32_t i=0; i<n_samples; i++)
	{
        float inVoltage = plugin->in[i];
        plugin->ToneStackTree->setInputValue(inVoltage);
        plugin->ToneStackTree->cycleWave();
		plugin->out_1[i] = (float)(plugin->ToneStackTree->getOutputValue());
	}
    // Downsample?
}

/**********************************************************************************************************************************************************/

void ToneStack::cleanup(LV2_Handle instance)
{
    delete ((ToneStack *) instance);
}

/**********************************************************************************************************************************************************/

const void* ToneStack::extension_data(const char* uri)
{
    return NULL;
}
