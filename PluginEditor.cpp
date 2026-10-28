#include "PluginEditor.h"

ElKrazieKeyTempoFinderAudioProcessorEditor::ElKrazieKeyTempoFinderAudioProcessorEditor(ElKrazieKeyTempoFinderAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(520, 300);
    title.setText("EL KRAZIE  |  KEY & TEMPO FINDER", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setFont(juce::Font(juce::FontOptions(23.0f, juce::Font::bold)));
    addAndMakeVisible(title);
    keyLabel.setText("KEY: Listening...", juce::dontSendNotification);
    keyLabel.setJustificationType(juce::Justification::centred);
    keyLabel.setFont(juce::Font(juce::FontOptions(30.0f, juce::Font::bold)));
    addAndMakeVisible(keyLabel);
    tempoLabel.setText("TEMPO: -- BPM", juce::dontSendNotification);
    tempoLabel.setJustificationType(juce::Justification::centred);
    tempoLabel.setFont(juce::Font(juce::FontOptions(28.0f)));
    addAndMakeVisible(tempoLabel);
    statusLabel.setText("Play audio through this plugin to analyse", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(statusLabel);
    startTimerHz(4);
}

void ElKrazieKeyTempoFinderAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(18, 18, 24));
    g.setColour(juce::Colour(160, 85, 255));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10.0f), 18.0f);
    g.setColour(juce::Colour(18, 18, 24));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(13.0f), 15.0f);
    g.setColour(juce::Colour(190, 145, 255));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(13.0f), 15.0f, 2.0f);
}

void ElKrazieKeyTempoFinderAudioProcessorEditor::resized()
{
    title.setBounds(25, 28, getWidth() - 50, 40);
    keyLabel.setBounds(25, 92, getWidth() - 50, 48);
    tempoLabel.setBounds(25, 148, getWidth() - 50, 45);
    statusLabel.setBounds(25, 220, getWidth() - 50, 30);
}

void ElKrazieKeyTempoFinderAudioProcessorEditor::timerCallback()
{
    keyLabel.setText("KEY: " + processor.getDetectedKey(), juce::dontSendNotification);
    const double bpm = processor.getDetectedTempo();
    tempoLabel.setText(bpm > 0.0 ? "TEMPO: " + juce::String(bpm, 1) + " BPM" : "TEMPO: -- BPM",
                       juce::dontSendNotification);
    statusLabel.setText(processor.getStatus(), juce::dontSendNotification);
}
