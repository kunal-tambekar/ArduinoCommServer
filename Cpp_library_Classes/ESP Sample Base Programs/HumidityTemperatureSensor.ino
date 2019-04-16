#include <dht.h>

dht DHT;

int dhtPin;
void setup()
{
  Serial.begin(115200);
  dhtPin = 2;
}

void loop()
{
  // READ DATA
  int chk = DHT.read11(dhtPin);
  switch (chk)
  {
    case DHTLIB_OK:  
		  Serial.print("OK,\t"); 
		  break;
    case DHTLIB_ERROR_CHECKSUM: 
		  Serial.print("Checksum error,\t"); 
		  break;
    case DHTLIB_ERROR_TIMEOUT: 
		  Serial.print("Time out error,\t"); 
		  break;
    default: 
		  Serial.print("Unknown error,\t"); 
		  break;
  }
  // DISPLAY DATA
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.print("TEMPERATURE(C): ");
  Serial.println(DHT.temperature, 1);
}
