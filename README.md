# Procedural Terrain Generation

### Controls
 - WASD  - Movement
 - Mouse - Move camera
 - F1 - Toggle settings panel
 - F2 - Toggle wireframe mode
 - F3 - Reload Terrain (depending on the terrain resolution this could cause the program to hang for a few seconds)

 - ESC - Close the program

### Presets
Select some presets with the radio buttons (don't forget to reload the terrain after selecting a preset).
If you want to be able to customize terrain select "No Preset".

### Settings
 - Movement Speed - Changes camera movement speed
 - Color Picker - Change the terrain color
 - Scale - Add variation to noise map
 - Octaves - Number of noise layers
 - Persistance - A multiplier that determines how quickly the amplitudes change for each octave
 - Lacunarity - A multiplier that determines how much the noise function should increase in frequency for each octave (i.e. higher frequency, more detail)
 - Height - Height multiplier for each vertex after noise map is applied
 - Floor - Change the level of the terrain floor. If set to 0 the floor is disabled.
 - Resolution - Specifies the number of vertices^2 to be rendered (e.g. a resolution of 64 would result in 64 x 64 vertices to be rendered)
 - Noise Type - Changes what noise algorithm to use for the map
 - Seed - Randomize the seed by clicking the button to get variations of the terrain using current settings.
 - Light Direction - Adjust light direction (x, y, z)