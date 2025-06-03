# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=App - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to App - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "App - Win32 Debug" && "$(CFG)" != "App - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "App.mak" CFG="App - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "App - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "App - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "App - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\App.exe"

CLEAN : 
	-@erase ".\WinDebug\App.exe"
	-@erase ".\WinDebug\Stdafx.obj"
	-@erase ".\WinDebug\app.res"
	-@erase ".\WinDebug\App.obj"
	-@erase ".\WinDebug\App.pch"
	-@erase ".\WinDebug\Date.obj"
	-@erase ".\WinDebug\Mainfrm.obj"
	-@erase ".\WinDebug\App.ilk"
	-@erase ".\WinDebug\App.pdb"
	-@erase ".\WinDebug\vc40.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/App.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/app.res" /i "..\include" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/App.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/App.pdb" /debug /machine:I386 /out:"$(OUTDIR)/App.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Stdafx.obj" \
	"$(INTDIR)/App.obj" \
	"$(INTDIR)/Date.obj" \
	"$(INTDIR)/Mainfrm.obj" \
	"$(INTDIR)/app.res"

"$(OUTDIR)\App.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "App - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\App.exe"

CLEAN : 
	-@erase ".\WinRel\App.exe"
	-@erase ".\WinRel\Stdafx.obj"
	-@erase ".\WinRel\app.res"
	-@erase ".\WinRel\App.obj"
	-@erase ".\WinRel\App.pch"
	-@erase ".\WinRel\Date.obj"
	-@erase ".\WinRel\Mainfrm.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/App.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/app.res" /i "..\include" /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/App.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/App.pdb" /machine:I386 /out:"$(OUTDIR)/App.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Stdafx.obj" \
	"$(INTDIR)/App.obj" \
	"$(INTDIR)/Date.obj" \
	"$(INTDIR)/Mainfrm.obj" \
	"$(INTDIR)/app.res"

"$(OUTDIR)\App.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL_PROJ=
################################################################################
# Begin Target

# Name "App - Win32 Debug"
# Name "App - Win32 Release"

!IF  "$(CFG)" == "App - Win32 Debug"

!ELSEIF  "$(CFG)" == "App - Win32 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Stdafx.cpp
DEP_CPP_STDAF=\
	".\Stdafx.h"\
	

!IF  "$(CFG)" == "App - Win32 Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/App.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\Stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\App.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "App - Win32 Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/App.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\Stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\App.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\app.rc
DEP_RSC_APP_R=\
	".\..\INCLUDE\BLOSSOM.ICO"\
	

"$(INTDIR)\app.res" : $(SOURCE) $(DEP_RSC_APP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\App.cpp
DEP_CPP_APP_C=\
	".\Stdafx.h"\
	".\App.h"\
	".\Mainfrm.h"\
	

!IF  "$(CFG)" == "App - Win32 Debug"


"$(INTDIR)\App.obj" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"\
 "$(INTDIR)\App.pch"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ELSEIF  "$(CFG)" == "App - Win32 Release"


"$(INTDIR)\App.obj" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date.cpp
DEP_CPP_DATE_=\
	".\Stdafx.h"\
	".\App.h"\
	".\Date.h"\
	

!IF  "$(CFG)" == "App - Win32 Debug"


"$(INTDIR)\Date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"\
 "$(INTDIR)\App.pch"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ELSEIF  "$(CFG)" == "App - Win32 Release"


"$(INTDIR)\Date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mainfrm.cpp
DEP_CPP_MAINF=\
	".\Stdafx.h"\
	".\App.h"\
	".\Mainfrm.h"\
	".\Date.h"\
	

!IF  "$(CFG)" == "App - Win32 Debug"


"$(INTDIR)\Mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\App.pch"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ELSEIF  "$(CFG)" == "App - Win32 Release"


"$(INTDIR)\Mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\App.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
