#include <IRremote.h>

int RECV_PIN = 11;
int RED_PIN = 5;
int RED_PIN_VALUE = 0;
int GREEN_PIN = 6;
int GREEN_PIN_VALUE = 0;
int BLUE_PIN = 9;
int BLUE_PIN_VALUE = 0;
int FADE_SPEED = 3;

unsigned long PORTS[6] = {
  0x5EA17887, // off
  0x1FEA05F, // computer
  0x1FEE01F, // xbox 360
  0x1FE10EF, // wii
  0x1FE906F, // xbox one
  0x1FED827  // chromecast
};

char* COLORS[6] = {
  "000000", // black
  "00ff00", // green
  "0000ff", // blue
  "95FFFF", // light blue
  "0000ff", // blue
  "ff0000"  // red

};

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.println("Hello world!");

  Serial.begin(9600); // Start serial
  irrecv.enableIRIn(); // Start the receiver

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // used to test the lights
  lightTest();
}

void loop() {
  if (irrecv.decode(&results)) {
    for (int i = 0; i < 6; i++)
    {
      if (results.value == PORTS[i])
      {
        setLights(COLORS[i]);
        break;
      }
    }
    irrecv.resume(); // Receive the next value
  }
}

void p(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

void setLights(char* color_code) {
  String color_string = color_code;

  // start convoluted way to get parts of the color hex
  String r_str = color_string.substring(0, 2);
  String g_str = color_string.substring(2, 4);
  String b_str = color_string.substring(4, 6);

  char r_hex[3];
  char b_hex[3];
  char g_hex[3];

  r_str.toCharArray(r_hex, 3);
  g_str.toCharArray(g_hex, 3);
  b_str.toCharArray(b_hex, 3);
  // end convoluted way to get parts of the color hex

  // conver the hex parts to decimal
  int r_dec = strtol(r_hex, NULL, 16);
  int g_dec = strtol(g_hex, NULL, 16);
  int b_dec = strtol(b_hex, NULL, 16);

  p("Setting color code to: %s", color_code);
  p("r: %d", r_dec);
  p("g: %d", g_dec);
  p("b: %d", b_dec);

  // we want to fade the color change. So this will loop and change every color by 1 (if necessary) until they line up.
  while (RED_PIN_VALUE != r_dec || GREEN_PIN_VALUE != g_dec || BLUE_PIN_VALUE != b_dec) {

  	// we only want to change the red pin if it's not already correct
    if (r_dec != RED_PIN_VALUE) {
      if (r_dec < RED_PIN_VALUE ) {
        setRed(RED_PIN_VALUE - 1);
      } else {
        setRed(RED_PIN_VALUE + 1);
      }
    }

	// we only want to change the green pin if it's not already correct
    if (g_dec != GREEN_PIN_VALUE) {
      if (g_dec < GREEN_PIN_VALUE ) {
        setGreen(GREEN_PIN_VALUE - 1);
      } else {
        setGreen(GREEN_PIN_VALUE + 1);
      }
    }

    // we only want to change the blue pin if it's not already correct
	if (b_dec != BLUE_PIN_VALUE) {
      if (b_dec < BLUE_PIN_VALUE ) {
        setBlue(BLUE_PIN_VALUE - 1);
      } else {
        setBlue(BLUE_PIN_VALUE + 1);
      }
    }

    delay(FADE_SPEED);

  }

}

void setRed(int val) {
  RED_PIN_VALUE = val;
  analogWrite(RED_PIN, val);
}

void setGreen(int val) {
  GREEN_PIN_VALUE = val;
  analogWrite(GREEN_PIN, val);
}

void setBlue (int val) {
  BLUE_PIN_VALUE = val;
  analogWrite(BLUE_PIN, val);
}

void lightTest() {
  Serial.println("Starting light test sequence");
  int i;

  // fade through the rgb scale
  for (i = 0; i < 256; i++) {
    setRed(i);
    delay(5);
  }
  for (i = 255; i >= 0; i--) {
    setRed(i);
    delay(5);
  }

  for (i = 0; i < 256; i++) {
    setGreen(i);
    delay(5);
  }
  for (i = 255; i >= 0; i--) {
    setGreen(i);
    delay(5);
  }

  for (i = 0; i < 256; i++) {
    setBlue(i);
    delay(5);
  }
  for (i = 255; i >= 0; i--) {
    setBlue(i);
    delay(5);
  }

  // cycle through each of the programmed colors
  for (int i = 0; i < 6; i++) {
    setLights(COLORS[i]);
  }

  // default to the lights being off
  setLights("000000");

  Serial.println("Light test sequence has completed, how did it look?");
}

