@call "..\environment.bat" %1 mingw cfsd

@if %ERRORLEVEL% == 0 set MODULE_NAME=qtsingleinstance
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=qmime
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=urlfilter
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "USE_QTTYPES:BOOL=0" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=squeeze
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "USE_QTTYPES:BOOL=0" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 set PATH=%MODULES_DIR%\%MODULE_NAME%\src;%PATH%
@if %ERRORLEVEL% == 0 set PATH=%TMP_DIR%;%PATH%


@if %ERRORLEVEL% == 0 set MODULE_NAME=zlib-1.2.5
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\ap_engine\1.0\3rdparty\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install

@if %ERRORLEVEL% == 0 set MODULE_NAME=libpng-1.5.0
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "SKIP_INSTALL_LIBRARIES:BOOL=0" -D "SKIP_INSTALL_ALL:BOOL=0" -D "PNG_SHARED:BOOL=1" -D "PNG_STATIC:BOOL=0" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\ap_engine\1.0\3rdparty\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install

@if %ERRORLEVEL% == 0 set MODULE_NAME=jpeg-win
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\ap_engine\1.0\3rdparty\%MODULE_NAME%\src"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install

@if %ERRORLEVEL% == 0 set MODULE_NAME=gd-2.0.35
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\ap_engine\1.0\3rdparty\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install

@if %ERRORLEVEL% == 0 set MODULE_NAME=ap_engine
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%MODULES_DIR%\%MODULE_NAME%\1.0\src"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=curl
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "DISABLED_THREADSAFE:BOOL=1" -D "BUILD_CURL_EXE:BOOL=0" -D "BUILD_CURL_TESTS:BOOL=0" -D "CURL_ZLIB:BOOL=0" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%THIRD_PARTY_DIR%\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=thumbnailer
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%PROJECT_DIR%\3rdparty\%MODULE_NAME%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install


@if %ERRORLEVEL% == 0 set MODULE_NAME=cfsd
@if %ERRORLEVEL% == 0 set TMP_DIR=%BUILD_DIR%\%MODULE_NAME%
@if %ERRORLEVEL% == 0 if not exist %TMP_DIR% mkdir %TMP_DIR%
@if %ERRORLEVEL% == 0 cd %TMP_DIR%

@if %ERRORLEVEL% == 0 cmake -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE:STRING=%1" -D "CMAKE_INSTALL_PREFIX:STRING=%WORK_DIR%" "%PROJECT_DIR%"
@if %ERRORLEVEL% == 0 mingw32-make
@if %ERRORLEVEL% == 0 mingw32-make install
