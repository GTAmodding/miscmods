@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"

cl /LD /I %RWSDK34% /I C:\Users\aap\src\lpng1634 "C:\Users\aap\src\lpng1634\projects\vstudio\Release Library\libpng16.lib" "C:\Users\aap\src\lpng1634\projects\vstudio\Release Library\zlib.lib" screenshot.cpp /Fescreenshot.dll
cl /LD vehicleflare.cpp /Fevehicleflare.dll
cl /LD iiifont.cpp /Feiiifont.dll
cl /LD noreplay.cpp /Fenoreplay.dll
cl /LD miscfix.cpp /Femiscfix.dll
cl /LD noskin.cpp /Fenoskin.dll
cl /LD /EHsc setfov.cpp /Fesetfov.dll
cl /LD lodfix.cpp /Felodfix.dll
cl /LD extradirfix.cpp /Feextradirfix.dll
cl /LD overridecd.cpp /Feoverridecd.dll
cl /LD portablegta.cpp /Feportablegta.dll
cl /LD hydraulics.cpp /Fehydraulics.dll
