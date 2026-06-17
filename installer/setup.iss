#define MyAppName "PLACE"
#define MyAppVersion "1.0"
#define MyAppPublisher "leve"
#define MyAppURL ""

[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={commoncf64}\VST3
DefaultGroupName={#MyAppName}
DisableDirPage=yes
DisableProgramGroupPage=yes
OutputDir=output
OutputBaseFilename=PLACE-v1.0-Setup
Compression=lzma2/ultra64
SolidCompression=yes
UninstallDisplayName={#MyAppName}
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\build\PLACE_artefacts\Release\VST3\PLACE.vst3\*"; DestDir: "{commoncf64}\VST3\PLACE.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\README.md"; DestDir: "{app}\docs"; Flags: ignoreversion
Source: "..\LICENSE"; DestDir: "{app}\docs"; Flags: ignoreversion

[Icons]
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"

[UninstallDelete]
Type: filesandordirs; Name: "{commoncf64}\VST3\PLACE.vst3"
