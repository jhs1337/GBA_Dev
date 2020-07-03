# Microsoft Developer Studio Project File - Name="skiing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=skiing - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "skiing.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "skiing.mak" CFG="skiing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "skiing - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "skiing - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "skiing - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f skiing.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "skiing.exe"
# PROP BASE Bsc_Name "skiing.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "domake"
# PROP Rebuild_Opt "all"
# PROP Target_File "Skiing.gba"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "skiing - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f skiing.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "skiing.exe"
# PROP BASE Bsc_Name "skiing.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "domake"
# PROP Rebuild_Opt "all"
# PROP Target_File "Skiing.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "skiing - Win32 Release"
# Name "skiing - Win32 Debug"

!IF  "$(CFG)" == "skiing - Win32 Release"

!ELSEIF  "$(CFG)" == "skiing - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\gfx.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\mysprite.c
# End Source File
# Begin Source File

SOURCE=.\sprite.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "gfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gfx\ccrash.h
# End Source File
# Begin Source File

SOURCE=.\gfx\credits.h
# End Source File
# Begin Source File

SOURCE=.\gfx\end.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gamebg.h
# End Source File
# Begin Source File

SOURCE=.\gfx\logo.h
# End Source File
# Begin Source File

SOURCE=.\gfx\menu1.h
# End Source File
# Begin Source File

SOURCE=.\num\numbers.h
# End Source File
# Begin Source File

SOURCE=.\sprite\objects.h
# End Source File
# Begin Source File

SOURCE=.\gfx\ski.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\gba.h
# End Source File
# Begin Source File

SOURCE=.\gfx.h
# End Source File
# Begin Source File

SOURCE=.\keypad.h
# End Source File
# Begin Source File

SOURCE=.\mysprite.h
# End Source File
# Begin Source File

SOURCE=.\screenmode.h
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\domake.bat
# End Source File
# Begin Source File

SOURCE=.\dorun.bat
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Group
# End Target
# End Project
