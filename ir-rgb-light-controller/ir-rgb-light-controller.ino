#include <IRremote.h>

int RECV_PIN = 11;
int RED_PIN = 5;
int GREEN_PIN = 6;
int BLUE_PIN = 3;
int FADE_SPEED = 5;

unsigned long PORTS[6] = {
  0x0000000, // off
  0x1FEA05F, // computer
  0x1FEE01F, // xbox 360
  0x1FE10EF, // wii
  0x1FE906F, // xbox one
  0x1FED827
};

char* COLORS[6] = {
  "000000",
  "ff9900",
  "00ff00",
  "cc00cc",
  "0000ff",
  "ff0000"

};

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  Serial.println("Hello world!");

  // used to test the lights
  for (int i = 0; i < 6; i++) {
    setLights(COLORS[i]);
    delay(500);
  }
  setLights("000000");
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

  String r_str = color_string.substring(0, 2);
  String g_str = color_string.substring(2, 4);
  String b_str = color_string.substring(4, 6);

  char r_hex[3];
  char b_hex[3];
  char g_hex[3];

  r_str.toCharArray(r_hex, 3);
  g_str.toCharArray(g_hex, 3);
  b_str.toCharArray(b_hex, 3);

  int r_dec = strtol(r_hex, NULL, 16);
  int g_dec = strtol(g_hex, NULL, 16);
  int b_dec = strtol(b_hex, NULL, 16);

  p("Setting color code to: %s", color_code);
  p("r: %d", r_dec);
  p("g: %d", g_dec);
  p("b: %d", b_dec);
}

