REM NOT WORKING
REM IT DOESN'T SEEM THAT HXCPP HAS SUPPORT FOR WINDOWS64

mkdir obj
mkdir obj\mscv
cd libffi-mini\Windows
REM PREPROCESSING .S file
cl -nologo -DHX_WINDOWS -GR -Zi -c -EHs -D_CRT_SECURE_NO_DEPRECATE -wd4996 -EP -I./ win64.S >../../obj/mscv/win64
REM COMPILING PREPROCESSED FILE
../../ml64.exe ../../obj/mscv/win64.obj ../../obj/mscv/win64
del ../../obj/mscv/win64.obj

REM converting from OMF to COFF
REM reference http://www.masm32.com/board/index.php
link -edit win64.obj
move win64.obj ../../obj/mscv


cd ../../
REM haxelib run hxcpp Build.xml -DHXCPP_M64

cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include ./libffi-mini/Windows//ffi64.c -Foobj/mscv/ffi64.obj
cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include ./libffi-mini/Windows//prep_cif.c -Foobj/mscv/prep_cif.obj

cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include ./libffi-mini/Windows//raw_api.c -Foobj/mscv/raw_api.obj
cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include ./libffi-mini/Windows//types.c -Foobj/mscv/types.obj

cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include ./libffi-mini/Windows//win64.S -Foobj/mscv/win64.obj

cl.exe -I./libffi-mini/Windows/ -nologo -O2 -MT -DHX_WINDOWS -GR -Zi -c -EHs -IC:\Motion-Twin\haxe\lib\hxcpp\2,08,0//include -D_CRT_SECURE_NO_DEPRECATE -wd4996 -Ie:/VS8//PlatformSDK/Include .//libffi.cpp -Foobj/mscv/libffi.obj
win64\link.exe -out:obj/lib/hxffi.ndll -nologo -machine:x86 -dll -libpath:lib -libpath:e:\VS8\DirectXSDK\Lib\x86 -libpath:e:/VS8/PlatformSDK/Lib @win64\all_objs wsock32.lib

pause
