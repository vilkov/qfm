@call "..\environment.bat" %1 mingw qfm

@if %ERRORLEVEL% == 0 set MODULE_NAME=qfm
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%PROJECT_DIR%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install
