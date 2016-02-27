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
  int r = strtol(substr(color_code, 0, 2), NULL, 16);
  int g = strtol(substr(color_code, 2, 2), NULL, 16);
  int b = strtol(substr(color_code, 4, 2), NULL, 16);

  p("Setting color code to: %s", color_code);
  p("r: %d", r);
  p("g: %d", g);
  p("b: %d", b);
}

char* substr(char* str, int strt, int len) {
  char* response = "00";
  int pos = strt;

  for (int i = 0; i < len; i++ )
  {
    if (pos > strlen(str)) {
      break;
    }

    response[i] = str[pos];
    pos++;
  }

  return response;
}
