mkdir obj
mkdir obj\mscv
cd libffi-mini\Windows
REM PREPROCESSING .S file
cl -nologo -DHX_WINDOWS -GR -Zi -c -EHs -D_CRT_SECURE_NO_DEPRECATE -wd4996 -EP -I./ win32.S >../../obj/mscv/win32
REM COMPILING PREPROCESSED FILE
ml -safeseh -c ../../obj/mscv/win32.obj ../../obj/mscv/win32
del ../../obj/mscv/win32.obj

REM converting from OMF to COFF
REM reference http://www.masm32.com/board/index.php
link -edit win32.obj
move win32.obj ../../obj/mscv


cd ../../
haxelib run hxcpp Build.xml
pause
