#include "PluginProcessor.h"
#include "PluginEditor.h"

ElKrazieKeyTempoFinderAudioProcessor::ElKrazieKeyTempoFinderAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    energyHistory.reserve(512);
}

void ElKrazieKeyTempoFinderAudioProcessor::prepareToPlay(double sr, int)
{
    currentSampleRate = sr;
    analysisCounter = 0;
    previousEnergy = 0.0f;
    energyHistory.clear();
    chroma.fill(0.0);
    tempo.store(0.0);
    keyIndex.store(-1);
    statusCode.store(0);
}

bool ElKrazieKeyTempoFinderAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return in == out && (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo());
}

void ElKrazieKeyTempoFinderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    analyseBlock(buffer, n);
    // Analysis-only plugin: pass audio through unchanged.
}

void ElKrazieKeyTempoFinderAudioProcessor::analyseBlock(const juce::AudioBuffer<float>& buffer, int n)
{
    if (n <= 0) return;
    double sum = 0.0;
    for (int c = 0; c < buffer.getNumChannels(); ++c) {
        const auto* p = buffer.getReadPointer(c);
        for (int i = 0; i < n; ++i) sum += double(p[i]) * double(p[i]);
    }
    const float energy = float(sum / double(n * juce::jmax(1, buffer.getNumChannels())));
    const float flux = juce::jmax(0.0f, energy - previousEnergy);
    previousEnergy = energy;

    // Store short-window energy changes for a rough beat-period estimate.
    energyHistory.push_back(flux);
    if (energyHistory.size() > 512) energyHistory.erase(energyHistory.begin());
    ++analysisCounter;

    // Lightweight pitch-class estimate using zero-crossing-derived coarse spectral bins
    // is intentionally conservative; this is a starter analysis tool, not a mastering-grade tuner.
    if ((analysisCounter % 8) == 0 && energy > 1.0e-7f) {
        double crossings = 0.0;
        for (int c = 0; c < buffer.getNumChannels(); ++c) {
            const auto* p = buffer.getReadPointer(c);
            for (int i = 1; i < n; ++i)
                if ((p[i - 1] <= 0.0f && p[i] > 0.0f) || (p[i - 1] >= 0.0f && p[i] < 0.0f))
                    crossings += 1.0;
        }
        const double rate = crossings / double(n * juce::jmax(1, buffer.getNumChannels())) * currentSampleRate * 0.5;
        if (rate > 30.0 && rate < 5000.0) {
            const int midi = juce::jlimit(0, 127, int(std::round(69.0 + 12.0 * std::log2(rate / 440.0))));
            chroma[size_t(midi % 12)] += energy;
            double best = 0.0;
            int idx = -1;
            for (int k = 0; k < 12; ++k) if (chroma[size_t(k)] > best) { best = chroma[size_t(k)]; idx = k; }
            keyIndex.store(idx);
        }
    }

    if (energyHistory.size() >= 128 && (analysisCounter % 32) == 0) {
        const int minLag = juce::jmax(1, int(currentSampleRate / double(n) * 60.0 / 180.0));
        const int maxLag = juce::jmin(int(energyHistory.size() / 2), int(currentSampleRate / double(n) * 60.0 / 60.0));
        double bestScore = 0.0;
        int bestLag = 0;
        for (int lag = minLag; lag <= maxLag; ++lag) {
            double score = 0.0;
            for (int i = lag; i < int(energyHistory.size()); ++i)
                score += double(energyHistory[size_t(i)]) * double(energyHistory[size_t(i - lag)]);
            if (score > bestScore) { bestScore = score; bestLag = lag; }
        }
        if (bestLag > 0) {
            const double secondsPerWindow = double(n) / currentSampleRate;
            const double bpm = 60.0 / (double(bestLag) * secondsPerWindow);
            if (bpm >= 60.0 && bpm <= 180.0) { tempo.store(bpm); statusCode.store(1); }
        }
    }
}

juce::String ElKrazieKeyTempoFinderAudioProcessor::getDetectedKey() const
{
    static const char* names[] = { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    const int k = keyIndex.load();
    return k >= 0 && k < 12 ? juce::String(names[k]) : juce::String("Listening...");
}

juce::String ElKrazieKeyTempoFinderAudioProcessor::getStatus() const
{
    return statusCode.load() == 1 ? "Analysis running" : "Play audio to analyse";
}

juce::AudioProcessorEditor* ElKrazieKeyTempoFinderAudioProcessor::createEditor()
{
    return new ElKrazieKeyTempoFinderAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ElKrazieKeyTempoFinderAudioProcessor();
}
