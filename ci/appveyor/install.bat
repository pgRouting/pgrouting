@echo off
Setlocal EnableDelayedExpansion EnableExtensions

echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

if not defined APPVEYOR_BUILD_FOLDER set APPVEYOR_BUILD_FOLDER=%CD%

cd %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined SEVENZIP set SEVENZIP=7z


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

cd C:\build

if not exist "C:\build\boost_%BOOST_VER_USC%" (

    if not exist build\downloads\boost_%BOOST_VER_USC%-zip (
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VER_USC%/boost_%BOOST_VER_USC%.zip
        echo Done downloading Boost.
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x boost_1_58_0.zip
    echo Done extractig.
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
)

::
:: =========================================================

echo.
echo ======================================================
echo Installation of Boost done.
echo Platform - %platform%
echo ======================================================
echo.

endlocal & set PATH=%PATH%&

goto :eof
