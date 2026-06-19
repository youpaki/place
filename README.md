# PLACE

**by leve**

Create space for vocals instantly.

## Overview

PLACE is a stereo VST3 effect plugin designed to create room for vocals inside an instrumental mix. It works on stereo buses using Mid/Side processing to carve space in the center channel where vocals sit, while maintaining stereo width and energy.

## Installation

### Using the Installer

1. Download `PLACE-v1.0-Setup.exe` from [Releases](https://github.com/youpaki/place/releases)
2. Run the installer
3. The plugin will be installed to `C:\Program Files\Common Files\VST3\PLACE.vst3`
4. Restart your DAW or rescan plugins

### Manual Installation

Copy `PLACE.vst3` to:
```
C:\Program Files\Common Files\VST3\
```

## Controls

### PLACE (0-100%)

The primary control. Determines how much room is created for vocals in the mid channel.

- **0%**: No processing
- **50%**: Moderate vocal space
- **100%**: Maximum vocal space

Applies a multi-band EQ curve on the mid channel with a high-shelf bell at 12 kHz (-7 dB) to tame harsh ultra-high frequencies.

### SIDE BASS REMOVER (0-100%)

Removes low frequencies from the side channel using a fixed 90 Hz Butterworth high-pass filter with dry/wet blending.

- **0%**: No filtering (dry signal)
- **100%**: Full high-pass at 90 Hz (wet signal)

## Automation

Both parameters are fully automatable:

| Parameter | Range | Default |
|-----------|-------|---------|
| PLACE | 0-100% | 0% |
| SIDE BASS REMOVER | 0-100% | 0% |

## Troubleshooting

### Plugin not showing in DAW

- Rescan plugins in your DAW's plugin manager
- Verify `PLACE.vst3` exists in `C:\Program Files\Common Files\VST3\`
- Restart your DAW

### Audio artifacts

- Reduce PLACE if you hear excessive thinning
- Use SIDE BASS REMOVER to clean up low-end buildup in the stereo field

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
3. Run `ISCC.exe installer/setup.iss` from the repo root

## Technical Details

- **Format**: VST3
- **Channels**: Stereo in, Stereo out
- **Latency**: Zero
- **Processing**: Mid/Side domain
- **Sample Rate**: Any (44.1 kHz - 192 kHz)
- **CPU**: < 1% on modern hardware

## License

See [LICENSE](LICENSE) for details.
