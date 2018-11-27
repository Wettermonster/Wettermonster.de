/*Code-WeatherMeters-Demo
 * 
 * Dieser Code soll dabei helfen, die Weather Meters der Firma Sparkfun perfekt in seine Projekte zu implementieren.
 * Als Board wird ein NodeMCU mit dem populären ESP8266 verwendet.
 * Die Verkabelung sieht dabei wie folgt aus:
 * 
 * 1. Kabel Niederschlagssensor => GND
 * 2. Kabel Niederschlagssensor => D4
 * 
 * 1. Kabel Windmesser => GND
 * 2. Kabel Windmesser => D5
 * 
 * 1. Kabel Windrichtungssensor => 3,3V
 * 2. Kabel Windrichtungssensor => A0 + parallel über einen 10k-Widerstand an GND 
 * 
 * 
 * Dieses Material steht unter der Creative-Commons-Lizenz Namensnennung-Nicht kommerziell 4.0 International.
 * Um eine Kopie dieser Lizenz zu sehen, besuchen Sie http://creativecommons.org/licenses/by-nc/4.0/.
*/
const int interval = 5;

float Percipitation;
float numClicksRain;
float windSpeed;
float numRevsAnemometer;
char* windDirection;
volatile unsigned long previousTimeRain=0, previousTimeSpeed=0, delayTime=20;

void countAnemometer() {
	if((millis() - previousTimeSpeed) > delayTime) {
		numRevsAnemometer++;
 		previousTimeSpeed = millis();
	}
}

void countRain() {
	if((millis() - previousTimeRain) > delayTime) {
		numClicksRain++;
		previousTimeRain = millis();
	}
}

void readWeatherMeters() {

	windSpeed = (numRevsAnemometer / (interval * 60)) * 2.4;
	numRevsAnemometer = 0;

	Percipitation = 0.2794 * (numClicksRain * 60 / interval);
	numClicksRain = 0;

	int windDirectionVoltage = analogRead(A0);

	if (windDirectionVoltage >= 212 && windDirectionVoltage < 273)    {windDirection = "N";}
	else if (windDirectionVoltage >= 577 && windDirectionVoltage < 665) {windDirection = "NNE";}
	else if (windDirectionVoltage >= 483 && windDirectionVoltage < 577) {windDirection = "NE";}
	else if (windDirectionVoltage >= 929 && windDirectionVoltage < 943) {windDirection = "ENE";}
	else if (windDirectionVoltage >= 906 && windDirectionVoltage < 929) {windDirection = "E";}
	else if (windDirectionVoltage >= 943 && windDirectionVoltage < 1023){windDirection = "ESE";}
	else if (windDirectionVoltage >= 788 && windDirectionVoltage < 858) {windDirection = "SE";}
	else if (windDirectionVoltage >= 858 && windDirectionVoltage < 906) {windDirection = "SSE";}
	else if (windDirectionVoltage >= 665 && windDirectionVoltage < 748) {windDirection = "S";}
	else if (windDirectionVoltage >= 748 && windDirectionVoltage < 799) {windDirection = "SSW";}
	else if (windDirectionVoltage >= 348 && windDirectionVoltage < 399) {windDirection = "SW";}
	else if (windDirectionVoltage >= 399 && windDirectionVoltage < 483) {windDirection = "WSW";}
	else if (windDirectionVoltage >= 0 && windDirectionVoltage < 106)   {windDirection = "W";}
	else if (windDirectionVoltage >= 163 && windDirectionVoltage < 212) {windDirection = "WNW";}
	else if (windDirectionVoltage >= 106 && windDirectionVoltage < 163) {windDirection = "NW";}
	else if (windDirectionVoltage >= 273 && windDirectionVoltage < 348) {windDirection = "NNW";}
}

void setup() {
	Serial.begin(115200);

	pinMode(2, INPUT_PULLUP);
	attachInterrupt(2, countRain, FALLING);
	pinMode(14, INPUT_PULLUP);
	attachInterrupt(14, countAnemometer, FALLING);

}

void loop() {

	delay(interval * 60000);

	readWeatherMeters();

	Serial.print("Windgeschwindigkeit: ");
  Serial.print(windSpeed);
  Serial.println("km/h");
  Serial.print("Niederschlag: ");
  Serial.print(Percipitation);
  Serial.println("mm/h");
  Serial.print("Windrichtung: ");
  Serial.println(windDirection);
  Serial.println("...............................................");

}
