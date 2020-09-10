@echo off

set __OLD_vcvars64=%vcvars64%
set CLICOLOR_FORCE=1

set NINJAFLAGS=%*

call ensure-vcvars64

ninja %NINJAFLAGS% -C build install

set vcvars64=%__OLD_vcvars64%
set __OLD_vcvars64=
exit /B %ERRORLEVEL%
