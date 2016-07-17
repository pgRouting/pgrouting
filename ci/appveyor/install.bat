@echo off

:: each sub-script starts and end on this directory:

Setlocal EnableDelayedExpansion EnableExtensions

echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined MSVC_VER set MSVC_VER=12.0
if not defined BUILD_ROOT_DIR set BUILD_ROOT_DIR=c:\build
if not defined DOWNLOADS_DIR set BUILD_ROOT_DIR=%APPVEYOR_BUILD_FOLDER%\downloads

if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PG_VERSION set PG_VERSION=2.2.2
if not defined BOOST_VERSION set BOOST_VERSION=1.58.0


:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

::
:: =========================================================

:: create a download directory:
cd %APPVEYOR_BUILD_FOLDER%
mkdir %DOWNLOADS_DIR% 2>NUL


:: =========================================================
:: Download and install cmake
::

for /f "tokens=*  delims=" %%a in ('cmake --version') do (
    set CURR_CMAKE=%%a& goto _ExitForLoop1
)
:_ExitForLoop1
set CURR_CMAKE=%CURR_CMAKE:~14%

if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
    echo cmake %CMAKE_VERSION% already installed
) else (
    echo Downoading cmake %CMAKE_VERSION%
    pushd %DOWNLOADS_DIR%
    curl -L -O -S -s https://cmake.org/files/v3.5/cmake-%CMAKE_VERSION%-win32-%plataform%.msi
    popd

    echo Installing cmake %CMAKE_VERSION%
    pushd %DOWNLOADS_DIR%
    start /wait msiexec /i cmake-%CMAKE_VERSION%-win32-%plataform%.msi /qn
    popd

    for /f "tokens=*  delims=" %%a in ('cmake --version') do (
        set CURR_CMAKE=%%a& goto _ExitForLoop2
    )
    :_ExitForLoop2
    set CURR_CMAKE=%CURR_CMAKE:~14%
    if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
        echo cmake %CMAKE_VERSION% installed
    ) else (
        echo something went wrong on cmake installation!!!!!!!!!
    )
)

::
:: =========================================================



:: =========================================================
:: Download and install Postgis
::

if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
    cd %APPVEYOR_BUILD_FOLDER%
    if not exist %DOWNLOADS_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
        echo Downoading postGIS %PG_VERSION%
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://winnie.postgis.net/download/windows/pg94/buildbot/postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
        popd
        if not exist %DOWNLOADS_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
            echo something went wrong on postgis %PG_VERSION% download !!!!!!!!!
        )
    )

    echo Extracting postGIS %PG_VERSION%
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR%\ postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
    popd

    echo Installing postGIS %PG_VERSION%
    xcopy /e /y /q %BUILD_ROOT_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48 C:\Progra~1\PostgreSQL\9.4

    if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
        echo something went wrong on postGIS %PG_VERSION% installation !!!!!!!!!
        dir %DOWNLOADS_DIR%
        dir C:\Progra~1\PostgreSQL\9.4\
    ) else (
        echo postGIS %PG_VERSION% %arch% installed
    )
) else (
    echo postGIS %PG_VERSION% %arch% already installed
)

::
:: =========================================================



:: =========================================================
:: Download and install Boost
::

:: deducing variables
set BOOST_VER_USC=%BOOST_VERSION:.=_%
set BOOST_SHORT_VER=%BOOST_VERSION:.0=%
set BOOST_INSTALL_DIR=%BUILD_ROOT_DIR%\local
set BOOST_INCLUDE_DIR=%BOOST_INSTALL_DIR%\include\boost-%BOOST_SHORT_VER%
set BOOST_LIBRARY_DIR=%BOOST_INSTALL_DIR%\lib
set BOOST_THREAD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_thread-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_SYSTEM_LIB=%BOOST_INSTALL_DIR%\lib\libboost_system-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_WILDCARD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_*-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.libs

set BOOST_SRC_DIR=%BUILD_ROOT_DIR%\boost_%BOOST_VER_USC%
set MSBUILD_CONFIGURATION=%CONFIGURATION%

:: DEBUGING
echo BOOST_VERSION %BOOST_VERSION%
echo BOOST_VER_USC %BOOST_VER_USC%
echo BOOST_SHORT_VER %BOOST_SHORT_VER%
echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
echo BOOST_INSTALL_DIR %BOOST_INSTALL_DIR%
echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
echo BOOST_WILDCARD_LIB %BOOST_WILDCARD_LIB%

:: check that everything needed from boost is there
if not exist "%BOOST_INCLUDE_DIR%\" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_LIBRARY_DIR%\" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_THREAD_LIB%" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_SYSTEM_LIB%" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_WILDCARD_LIB%" ( set BOOST_INSTALL_FLAG=1 )

:: DEBUGING
echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%

if %BOOST_INSTALL_FLAG% EQU 1 (

    dir %DOWNLOADS_DIR%
    
    :: check if it needs to be downloaded
    if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
        pushd %DOWNLOADS_DIR%
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        popd
        if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
            echo something went wrong on boost %BOOST_VERSION% download !!!!!!!!!
            dir %DOWNLOADS_DIR%
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR%\ downloads/Boost_%BOOST_VER_USC%.zip
    popd
    dir %BOOST_SRC_DIR%
    if not exist %BOOST_SRC_DIR% (
        echo something went wrong on boost extraction!!!!!!!!!
    )

    if not exist "%BOOST_SRC_DIR%\b2.exe" (
        dir %BOOST_SRC_DIR%
        echo %BOOST_SRC_DIR%\b2.exe missing
        pushd %BOOST_SRC_DIR%
        call "bootstrap.bat"
        popd
        dir %BOOST_SRC_DIR%
    )

    if not exist %BOOST_INCLUDE_DIR%\ (
        pushd %BOOST_SRC_DIR%
        @echo on
        b2 toolset=%BOOST_TOOLSET% variant=release link=static threading=multi address-model=%BOOST_ADDRESS_MODEL% ^
            --with-thread --with-system --prefix=%COMMON_INSTALL_DIR% -d0 install
        @echo off
        popd
    ) else (
        echo Boost_%BOOST_VERSION% already installed
    )
)



::set MSVC_VER=12.0
::set RUNTIME=msvc%MSVC_VER:.=%
::set BOOST_TOOLSET=msvc-%MSVC_VER%
::set CMAKE_GENERATOR=Visual Studio %MSVC_VER:.0=% %MSVC_YEAR%
::
::if "%2"=="x86" (
    ::set PLATFORM=x86
    ::set BOOST_ADDRESS_MODEL=32
::) else if "%2"=="x64" (
    ::set PLATFORM=x64
    ::set BOOST_ADDRESS_MODEL=64
    ::set CMAKE_GENERATOR=%CMAKE_GENERATOR% Win64
::) else (
    ::echo %USAGE%
    ::exit /B 1
::)







::if not exist "C:\local\boost_%BOOST_VER_USC%\lib%arch%-msvc-14.0" (
    :: download installer??
::    if not exist downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
::        cd downloads
::        echo Downloading Boost %BOOST_VERSION% %arch% bits...
::        curl --silent --fail --location --max-time 1600 --connect-timeout 30 http://sourceforge.net/projects/boost/files/boost-binaries/%BOOST_VERSION%/boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe/download
::        cd ..
::        if not exist downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
::            echo something went wrong on boost %BOOST_VERSION% %arch% bits extraction!!!!!!!!!
::        )
::    ) else (
::        echo Boost %BOOST_VERSION% %arch% bits already downloaded
::    )

::    echo Installing Boost %BOOST_VERSION% %arch% bits...
::    downloads\boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe /silent /verysilent /sp- /suppressmsgboxes
::    echo Done installing.
::) else (
::    echo Boost %BOOST_VERSION% %arch% bits already installed
::)
::
:: =========================================================

cd c:\build

echo.
echo ======================================================
echo Installation of Boost done.
echo Platform - %platform%
echo Boost - C:\build\boost_%BOOST_VER_USC%
echo ======================================================
echo.

endlocal & set PATH=%PATH%&

goto :eof
