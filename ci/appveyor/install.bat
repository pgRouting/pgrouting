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
:: replace dots with underscores
set CMAKE_VER_USC=%CMAKE_VERSION:.=_%

if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
:: replace dots with underscores
set BOOST_VER_USC=%BOOST_VERSION:.=_%


:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

::
:: =========================================================

:: create a download directory:
mkdir build\downloads 2>NUL

:: CMake 3.5.2 (upgrade) workaround

cmake --version
echo 1-------------

for /f "delims=" %%a in ('cmake --version') do @set CURR_CMAKE=%%a
echo CURR_CMAKE %CURR_CMAKE%
echo 2-------------
set CURR_CMAKE=%CURR_CMAKE:~13%
echo CURR_CMAKE %CURR_CMAKE%
echo 3-------------
echo CMAKE_VERSION %CMAKE_VERSION%
echo 4-------------

if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
    echo cmake %CMAKE_VERSION% already installed
) else (
    
    echo Downoading cmake %CMAKE_VERSION%
    curl -L -O -S -s --output build\downloads\cmake-%CMAKE_VERSION%-win32-x86.msi https://cmake.org/files/v3.5/cmake-%CMAKE_VERSION%-win32-%plataform%.msi
    echo Installing cmake %CMAKE_VERSION%
    start /wait msiexec /i build\downloads\cmake-%CMAKE_VERSION%-win32-%plataform%.msi /qn


    cmake --version > temp.txt
    set /p CURR_CMAKE=<temp.txt
    echo new CURR_CMAKE %CURR_CMAKE%
    if %CURR_CMAKE% == "cmake version %CMAKE_VERSION%" (
        echo something went wrong on cmake installation download !!!!!!!!!
        echo "cmake %CMAKE_VERSION% already installed
    )
)

:: PostGIS 2.2.2
echo Downoading postGIS
curl -L -O -S -s --output build\downloads\postgis-pg94-binaries-2.2.2w64gcc48.zip http://winnie.postgis.net/download/windows/pg94/buildbot/postgis-pg94-binaries-2.2.2w64gcc48.zip
echo Extracting postGIS
7z x -oc:\build\ build\downloads\postgis-pg94-binaries-2.2.2w64gcc48.zip
echo Installing postGIS
xcopy /e /y /q postgis-pg94-binaries-2.2.2w64gcc48 C:\Progra~1\PostgreSQL\9.4


:: =========================================================
:: Download and install Boost
::



    


if not exist "build\boost_%BOOST_VER_USC%" (

    if not exist build\downloads\boost_%BOOST_VER_USC%-zip (
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s --output build\downloads\boost_%BOOST_VER_USC%-zip http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        echo Done downloading Boost.
        if not exist build\downloads\boost_%BOOST_VER_USC%-zip (
            echo something went wrong on boost download !!!!!!!!!
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x -oc:\build\ Boost_%BOOST_VER_USC%.zip
    echo Done extractig.
    if not exist "build\boost_%BOOST_VER_USC%" (
        echo something went wrong on boos extraction!!!!!!!!!
    )
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
)

if not exist "C:\local\boost_%BOOST_VER_USC%\lib%arch%-msvc-14.0" (
    :: download installer??
    if not exist build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
        echo Downloading Boost %BOOST_VERSION% %arch% bits...
        curl --silent --fail --location --max-time 1600 --connect-timeout 30 --output build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe http://sourceforge.net/projects/boost/files/boost-binaries/%BOOST_VERSION%/boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe/download
        echo Done downloading.
    ) else (
        echo Boost %BOOST_VERSION% %arch% bits already downloaded
    )

    echo Installing Boost %BOOST_VERSION% %arch% bits...
    build\downloads\boost_%BOOST_VER_USC%-msvc-13.0-%arch%.exe /silent /verysilent /sp- /suppressmsgboxes
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
