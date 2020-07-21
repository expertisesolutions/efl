@echo off
:: About `setlocal EnableDelayedExpansion`:
::   Needed because batch reads lines long before executing them, which means
::   it will expand variables before it has even been set (yes), so sometimes
::   they will come with trash-or-none value. This enables the usage of
::   `!variable!`, which (differently from `%variable%`) expands the variable
::   value when executing.

call :main || (echo Failed to ensure vcvars64 is found)
exit /B %errorlevel%

:check_path
    set vcvars_in_path=0
    (
        where vcvars64 2>nul
        call set vcvars_in_path=%%errorlevel%%
    )
    if "%vcvars_in_path%" == "0" (
        echo [GOOD] vcvars64 found in PATH.
        vcvars64
        exit /B 0
    ) else (
        echo [INFO] vcvars64 not found in PATH.
    )
exit /B 0

:try_version
    set edition=%~1
    set _vcvars64="C:\Program Files (x86)\Microsoft Visual Studio\2019\%edition%\VC\Auxiliary\Build\vcvars64.bat"
    if not exist %_vcvars64% (
        exit /B 1
	)
    call set vcvars64=%%_vcvars64%%
exit /B 0

:find_vs_vcvars64
    echo Searching for vcvars64 in VS installations...

    for %%v in (Enterprise Professional Community) do (
        echo Trying edition: %%v
        call :try_version %%v
        if not defined vcvars64 (
            echo [INFO] No vcvars64 for "%%v" edition
        ) else (
            call echo [GOOD] Found vcvars in %%vcvars64%%
            exit /B 0
        )
	)
exit /B 0

:main
    if not defined DevEnvDir (
        echo DevEnvDir not set, looking for vcvars64...
        call :check_path

        if not defined vcvars64 (
            call :find_vs_vcvars64
        )
        call echo vcvars64: %%vcvars64%%
        call %%vcvars64%%
    ) else (
        echo DevEnvDir already set. Nothing to be done here.
	)
exit /B 0
