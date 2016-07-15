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
    cd downloads

    if not exist "c:\build\downloads\boost_%BOOST_VER_USC%-zip" (
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        echo Done downloading Boost.
        if not exist "c:\build\downloads\boost_%BOOST_VER_USC%-zip" (
            echo something went wrong!!!!!!!!!
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x -oc:\build\ Boost_%BOOST_VER_USC%.zip
    echo Done extractig.
    if not exist "C:\build\boost_%BOOST_VER_USC%" (
        echo something went wrong!!!!!!!!!
    )
    cd ..
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
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
