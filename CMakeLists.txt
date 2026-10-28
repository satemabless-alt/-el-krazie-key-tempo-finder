cmake_minimum_required(VERSION 3.22)
project(ElKrazieKeyTempoFinder VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.6
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(ElKrazieKeyTempoFinder
  COMPANY_NAME "El Krazie"
  BUNDLE_ID "com.elkrazie.keytempofinder"
  PLUGIN_MANUFACTURER_CODE ElKr
  PLUGIN_CODE KtFn
  FORMATS VST3 Standalone
  PRODUCT_NAME "El Krazie Key & Tempo Finder"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(ElKrazieKeyTempoFinder)

target_sources(ElKrazieKeyTempoFinder PRIVATE
  Source/PluginProcessor.cpp
  Source/PluginEditor.cpp)

target_compile_features(ElKrazieKeyTempoFinder PRIVATE cxx_std_17)
target_compile_definitions(ElKrazieKeyTempoFinder PUBLIC
  JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
target_link_libraries(ElKrazieKeyTempoFinder PRIVATE
  juce::juce_audio_utils
  juce::juce_dsp
  PUBLIC juce::juce_recommended_config_flags
         juce::juce_recommended_lto_flags
         juce::juce_recommended_warning_flags)
