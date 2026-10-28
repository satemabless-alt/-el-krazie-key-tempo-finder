# El Krazie Key & Tempo Finder

A starter JUCE audio-analysis plugin project for building on GitHub Actions. It passes audio through unchanged and shows rough key/tempo estimates. These estimates are experimental and should not be treated as a professional-grade music-theory analysis.

## Build on GitHub
1. Upload the contents of this folder into the root of your GitHub repository (not the ZIP file itself).
2. Commit the files to the `main` branch.
3. Open the **Actions** tab.
4. Select **Build El Krazie Key & Tempo Finder**.
5. Click **Run workflow** if it is not already running.
6. When complete, open the successful run and download the `El-Krazie-Key-Tempo-Finder-Windows` artifact.

## Notes
- Requires internet access during build so CMake can fetch JUCE.
- The output is a VST3 plugin and a standalone build.
- The first analysis implementation is a starter, not a guaranteed accurate key/tempo detector.
