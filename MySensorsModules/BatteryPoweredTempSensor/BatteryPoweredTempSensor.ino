
// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached 
#define MY_RADIO_RF24
//#define MY_RADIO_RFM69
//#define MY_RS485

#include <SPI.h>
#include <MySensors.h>  
#include <DHT.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 3

// Set this offset if the sensor has a permanent small offset to the real temperatures.
// In Celsius degrees (as measured by the device)
#define SENSOR_TEMP_OFFSET 2.2

// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 60000;

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1


float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;
bool metric = true;
int oldBatteryPcnt = 0;
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
DHT dht;


void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("TemperatureAndHumidity", "1.2");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);

  metric = getControllerConfig().isMetric;
}


void setup()
{
      // use the 1.1 V internal reference

  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht.getMinimumSamplingPeriod());
}


void loop()      
{  

  // Force reading sensor, so it works also after sleep()
  dht.readSensor(true);
  // Get temperature from DHT library
  float temperature = dht.getTemperature()-3;
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT!");
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp = temperature;

    // apply the offset before converting to something different than Celsius degrees
    temperature += SENSOR_TEMP_OFFSET;

    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    // Reset no updates counter
    nNoUpdatesTemp = 0;
    send(msgTemp.set(temperature, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp++;
  }

  // Get humidity from DHT library
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
    // Reset no updates counter
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum++;
  }

  // Sleep for a while to save energy
  sleep(UPDATE_INTERVAL); 
}
