#include "Sound.h"

float PRNG::next()
{
    s = 1664525u * s + 1013904223u; 
    return ((s >> 9) * (1.0f / 8388608.0f)) * 2.f - 1.f;
}

sf::SoundBuffer makePewSound(
    unsigned sampleRate,
    float durSec,      // short and light
    float f0,          // start freq (brighter)
    float f1,          // end freq (falls quickly)
    float sweepShape,   // exponential sweep curvature (higher = quicker drop)
    float harm2,       // 2nd harmonic amount (sparkle)
    float attack,     // 3 ms soft attack
    float decay,       // exponential decay (seconds)
    float tail,        // low tail multiplier
    float noiseAmt,    // very subtle noise
    float echoDelay,  // 6 ms slap
    float echoGain    // very light echo
) 
{
    const size_t N = static_cast<size_t>(durSec * sampleRate);
    std::vector<sf::Int16> out(N, 0);

    // Precompute echo buffer size
    size_t echoSamp = std::max<size_t>(1, static_cast<size_t>(echoDelay * sampleRate));
    std::vector<float> dryBuf(N, 0.f);

    PRNG rng;
    double phase = 0.0;
    double phase2 = 0.0;
    const float invSR = 1.f / sampleRate;

    for (size_t i = 0; i < N; ++i) {
        float t = i * invSR;

        // Exponential downward chirp: freq(t) = f1 + (f0 - f1)*exp(-k t)
        float k = sweepShape / durSec;
        float freq = f1 + (f0 - f1) * std::exp(-k * t);
        if (freq < 25.f) freq = 25.f;

        // Advance phases (fundamental and 2nd harmonic)
        phase += (2.0 * PI * freq) * invSR;
        phase2 += (2.0 * PI * (2.0 * freq)) * invSR;

        // Tone (mostly sine; tiny 2nd harmonic)
        float tone = std::sin((float)phase) + harm2 * std::sin((float)phase2);

        // Micro-noise (barely there, to avoid sterility)
        float noise = rng.next() * 0.4f;

        // ADSR-like envelope: quick attack, exponential decay, tiny tail
        float a = std::clamp(t / attack, 0.f, 1.f);
        float d = std::exp(-t / decay);
        float env = (a * d) * (tail + (1.f - tail)); // tail keeps a soft end instead of abrupt stop

        // Very gentle high-end rolloff over time (simple multiplicative tilt)
        float tilt = 1.f - 0.25f * (t / durSec); // fades a bit darker as it decays

        float s = tilt * env * (0.97f * tone + noiseAmt * noise);

        dryBuf[i] = s;
    }

    // Tiny echo (single-tap), very low gain, no feedback
    for (size_t i = 0; i < N; ++i) {
        float y = dryBuf[i];
        if (i >= echoSamp) y += echoGain * dryBuf[i - echoSamp];
        // Soft limiter (no obvious distortion)
        float lim = y * 30000.f;
        if (lim > 32767.f) lim = 32767.f;
        if (lim < -32768.f) lim = -32768.f;
        out[i] = static_cast<sf::Int16>(lim);
    }

    sf::SoundBuffer buf;
    buf.loadFromSamples(out.data(), out.size(), 1, sampleRate);
    return buf;
}

sf::SoundBuffer makeReloadSound(
    unsigned sampleRate,
    float durSec,      // short and light
    float f0,          // start freq (brighter)
    float f1,          // end freq (falls quickly)
    float sweepShape,   // exponential sweep curvature (higher = quicker drop)
    float harm2,       // 2nd harmonic amount (sparkle)
    float attack,     // 3 ms soft attack
    float decay,       // exponential decay (seconds)
    float tail,        // low tail multiplier
    float noiseAmt,    // very subtle noise
    float echoDelay,  // 6 ms slap
    float echoGain    // very light echo
)
{
    const size_t N = static_cast<size_t>(durSec * sampleRate);
    std::vector<sf::Int16> out(N, 0);

    // Precompute echo buffer size
    size_t echoSamp = std::max<size_t>(1, static_cast<size_t>(echoDelay * sampleRate));
    std::vector<float> dryBuf(N, 0.f);

    PRNG rng;
    double phase = 0.0;
    double phase2 = 0.0;
    const float invSR = 1.f / sampleRate;

    for (size_t i = 0; i < N; ++i) {
        float t = i * invSR;

        // Exponential downward chirp: freq(t) = f1 + (f0 - f1)*exp(-k t)
        float k = sweepShape / durSec;
        float freq = f1 + (f0 - f1) * std::exp(-k * t);
        if (freq < 25.f) freq = 25.f;

        // Advance phases (fundamental and 2nd harmonic)
        phase += (2.0 * PI * freq) * invSR;
        phase2 += (2.0 * PI * (2.0 * freq)) * invSR;

        // Tone (mostly sine; tiny 2nd harmonic)
        float tone = std::sin((float)phase) + harm2 * std::sin((float)phase2);

        // Micro-noise (barely there, to avoid sterility)
        float noise = rng.next() * 0.4f;

        // ADSR-like envelope: quick attack, exponential decay, tiny tail
        float a = std::clamp(t / attack, 0.f, 1.f);
        float d = std::exp(-t / decay);
        float env = (a * d) * (tail + (1.f - tail)); // tail keeps a soft end instead of abrupt stop

        // Very gentle high-end rolloff over time (simple multiplicative tilt)
        float tilt = 1.f - 0.25f * (t / durSec); // fades a bit darker as it decays

        float s = tilt * env * (0.97f * tone + noiseAmt * noise);

        dryBuf[i] = s;
    }

    // Tiny echo (single-tap), very low gain, no feedback
    for (size_t i = 0; i < N; ++i) {
        float y = dryBuf[i];
        if (i >= echoSamp) y += echoGain * dryBuf[i - echoSamp];
        // Soft limiter (no obvious distortion)
        float lim = y * 30000.f;
        if (lim > 32767.f) lim = 32767.f;
        if (lim < -32768.f) lim = -32768.f;
        out[i] = static_cast<sf::Int16>(lim);
    }

    sf::SoundBuffer buf;
    buf.loadFromSamples(out.data(), out.size(), 1, sampleRate);
    return buf;
}