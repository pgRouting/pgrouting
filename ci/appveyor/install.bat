@echo off

set LOCAL_DEBUG=1

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

if not defined CMAKE_VERSION set CMAKE_VERSION=3.5.2
if not defined PG_VERSION set PG_VERSION=2.2.2
if not defined BOOST_VERSION set BOOST_VERSION=1.58.0
if not defined CGAL_VERSION set CGAL_VERSION=4.8.1


set CMAKE_GENERATOR=Visual Studio %MSVC_VER:.0=% %MSVC_YEAR%
if "%platform%"=="x64" (
    set CMAKE_GENERATOR=%CMAKE_GENERATOR% Win64
)

:: Determine if arch is 32/64 bits
if /I "%platform%"=="x86" ( set arch=32) else ( set arch=64)

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

if "%CURR_CMAKE%" == "%CMAKE_VERSION%" (
    echo cmake %CMAKE_VERSION% already installed
) else (
    echo Downoading cmake %CMAKE_VERSION%
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
            if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
            Exit \B 1
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
        if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
        if defined LOCAL_DEBUG dir C:\Progra~1\PostgreSQL\9.4\
        Exit \B 1
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

set "BOOST_LOCAL_DEBUG=1"
call ci/appveyor/install-boost.bat
:: deducing variables
:: set BOOST_VER_USC=%BOOST_VERSION:.=_%
:: set BOOST_SHORT_VER=%BOOST_VER_USC:_0=%


:: set BOOST_INSTALL_DIR=%COMMON_INSTALL_DIR%
:: set BOOST_INCLUDE_DIR=%BOOST_INSTALL_DIR%\include\boost-%BOOST_SHORT_VER%
:: set BOOST_LIBRARY_DIR=%BOOST_INSTALL_DIR%\lib
:: set BOOST_THREAD_LIB=%BOOST_INSTALL_DIR%\lib\libboost_thread-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
:: set BOOST_SYSTEM_LIB=%BOOST_INSTALL_DIR%\lib\libboost_system-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
:: set BOOST_ADDRESS_MODEL=%arch%
:: set BOOST_TOOLSET=msvc-%MSVC_VER%
:: set BOOST_SRC_DIR=%BUILD_ROOT_DIR%\boost_%BOOST_VER_USC%
:: set MSBUILD_CONFIGURATION=%CONFIGURATION%

:: DEBUGING
if defined LOCAL_DEBUG (
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
    echo CMAKE_GENERATOR %CMAKE_GENERATOR%
)

:: check that everything needed from boost is there
:: set BOOST_INSTALL_FLAG=10
:: if not exist %BOOST_INCLUDE_DIR%\ ( set BOOST_INSTALL_FLAG=1 )
:: if not exist %BOOST_LIBRARY_DIR%\ ( set BOOST_INSTALL_FLAG=2 )
:: if not exist %BOOST_THREAD_LIB% ( set BOOST_INSTALL_FLAG=3 )
:: if not exist %BOOST_SYSTEM_LIB% ( set BOOST_INSTALL_FLAG=4 )
:: 
:: DEBUGING
:: echo BOOST_INSTALL_FLAG %BOOST_INSTALL_FLAG%

::echo ==================================== BOOST
::if not "%BOOST_INSTALL_FLAG%"=="10" (

    :: download if needed
    ::if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
        ::echo ***** Downloading Boost %BOOST_VERSION% ...
        ::pushd %DOWNLOADS_DIR%
        ::curl -L -O -S -s http://downloads.sourceforge.net/project/boost/boost/%BOOST_VERSION%/boost_%BOOST_VER_USC%.zip
        ::popd
        ::if not exist %DOWNLOADS_DIR%\boost_%BOOST_VER_USC%.zip (
            ::echo something went wrong on boost %BOOST_VERSION% download !!!!!!!!!
            ::if defined LOCAL_DEBUG dir %DOWNLOADS_DIR%
            ::Exit \B 1
        ::)
    ::) else (
        ::echo **** Boost_%BOOST_VER_USC%  already downloaded
    ::)
::
    ::echo **** Extracting Boost_%BOOST_VERSION%.zip ...
    ::pushd %DOWNLOADS_DIR%
    ::7z x -o%BUILD_ROOT_DIR%\ boost_%BOOST_VER_USC%.zip
    ::popd
    ::if not exist %BOOST_SRC_DIR% (
        ::echo something went wrong on boost extraction!!!!!!!!!
        ::if defined LOCAL_DEBUG dir %BOOST_SRC_DIR%
        ::Exit \B 1
    ::)
::
    ::echo **** Excuting bootstrap.bat...
    ::if not exist "%BOOST_SRC_DIR%\b2.exe" (
        ::pushd %BOOST_SRC_DIR%
        ::call "bootstrap.bat"
        ::popd
        ::if not exist "%BOOST_SRC_DIR%\b2.exe" (
            ::echo something went wrong on booststrap.bat execution!!!!!!!!!
            ::if defined LOCAL_DEBUG dir %BOOST_SRC_DIR%
            ::Exit \B 1
        ::)
    ::)
::
    ::echo **** Excuting  %BOOST_SRC_DIR%\b2.exe ...
    ::pushd %BOOST_SRC_DIR%
    ::if defined LOCAL_DEBUG @echo on
    ::b2 toolset=%BOOST_TOOLSET% variant=release link=static threading=multi address-model=%BOOST_ADDRESS_MODEL% ^
        ::--with-thread --with-system --prefix=%BOOST_INSTALL_DIR% -d0 install
    ::if defined LOCAL_DEBUG @echo off
    ::popd
::) else (
    ::echo Boost_%BOOST_VERSION% already installed
::)
::echo ====================================

:: =========================================================
:: =========================================================
:: =========================================================


echo ==================================== CGAL

if exist dir C:/build/local/msvc120/x64/include/CGAL (
    echo CGAL already installed
    goto _ExitCGAL
)

if not defined GMP_SRC_DIR set GMP_SRC_DIR=%BUILD_ROOT_DIR%\gmp\%PLATFORM%
pushd %BUILD_ROOT_DIR%
mkdir gmp\%PLATFORM% 2>NUL
popd
if defined LOCAL_DEBUG (
    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%
)


echo ----------------------------------- GMP -- CGAL prerequisite
if not exist %GMP_SRC_DIR%\gmp.COPYING (
    if not exist %DOWNLOADS_DIR%\gmp-all-CGAL-3.9.zip (
        echo Downoading gmp-all-CGAL-3.9.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/%PLATFORM%/GMP/5.0.1/gmp-all-CGAL-3.9.zip
        if not exist %DOWNLOADS_DIR%\gmp-all-CGAL-3.9.zip (
            echo Something went wrong Downoading CGAL-%CGAL_VERSION%.zip
        )
        popd
    )

    echo Extracting gmp-all-CGAL-3.9.zip
    pushd %DOWNLOADS_DIR%
    7z x -o%GMP_SRC_DIR% gmp-all-CGAL-3.9.zip
    popd
) else (
    echo GMP already installed at %GMP_SRC_DIR%
)
if defined LOCAL_DEBUG (
    dir %GMP_SRC_DIR%
)
echo -----------------------------------

echo ----------------------------------- MPFR -- CGAL prerequisite
if not exist %GMP_SRC_DIR%\mpfr.COPYING (
    if not exist %DOWNLOADS_DIR%\mpfr-all-CGAL-3.9.zip (
        echo Downoading mpfr-all-CGAL-3.9.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/%PLATFORM%/MPFR/3.0.0/mpfr-all-CGAL-3.9.zip
        if not exist %DOWNLOADS_DIR%\mpfr-all-CGAL-3.9.zip (
            echo Something went wrong Downoading CGAL-%CGAL_VERSION%.zip
        )
        popd
    )
    echo Extracting mpfr-all-CGAL-3.9.zip
    pushd %DOWNLOADS_DIR%
    7z x -o%GMP_SRC_DIR% mpfr-all-CGAL-3.9.zip
    popd
) else (
    echo GMP already installed at %GMP_SRC_DIR%
)
if defined LOCAL_DEBUG (
    dir %GMP_SRC_DIR%
)
echo -----------------------------------

if not exist %BUILD_ROOT_DIR%\CGAL-%CGAL_VERSION%\ (
    if not exist %DOWNLOADS_DIR%\CGAL-%CGAL_VERSION%.zip (
        echo Downoading CGAL-%CGAL_VERSION%.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://github.com/CGAL/cgal/releases/download/releases/CGAL-%CGAL_VERSION%/CGAL-%CGAL_VERSION%.zip
        if not exist %DOWNLOADS_DIR%\CGAL-%CGAL_VERSION%.zip (
            echo Something went wrong Downoading CGAL-%CGAL_VERSION%.zip
        )
        popd
    )
    echo Extracting CGAL-%CGAL_VERSION%.zip
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR% CGAL-4.8.1.zip
    popd
)

set CGAL_SRC_DIR=%BUILD_ROOT_DIR%\CGAL-%CGAL_VERSION%
set CGAL_BUILD_DIR=%CGAL_SRC_DIR%\build\%RUNTIME%\%PLATFORM%
set GMP_LIB_NAME=libgmp-10.lib
set MPFR_LIB_NAME=libmpfr-4.lib

if defined LOCAL_DEBUG (
    echo CGAL_SRC_DIR %CGAL_SRC_DIR%
    dir %CGAL_SRC_DIR%

    echo CGAL_BUILD_DIR %CGAL_BUILD_DIR%
)


mkdir %CGAL_BUILD_DIR% %2>null
pushd %CGAL_BUILD_DIR%

@echo on
cmake -G "%CMAKE_GENERATOR%" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=%COMMON_INSTALL_DIR% ^
    -DBoost_USE_MULTITHREADED=ON ^
    -DCGAL_Boost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=OFF ^
    -DBoost_INCLUDE_DIR:PATH=%BOOST_INCLUDE_DIR% ^
    -DBOOST_LIBRARYDIR=%BOOST_LIBRARY_DIR% ^
    -DGMP_INCLUDE_DIR=%GMP_SRC_DIR%\include ^
    -DMPFR_INCLUDE_DIR=%GMP_SRC_DIR%\include ^
    -DGMP_LIBRARIES=%GMP_SRC_DIR%\lib\%GMP_LIB_NAME% ^
    -DMPFR_LIBRARIES=%GMP_SRC_DIR%\lib\%MPFR_LIB_NAME%  ..\..\..\
echo *******************                              calling msbuild CGAL.sln
msbuild CGAL.sln /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
set PROJ_EXT=vcxproj
echo PROJ_EXT %PROJ_EXT%
echo *******************                              calling msbuild INSTALL.%PROJ_EXT%
msbuild INSTALL.%PROJ_EXT% /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
@echo off
popd

dir %COMMON_INSTALL_DIR%
dir %CGAL_BUILD_DIR%

dir C:/build/local/msvc120/x64/share/doc/CGAL-4.8.1
dir C:/build/local/msvc120/x64/include/CGAL


if defined LOCAL_DEBUG (
    echo DOWNLOADS_DIR %DOWNLOADS_DIR%
    dir %DOWNLOADS%

    echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
    dir %BUILD_ROOT_DIR%

    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%
)
popd
:_ExitCGAL
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
