@echo off

:: each sub-script starts and end on this directory:
cd c:\build

Setlocal EnableDelayedExpansion EnableExtensions

echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%
cd %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PG_VERSION set PG_VERSION=2.2.2

if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
:: replace dots with underscores
set BOOST_VER_USC=%BOOST_VERSION:.=_%


:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

::
:: =========================================================

:: create a download directory:
cd %APPVEYOR_BUILD_FOLDER%
mkdir downloads 2>NUL


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
    curl -L -O -S -s --output downloads\cmake-%CMAKE_VERSION%-win32-x86.msi https://cmake.org/files/v3.5/cmake-%CMAKE_VERSION%-win32-%plataform%.msi
    echo Installing cmake %CMAKE_VERSION%
    start /wait msiexec /i downloads\cmake-%CMAKE_VERSION%-win32-%plataform%.msi /qn

    for /f "tokens=*  delims=" %%a in ('cmake --version') do (
        set CURR_CMAKE=%%a& goto _ExitForLoop2
    )
    :_ExitForLoop2
    set CURR_CMAKE=%CURR_CMAKE:~14%
    if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
        echo something went wrong on cmake installation download !!!!!!!!!
        echo "cmake %CMAKE_VERSION% already installed
    )
)

::
:: =========================================================

cd %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Download and install Postgis
::

if not exist "C:\Progra~1\PostgreSQL\9.4\postgis-pg94-binaries-2.2.2w64gcc48" (
    echo Downoading postGIS
    curl -L -O -S -s --output downloads\postgis-pg94-binaries-2.2.2w64gcc48-zip http://winnie.postgis.net/download/windows/pg94/buildbot/postgis-pg94-binaries-2.2.2w64gcc48.zip
    echo Extracting postGIS
    7z x -oc:\build\ downloads\postgis-pg94-binaries-2.2.2w64gcc48-zip
    echo Installing postGIS
    copy /e /y /q c:\build\postgis-pg94-binaries-2.2.2w64gcc48 C:\Progra~1\PostgreSQL\9.4
) else (
    echo postGIS %PG_VERSION% already installed
)

:: =========================================================
:: Download and install Boost
::


if not exist "c:\build\boost_%BOOST_VER_USC%" (

    if not exist downloads\boost_%BOOST_VER_USC%-zip (
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s --output downloads\boost_%BOOST_VER_USC%-zip http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        echo Done downloading Boost.
        if not exist downloads\boost_%BOOST_VER_USC%-zip (
            echo something went wrong on boost download !!!!!!!!!
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x -oc:\build\ Boost_%BOOST_VER_USC%.zip
    echo Done extractig.
    if not exist "c\build\boost_%BOOST_VER_USC%" (
        echo something went wrong on boos extraction!!!!!!!!!
    )
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
)

if not exist "C:\local\boost_%BOOST_VER_USC%\lib%arch%-msvc-14.0" (
    :: download installer??
    if not exist downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
        echo Downloading Boost %BOOST_VERSION% %arch% bits...
        curl --silent --fail --location --max-time 1600 --connect-timeout 30 --output downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe http://sourceforge.net/projects/boost/files/boost-binaries/%BOOST_VERSION%/boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe/download
        echo Done downloading.
    ) else (
        echo Boost %BOOST_VERSION% %arch% bits already downloaded
    )

    echo Installing Boost %BOOST_VERSION% %arch% bits...
    downloads\boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe /silent /verysilent /sp- /suppressmsgboxes
    echo Done installing.
) else (
    echo Boost %BOOST_VERSION% %arch% bits already installed
)
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
