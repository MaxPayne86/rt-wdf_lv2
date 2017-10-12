#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lv2.h>
#include "wdfJTM45Tree.hpp"

/**********************************************************************************************************************************************************/

#define PLUGIN_URI "http://moddevices.com/plugins/mod-devel/Gain"
#define TAMANHO_DO_BUFFER 1024
enum {IN, OUT_1, GAIN, PLUGIN_PORT_COUNT};

/**********************************************************************************************************************************************************/

class GainControl
{
public:
    GainControl() {}
    ~GainControl() {}
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

    float g;
};

/**********************************************************************************************************************************************************/

static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    GainControl::instantiate,
    GainControl::connect_port,
    GainControl::activate,
    GainControl::run,
    GainControl::deactivate,
    GainControl::cleanup,
    GainControl::extension_data
};

/**********************************************************************************************************************************************************/

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}

/**********************************************************************************************************************************************************/

LV2_Handle GainControl::instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features)
{
    GainControl *plugin = new GainControl();

    plugin->g = 1;

    return (LV2_Handle)plugin;
}

/**********************************************************************************************************************************************************/

void GainControl::activate(LV2_Handle instance)
{
    // TODO: include the activate function code here
}

/**********************************************************************************************************************************************************/

void GainControl::deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}

/**********************************************************************************************************************************************************/

void GainControl::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    GainControl *plugin;
    plugin = (GainControl *) instance;

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
    }
}

/**********************************************************************************************************************************************************/

void GainControl::run(LV2_Handle instance, uint32_t n_samples)
{
    GainControl *plugin;
    plugin = (GainControl *) instance;
    float g_before = plugin->g;
    plugin->g = powf(10.0f, (*plugin->gain)/20.0f);

    for (uint32_t i=0; i<n_samples; i++)
	{
		plugin->out_1[i] = (g_before + ((plugin->g - g_before)/n_samples) * i) * plugin->in[i];
	}

}

/**********************************************************************************************************************************************************/

void GainControl::cleanup(LV2_Handle instance)
{
    delete ((GainControl *) instance);
}

/**********************************************************************************************************************************************************/

const void* GainControl::extension_data(const char* uri)
{
    return NULL;
}
