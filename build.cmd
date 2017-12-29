@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"

cl /LD miscfix.cpp /Femiscfix.dll
cl /LD noskin.cpp /Fenoskin.dll
cl /LD /EHsc setfov.cpp /Fesetfov.dll
cl /LD lodfix.cpp /Felodfix.dll
cl /LD extradirfix.cpp /Feextradirfix.dll
cl /LD overridecd.cpp /Feoverridecd.dll
cl /LD portablegta.cpp /Feportablegta.dll
cl /LD hydraulics.cpp /Fehydraulics.dll
