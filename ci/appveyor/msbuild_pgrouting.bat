@echo off

set PG_VER="%1"

rem doskey cmake="%PROGRAMFILES% (x86)\CMake\bin\cmake.exe"
echo APPVEYOR=%APPVEYOR%

echo MSVC_VER %MSVC_VER%
echo BUILD_ROOT_DIR %BUILD_ROOT_DIR%
echo CGAL_LIBRARIES %CGAL_LIBRARIES%
echo GMP_LIBRARIES %GMP_LIBRARIES%
echo MPFR_LIBRARIES %MPFR_LIBRARIES%
echo CMAKE_GENERATOR %CMAKE_GENERATOR%
echo BOOST_THREAD_LIB %BOOST_THREAD_LIB%
echo BOOST_SYSTEM_LIB %BOOST_SYSTEM_LIB%
echo BOOST_INCLUDE_DIR %BOOST_INCLUDE_DIR%
echo PLATFORM %PLATFORM%
echo GMP_INCLUDE_DIR %GMP_INCLUDE_DIR%
echo CGAL_INCLUDE_DIR %CGAL_INCLUDE_DIR%



set PGROUTING_SRC_DIR=%~dp0..\..\

set RUNTIME=msvc%MSVC_VER:.=%

echo PG_VER="%PG_VER%"
echo MSVC_VER="%MSVC_VER%"
echo RUNTIME="%RUNTIME%"
echo CMAKE_GENERATOR="%CMAKE_GENERATOR%"
echo REBUILD="%REBUILD%"

set VS100COMNTOOLS=%PROGRAMFILES% (x86)\Microsoft Visual Studio 10.0\Common7\Tools\
rem TODO:
set VCVARSALL_BAT_PATH="%PROGRAMFILES% (x86)\Microsoft Visual Studio %MSVC_VER%\VC\vcvarsall.bat"
if "%RUNTIME%"=="msvc100" if "%PLATFORM%"=="x64" (
    set OPTION=x86
) else if "%PLATFORM%"=="x86" (
    set OPTION=x86
) else if "%PLATFORM%"=="x64" (
    set OPTION=amd64
)

call %VCVARSALL_BAT_PATH% %OPTION%

if "%RUNTIME%"=="msvc100" if "%PLATFORM%"=="x64" (
    rem quotes to avoid escape error
    set INCLUDE="%INCLUDE%;%PROGRAMFILES%\Microsoft SDKs\Windows\v7.1\Include"
    if "%PLATFORM%"=="x86" (
        set LIB="%LIB%;%PROGRAMFILES%\Microsoft SDKs\Windows\v7.1\Lib"
    ) else if "%PLATFORM%"=="x64" (
        set LIB="%LIB%;%PROGRAMFILES%\Microsoft SDKs\Windows\v7.1\Lib\x64"
    )
)

rem remove quotes
set INCLUDE=%INCLUDE:"=%
set LIB=%LIB:"=%

rem echo INCLUDE="%INCLUDE%"
rem echo LIB="%LIB%"

path %PATH%;%PROGRAMFILES% (x86)\CMake\bin

set PGROUTING_BUILD_DIR=%PGROUTING_SRC_DIR%build\pg%PG_VER:.=%\%PLATFORM%
set PGROUTING_INSTALL_DIR=%PGROUTING_SRC_DIR%lib\pg%PG_VER:.=%\%PLATFORM%
if "%PLATFORM%"=="x86" (
    set POSTGRESQL_DIR=%PROGRAMFILES% ^(x86^)\PostgreSQL\%PG_VER%
) else (
    set POSTGRESQL_DIR=%PROGRAMFILES%\PostgreSQL\%PG_VER%
)

echo PGROUTING_BUILD_DIR="%PGROUTING_BUILD_DIR%"
echo PGROUTING_INSTALL_DIR="%PGROUTING_INSTALL_DIR%"
echo POSTGRESQL_DIR="%POSTGRESQL_DIR%"


rem ### pgRouting ###

if exist %PGROUTING_BUILD_DIR% (
    rmdir /S /Q %PGROUTING_BUILD_DIR%
)

mkdir %PGROUTING_BUILD_DIR%
pushd %PGROUTING_BUILD_DIR%
@echo on
cmake -G "%CMAKE_GENERATOR%" -DPOSTGRESQL_INCLUDE_DIR:PATH="%POSTGRESQL_DIR%\include;%POSTGRESQL_DIR%\include\server;%POSTGRESQL_DIR%\include\server\port;%POSTGRESQL_DIR%\include\server\port\win32;%POSTGRESQL_DIR%\include\server\port\win32_msvc" ^
    -DPOSTGRESQL_LIBRARIES:FILEPATH="%POSTGRESQL_DIR%\lib\postgres.lib" ^
    -DPOSTGRESQL_EXECUTABLE:FILEPATH="%POSTGRESQL_DIR%\bin\postgres.exe" ^
    -DPOSTGRESQL_PG_CONFIG:FILEPATH="%POSTGRESQL_DIR%\bin\pg_config.exe" ^
    -DBoost_INCLUDE_DIR:PATH=%BOOST_INCLUDE_DIR% ^
    -DBOOST_THREAD_LIBRARIES:FILEPATH="%BOOST_THREAD_LIB%;%BOOST_SYSTEM_LIB%" ^
    -DCGAL_INCLUDE_DIR:PATH="%CGAL_INCLUDE_DIR%;%GMP_INCLUDE_DIR%" ^
    -DCGAL_LIBRARIES:FILEPATH=%CGAL_LIBRARIES% ^
    -DGMP_LIBRARIES:FILEPATH="%GMP_LIBRARIES%;%MPFR_LIBRARIES%"  ..\..\..\
@echo off
popd

