#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "amp.h"

#define AMP_UI_URI  "http://bado.so/bad-amp#gtk"

typedef struct {
	GtkWidget* gain_control;
	float gain;

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;
} AmpGui;

static int
value_changed_cb(GtkWidget* widget, gpointer data) {
	AmpGui *pluginGui = (AmpGui*)data;

	pluginGui->gain = (float)gtk_range_get_value((GtkRange*)widget);
	pluginGui->write_function(pluginGui->controller, AMP_GAIN, sizeof(pluginGui->gain), 0, &pluginGui->gain);
}

static GtkWidget*
make_gui(AmpGui* pluginGui) {
	GtkWidget* container = gtk_vbox_new(FALSE, 2);

	pluginGui->gain_control = gtk_vscale_new_with_range(-90, 24, 1);
	gtk_range_set_inverted((GtkRange*)pluginGui->gain_control, TRUE);
	gtk_range_set_value((GtkRange*)pluginGui->gain_control, 0);
	g_signal_connect_after(pluginGui->gain_control, "value_changed",
		G_CALLBACK(value_changed_cb), pluginGui);
	gtk_container_add((GtkContainer*)container, (GtkWidget*)pluginGui->gain_control);
	gtk_widget_set_size_request(container, 50,200);

	return container;
}

static LV2UI_Handle
instantiate(const struct _LV2UI_Descriptor * descriptor,
	const char* plugin_uri, const char* bundle_path,
	LV2UI_Write_Function write_function,
	LV2UI_Controller controller, LV2UI_Widget* widget,
	const LV2_Feature* const* features) {

	if (strcmp(plugin_uri, AMP_URI) != 0) {
		fprintf(stderr, "AMP_UI error: this GUI does not support plugin with URI %s\n", plugin_uri);
		return NULL;
	}

	AmpGui* pluginGui = (AmpGui*)malloc(sizeof(AmpGui));

	if (pluginGui == NULL) return NULL;

	pluginGui->controller = controller;
	pluginGui->write_function = write_function;

	*widget = (LV2UI_Widget)make_gui(pluginGui);

	return (LV2UI_Handle)pluginGui;
}

static void
cleanup(LV2UI_Handle ui) {
	AmpGui* pluginGui = (AmpGui*) ui;

	free(pluginGui);
}

static void
port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size,
	uint32_t format, const void* buffer) {
	AmpGui* pluginGui = (AmpGui*) ui;
	float* pval = (float*) buffer;

	if ((format != 0) || (port_index < 0) || (port_index >= AMP_N_PORTS)) {
		return;
	}

	gtk_range_set_value((GtkRange*)pluginGui->gain_control, *pval);
}

static const void*
extension_data(const char* uri) { return NULL; }

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
