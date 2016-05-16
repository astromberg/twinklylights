#include <Adafruit_NeoPixel.h>
#include <Console.h>
 
#define PIN      6
#define N_LEDS 100
#define N_FLIES 5
#define FLY_WIDTH 3
#define SKIP_FLY -1
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_RGB + NEO_KHZ800);

int positions[N_FLIES];
int togo[N_FLIES];
// 0 -> 100% of the color in question
int intensities[N_FLIES];

int COLOR_R = 120;
int COLOR_G = 60;
int COLOR_B = 0;

bool inRunOutPhase = true;
bool inWanderingPhase = false;
bool inTwinklyPhase = false;

void setup() {
//  Bridge.begin();
//  Console.begin(); 
//
//  while (!Console); 
//
//  Console.println("Hi, what's your name?");

  randomSeed(analogRead(A0));

  strip.begin();

  startRunOutPhase();
}
 
void loop() {
  if (inRunOutPhase) {
    runout();
  } else if (inWanderingPhase) {
    wander();
//  } else if (inTwinklyPhase) {
//    twinkle();
  }
}

void startRunOutPhase() {
  // Randomly position the flies initially

  for (uint16_t i = 0; i < N_FLIES; i++) {
    positions[i] = random(0, N_LEDS);
  }
}

// The startup, either when the flies are shot out to their position or slowly glow into existence.
void runout() {

  for (uint16_t i=0; i < N_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  
  for (uint16_t i=0; i < N_FLIES; i++) {
    strip.setPixelColor(positions[i],
        scaleByIntensity(COLOR_R, intensities[i]),
        scaleByIntensity(COLOR_G, intensities[i]),
        scaleByIntensity(COLOR_B, intensities[i]));
  }

  for (uint16_t i=0; i < N_FLIES; i++) {
    intensities[i] = intensities[i] + random(0, 2);
  }

  if (intensities[0] > 100) {
    inRunOutPhase = false;
    inWanderingPhase = true;
  }

  strip.show();
  delay(10);
}

static int scaleByIntensity(int c, int f) {
  return (int) ((float) c * (float) f / 100);
}

void wander() {
  // Decay the color left by the flies
  for (int i=0; i < N_LEDS; i++) {
    uint32_t c = strip.getPixelColor(i);
    uint8_t r = (uint8_t) (c >> 16);
    uint8_t g = (uint8_t) (c >>  8);
    uint8_t b = (uint8_t) c;
    int new_r = scaleByIntensity(r, 70);
    int new_g = scaleByIntensity(g, 70);
    int new_b = scaleByIntensity(b, 70);
    strip.setPixelColor(i, new_r, new_g, new_b);
  }

  // Calculate new positions for the flies
  for (uint16_t i=0; i < N_FLIES; i++) {
    if (positions[i] == SKIP_FLY) {
      continue;
    }
    // The run for this current fly has ended, time to decide if it should take off again
//    if (togo[i] == 0) {
//      // A one in three chance it will go on a "run"
//      if (rand(0, 3) == 1) {
//        
//      } else {
//        // Just hang out 
//      }
//    }
    int change = random(-1, 2);
    if (positions[i] + change < 0) {
      positions[i] = N_LEDS + positions[i] + change - 1;
    } else {
      positions[i] = (positions[i] + change) % N_LEDS;
    }
  }

  // Set the colors and brightness for the flies
  for (uint16_t i=0; i < N_FLIES; i++) {
    strip.setPixelColor(positions[i],
        scaleByIntensity(COLOR_R, intensities[i]),
        scaleByIntensity(COLOR_G, intensities[i]),
        scaleByIntensity(COLOR_B, intensities[i]));
  }

  // Render!
  strip.show();

  // Lay low until the next episode.
  delay(60);
}

void twinkle() {
  
}
