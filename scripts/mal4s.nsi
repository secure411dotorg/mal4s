# This installs two files, mal4s.exe and mal4s.ico, creates a start menu shortcut, builds an uninstaller, and
# adds uninstall information to the registry for Add/Remove Programs
 
# To get started, put this script into a folder with the two files (mal4s.exe, mal4s.ico, and license.rtf -
# You'll have to create these yourself) and run makensis on it
 
# If you change the names "mal4s.exe", "mal4s.ico", or "license.rtf" you should do a search and replace - they
# show up in a few places.
# All the other settings can be tweaked by editing the !defines at the top of this script
!define APPNAME "Mal4s"
!define COMPANYNAME "Dissect Cyber Inc"
!define DESCRIPTION "Mal4s is a free open source data visualization tool designed to make it easy to see clusters of related Internet resources."
# These three must be integers
!define VERSIONMAJOR 1
!define VERSIONMINOR 2
!define VERSIONBUILD 1
# These will be displayed by the "Click here for support information" link in "Add/Remove Programs"
# It is possible to use "mailto:" links in here to open the email client
!define HELPURL "https://github.com/secure411dotorg/mal4s/wiki" # "Support Information" link
!define UPDATEURL "https://github.com/secure411dotorg/mal4s/wiki/Current-Windows-Install" # "Product Updates" link
!define ABOUTURL "https://github.com/secure411dotorg/mal4s/blob/master/README.md" # "Publisher" link
# This is the size (in kB) of all the files copied into "Program Files"
!define INSTALLSIZE 20889
 
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "C:\${APPNAME}"
 
# rtf or txt file - remember if it is txt, it must be in the DOS text format (\r\n)
LicenseData "license.rtf"
# This will be in the installer/uninstaller's title bar
Name "${COMPANYNAME} - ${APPNAME}"
Icon "mal4s.ico"
outFile "Mal4s-${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}.exe"
 
!include LogicLib.nsh
 
# Just three pages - license agreement, install location, and installation
page license
page directory
Page instfiles
 
!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend
 
function .onInit
	setShellVarContext all
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "install"
	# Files for the install directory - to build the installer, these should be in the same directory as the install script (this file)
	setOutPath $INSTDIR
	# Files added here should be removed by the uninstaller (see section "uninstall")
	file "mal4s.exe"
	file "mal4s.ico"
	file "freetype6.dll"
	file "glew32.dll"
	file "libFLAC-8.dll"
	file "libjpeg-9.dll"
	file "libmikmod-2.dll"
	file "libmodplug-1.dll"
	file "libogg-0.dll"
	file "libpng16-16.dll"
	file "libtiff-5.dll"
	file "libvorbis-0.dll"
	file "libvorbisfile-3.dll"
	file "libwebp-4.dll"
	file "pcre3.dll"
	file "SDL2.dll"
	file "SDL2_image.dll"
	file "SDL2_mixer.dll"
	file "smpeg2.dll"
	file "zlib1.dll"
	# data directory
	setOutPath "$INSTDIR\data"
	file "data\always-open-chrome.png"
	file "data\apwg.conf"
	file "data\beam.png"
	file "data\bloom.tga"
	file "data\bloom_alpha.tga"
	file "data\copy-click.wav"
	file "data\dissectcyber.png"
	file "data\dnsm.conf"
	file "data\f5.wav"
	file "data\f7.wav"
	file "data\f9.wav"
	file "data\file.png"
	file "data\gource.style"
	file "data\httpm.conf"
	file "data\newns.conf"
	file "data\sample--newns.captions"
	file "data\sample--newns.mal4s"
	file "data\user.png"
	# data\fonts
	setOutPath "$INSTDIR\data\fonts"
	file "data\fonts\FreeSans.ttf"
	file "data\fonts\README"
	# data\shaders
	setOutPath "$INSTDIR\data\shaders"
	file "data\shaders\bloom.frag"
	file "data\shaders\bloom.vert"
	file "data\shaders\shadow.frag"
	file "data\shaders\shadow.vert"
	file "data\shaders\text.frag"
	file "data\shaders\text.vert"
	# Add any other files for the install directory (license files, app data, etc) here
 
	# Uninstaller - See function un.onInit and section "uninstall" for configuration
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	# Start Menu
	createDirectory "$SMPROGRAMS\${COMPANYNAME}"
	createShortCut "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk" "$INSTDIR\mal4s.exe" "" "$INSTDIR\mal4s.ico"
 
	# Registry information for add/remove programs
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${COMPANYNAME} - ${APPNAME} - ${DESCRIPTION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\mal4s.ico$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd
 
# Uninstaller
 
function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "uninstall"
 
	# Remove Start Menu launcher
	delete "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk"
	# Try to remove the Start Menu folder - this will only happen if it is empty
	rmDir "$SMPROGRAMS\${COMPANYNAME}"
 
	# Remove files
	delete "$INSTDIR\mal4s.exe"
	delete "$INSTDIR\mal4s.ico"
	delete "$INSTDIR\freetype6.dll"
	delete "$INSTDIR\glew32.dll"
	delete "$INSTDIR\libFLAC-8.dll"
	delete "$INSTDIR\libjpeg-9.dll"
	delete "$INSTDIR\libmikmod-2.dll"
	delete "$INSTDIR\libmodplug-1.dll"
	delete "$INSTDIR\libogg-0.dll"
	delete "$INSTDIR\libpng16-16.dll"
	delete "$INSTDIR\libtiff-5.dll"
	delete "$INSTDIR\libvorbis-0.dll"
	delete "$INSTDIR\libvorbisfile-3.dll"
	delete "$INSTDIR\libwebp-4.dll"
	delete "$INSTDIR\pcre3.dll"
	delete "$INSTDIR\SDL2.dll"
	delete "$INSTDIR\SDL2_image.dll"
	delete "$INSTDIR\SDL2_mixer.dll"
	delete "$INSTDIR\smpeg2.dll"
	delete "$INSTDIR\zlib1.dll"
	# data directory
	delete "$INSTDIR\data\always-open-chrome.png"
	delete "$INSTDIR\data\apwg.conf"
	delete "$INSTDIR\data\beam.png"
	delete "$INSTDIR\data\bloom.tga"
	delete "$INSTDIR\data\bloom_alpha.tga"
	delete "$INSTDIR\data\copy-click.wav"
	delete "$INSTDIR\data\dissectcyber.png"
	delete "$INSTDIR\data\dnsm.conf"
	delete "$INSTDIR\data\f5.wav"
	delete "$INSTDIR\data\f7.wav"
	delete "$INSTDIR\data\f9.wav"
	delete "$INSTDIR\data\file.png"
	delete "$INSTDIR\data\gource.style"
	delete "$INSTDIR\data\httpm.conf"
	delete "$INSTDIR\data\newns.conf"
	delete "$INSTDIR\data\sample--newns.captions"
	delete "$INSTDIR\data\sample--newns.mal4s"
	delete "$INSTDIR\data\user.png"
	# data\fonts
	delete "$INSTDIR\data\fonts\FreeSans.ttf"
	delete "$INSTDIR\data\fonts\README"
	# data\shaders
	delete "$INSTDIR\data\shaders\bloom.frag"
	delete "$INSTDIR\data\shaders\bloom.vert"
	delete "$INSTDIR\data\shaders\shadow.frag"
	delete "$INSTDIR\data\shaders\shadow.vert"
	delete "$INSTDIR\data\shaders\text.frag"
	delete "$INSTDIR\data\shaders\text.vert"
 
	# Always delete uninstaller as the last action
	delete $INSTDIR\uninstall.exe
 
	# Try to remove the install directory - this will only happen if it is empty
	rmDir $INSTDIR
 
	# Remove uninstaller information from the registry
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
sectionEnd
