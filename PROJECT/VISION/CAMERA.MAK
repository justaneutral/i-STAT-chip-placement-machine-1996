SOFT_PATH = c:\utils\camera\xvl\pc

COMPILER=cl
RSC=rc
LINKER = link

LIBPATH=$(SOFT_PATH)\lib
INCPATH=$(SOFT_PATH)\include

CFLAGS=/W3 /GX /Od /c /D_WINDOWS /DWIN32 /D__NT__ /I$(INCPATH)
#CFLAGS=/W3 /GX /Od /c /Zi /D_WINDOWS /DWIN32 /D__NT__ /I$(INCPATH)

WNTOBJS=camera.obj cmndlg.obj cproc.obj csetup.obj cimage.obj wnt.res

WNTLINK=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 /SUBSYSTEM:windows /INCREMENTAL:no /MACHINE:I386 /OUT:camera.exe

#WNTLINK=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
# advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /DEBUG\
# /SUBSYSTEM:windows /INCREMENTAL:no /MACHINE:I386 /OUT:camera.exe

DEP_WNT=camera.ico camera2.ico camerarsc.h camera.h
RCS_PROJ=/dWIN32 /d__NT__ /l 0x409 /fownt.res /i$(INCPATH)

camera.exe: $(WNTOBJS) camera.h
    $(LINKER) $(WNTLINK) $(WNTOBJS) $(LIBPATH)\wntxpgml.lib

camera.obj:  camera.c
        $(COMPILER) $(CFLAGS) camera.c

cmndlg.obj: cmndlg.c
        $(COMPILER) $(CFLAGS) cmndlg.c

cproc.obj:    cproc.c
        $(COMPILER) $(CFLAGS) cproc.c

csetup.obj:    csetup.c
        $(COMPILER) $(CFLAGS) csetup.c

cimage.obj:    cimage.c
        $(COMPILER) $(CFLAGS) cimage.c

wnt.res:    camera.rc $(DEP_WNT)
   $(RSC) $(RCS_PROJ) camera.rc

