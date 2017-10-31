#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lv2.h>
#include "wdfJTM45Tree.hpp"

/**********************************************************************************************************************************************************/

#define PLUGIN_URI "http://aidadsp.cc/plugins/wt-rdf_lv2/JTM45"
#define TAMANHO_DO_BUFFER 1024
enum {IN, OUT_1, GAIN, VOLUME, PLUGIN_PORT_COUNT};

/**********************************************************************************************************************************************************/

class JTM45
{
public:
    JTM45() {}
    ~JTM45() {}
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features);
    static void activate(LV2_Handle instance);
    static void deactivate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void *data);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void cleanup(LV2_Handle instance);
    static const void* extension_data(const char* uri);
    float *in;
    float *out_1;
    float *gain;
    float *volume;

    float g;
    float v;
    
    wdfJTM45Tree *JTM45plugin;
private:
    //wdfJTM45Tree JTM45;
};

/**********************************************************************************************************************************************************/

static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    JTM45::instantiate,
    JTM45::connect_port,
    JTM45::activate,
    JTM45::run,
    JTM45::deactivate,
    JTM45::cleanup,
    JTM45::extension_data
};

/**********************************************************************************************************************************************************/

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}

/**********************************************************************************************************************************************************/

LV2_Handle JTM45::instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features)
{
    JTM45 *plugin = new JTM45();
    plugin->JTM45plugin = new wdfJTM45Tree();
    
    plugin->g = 0.1;
    plugin->v = 0.1;
    
    plugin->JTM45plugin->initTree();
    //JTM45plugin->setSamplerate(48000);
    plugin->JTM45plugin->adaptTree();
    
    plugin->JTM45plugin->setParam(0, plugin->v); // Volume
    plugin->JTM45plugin->setParam(1, plugin->g); // Gain

    return (LV2_Handle)plugin;
}

/**********************************************************************************************************************************************************/

void JTM45::activate(LV2_Handle instance)
{
    // TODO: include the activate function code here
}

/**********************************************************************************************************************************************************/

void JTM45::deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}

/**********************************************************************************************************************************************************/

void JTM45::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    JTM45 *plugin;
    plugin = (JTM45 *) instance;

    switch (port)
    {
        case IN:
            plugin->in = (float*) data;
            break;
        case OUT_1:
            plugin->out_1 = (float*) data;
            break;
        case GAIN:
            plugin->gain = (float*) data;
            break;
        case VOLUME:
            plugin->volume = (float*) data;
            break;
    }
}

/**********************************************************************************************************************************************************/

void JTM45::run(LV2_Handle instance, uint32_t n_samples)
{
    JTM45 *plugin;
    plugin = (JTM45 *) instance;
    
    plugin->g = *plugin->gain;
    plugin->v = *plugin->volume;

    plugin->JTM45plugin->setParam(0, plugin->v); // Volume
    plugin->JTM45plugin->setParam(1, plugin->g); // Gain
    
    // Oversample?
    for (uint32_t i=0; i<n_samples; i++)
	{
        float inVoltage = plugin->in[i];
        plugin->JTM45plugin->setInputValue(inVoltage);
        plugin->JTM45plugin->cycleWave();
		plugin->out_1[i] = (float)(plugin->JTM45plugin->getOutputValue());
	}
    // Downsample?
}

/**********************************************************************************************************************************************************/

void JTM45::cleanup(LV2_Handle instance)
{
    delete ((JTM45 *) instance);
}

/**********************************************************************************************************************************************************/

const void* JTM45::extension_data(const char* uri)
{
    return NULL;
}
