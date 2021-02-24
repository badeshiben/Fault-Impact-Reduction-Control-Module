@echo off
set COMPILER=cl 
				set COMPFLAGS=/c  /GR /W3 /EHs /nologo /MD /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /D_SECURE_SCL=0  /DMATLAB_MEX_FILE -I"D:\MATLAB_x86\extern\include" -I"D:\MATLAB_x86\simulink\include" /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /D_SECURE_SCL=0  /DMATLAB_MEX_FILE  /DMATLAB_MEX_FILE 
				set OPTIMFLAGS=/O2 /Oy- /DNDEBUG 
				set DEBUGFLAGS=/Z7 
				set LINKER=link 
				set LINKFLAGS= /nologo  /manifest kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /incremental:NO /export:%ENTRYPOINT% /DLL /LIBPATH:"D:\MATLAB_x86\extern\lib\win32\microsoft" libmx.lib libmex.lib libmat.lib 
				set LINKDEBUGFLAGS=/debug /PDB:"%OUTDIR%%MEX_NAME%.mexw32.pdb" 
				set NAME_OUTPUT=/out:"%OUTDIR%%MEX_NAME%%MEX_EXT%" 
set PATH=E:\software\Visual Studio\VC\Bin;E:\software\Visual Studio\VC\Bin\VCPackages;E:\software\Visual Studio\Common7\IDE;E:\software\Visual Studio\Common7\Tools;E:\software\Visual Studio\Common7\Tools\bin;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\\Bin;;%MATLAB_BIN%;%PATH%
set INCLUDE=E:\software\Visual Studio\VC\ATLMFC\INCLUDE;E:\software\Visual Studio\VC\INCLUDE;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\\include;%INCLUDE%
set LIB=E:\software\Visual Studio\VC\ATLMFC\LIB;E:\software\Visual Studio\VC\Lib;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\\Lib;%LIB%
set LIBPATH=E:\software\Visual Studio\VC\Lib;E:\software\Visual Studio\VC\ATLMFC\LIB;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\\lib;D:\MATLAB_x86\extern\lib\win32;;%LIBPATH%

nmake -f CART3_BasicTest_5states_sfun.mak
