@echo off

:: each sub-script starts and end on this directory:
cd c:\build

Setlocal EnableDelayedExpansion EnableExtensions

echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

::
:: =========================================================

:: create a download directory:
mkdir build\downloads 2>NUL

::# Boost 1.58.0
::#- curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.zip
::#- 7z x boost_1_58_0.zip




:: =========================================================
:: Download and install Boost
::


if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
:: replace dots with underscores
set BOOST_VER_USC=%BOOST_VERSION:.=_%

    


if not exist "c:\build\boost_%BOOST_VER_USC%" (
    cd c:\build\downloads

    if not exist "c:\build\downloads\boost_%BOOST_VER_USC%-zip" (
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        echo Done downloading Boost.
        if not exist "c:\build\downloads\boost_%BOOST_VER_USC%-zip" (
            echo something went wrong on boost download !!!!!!!!!
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x -oc:\build\ Boost_%BOOST_VER_USC%.zip
    echo Done extractig.
    if not exist "c:\build\boost_%BOOST_VER_USC%" (
        echo something went wrong!!!!!!!!!
    )
    cd ..
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
)

if not exist "C:\local\boost_%BOOST_VER_USC%\lib%arch%-msvc-14.0" (
    :: download installer??
    if not exist build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
        echo Downloading Boost %BOOST_VERSION% %arch% bits...
        curl --silent --fail --location --max-time 1600 --connect-timeout 30 --output build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe http://sourceforge.net/projects/boost/files/boost-binaries/%BOOST_VERSION%/boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe/download
        echo Done downloading.
    ) else (
        echo Boost %BOOST_VERSION% %arch% bits already downloaded
    )
    echo Installing Boost %BOOST_VERSION% %arch% bits...
    build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe /silent /verysilent /sp- /suppressmsgboxes
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
