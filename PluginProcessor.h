#pragma once
#include <JuceHeader.h>
#include <array>
#include <atomic>
#include <vector>

class ElKrazieKeyTempoFinderAudioProcessor : public juce::AudioProcessor
{
public:
    ElKrazieKeyTempoFinderAudioProcessor();
    ~ElKrazieKeyTempoFinderAudioProcessor() override = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
    juce::String getDetectedKey() const;
    double getDetectedTempo() const noexcept { return tempo.load(); }
    juce::String getStatus() const;
private:
    void analyseBlock(const juce::AudioBuffer<float>&, int);
    std::atomic<double> tempo { 0.0 };
    std::atomic<int> keyIndex { -1 };
    std::atomic<int> statusCode { 0 };
    double currentSampleRate = 44100.0;
    int analysisCounter = 0;
    float previousEnergy = 0.0f;
    std::vector<float> energyHistory;
    std::array<double, 12> chroma {};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ElKrazieKeyTempoFinderAudioProcessor)
};
