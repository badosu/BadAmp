#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Dial.H>
#include <FL/x.H>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "amp.h"

typedef struct {
	Fl_Dial *volume_control;
	float gain;

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;
} AmpGui;

void volume_changed(Fl_Widget* widget, void* data) {
	Fl_Dial* dial = (Fl_Dial*)widget;
	AmpGui *pluginGui = (AmpGui*) data;

	pluginGui->gain = dial->value();
	pluginGui->write_function(pluginGui->controller, AMP_GAIN, sizeof(pluginGui->gain), 0, &pluginGui->gain);
}

Fl_Window* create_window(void* parentXwindow, AmpGui* pluginGui) {
	fl_open_display();

	Fl_Window* window = new Fl_Double_Window(300, 160);

	window->begin();
	pluginGui->volume_control = new Fl_Dial(100, 20, 100, 100, "Gain");
	pluginGui->volume_control->range(-90, 24);
	pluginGui->volume_control->step(0.1);
	pluginGui->volume_control->callback(volume_changed, (void *)pluginGui);
	window->end();

	fl_embed(window, (Window)parentXwindow);

	return window;
}

	static int
idle(LV2UI_Handle handle)
{
	Fl::check();
	Fl::flush();

	return 0;
}

static const LV2UI_Idle_Interface idle_iface = { idle };

extern LV2UI_Handle
instantiate(const struct _LV2UI_Descriptor * descriptor,
		const char* plugin_uri, const char* bundle_path,
		LV2UI_Write_Function write_function,
		LV2UI_Controller controller, LV2UI_Widget* widget,
		const LV2_Feature* const * features) {

	if (strcmp(plugin_uri, AMP_URI) != 0) {
		fprintf(stderr, "AMP_UI error: this GUI does not support plugin with URI %s\n", plugin_uri);
		return NULL;
	}

	AmpGui* pluginGui = (AmpGui*)malloc(sizeof(AmpGui));

	if (pluginGui == NULL) return NULL;

	void* parentXwindow = 0;
	LV2UI_Resize* resize;
	int i;
	for (i = 0; features[i]; ++i) {
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parentXwindow = features[i]->data;
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
		}
	}

	Fl_Window* window = create_window(parentXwindow, pluginGui);

	pluginGui->controller = controller;
	pluginGui->write_function = write_function;

	if (resize) {
		resize->ui_resize(resize->handle, window->w(), window->h());
	}

	return (LV2UI_Handle)pluginGui;
}

extern void
cleanup(LV2UI_Handle ui) {
	AmpGui* pluginGui = (AmpGui*) ui;

	free(pluginGui);
}

extern void
port_event(LV2UI_Handle ui,
		uint32_t port_index,
		uint32_t buffer_size,
		uint32_t format,
		const void * buffer) {
	AmpGui* pluginGui = (AmpGui*) ui;
	float* pval = (float*) buffer;

	if ((format != 0) || (port_index < 0) || (port_index >= AMP_N_PORTS)) {
		return;
	}

	pluginGui->volume_control->value(*pval);
}

extern const void*
extension_data(const char* uri) {
	if (!strcmp(uri, LV2_UI__idleInterface)) { return &idle_iface; }

	return NULL;
}

static LV2UI_Descriptor descriptor = {
	AMP_UI_URI, instantiate, cleanup, port_event, extension_data
};

const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index) {
	switch (index) {
		case 0:  return &descriptor;
		default: return NULL;
	}
}
