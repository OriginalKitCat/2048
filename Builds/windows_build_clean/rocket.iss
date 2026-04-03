[Setup]
AppName=2048
AppVersion=1.0
AppPublisher=KitCat
AppPublisherURL=https://tiny.cc/kit2048
AppCopyright=© 2026 KitCat / GNU GPLv3
DefaultDirName={autopf}\2048
DefaultGroupName=2048
OutputDir=installer
OutputBaseFilename=2048-setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
LicenseFile=LICENSE.txt
SetupIconFile=dist\bin\share\icons\logo\logo.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "dist\bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\2048"; Filename: "{app}\2048.exe"
Name: "{group}\{cm:UninstallProgram,2048}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\2048"; Filename: "{app}\2048.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\2048.exe"; Description: "{cm:LaunchProgram,2048}"; Flags: nowait postinstall skipifsilent