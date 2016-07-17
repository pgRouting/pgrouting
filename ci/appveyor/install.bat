@echo off

set LOCAL_DEBUG=1

Setlocal EnableDelayedExpansion EnableExtensions

if defined LOCAL_DEBUG echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined MSVC_VER set MSVC_VER=12.0
if not defined BUILD_ROOT_DIR set BUILD_ROOT_DIR=c:\build
if not defined DOWNLOADS_DIR set DOWNLOADS_DIR=%APPVEYOR_BUILD_FOLDER%\downloads
if not defined COMMON_INSTALL_ROOT_DIR set COMMON_INSTALL_ROOT_DIR=%BUILD_ROOT_DIR%\local

if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PG_VERSION set PG_VERSION=2.2.2
if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
if not defined CGAL_VERSION set CGAL_VERSION=4.8.1



:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

::
:: =========================================================

:: create a download directory:
pushd %APPVEYOR_BUILD_FOLDER%
mkdir downloads 2>NUL
popd
if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%


:: =========================================================
:: Download and install cmake
::

echo ==================================== CMAKE
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
echo ====================================

::
:: =========================================================



:: =========================================================
:: Download and install Postgis
::

echo ==================================== POSTGIS
if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
    cd %APPVEYOR_BUILD_FOLDER%
    if not exist %DOWNLOADS_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
        echo Downoading postGIS %PG_VERSION%
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://winnie.postgis.net/download/windows/pg94/buildbot/postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
        popd
        if not exist %DOWNLOADS_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
            echo something went wrong on postgis %PG_VERSION% download !!!!!!!!!
            dir %DOWNLOADS_DIR%
        )
    )

    echo Extracting postGIS %PG_VERSION%
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR%\ postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
    popd

    echo **** Installing postGIS %PG_VERSION%
    xcopy /e /y /q %BUILD_ROOT_DIR%\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48 C:\Progra~1\PostgreSQL\9.4

    if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
        echo something went wrong on postGIS %PG_VERSION% installation !!!!!!!!!
        dir %DOWNLOADS_DIR%
        dir C:\Progra~1\PostgreSQL\9.4\
    ) else (
        echo **** postGIS %PG_VERSION% %arch% installed
    )
) else (
    echo postGIS %PG_VERSION% %arch% already installed
)
echo ====================================

::
:: =========================================================



:: =========================================================
:: Download and install Boost
::

:: deducing variables
set BOOST_VER_USC=%BOOST_VERSION:.=_%
set BOOST_SHORT_VER=%BOOST_VER_USC:_0=%

set RUNTIME=msvc%MSVC_VER:.=%
set COMMON_INSTALL_DIR=%COMMON_INSTALL_ROOT_DIR%\%RUNTIME%\%PLATFORM%

set BOOST_INSTALL_DIR=%COMMON_INSTALL_DIR%
set BOOST_INCLUDE_DIR=%BOOST_INSTALL_DIR%\include\boost-%BOOST_SHORT_VER%
set BOOST_LIBRARY_DIR=%BOOST_INSTALL_DIR%\lib
set BOOST_THREAD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_thread-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_SYSTEM_LIB=%BOOST_INSTALL_DIR%\lib\libboost_system-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_WILDCARD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_*-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.libs
set BOOST_ADDRESS_MODEL=%arch%
set BOOST_TOOLSET=msvc-%MSVC_VER%
set BOOST_SRC_DIR=%BUILD_ROOT_DIR%\boost_%BOOST_VER_USC%
set MSBUILD_CONFIGURATION=%CONFIGURATION%
set CMAKE_GENERATOR=Visual Studio %MSVC_VER:.0=% %MSVC_YEAR%
if "%plataform%"=="x64" (
    set CMAKE_GENERATOR=%CMAKE_GENERATOR% Win64
)

:: DEBUGING
if defined LOCAL_DEBUG echo BOOST_VERSION %BOOST_VERSION%
if defined LOCAL_DEBUG echo BOOST_VER_USC %BOOST_VER_USC%
if defined LOCAL_DEBUG echo BOOST_SHORT_VER %BOOST_SHORT_VER%
if defined LOCAL_DEBUG echo BOOST_INSTALL_DIR %BOOST_INSTALL_DIR%
if defined LOCAL_DEBUG echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
if defined LOCAL_DEBUG echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
if defined LOCAL_DEBUG echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
if defined LOCAL_DEBUG echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
if defined LOCAL_DEBUG echo BOOST_WILDCARD_LIB %BOOST_WILDCARD_LIB%
if defined LOCAL_DEBUG echo BOOST_ADDRESS_MODEL %BOOST_ADDRESS_MODEL%
if defined LOCAL_DEBUG echo BOOST_TOOLSET %BOOST_TOOLSET%
if defined LOCAL_DEBUG echo CMAKE_GENERATOR %CMAKE_GENERATOR%

:: check that everything needed from boost is there
if not exist "%BOOST_INCLUDE_DIR%\" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_LIBRARY_DIR%\" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_THREAD_LIB%" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_SYSTEM_LIB%" ( set BOOST_INSTALL_FLAG=1 )
if not exist "%BOOST_WILDCARD_LIB%" ( set BOOST_INSTALL_FLAG=1 )

:: DEBUGING
echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%

echo ==================================== BOOST
if %BOOST_INSTALL_FLAG% EQU 1 (

    :: check if it needs to be downloaded
    if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
        echo Downloading Boost %BOOST_VERSION% ...
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        popd
        if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
            echo something went wrong on boost %BOOST_VERSION% download !!!!!!!!!
            if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
        )
    ) else (
        echo **** Boost_%BOOST_VER_USC%  already downloaded
    )

    echo **** Extracting Boost_%BOOST_VERSION%.zip ...
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR%\ boost_%BOOST_VER_USC%.zip
    popd
    if not exist %BOOST_SRC_DIR% (
        echo something went wrong on boost extraction!!!!!!!!!
        if defined LOCAL_DEBUG dir %BOOST_SRC_DIR%

    )

    echo **** Excuting bootstrap.bat...
    if not exist "%BOOST_SRC_DIR%\b2.exe" (
        pushd %BOOST_SRC_DIR%
        call "bootstrap.bat"
        popd
        if not exist "%BOOST_SRC_DIR%\b2.exe" (
            echo something went wrong on booststrap.bat execution!!!!!!!!!
            if defined LOCAL_DEBUG dir %BOOST_SRC_DIR%
        )
    )

    echo **** Excuting  %BOOST_SRC_DIR%\b2.exe ...
    if not exist %BOOST_INCLUDE_DIR%\ (
        pushd %BOOST_SRC_DIR%
        if defined LOCAL_DEBUG @echo on
        b2 toolset=%BOOST_TOOLSET% variant=release link=static threading=multi address-model=%BOOST_ADDRESS_MODEL% ^
            --with-thread --with-system --prefix=%BOOST_INSTALL_DIR% -d0 install
        if defined LOCAL_DEBUG @echo off
        popd

        set BOOST_INSTALL_FLAG=0
        echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%
        if not exist %BOOST_INCLUDE_DIR%\ ( set BOOST_INSTALL_FLAG=1 )
        echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%
        if not exist %BOOST_LIBRARY_DIR%\ ( set BOOST_INSTALL_FLAG=2 )
        if not exist %BOOST_THREAD_LIB% ( set BOOST_INSTALL_FLAG=3 )
        if not exist %BOOST_SYSTEM_LIB% ( set BOOST_INSTALL_FLAG=4 )
        if not exist %BOOST_WILDCARD_LIB% ( set BOOST_INSTALL_FLAG=5 )

        if %BOOST_INSTALL_FLAG% NEQ 0 (
            echo something went wrong on %BOOST_SRC_DIR%\b2.exe execution!!!!!!!!!

            if defined LOCAL_DEBUG (
                echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
                dir %BOOST_INCLUDE_DIR%

                echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
                dir %BOOST_LIBRARY_DIR%
                echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
                echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
            )
        )
    ) else (
        echo Boost_%BOOST_VERSION% already installed
    )
) else (
    echo Boost_%BOOST_VERSION% already installed
    echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%
)
echo ====================================

::
:: =========================================================


echo ==================================== CGAL

pushd %DOWNLOADS_DIR%
curl -L -O -S -s http://github.com/CGAL/cgal/releases/download/releases/CGAL-%CGAL_VERSION%/CGAL-%CGAL_VERSION%.zip
curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/x64/GMP/5.0.1/gmp-all-CGAL-3.9.zip
curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/x64/MPFR/3.0.0/mpfr-all-CGAL-3.9.zip

if not defined GMP_SRC_DIR set GMP_SRC_DIR=%BUILD_ROOT_DIR%\gmp\%plataform%
mkdir %GMP_SRC_DIR%  2>NUL
if defined LOCAL_DEBUG (
    echo platafrom %platafrom%
    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%
)

7z x -o%BUILD_ROOT_DIR% CGAL-4.8.1.zip
7z x -o%GMP_SRC_DIR% gmp-all-CGAL-3.9.zip
7z x -o%GMP_SRC_DIR% mpfr-all-CGAL-3.9.zip


if defined LOCAL_DEBUG (
    echo DOWNLOADS_DIR %DOWNLOADS_DIR%
    dir %DOWNLOADS%

    echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
    dir %BUILD_ROOT_DIR%

    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%
)
popd
echo ====================================



echo.
echo ======================================================
echo Installation of Prerequisites done.
echo Platform - %platform%
echo    cmake - %CMAKE_VERSION% 
echo    Boost - %BOOST_VERSION%
echo  postGIS - %PG_VERSION% %arch% 
echo     CGAL - %CGAL_VERSION% %arch% 
echo ======================================================
echo.

endlocal & set PATH=%PATH%&

goto :eof
