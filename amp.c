#include <math.h>
#include <stdlib.h>

#include "lv2.h"
#include "amp.h"

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

typedef struct {
	const float* gain;
	const float* input;
	float*       output;
} Amp;

static LV2_Handle
instantiate(const LV2_Descriptor* descriptor, double rate,
	const char* bundle_path, const LV2_Feature* const* features)
{
	Amp* amp = (Amp*)malloc(sizeof(Amp));

	return (LV2_Handle)amp;
}

static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
	Amp* amp = (Amp*)instance;

	switch ((PortIndex)port) {
		case AMP_GAIN:
			amp->gain = (const float*)data;
			break;
		case AMP_INPUT:
			amp->input = (const float*)data;
			break;
		case AMP_OUTPUT:
			amp->output = (float*)data;
			break;
	}
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
	const Amp* amp = (const Amp*)instance;

	const float        gain   = *(amp->gain);
	const float* const input  = amp->input;
	float* const       output = amp->output;

	const float coef = DB_CO(gain);

	uint32_t pos;
	for (pos = 0; pos < n_samples; pos++) {
		output[pos] = input[pos] * coef;
	}
}

static void
cleanup(LV2_Handle instance)
{
	free(instance);
}

static void
activate(LV2_Handle instance)
{
}

static void
deactivate(LV2_Handle instance)
{
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

static const LV2_Descriptor descriptor = {
	AMP_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	  case 0:  return &descriptor;
	  default: return NULL;
	}
}
