@echo off

call :has_arg --help %* || call :has_arg -h %*
if %errorlevel% == 0 (
    echo Usage: configure [-h] [--verbose] [--builddir BUILDDIR] [any extra arguments for meson]
    echo:
    echo Optional arguments:
    echo   -h, --help                    Shows this message and leaves
    echo   --verbose                     Verbose compilation (shows compile flags when building targets^)
    echo   --builddir BUILDDIR           Sets custom build directory
    exit /B 0
)

:: Receive extra-flags
set MESONFLAGS_EXTRA=%*

:: Check if a custom build dir is being passed in args
set BUILDDIR=build

:: (The space before the value is necessary for proper token evaluation)
call set _BUILDDIR=%%MESONFLAGS_EXTRA:*--builddir ==%%
(
    setlocal EnableDelayedExpansion
    if not "!_BUILDDIR!" == "*--builddir ==" (
        for /f "tokens=1 delims== " %%d in ("!_BUILDDIR!") do (
            set BUILDDIR=%%d
            echo - Build directory: "!BUILDDIR!"
            set pattern=--builddir !BUILDDIR!
            call set MESONFLAGS_EXTRA=%%MESONFLAGS_EXTRA:!pattern!=%%
        )
        echo !MESONFLAGS_EXTRA!
    ) else (
        echo - Build directory: build (default^)
    )
)
set _BUILDDIR=

:: Look for verbosity of this script and additional flags for meson
(echo %MESONFLAGS_EXTRA% | findstr /i /c:"--verbose" >nul) && set VERBOSE=ON

if defined VERBOSE (
    echo Verbose ON.
    set MESONFLAGS_EXTRA=%MESONFLAGS_EXTRA:--verbose=%
) else (
    echo Verbose OFF.
)

call :main || (echo Build configure failed.)
exit /B %errorlevel%

:: Removes head from a list of args and stores in a variable.
:: For example, by running:
::   `call :remove_head tail first second third`
:: A variable `tail` will be created with the value `second third`.
:remove_head
    set tail_variable=%1

    call set _all=%*
    if not "%3" == "" (
        call set tail=%%_all:*%2=%%
        set "%tail_variable% = %tail%"
    ) else (
        call set %tail_variable% =
    )
    set tail_variable=
exit /B 0

:: Checks if a specific argument is in argument-list.
:: Returns (sets errorlevel) to 0 if it is, 1 otherwise.
::
:: has_arg <arg> <arg list>
:has_arg
    setlocal
    set argname="%1"
    set argname=%argname:"=%

    call :remove_head tail %*

    call set tail=" %%tail%% "

    setlocal EnableDelayedExpansion
    call set cutted=%%tail: %argname% = %%
    if x!cutted!x == x!tail!x (
        exit /B 1
    )
exit /B 0

:check_env_vars
    @echo Checking if necessarry environment variables were set...

    set envfile=env.bat
    if exist %envfile% (
        @echo - Found %envfile% file.
        call %envfile%
        echo vcpkg_toolchain_file: %vcpkg_toolchain_file%
    ) else (
        @echo - File %envfile% doesn't exists. Relying on previously set environment variables...
    )
    set envfile=

    if not defined openssl_dir (
        echo [INFO] "OPENSSL_DIR" environment variable is not defined. Relying
        echo on meson to properly find OpenSSL with standard
        echo `dependency('openssl'^)` command. If meson does not find OpenSSL,
        echo consider setting it to your current OPENSSL library directory. In
        echo this case, you may consider creating a env.bat file containing:
        echo
        echo     @set OPENSSL_DIR=^<your OpenSSL directory^>
    )

    set pkg_config_libdir=thistotallydoesnotexist
exit /B 0


:setup_flags
    @echo ------------------------------
    @echo Setting up build flags...

    :: ---------------------------------
    :: Compilers
    set CC=clang-cl
    @echo - C Compiler: %CC%
    set CXX=clang-cl
    @echo - C++ Compiler: %CXX%

    :: ---------------------------------
    :: Windows terminal specific options
    set CFLAGS=-fansi-escape-codes -Wno-implicit-fallthrough %CFLAGS%


    if defined VERBOSE (
        set CFLAGS=-v %CFLAGS%
    )

    @echo - Using CFLAGS=%CFLAGS%

    :: ------------------------------------------------------
    set MESONFLAGS=^
            -Dnls=false^
            -Dsystemd=false^
            -Dgstreamer=false^
            -Ddbus=false^
            -Daudio=false^
            -Davahi=false^
            -Dv4l2=false^
            -Delua=false^
            -Dx11=false^
            -Dphysics=false^
            -Dethumb=false^
            -Demotion=false^
            -Deeze=false^
            -Dbuild-examples=false^
            -Dpulseaudio=false^
            -Dharfbuzz=false^
            -Dfribidi=false^
            -Dfontconfig=false^
            -Dedje-sound-and-video=false^
            -Dlibmount=false^
            "-Devas-loaders-disabler=gst,pdf,ps,raw,svg,rsvg,xcf,bmp,dds,generic,gif,ico,jp2k,pmaps,psd,tga,tgv,tiff,wbmp,webp,xpm,json"^
            -Dopengl=none^
            "-Demotion-loaders-disabler=gstreamer1,libvlc,xine"^
            -Dbindings=^
            --buildtype=debug^
            --native-file native-file-windows.txt

    echo %vcpkg_toolchain_file%
    call set MESONFLAGS=%MESONFLAGS% -Dcmake_args="-DCMAKE_TOOLCHAIN_FILE=%vcpkg_toolchain_file%"

    if defined openssl_dir (
        set MESONFLAGS=-Dopenssl_dir="%OPENSSL_DIR:"=%" %MESONFLAGS%
    )

    (
        setlocal EnableDelayedExpansion
        echo ------------------------------
        echo Meson flags:
        echo     %MESONFLAGS:           =%
        echo ------------------------------
        echo Extra flags: !MESONFLAGS_EXTRA!
        echo ------------------------------
        endlocal
    )
    set MESONFLAGS=%MESONFLAGS:            = %
exit /B 0

:generate_build
    setlocal EnableDelayedExpansion
    @echo ------------------------------
    @echo Generating build...
    call ensure-vcvars64
    if not defined devenvdir (
        echo Failed: DevEnvDir still not accessible
        exit /B 0
    )
    meson !BUILDDIR! !MESONFLAGS! !MESONFLAGS_EXTRA!
exit /B 0

:save_old_vars
    @echo ------------------------------
    set __OLD_vcvars64=%vcvars64%
    set __OLD_CC=%CC%
    set __OLD_CXX=%CXX%
    set __OLD_CFLAGS=%CFLAGS%
    set __OLD_MESONFLAGS=%MESONFLAGS%
exit /B 0

:restore_old_vars
    @echo ------------------------------
    set vcvars64=%__OLD_vcvars64%
    set CC=%__OLD_CC%
    set CXX=%__OLD_CXX%
    set CFLAGS=%__OLD_CFLAGS%
    set MESONFLAGS=%__OLD_MESONFLAGS%

    set __OLD_vcvars64=
    set __OLD_CC=
    set __OLD_CXX=
    set __OLD_CFLAGS=
    set __OLD_MESONFLAGS=
exit /B 0

:main
    call :save_old_vars
    call :check_env_vars || (echo Environment Variables check failed) && exit /B 1
    call :setup_flags
    call :generate_build || (echo Meson failed) && exit /B 1
    call :restore_old_vars
exit /B 0
