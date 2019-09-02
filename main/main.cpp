//ESP libs
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string>

//my libs
#include "mWifiClient.h"
#include "mMqttClient.h"

#include "main.h"
#include "neopixel.h"
#include "effects/snake.h"

using namespace std;

static const char *logTag = "Main";

static string mqttDataTopic = CONFIG_MQTT_DATA_TOPIC;
static string mqttCommandTopic = CONFIG_MQTT_COMMAND_TOPIC;
static string mqttCommandReturnTopic; // will be the mqttCommandTopic + "Ret"
static const string mqttBrokerAddress = CONFIG_MQTT_BROKER_ADDRESS;

extern "C"
{
	void app_main(void);
}

void app_main()
{
	Main m = Main();
	m.app_main();
}

void Main::app_main(void)
{
	// wifiClient = new mWifiClient(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, 5);
	// StartWifi();
	// mqttClient = new mMqttClient(mqttBrokerAddress);
	// mqttCommandReturnTopic = mqttCommandTopic + "Ret";
	// mqttClient->Subscribe(mqttCommandTopic, 1, Main::HandleMqttMessage, this);

	int pixelCount = 300;
	Pixels* pixels = new Pixels(GPIO_NUM_13, pixelCount, Pixels::StripType::ws6812, RMT_CHANNEL_0);

	while(true)
	{
		Snake::Run(pixels, pixelCount, 18, 8);
	}
}

void Main::StartWifi()
{
	int retryCount = 0;
	while (!wifiClient->Connect(++retryCount * 5000) && retryCount < 5)
	{
		ESP_LOGI(logTag, "Failed to connect. Retrying.. %d/5", retryCount);
	}
}

void Main::HandleMqttMessage(string topic, string message, void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessCommand(message);
}

void Main::ProcessCommand(string command)
{
	size_t separatorPos = command.find(':');
	if (separatorPos != string::npos)
	{
		string variableName = command.substr(0, separatorPos);
		string valueString = command.substr(separatorPos + 1, string::npos);
	}
}