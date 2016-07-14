@echo off
Setlocal EnableDelayedExpansion EnableExtensions

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

:: first create some necessary directories:
mkdir build\downloads 2>NUL



:: =========================================================
:: Download and install Boost
::

cd %APPVEYOR_BUILD_FOLDER%
if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
:: replace dots with underscores
set BOOST_VER_USC=%BOOST_VERSION:.=_%

if not exist build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe (
    echo Downloading Boost %BOOST_VERSION% %arch% bits...
    curl --silent --fail --location --max-time 1600 --connect-timeout 30 --output build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe http://sourceforge.net/projects/boost/files/boost-binaries/%BOOST_VERSION%/boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe/download
    echo Done downloading.
) else (
    echo Boost %BOOST_VERSION% %arch% bits already downloaded
)

if not exist "C:\local\boost_%BOOST_VER_USC%\lib%arch%-msvc-14.0" (
    echo Installing Boost %BOOST_VERSION% %arch% bits...
    build\downloads\boost_%BOOST_VER_USC%-msvc-14.0-%arch%.exe /silent /verysilent /sp- /suppressmsgboxes
    echo Done installing.
) else (
    echo Boost %BOOST_VERSION% %arch% bits already installed
)

::
:: =========================================================

echo.
echo ======================================================
echo Installation of Lua %LUA_VER%, OpenSSL and Boost done.
echo Platform - %platform%
echo ======================================================
echo.

endlocal & set PATH=%PATH%&

goto :eof
