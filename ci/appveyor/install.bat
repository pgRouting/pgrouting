@echo off

::set LOCAL_DEBUG=1

Setlocal EnableDelayedExpansion EnableExtensions

if defined LOCAL_DEBUG echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%
echo platform %platform%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined MSVC_VER set MSVC_VER=15.0
if not defined RUNTIME set RUNTIME=msvc%MSVC_VER:.=%
if not defined MSVC_YEAR set MSVC_YEAR=2017
if not defined BUILD_ROOT_DIR set BUILD_ROOT_DIR=c:\build
if not defined DOWNLOADS_DIR set DOWNLOADS_DIR=%APPVEYOR_BUILD_FOLDER%\downloads
if not defined COMMON_INSTALL_DIR set COMMON_INSTALL_DIR=%BUILD_ROOT_DIR%\local\%RUNTIME%\%PLATFORM%

:: for cmake its the min version
if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PGIS_VERSION set PGIS_VERSION=3.2
if not defined BOOST_VERSION set BOOST_VERSION=1.65.0
set PG_VER_NO_DOT=pg%PG_VER%


set CMAKE_GENERATOR=Visual Studio 15 2017 Win64

:: Determine if arch is 64 bits
set arch=64


:: Determine compiler used to build postgis
:: set GCC=81

::
:: =========================================================

:: create a download directory:
pushd %APPVEYOR_BUILD_FOLDER%
mkdir downloads 2>NUL
popd
mkdir %COMMON_INSTALL_DIR% 2>NUL
if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
if defined LOCAL_DEBUG dir %COMMON_INSTALL_DIR%


:: =========================================================
:: Download and install cmake
::

echo ==================================== CMAKE
for /f "tokens=*  delims=" %%a in ('cmake --version') do (
    set CURR_CMAKE=%%a& goto _ExitForLoop1
)
:_ExitForLoop1
set CURR_CMAKE=%CURR_CMAKE:~14%
echo cmake %CURR_CMAKE% found

CALL ci\appveyor\vercompare.bat "%CURR_CMAKE%" "%CMAKE_VERSION%"

echo "errorlevel %ERRORLEVEL%"

if %ERRORLEVEL% EQU 0 (
    echo cmake %CMAKE_VERSION% already installed
) else (
    CALL SET
    echo Downloading cmake %CMAKE_VERSION%
    pushd %DOWNLOADS_DIR%
    curl -L -O -S -s https://cmake.org/files/v3.5/cmake-%CMAKE_VERSION%-win32-%platform%.msi
    popd

    echo Installing cmake %CMAKE_VERSION%
    pushd %DOWNLOADS_DIR%
    start /wait msiexec /i cmake-%CMAKE_VERSION%-win32-%platform%.msi /qn
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
        Exit \B 1
    )
)
echo ====================================

::
:: =========================================================



:: =========================================================
:: Download and install Postgis
::

set PGIS_WILD_FILE=postgis-pg12-binaries-3.2.0devw64gcc81
set PGIS_FILE=postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%w%arch%gcc%GCC%.zip

echo %PGIS_WILD_FILE%
echo %PGIS_FILE%

echo ==================================== POSTGIS
if not exist "C:\Progra~1\PostgreSQL\%PG_VER%\%PGIS_WILD_FILE%" (
    cd %APPVEYOR_BUILD_FOLDER%
    if not exist %DOWNLOADS_DIR%\postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%w%arch%gcc%GCC%.zip (
        echo Downloading PostGIS %PGIS_VERSION%
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://winnie.postgis.net/download/windows/appveyor/postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%w%arch%gcc%GCC%.zip
        popd
        if not exist %DOWNLOADS_DIR%\postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%w%arch%gcc%GCC%.zip (
            echo something went wrong on PostGIS %PGIS_VERSION% download !!!!!!!!!
            if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
            Exit \B 1
        )
    )

    echo Extracting PostGIS %PGIS_VERSION%
    pushd %DOWNLOADS_DIR%
    dir
    7z x -o%BUILD_ROOT_DIR%\ postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%w%arch%gcc%GCC%.zip
    dir
    popd

    echo **** Installing postGIS %PGIS_VERSION%
    dir %BUILD_ROOT_DIR%\postgis*
    dir C:\Progra~1\PostgreSQL\
    dir C:\Progra~1\PostgreSQL\%PG_VER%\postgis*
    xcopy /e /y /q %BUILD_ROOT_DIR%\postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION%*w%arch%gcc%GCC% C:\Progra~1\PostgreSQL\%PG_VER%
    dir %BUILD_ROOT_DIR%\postgis*
    dir C:\Progra~1\PostgreSQL\%PG_VER%\postgis*

    if not exist "C:\Progra~1\PostgreSQL\%PG_VER%\%PGIS_WILD_FILE%" (
        echo something went wrong on PostGIS %PGIS_VERSION% installation
        if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
        if defined LOCAL_DEBUG dir C:\Progra~1\PostgreSQL\%PG_VER%\postgis*
    ) else (
        echo **** PostGIS %PGIS_VERSION% %arch% installed
    )
) else (
    echo PostGIS %PGIS_VERSION% %arch% already installed
)
echo ====================================



:: =========================================================
:: Download and install Boost
::

dir C:\Libraries


set BOOST_INCLUDE_DIR=C:\Libraries\boost_1_69_0


endlocal & (
    set BOOST_INCLUDE_DIR=%BOOST_INCLUDE_DIR%
    set CMAKE_GENERATOR=%CMAKE_GENERATOR%
)

goto :eof
