@echo off

Setlocal EnableDelayedExpansion EnableExtensions

if defined CGAL_LOCAL_DEBUG echo APPVEYOR_BUILD_FOLDER %APPVEYOR_BUILD_FOLDER%

:: =========================================================
:: Set some defaults. Infer some variables.
::

if not defined BUILD_ROOT_DIR goto _error
if not defined COMMON_INSTALL_DIR goto _error
if not defined PLATFORM goto _error
if not defined DOWNLOADS_DIR goto _error
if not defined CGAL_VERSION goto _error
if not defined RUNTIME goto _error
if not defined CMAKE_GENERATOR goto _error
if not defined BOOST_INCLUDE_DIR goto _error
if not defined BOOST_LIBRARY_DIR goto _error

if defined CGAL_LOCAL_DEBUG (
    echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
    echo COMMON_INSTALL_DIR %COMMON_INSTALL_DIR%
    echo PLATFORM %PLATFORM%
    echo DOWNLOADS_DIR %DOWNLOADS_DIR%
    echo CGAL_VERSION %CGAL_VERSION%
    echo RUNTIME %RUNTIME%
    echo CMAKE_GENERATOR %CMAKE_GENERATOR%
    echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
    echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
)

set MSBUILD_CONFIGURATION=RelWithDebInfo

::
:: =========================================================

:: create a download directory:
mkdir %DOWNLOADS_DIR% 2>NUL
mkdir %COMMON_INSTALL_DIR% 2>NUL

if defined CGAL_LOCAL_DEBUG dir %DOWNLOADS_DIR%
if defined CGAL_LOCAL_DEBUG dir %COMMON_INSTALL_DIR%


if not defined GMP_SRC_DIR set GMP_SRC_DIR=%BUILD_ROOT_DIR%\gmp\%PLATFORM%
pushd %BUILD_ROOT_DIR%
mkdir gmp\%PLATFORM% 2>NUL
popd
if defined CGAL_LOCAL_DEBUG (
    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%
)


echo ----------------------------------- GMP
if not exist %GMP_SRC_DIR%\gmp.COPYING (
    if not exist %DOWNLOADS_DIR%\gmp-all-CGAL-3.9.zip (
        echo Downloading gmp-all-CGAL-3.9.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/%PLATFORM%/GMP/5.0.1/gmp-all-CGAL-3.9.zip
        if not exist %DOWNLOADS_DIR%\gmp-all-CGAL-3.9.zip (
            echo Something went wrong Downloading gmp-all-CGAL-3.9.zip
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
set GMP_LIBRARIES=%GMP_SRC_DIR%\lib\libgmp-10.lib
set GMP_INCLUDE_DIR=%GMP_SRC_DIR%\include
if defined CGAL_LOCAL_DEBUG (
    echo GMP_LIBRARIES %GMP_LIBRARIES%
    dir %GMP_SRC_DIR%
)
echo -----------------------------------

echo ----------------------------------- MPFR
if not exist %GMP_SRC_DIR%\mpfr.COPYING (
    if not exist %DOWNLOADS_DIR%\mpfr-all-CGAL-3.9.zip (
        echo Downloading mpfr-all-CGAL-3.9.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/%PLATFORM%/MPFR/3.0.0/mpfr-all-CGAL-3.9.zip
        if not exist %DOWNLOADS_DIR%\mpfr-all-CGAL-3.9.zip (
            echo Something went wrong Downloading mpfr-all-CGAL-3.9.zip
        )
        popd
    )
    echo Extracting mpfr-all-CGAL-3.9.zip
    pushd %DOWNLOADS_DIR%
    7z x -o%GMP_SRC_DIR% mpfr-all-CGAL-3.9.zip
    popd
) else (
    echo MPFR already installed at %GMP_SRC_DIR%
)
set MPFR_LIBRARIES=%GMP_SRC_DIR%\lib\libmpfr-4.lib

if defined CGAL_LOCAL_DEBUG (
    dir %GMP_SRC_DIR%
)
echo -----------------------------------

echo ==================================== CGAL

if exist %COMMON_INSTALL_DIR%/include/CGAL (
    echo CGAL already installed
    goto _ExitCGAL
)

if not exist %BUILD_ROOT_DIR%\CGAL-%CGAL_VERSION%\ (
    if not exist %DOWNLOADS_DIR%\CGAL-%CGAL_VERSION%.zip (
        echo Downloading CGAL-%CGAL_VERSION%.zip
        pushd %DOWNLOADS_DIR%
        curl -L -O -S -s http://github.com/CGAL/cgal/releases/download/releases/CGAL-%CGAL_VERSION%/CGAL-%CGAL_VERSION%.zip
        if not exist %DOWNLOADS_DIR%\CGAL-%CGAL_VERSION%.zip (
            echo Something went wrong Downoading CGAL-%CGAL_VERSION%.zip
        )
        popd
    )
    echo Extracting CGAL-%CGAL_VERSION%.zip
    pushd %DOWNLOADS_DIR%
    7z x -o%BUILD_ROOT_DIR% CGAL-%CGAL_VERSION%.zip
    popd
)

set CGAL_SRC_DIR=%BUILD_ROOT_DIR%\CGAL-%CGAL_VERSION%
set CGAL_BUILD_DIR=%CGAL_SRC_DIR%\build\%RUNTIME%\%PLATFORM%

if defined CGAL_LOCAL_DEBUG (
    echo CGAL_SRC_DIR %CGAL_SRC_DIR%
    dir %CGAL_SRC_DIR%

    echo CGAL_BUILD_DIR %CGAL_BUILD_DIR%
)



set PROJ_EXT=vcxproj
echo PROJ_EXT %PROJ_EXT%

mkdir %CGAL_BUILD_DIR% %2>null
pushd %CGAL_BUILD_DIR%
cmake -G "%CMAKE_GENERATOR%" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=%COMMON_INSTALL_DIR% ^
    -DBoost_USE_MULTITHREADED=ON ^
    -DCGAL_Boost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=OFF ^
    -DBoost_INCLUDE_DIR:PATH=%BOOST_INCLUDE_DIR% ^
    -DBOOST_LIBRARYDIR=%BOOST_LIBRARY_DIR% ^
    -DGMP_INCLUDE_DIR=%GMP_SRC_DIR%\include ^
    -DMPFR_INCLUDE_DIR=%GMP_SRC_DIR%\include ^
    -DGMP_LIBRARIES=%GMP_LIBRARIES% ^
    -DMPFR_LIBRARIES=%MPFR_LIBRARIES%  ..\..\..\
msbuild CGAL.sln /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
msbuild INSTALL.%PROJ_EXT% /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
popd

if defined CGAL_LOCAL_DEBUG (
    echo DOWNLOADS_DIR %DOWNLOADS_DIR%
    dir %DOWNLOADS%

    echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
    dir %BUILD_ROOT_DIR%

    echo GMP_SRC_DIR %GMP_SRC_DIR%
    dir %GMP_SRC_DIR%

    dir C:/build/local/msvc120/x64/include/CGAL
)
:_ExitCGAL
echo --------------------------------
set CGAL_LIBRARIES=%COMMON_INSTALL_DIR%\lib\libCGAL-vc%MSVC_VER:.=%-mt-%CGAL_VERSION%.lib&
set CGAL_INCLUDE_DIR=%COMMON_INSTALL_DIR%\include
echo Installation of CGAL done.
echo Environment variables set:

echo GMP_LIBRARIES %GMP_LIBRARIES%
echo MPFR_LIBRARIES %MPFR_LIBRARIES%
echo CGAL_LIBRARIES %CGAL_LIBRARIES%
echo CGAL_INCLUDE_DIR %CGAL_INCLUDE_DIR%
echo GMP_INCLUDE_DIR %GMP_INCLUDE_DIR%

echo ======================================================

endlocal & (
    set GMP_LIBRARIES=%GMP_LIBRARIES%
    set MPFR_LIBRARIES=%MPFR_LIBRARIES%
    set CGAL_LIBRARIES=%CGAL_LIBRARIES%
    set CGAL_INCLUDE_DIR=%CGAL_INCLUDE_DIR%
    set GMP_INCLUDE_DIR=%GMP_INCLUDE_DIR%
)

goto :eof

:_error
echo an environment variable is missing:
echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
echo COMMON_INSTALL_DIR %COMMON_INSTALL_DIR%
echo PLATFORM %PLATFORM%
echo DOWNLOADS_DIR %DOWNLOADS_DIR%
echo CGAL_VERSION %CGAL_VERSION%
echo RUNTIME %RUNTIME%
echo CMAKE_GENERATOR %CMAKE_GENERATOR%
echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
echo BOOST_LIBRARY_DIR %BOOST_LIBRARY_DIR%
echo MSBUILD_CONFIGURATION %MSBUILD_CONFIGURATION%


