# Microsoft Developer Studio Project File - Name="T3ED" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=T3ED - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "T3ED.MAK".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "T3ED.MAK" CFG="T3ED - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "T3ED - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "T3ED - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "T3ED - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "C:\Programme\DirectX 9.0 SDK\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib d3drm.lib dxguid.lib d3d9.lib /nologo /subsystem:windows /machine:I386 /libpath:"H:\ohjelmointi\dx81sdk\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "T3ED - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ddraw.lib d3drm.lib dxguid.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Programme\DirectX 9.0 SDK\Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "T3ED - Win32 Release"
# Name "T3ED - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BlockPaste.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockProp.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockProps.cpp
# End Source File
# Begin Source File

SOURCE=.\ClearAll.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpandBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpandDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtraObjProps.cpp
# End Source File
# Begin Source File

SOURCE=.\FindPolygonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FrdFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\LodGenProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\LodStartEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveTo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewobjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\QFSView.cpp
# End Source File
# Begin Source File

SOURCE=.\RayTraceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetVisiDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\smooth.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLightProps.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\T3ED.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\T3ED.hpj

!IF  "$(CFG)" == "T3ED - Win32 Release"

USERDEP__T3ED_="$(ProjDir)\hlp\AfxCore.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=T3ED
InputPath=.\hlp\T3ED.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "T3ED - Win32 Debug"

USERDEP__T3ED_="$(ProjDir)\hlp\AfxCore.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=T3ED
InputPath=.\hlp\T3ED.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\T3ED.rc
# End Source File
# Begin Source File

SOURCE=.\T3EDDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\T3EDFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\T3EDView.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureProps.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackView.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualRoadAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\VRoadHeightsSpdFileDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BlockPaste.h
# End Source File
# Begin Source File

SOURCE=.\BlockProp.h
# End Source File
# Begin Source File

SOURCE=.\BlockProps.h
# End Source File
# Begin Source File

SOURCE=.\ClearAll.h
# End Source File
# Begin Source File

SOURCE=.\ExpandBlock.h
# End Source File
# Begin Source File

SOURCE=.\ExpandDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExtraObjProps.h
# End Source File
# Begin Source File

SOURCE=.\FindPolygonDlg.h
# End Source File
# Begin Source File

SOURCE=.\FrdFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\LodGenProgress.h
# End Source File
# Begin Source File

SOURCE=.\LodStartEnd.h
# End Source File
# Begin Source File

SOURCE=.\MoveTo.h
# End Source File
# Begin Source File

SOURCE=.\NewobjDlg.h
# End Source File
# Begin Source File

SOURCE=.\PolyFlag.h
# End Source File
# Begin Source File

SOURCE=.\PolyProperties.h
# End Source File
# Begin Source File

SOURCE=.\QFSView.h
# End Source File
# Begin Source File

SOURCE=.\RayTraceDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetVisiDlg.h
# End Source File
# Begin Source File

SOURCE=.\smooth.h
# End Source File
# Begin Source File

SOURCE=.\SoundLightProps.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\T3ED.h
# End Source File
# Begin Source File

SOURCE=.\T3EDDoc.h
# End Source File
# Begin Source File

SOURCE=.\T3EDFrame.h
# End Source File
# Begin Source File

SOURCE=.\T3EDView.h
# End Source File
# Begin Source File

SOURCE=.\TextureProps.h
# End Source File
# Begin Source File

SOURCE=.\TrackView.h
# End Source File
# Begin Source File

SOURCE=.\Vectors.h
# End Source File
# Begin Source File

SOURCE=.\VirtualRoadAdjust.h
# End Source File
# Begin Source File

SOURCE=.\VRoadHeightsSpdFileDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\backimag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\Foo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\T3ED.ico
# End Source File
# Begin Source File

SOURCE=.\res\T3ED.rc2
# End Source File
# Begin Source File

SOURCE=.\res\T3EDDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\T3ED.cnt
# End Source File
# End Group
# Begin Group "tracer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jgtTracer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\T3ED.reg
# End Source File
# End Target
# End Project
