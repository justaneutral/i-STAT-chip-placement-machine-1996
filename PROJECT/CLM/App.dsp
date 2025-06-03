# Microsoft Developer Studio Project File - Name="App" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=App - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "App.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "App.mak" CFG="App - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "App - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "App - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "App - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /YX /FD /c
# ADD MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin\app.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "App - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\bin\app.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "App - Win32 Debug"
# Name "App - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\app.rc

!IF  "$(CFG)" == "App - Win32 Debug"

!ELSEIF  "$(CFG)" == "App - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\barcode\barcode.cpp
# End Source File
# Begin Source File

SOURCE=.\base_camera_parameters_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\bayState.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonColor.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleFake.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateProduct.cpp
# End Source File
# Begin Source File

SOURCE=.\createUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Date.cpp
# End Source File
# Begin Source File

SOURCE=.\EditColor.cpp
# End Source File
# Begin Source File

SOURCE=.\errorHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\errorLog.cpp
# End Source File
# Begin Source File

SOURCE=.\fetcher_sequence_parameters_dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\fileaccess\fileAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTimeOutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\get_value_from_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\getuserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\io\IOBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\machina\machina.cpp
# End Source File
# Begin Source File

SOURCE=.\machineSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\machineState.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Manipulator.cpp
# End Source File
# Begin Source File

SOURCE=.\motor_parameters_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MotorManipulatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\pick_place_parameters_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\pick_place_point_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProductListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProductTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\qci\QCIAxes.cpp
# End Source File
# Begin Source File

SOURCE=.\qci\QCICommands.cpp
# End Source File
# Begin Source File

SOURCE=.\serialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\smac\SMAC.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Stdafx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UsersListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\very_common_dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\vikingeditor\vikingeditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Wafer.cpp
# End Source File
# Begin Source File

SOURCE=.\wafer_camera_parameters_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\wafer_reference_point_dialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\App.h
# End Source File
# Begin Source File

SOURCE=.\barcode\barcode.h
# End Source File
# Begin Source File

SOURCE=.\base_camera_parameters_dialog.h
# End Source File
# Begin Source File

SOURCE=.\bayState.h
# End Source File
# Begin Source File

SOURCE=.\ButtonColor.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=..\commondll\commondll.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleFake.h
# End Source File
# Begin Source File

SOURCE=.\CreateProduct.h
# End Source File
# Begin Source File

SOURCE=.\createUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\Date.h
# End Source File
# Begin Source File

SOURCE=.\EditColor.h
# End Source File
# Begin Source File

SOURCE=.\errorHandle.h
# End Source File
# Begin Source File

SOURCE=.\errorLog.h
# End Source File
# Begin Source File

SOURCE=.\fetcher_sequence_parameters_dialog.h
# End Source File
# Begin Source File

SOURCE=..\fileaccess\fileaccess.h
# End Source File
# Begin Source File

SOURCE=.\FileTimeOutDialog.h
# End Source File
# Begin Source File

SOURCE=.\get_value_from_dialog.h
# End Source File
# Begin Source File

SOURCE=.\getuserDlg.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\io\IOBoard.h
# End Source File
# Begin Source File

SOURCE=.\machina\machina.h
# End Source File
# Begin Source File

SOURCE=.\machineSettings.h
# End Source File
# Begin Source File

SOURCE=.\machineState.h
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\Manipulator.h
# End Source File
# Begin Source File

SOURCE=.\motor_parameters_dialog.h
# End Source File
# Begin Source File

SOURCE=.\MotorManipulatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\pick_place_parameters_dialog.h
# End Source File
# Begin Source File

SOURCE=.\pick_place_point_dialog.h
# End Source File
# Begin Source File

SOURCE=.\ProductListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProductTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\qci\QCIAxes.h
# End Source File
# Begin Source File

SOURCE=.\qci\QCICommands.h
# End Source File
# Begin Source File

SOURCE=.\qci\QCICommDevice.h
# End Source File
# Begin Source File

SOURCE=.\qci\QCINBS10CommDevice.h
# End Source File
# Begin Source File

SOURCE=.\qci\QCINBS10Defines.h
# End Source File
# Begin Source File

SOURCE=.\serialPort.h
# End Source File
# Begin Source File

SOURCE=.\SetupScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\smac\SMAC.h
# End Source File
# Begin Source File

SOURCE=.\StatusScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE=.\UsersListDlg.h
# End Source File
# Begin Source File

SOURCE=.\very_common_dialog.h
# End Source File
# Begin Source File

SOURCE=..\vikingeditor\vikingeditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\Wafer.h
# End Source File
# Begin Source File

SOURCE=.\wafer_camera_parameters_dialog.h
# End Source File
# Begin Source File

SOURCE=.\wafer_reference_point_dialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icons\BLOSSOM.ICO
# End Source File
# End Group
# Begin Source File

SOURCE=..\commondll\Debug\commondll.lib
# End Source File
# Begin Source File

SOURCE=.\qci\QCNTCLD.LIB
# End Source File
# End Target
# End Project
