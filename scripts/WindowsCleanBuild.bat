pushd %~dp0\..\
rmdir /S /Q .vs
del /S /Q *.sln *.vcxproj *.vcxproj.filters *.vcxproj.user *.csproj *.csproj.filters *.csproj.user
call vendor\premake\bin\premake5.exe clean
popd
PAUSE 