@echo off

rem doskey cmake="%PROGRAMFILES% (x86)\CMake\bin\cmake.exe"
echo APPVEYOR=%APPVEYOR%

set BUILD_ROOT_DIR=c:\build
set BOOST_VER=1_58_0
set CGAL_VER=4.8.1
set GMP_LIB_NAME=libgmp-10.lib
set MPFR_LIB_NAME=libmpfr-4.lib
if "%APPVEYOR%"=="True" (
	set MSBUILD_CONFIGURATION=%CONFIGURATION%
) else (
	set MSBUILD_CONFIGURATION=RelWithDebInfo
)

set COMMON_INSTALL_ROOT_DIR=%BUILD_ROOT_DIR%\local
set BOOST_SRC_DIR=%BUILD_ROOT_DIR%\boost_%BOOST_VER%
set GMP_ROOT_DIR=%BUILD_ROOT_DIR%\gmp
set CGAL_SRC_DIR=%BUILD_ROOT_DIR%\CGAL-%CGAL_VER%

set PGROUTING_SRC_DIR=%~dp0..\..\
set USAGE="Usage: %~nx0 [pg_ver(9.0|9.1|9.2|9.3|9.4|9.5|9.6)] [platform(x86|x64)] (rebuild)"

if "%1"=="9.0" (
	set PG_VER=9.0
	set MSVC_VER=9.0
	set MSVC_YEAR=2008
	set PROJ_EXT=vcproj
) else if "%1"=="9.1" (
	set PG_VER=9.1
	set MSVC_VER=10.0
	set MSVC_YEAR=2010
	set PROJ_EXT=vcxproj
) else if "%1"=="9.2" (
	set PG_VER=9.2
	set MSVC_VER=10.0
	set MSVC_YEAR=2010
	set PROJ_EXT=vcxproj
) else if "%1"=="9.3" (
	set PG_VER=9.3
	set MSVC_VER=12.0
	set MSVC_YEAR=2013
	set PROJ_EXT=vcxproj
) else if "%1"=="9.4" (
	set PG_VER=9.4
	set MSVC_VER=12.0
	set MSVC_YEAR=2013
	set PROJ_EXT=vcxproj
) else if "%1"=="9.5" (
	set PG_VER=9.5
	set MSVC_VER=12.0
	set MSVC_YEAR=2013
	set PROJ_EXT=vcxproj
) else if "%1"=="9.6" (
	set PG_VER=9.6
	set MSVC_VER=12.0
	set MSVC_YEAR=2013
	set PROJ_EXT=vcxproj
) else (
	echo %USAGE%
	exit /B 1
)

set RUNTIME=msvc%MSVC_VER:.=%
set BOOST_TOOLSET=msvc-%MSVC_VER%
set CMAKE_GENERATOR=Visual Studio %MSVC_VER:.0=% %MSVC_YEAR%

if "%2"=="x86" (
	set PLATFORM=x86
	set BOOST_ADDRESS_MODEL=32
) else if "%2"=="x64" (
	set PLATFORM=x64
	set BOOST_ADDRESS_MODEL=64
	set CMAKE_GENERATOR=%CMAKE_GENERATOR% Win64
) else (
	echo %USAGE%
	exit /B 1
)

if "%3"=="rebuild" (
	set REBUILD=1
) else (
	set REBUILD=0
)

echo PG_VER="%PG_VER%"
echo MSVC_VER="%MSVC_VER%"
echo RUNTIME="%RUNTIME%"
echo BOOST_TOOLSET="%BOOST_TOOLSET%"
echo CMAKE_GENERATOR="%CMAKE_GENERATOR%"
echo PLATFORM="%PLATFORM%"
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

set COMMON_INSTALL_DIR=%COMMON_INSTALL_ROOT_DIR%\%RUNTIME%\%PLATFORM%
set CGAL_BUILD_DIR=%CGAL_SRC_DIR%\build\%RUNTIME%\%PLATFORM%
set GMP_DIR=%GMP_ROOT_DIR%\%PLATFORM%
set PGROUTING_BUILD_DIR=%PGROUTING_SRC_DIR%build\pg%PG_VER:.=%\%PLATFORM%
set PGROUTING_INSTALL_DIR=%PGROUTING_SRC_DIR%lib\pg%PG_VER:.=%\%PLATFORM%
if "%PLATFORM%"=="x86" (
	set POSTGRESQL_DIR=%PROGRAMFILES% ^(x86^)\PostgreSQL\%PG_VER%
) else (
	set POSTGRESQL_DIR=%PROGRAMFILES%\PostgreSQL\%PG_VER%
)

echo COMMON_INSTALL_DIR="%COMMON_INSTALL_DIR%"
echo CGAL_BUILD_DIR="%CGAL_BUILD_DIR%"
echo GMP_DIR="%GMP_DIR%"
echo PGROUTING_BUILD_DIR="%PGROUTING_BUILD_DIR%"
echo PGROUTING_INSTALL_DIR="%PGROUTING_INSTALL_DIR%"
echo POSTGRESQL_DIR="%POSTGRESQL_DIR%"

rem ### Boost ###
set BOOST_SHORT_VER=%BOOST_VER:_0=%
set BOOST_INCLUDE_DIR=%COMMON_INSTALL_DIR%\include\boost-%BOOST_SHORT_VER%
set BOOST_LIBRARY_DIR=%COMMON_INSTALL_DIR%\lib
set BOOST_THREAD_LIB=%COMMON_INSTALL_DIR%\lib\libboost_thread-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_SYSTEM_LIB=%COMMON_INSTALL_DIR%\lib\libboost_system-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.lib
set BOOST_WILDCARD_LIB=%COMMON_INSTALL_DIR%\lib\libboost_*-vc%MSVC_VER:.=%-mt-%BOOST_SHORT_VER%.libs

if not exist "%BOOST_SRC_DIR%\b2.exe" (
	pushd %BOOST_SRC_DIR%
	call "bootstrap.bat"
	popd
)
rem TODO:better rebuild
if exist %BOOST_INCLUDE_DIR%\ if %REBUILD%==1 (
	rmdir /S /Q %BOOST_INCLUDE_DIR%
	del /S /Q %BOOST_WILDCARD_LIB%
)
if not exist %BOOST_INCLUDE_DIR%\ (
	pushd %BOOST_SRC_DIR%
	@echo on
	b2 toolset=%BOOST_TOOLSET% variant=release link=static threading=multi address-model=%BOOST_ADDRESS_MODEL% ^
		--with-thread --with-system --prefix=%COMMON_INSTALL_DIR% -d0 install
	@echo off
	popd
)

rem ### CGAL ###
rem TODO:better rebuild
if exist %CGAL_BUILD_DIR%\ if %REBUILD%==1 (
	rmdir /S /Q %CGAL_BUILD_DIR%
)
if not exist %CGAL_BUILD_DIR%\ (
	mkdir %CGAL_BUILD_DIR%
	pushd %CGAL_BUILD_DIR%
	@echo on
	cmake -G "%CMAKE_GENERATOR%" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=%COMMON_INSTALL_DIR% ^
		-DBoost_USE_MULTITHREADED=ON -DCGAL_Boost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=OFF ^
		-DBoost_INCLUDE_DIR:PATH=%BOOST_INCLUDE_DIR% ^
		-DBOOST_LIBRARYDIR=%BOOST_LIBRARY_DIR% -DGMP_INCLUDE_DIR=%GMP_DIR%\include ^
		-DGMP_LIBRARIES=%GMP_DIR%\lib\%GMP_LIB_NAME% -DMPFR_INCLUDE_DIR=%GMP_DIR%\include ^
		-DMPFR_LIBRARIES=%GMP_DIR%\lib\%MPFR_LIB_NAME% -DWITH_CGAL_QT3=OFF -DWITH_CGAL_QT4=OFF ..\..\..\
	msbuild CGAL.sln /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
	msbuild INSTALL.%PROJ_EXT% /target:Build /property:Configuration=%MSBUILD_CONFIGURATION%
	@echo off
	popd
)

rem ### pgRouting ###
rem TODO:better rebuild
if exist %PGROUTING_BUILD_DIR%\ if %REBUILD%==1 (
	rmdir /S /Q %PGROUTING_BUILD_DIR%
)
if not exist %PGROUTING_BUILD_DIR%\ (
	mkdir %PGROUTING_BUILD_DIR%
	pushd %PGROUTING_BUILD_DIR%
	@echo on
	cmake -G "%CMAKE_GENERATOR%" -DPOSTGRESQL_INCLUDE_DIR:PATH="%POSTGRESQL_DIR%\include;%POSTGRESQL_DIR%\include\server;%POSTGRESQL_DIR%\include\server\port;%POSTGRESQL_DIR%\include\server\port\win32;%POSTGRESQL_DIR%\include\server\port\win32_msvc" ^
		-DPOSTGRESQL_LIBRARIES:FILEPATH="%POSTGRESQL_DIR%\lib\postgres.lib" -DPOSTGRESQL_EXECUTABLE:FILEPATH="%POSTGRESQL_DIR%\bin\postgres.exe" ^
		-DPOSTGRESQL_PG_CONFIG:FILEPATH="%POSTGRESQL_DIR%\bin\pg_config.exe" -DBoost_INCLUDE_DIR:PATH=%BOOST_INCLUDE_DIR% ^
		-DBOOST_THREAD_LIBRARIES:FILEPATH="%BOOST_THREAD_LIB%;%BOOST_SYSTEM_LIB%" ^
		-DCGAL_INCLUDE_DIR:PATH="%COMMON_INSTALL_DIR%\include;%GMP_DIR%\include" -DCGAL_LIBRARIES:FILEPATH=%COMMON_INSTALL_DIR%\lib\libCGAL-vc%MSVC_VER:.=%-mt-%CGAL_VER%.lib ^
		-DGMP_LIBRARIES:FILEPATH="%GMP_DIR%\lib\%GMP_LIB_NAME%;%GMP_DIR%\lib\%MPFR_LIB_NAME%" -DWITH_DD=ON ..\..\..\
	@echo off
	popd
)
