@echo off

::set LOCAL_DEBUG=1

Setlocal EnableDelayedExpansion EnableExtensions

if defined LOCAL_DEBUG echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%
echo platform %platform%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined MSVC_VER set MSVC_VER=12.0
if not defined RUNTIME set RUNTIME=msvc%MSVC_VER:.=%
if not defined MSVC_YEAR set MSVC_YEAR=2013
if not defined BUILD_ROOT_DIR set BUILD_ROOT_DIR=c:\build
if not defined DOWNLOADS_DIR set DOWNLOADS_DIR=%APPVEYOR_BUILD_FOLDER%\downloads
if not defined COMMON_INSTALL_DIR set COMMON_INSTALL_DIR=%BUILD_ROOT_DIR%\local\%RUNTIME%\%PLATFORM%

:: for cmake its the min version
if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PGIS_VERSION set PGIS_VERSION=2.3
if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
if not defined CGAL_VERSION set CGAL_VERSION=4.8.1
set PG_VER_NO_DOT=pg%PG_VER:.=%


set CMAKE_GENERATOR=Visual Studio %MSVC_VER:.0=% %MSVC_YEAR%
if "%platform%"=="x64" (
    set CMAKE_GENERATOR=%CMAKE_GENERATOR% Win64
)

:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

:: Determine compiler used to build postgis
if "%arch%"=="64" (set GCC=48) else (set GCC=481)

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

set PGIS_WILD_FILE=postgis-%PG_VER_NO_DOT%-binaries-%PGIS_VERSION*%w%arch%gcc%GCC%.zip
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

:: set BOOST_LOCAL_DEBUG=1
call ci/appveyor/install-boost.bat

::
:: CGAL & GMP & MPFR
::
::
set CGAL_LOCAL_DEBUG=1
call ci/appveyor/install-CGAL.bat


echo ======================================================
echo Installation of Prerequisites done.
echo Environment variables set:

echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
echo GMP_LIBRARIES %GMP_LIBRARIES%
echo MPFR_LIBRARIES %MPFR_LIBRARIES%
echo CGAL_LIBRARIES %CGAL_LIBRARIES%
echo CGAL_INCLUDE_DIR %CGAL_INCLUDE_DIR%
echo GMP_INCLUDE_DIR %GMP_INCLUDE_DIR%
echo CMAKE_GENERATOR %CMAKE_GENERATOR%

echo ======================================================
echo.

endlocal & (
    set BOOST_THREAD_LIB=%BOOST_THREAD_LIB%
    set BOOST_SYSTEM_LIB=%BOOST_SYSTEM_LIB%
    set BOOST_INCLUDE_DIR=%BOOST_INCLUDE_DIR%
    set GMP_LIBRARIES=%GMP_LIBRARIES%
    set MPFR_LIBRARIES=%MPFR_LIBRARIES%
    set CGAL_LIBRARIES=%CGAL_LIBRARIES%
    set CGAL_INCLUDE_DIR=%CGAL_INCLUDE_DIR%
    set GMP_INCLUDE_DIR=%GMP_INCLUDE_DIR%
    set CMAKE_GENERATOR=%CMAKE_GENERATOR%
)

goto :eof
