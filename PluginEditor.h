#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ElKrazieKeyTempoFinderAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                   private juce::Timer
{
public:
    explicit ElKrazieKeyTempoFinderAudioProcessorEditor(ElKrazieKeyTempoFinderAudioProcessor&);
    ~ElKrazieKeyTempoFinderAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void timerCallback() override;
    ElKrazieKeyTempoFinderAudioProcessor& processor;
    juce::Label title, keyLabel, tempoLabel, statusLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ElKrazieKeyTempoFinderAudioProcessorEditor)
};
