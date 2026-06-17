# PLACE

**by leve**

Create space for vocals instantly.

## Overview

PLACE is a stereo VST3 effect plugin designed to create room for vocals inside an instrumental mix. It works on stereo buses using Mid/Side processing to carve space in the center channel where vocals sit, while maintaining stereo width and energy.

## Installation

### Using the Installer

1. Download `PLACE-v1.0-Setup.exe`
2. Run the installer
3. The plugin will be installed to `C:\Program Files\Common Files\VST3\PLACE.vst3`
4. Restart your DAW or rescan plugins

### Manual Installation

Copy `PLACE.vst3` to:
```
C:\Program Files\Common Files\VST3\
```

## Controls

### SIZE MAXIMIZER (0-100%)

The primary control. Determines how much room is created for vocals in the mid channel.

- **0%**: No processing
- **50%**: Moderate vocal space (default)
- **100%**: Maximum vocal space

### SIDE BASS REMOVER (0-100%)

Removes low frequencies from the mid channel using a 24 dB/oct Linkwitz-Riley high-pass filter.

- **0%**: 20 Hz cutoff (essentially off)
- **100%**: 180 Hz cutoff

### MODE

- **Static**: Fixed EQ attenuation on mid channel
- **Vocal Follow**: Dynamic EQ that responds to sidechain input energy

### BYPASS

Engages/disengages processing. Fully automatable.

## Sidechain Setup

### Vocal Follow Mode

1. Route your vocal track to the sidechain input of PLACE
2. Set MODE to "Vocal Follow"
3. The SC indicator will show "SC ON" when sidechain is detected
4. PLACE will dynamically attenuate mid frequencies where the vocal has energy

### Supported DAWs

Sidechain routing varies by DAW:

- **Ableton Live**: Use the Sidechain dropdown in the plugin header
- **FL Studio**: Route vocal to a mixer track, then use the sidechain option in the PLACE slot
- **Studio One**: Drag a send from the vocal track to the PLACE sidechain input
- **Cubase**: Activate the sidechain in the plugin's VST3 panel
- **Reaper**: Use the Pin Connector to route vocal to PLACE's sidechain pins
- **Bitwig**: Use the sidechain input panel in the device
- **Logic Pro**: Use the Side Chain dropdown in the plugin header

## Automation

All parameters are fully automatable:

| Parameter | Range | Default |
|-----------|-------|---------|
| Size Maximizer | 0-100% | 50% |
| Side Bass Remover | 0-100% | 0% |
| Mode | Static / Vocal Follow | Static |
| Bypass | On / Off | Off |

## Troubleshooting

### Plugin not showing in DAW

- Rescan plugins in your DAW's plugin manager
- Verify `PLACE.vst3` exists in `C:\Program Files\Common Files\VST3\`
- Restart your DAW

### Sidechain not working

- Ensure your DAW supports VST3 sidechain
- Check that the SC indicator shows "SC ON"
- Verify the sidechain bus is enabled in your DAW's routing

### Audio artifacts

- Reduce SIZE MAXIMIZER if you hear excessive thinning
- Use SIDE BASS REMOVER to clean up low-mid buildup
- Switch to Static mode if dynamic mode causes pumping

## Build Instructions

### Prerequisites

- Windows 11
- Visual Studio 2022 with C++ desktop workload
- CMake 3.24+
- Git

### Building

```bash
git clone <repository-url>
cd place
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The VST3 plugin will be in `build/PLACE_artefacts/Release/VST3/`.

### Building the Installer

1. Build the Release configuration
2. Install [Inno Setup](https://jrsoftware.org/isinfo.php)
3. Open `installer/setup.iss` in Inno Setup Compiler
4. Click Build > Compile

## Technical Details

- **Format**: VST3
- **Channels**: Stereo in, Stereo out, Stereo sidechain
- **Latency**: Zero
- **Processing**: Mid/Side domain
- **Sample Rate**: Any (44.1 kHz - 192 kHz)
- **CPU**: < 1% on modern hardware

## License

See [LICENSE](LICENSE) for details.
