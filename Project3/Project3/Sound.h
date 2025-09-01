#pragma once
#include <SFML/Audio.hpp>

const double PI = 3.14159265358979323846;

struct PRNG {
    uint32_t s = 0x12345678u;
    float next();
};

// Soft, light “pew” energy gun
sf::SoundBuffer makePewSound(
    unsigned sampleRate = 30000,
    float durSec = 0.22f,      // short and light
    float f0 = 900.f,          // start freq (brighter)
    float f1 = 50.f,          // end freq (falls quickly)
    float sweepShape = 7.5f,   // exponential sweep curvature (higher = quicker drop)
    float harm2 = 0.18f,       // 2nd harmonic amount (sparkle)
    float attack = 0.003f,     // 3 ms soft attack
    float decay = 0.18f,       // exponential decay (seconds)
    float tail = 0.05f,        // low tail multiplier
    float noiseAmt = 0.05f,    // very subtle noise
    float echoDelay = 0.006f,  // 6 ms slap
    float echoGain = 0.18f    // very light echo
);
sf::SoundBuffer makeReloadSound(
    unsigned sampleRate = 30000,
    float durSec = 0.6f,      // short and light
    float f0 = 500.f,          // start freq (brighter)
    float f1 = 60.f,          // end freq (falls quickly)
    float sweepShape = 7.5f,   // exponential sweep curvature (higher = quicker drop)
    float harm2 = 0.18f,       // 2nd harmonic amount (sparkle)
    float attack = 0.003f,     // 3 ms soft attack
    float decay = 0.18f,       // exponential decay (seconds)
    float tail = 0.05f,        // low tail multiplier
    float noiseAmt = 0.05f,    // very subtle noise
    float echoDelay = 0.006f,  // 6 ms slap
    float echoGain = 0.18f    // very light echo
);