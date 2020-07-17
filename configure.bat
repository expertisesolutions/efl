@echo off
setlocal EnableDelayedExpansion

:: Receive extra-flags
set MESONFLAGS_EXTRA=%*

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

:check_env_vars
    @echo Checking if necessarry environment variables were set...

    set envfile=env.bat
    if exist %envfile% (
        @echo - Found %envfile% file.
        call %envfile%
    ) else (
        @echo - File %envfile% doesn't exists. Relying on previously set environment variables...
    )
    set envfile=

    set all_set=1
    if not defined OPENSSL_DIR set all_set=0

    if %all_set%==1 (
        @echo - Using OpenSSL: %OPENSSL_DIR%
    ) else (
        @echo At least one of the following variables were not set:
        @echo     - OPENSSL_DIR: %OPENSSL_DIR%
        @echo Please define them using by creating a "env.bat" file containing:
        @echo     @set OPENSSL_DIR=^<your OpenSSL directory^>
        exit /B 1
    )
    set all_set=

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
            -Dopenssl_dir="%OPENSSL_DIR:"=%"^
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
            -Deeze=false^
            -Dpulseaudio=false^
            -Dharfbuzz=false^
            -Dfribidi=false^
            -Dfontconfig=false^
            -Dedje-sound-and-video=false^
            -Dlibmount=false^
            "-Devas-loaders-disabler=gst,pdf,ps,raw,svg,rsvg,xcf,bmp,dds,eet,generic,gif,ico,jp2k,pmaps,png,psd,tga,tgv,tiff,wbmp,webp,xpm,json"^
            -Dopengl=none^
            "-Demotion-loaders-disabler=gstreamer1,libvlc,xine"^
            -Dbuild-examples=false^
            -Dbindings=^
            --buildtype=debug^
            --native-file native-file-windows.txt

    if exist build (
        @echo "- Build directory ("build") already exists. Old config will be wiped with `--wipe`."
        set MESONFLAGS=%MESONFLAGS% --wipe
    ) else (
        @echo No Creating new build directory.
    )

    set NLM=^


    set NL=^^^%NLM%%NLM%%NLM%%NLM%
    @echo Here %NL%we go
    @echo Meson flags: %MESONFLAGS:        =!NL!%
    @echo ------------------------------
    @echo Extra flags: !MESONFLAGS_EXTRA!
    @echo ------------------------------
    @set MESONFLAGS=%MESONFLAGS:            = %
exit /B 0

:generate_build
    @echo ------------------------------
    @echo Generating build...
    set vcvars64="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"

    if not defined DevEnvDir (
        call %vcvars64%
    )
    meson build %MESONFLAGS% %MESONFLAGS_EXTRA%
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
