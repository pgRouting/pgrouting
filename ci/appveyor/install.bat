@echo off

:: each sub-script starts and end on this directory:

Setlocal EnableDelayedExpansion EnableExtensions

echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

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
    cd downloads
    echo Downoading cmake %CMAKE_VERSION%
    curl -L -O -S -s https://cmake.org/files/v3.5/cmake-%CMAKE_VERSION%-win32-%plataform%.msi
    cd ..

    echo Installing cmake %CMAKE_VERSION%
    start /wait msiexec /i downloads\cmake-%CMAKE_VERSION%-win32-%plataform%.msi /qn

    for /f "tokens=*  delims=" %%a in ('cmake --version') do (
        set CURR_CMAKE=%%a& goto _ExitForLoop2
    )
    :_ExitForLoop2
    set CURR_CMAKE=%CURR_CMAKE:~14%
    if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
        echo cmake %CMAKE_VERSION% installed
    ) else (
        echo something went wrong on cmake installation download !!!!!!!!!
    )
)

::
:: =========================================================

cd %APPVEYOR_BUILD_FOLDER%


:: =========================================================
:: Download and install Postgis
::

if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
    if not exist downloads\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
        cd downloads
        echo Downoading postGIS %PG_VERSION%
        curl -L -O -S -s http://winnie.postgis.net/download/windows/pg94/buildbot/postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
        cd ..
        if not exist downloads\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip (
            echo something went wrong on postgis %PG_VERSION% download !!!!!!!!!
        )
    )
    echo Extracting postGIS %PG_VERSION%
    7z x -oc:\build\ downloads\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48.zip
    echo Installing postGIS %PG_VERSION%
    xcopy /e /y /q c:\build\postgis-pg94-binaries-%PG_VERSION%w%arch%gcc48 C:\Progra~1\PostgreSQL\9.4

    if not exist "C:\Progra~1\PostgreSQL\9.4\makepostgisdb_using_extensions.bat" (
        echo something went wrong on postGIS %PG_VERSION% installation !!!!!!!!!
        dir downloads
        dir C:\Progra~1\PostgreSQL\9.4\
    ) else (
        echo postGIS %PG_VERSION% %arch% installed
    )
) else (
    echo postGIS %PG_VERSION% %arch% already installed
)

::
:: =========================================================


cd %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Download and install Boost
::

if not exist "c:\build\boost_%BOOST_VER_USC%" (

    if not exist downloads\boost_%BOOST_VER_USC%.zip (
        cd downloads
        dir
        echo Downloading Boost %BOOST_VERSION% ...
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        cd ..
        dir downloads
        if not exist downloads\boost_%BOOST_VER_USC%.zip (
            echo something went wrong on boost %BOOST_VERSION% download !!!!!!!!!
        )
    ) else (
        echo Boost_%BOOST_VER_USC%  already downloaded
    )

    echo Extracting Boost_%BOOST_VERSION%.zip ...
    7z x -oc:\build\ downloads/Boost_%BOOST_VER_USC%.zip
    echo Done extractig.
    if not exist "c:\build\boost_%BOOST_VER_USC%" (
        echo something went wrong on boos extraction!!!!!!!!!
    )
) else (
    echo Boost_%BOOST_VER_USC% Already Extracted
)

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
