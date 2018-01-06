@echo off


Setlocal EnableDelayedExpansion EnableExtensions


if not defined BOOST_VERSION goto _error
if not defined COMMON_INSTALL_DIR goto _error
if not defined MSVC_VER goto _error
if not defined ARCH goto _error
if not defined BUILD_ROOT_DIR goto _error
if not defined CONFIGURATION goto _error

if defined BOOST_LOCAL_DEBUG (
    echo "install-boost.bat"
    echo received environment
    echo BOOST_VERSION %BOOST_VERSION%
    echo COMMON_INSTALL_DIR %COMMON_INSTALL_DIR%
    echo MSVC_VER %MSVC_VER%
    echo ARCH %ARCH%
    echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
    echo CONFIGURATION %CONFIGURATION%
)
:: create a download & install directories:
mkdir %APPVEYOR_BUILD_FOLDER%\downloads 2>NUL
mkdir %COMMON_INSTALL_DIR% 2>NUL
if defined BOOST_LOCAL_DEBUG dir %DOWNLOADS_DIR%
if defined BOOST_LOCAL_DEBUG dir %COMMON_INSTALL_DIR%

echo ==================================== BOOST


:: deducing variables
set BOOST_VER_USC=%BOOST_VERSION:.=_%
set BOOST_SHORT_VER=%BOOST_VER_USC:_0=%

set BOOST_INSTALL_DIR=%COMMON_INSTALL_DIR%
set BOOST_INCLUDE_DIR=%BOOST_INSTALL_DIR%\include\boost-%BOOST_SHORT_VER%
set BOOST_LIBRARY_DIR=%BOOST_INSTALL_DIR%\lib
set BOOST_THREAD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_thread-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_SYSTEM_LIB=%BOOST_INSTALL_DIR%\lib\libboost_system-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_ADDRESS_MODEL=%ARCH%
set BOOST_TOOLSET=msvc-%MSVC_VER%
set BOOST_SRC_DIR=%BUILD_ROOT_DIR%\boost_%BOOST_VER_USC%
set MSBUILD_CONFIGURATION=%CONFIGURATION%

if defined BOOST_LOCAL_DEBUG (
    echo BOOST_VERSION %BOOST_VERSION%
    echo BOOST_VER_USC %BOOST_VER_USC%
    echo BOOST_SHORT_VER %BOOST_SHORT_VER%
    echo BOOST_INSTALL_DIR %BOOST_INSTALL_DIR%
    echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
    echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
    echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
    echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
    echo BOOST_ADDRESS_MODEL %BOOST_ADDRESS_MODEL%
    echo BOOST_TOOLSET %BOOST_TOOLSET%
)

:: check that everything needed from boost is there
set BOOST_INSTALL_FLAG=10
if not exist %BOOST_INCLUDE_DIR%\ ( set BOOST_INSTALL_FLAG=1 )
if not exist %BOOST_LIBRARY_DIR%\ ( set BOOST_INSTALL_FLAG=2 )
if not exist %BOOST_THREAD_LIB% ( set BOOST_INSTALL_FLAG=3 )
if not exist %BOOST_SYSTEM_LIB% ( set BOOST_INSTALL_FLAG=4 )


if defined BOOST_LOCAL_DEBUG echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%


if not "%BOOST_INSTALL_FLAG%"=="10" (

    :: download if needed
    if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
        echo ***** Downloading Boost %BOOST_VERSION% ...
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        popd
        if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
            echo something went wrong on boost %BOOST_VERSION% download !!!!!!!!!
            if defined BOOST_LOCAL_DEBUG dir %DOWNLOADS_DIR%
            Exit \B 1
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
        if defined BOOST_LOCAL_DEBUG dir %BOOST_SRC_DIR%
        Exit \B 1
    )

    echo **** Executing bootstrap.bat...
    if not exist "%BOOST_SRC_DIR%\b2.exe" (
        pushd %BOOST_SRC_DIR%
        call "bootstrap.bat"
        popd
        if not exist "%BOOST_SRC_DIR%\b2.exe" (
            echo something went wrong on booststrap.bat execution!!!!!!!!!
            if defined BOOST_LOCAL_DEBUG dir %BOOST_SRC_DIR%
            Exit \B 1
        )
    )

    echo **** Executing  %BOOST_SRC_DIR%\b2.exe ...
    pushd %BOOST_SRC_DIR%
    if defined BOOST_LOCAL_DEBUG @echo on
    b2 toolset=%BOOST_TOOLSET% variant=release link=static threading=multi address-model=%BOOST_ADDRESS_MODEL% ^
        --with-thread --with-system --prefix=%BOOST_INSTALL_DIR% -d0 install
    if defined BOOST_LOCAL_DEBUG @echo off
    popd
) else (
    echo Boost_%BOOST_VERSION% already installed
)

echo ------------------------------
echo Environment variables set:

echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
echo ======================================================


endlocal & (
    set BOOST_INCLUDE_DIR=%BOOST_INCLUDE_DIR%
    set BOOST_LIBRARY_DIR=%BOOST_LIBRARY_DIR%
    set BOOST_THREAD_LIB=%BOOST_THREAD_LIB%
    set BOOST_SYSTEM_LIB=%BOOST_SYSTEM_LIB%
)

goto :eof

:_error
echo an environment variable is missing:
echo --  BOOST_VERSION
echo --  COMMON_INSTALL_DIR
echo --  MSVC_VER
echo --  ARCH
echo --  BUILD_ROOT_DIR
echo --  CONFIGURATION
echo --  BOOST_LOCAL_DEBUG  (if defined will print debug information)

