#include <QObject>
#include <QWidget>
#include <QDial>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>

#include <stdio.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "amp.h"

class AmpGui : public QWidget {
	Q_OBJECT

public:
    AmpGui(QWidget* parent = 0);

	QDial* volume_dial;
	QLabel* volume_label;
	float gain;

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;

public slots:
	void volumeChanged(int value);

};

AmpGui::AmpGui(QWidget *parent)
	: QWidget(parent) {
	volume_dial = new QDial();
	volume_label = new QLabel();

	volume_label->setText("0 dB");
	volume_dial->setRange(-900, 240);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(volume_dial);
	layout->addWidget(volume_label);
	setLayout(layout);
}

void AmpGui::volumeChanged(int value) {
	float gain = (float)volume_dial->value()/10.0f;

	volume_label->setText(QString("%1 dB").arg(gain));
	write_function(controller, AMP_GAIN, sizeof(gain), 0, &gain);
}

LV2UI_Handle
instantiate(const struct _LV2UI_Descriptor * descriptor,
		const char* plugin_uri, const char* bundle_path,
		LV2UI_Write_Function write_function,
		LV2UI_Controller controller, LV2UI_Widget* widget,
		const LV2_Feature* const * features) {

	if (strcmp(plugin_uri, AMP_URI) != 0) {
		fprintf(stderr, "AMP_UI error: this GUI does not support plugin with URI %s\n", plugin_uri);
		return NULL;
	}

	AmpGui* pluginGui = new AmpGui();
	*widget = pluginGui;

	if (pluginGui == NULL) return NULL;

	QObject::connect(pluginGui->volume_dial, SIGNAL(valueChanged(int)),
		pluginGui, SLOT(volumeChanged(int)));

	pluginGui->controller = controller;
	pluginGui->write_function = write_function;

	return (LV2UI_Handle)pluginGui;
}

void
cleanup(LV2UI_Handle ui) { }

void
port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size,
	uint32_t format, const void * buffer) {
	AmpGui* pluginGui = (AmpGui*) ui;
	float* pval = (float*) buffer;

	if ((format != 0) || (port_index < 0) || (port_index >= AMP_N_PORTS)) {
		return;
	}

	pluginGui->volume_dial->setValue((int)(*pval + 0.5));
}

const void*
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

#include "amp_gui_qt.moc.cpp"
