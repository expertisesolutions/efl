from collections import defaultdict
from dataclasses import dataclass
from itertools import chain
from functools import partial
from pathlib import Path
from pprint import pprint
from subprocess import run, CompletedProcess
from textwrap import dedent, indent
from typing import Callable, DefaultDict, Dict, List, Optional
import shutil
import sys
import os
import stat

import typer


@dataclass
class Configuration:
    builddir: Path
    verbose: bool
    environ: DefaultDict[str, str]
    mesonflags: List[str]
    mesonflags_extra: List[str]
    pkg_config_libdir: str


def log(text: str):
    print(dedent(text).strip())


def list_str(items: List[str], prefix: str = "             ") -> str:
    return indent("\n".join(items), prefix).strip()


def exit_if_failed(result: CompletedProcess):
    if result.returncode != 0:
        exit(result.returncode)


def ensure_vcvars64() -> Dict[str, str]:
    def run_vcvars(path):
        output = run([path, "&&", "set"], capture_output=True, encoding="utf-8").stdout
        environ = defaultdict(str)
        for line in output.split("\n"):
            try:
                varname, value = line.split("=", maxsplit=1)
                environ[varname.upper()] = value
            except ValueError:
                pass
        return environ

    try:
        return run_vcvars("vcvars64.bat")
    except FileNotFoundError:
        log(":: vcvars64 not found on path. Searching manually...")

        vs_prefix = Path("C:/Program Files (x86)/Microsoft Visual Studio")
        for version in vs_prefix.glob("*"):
            for edition in version.glob("*"):
                vcvars64_path = edition / "VC" / "Auxiliary" / "Build" / "vcvars64.bat"
                try:
                    return run_vcvars(vcvars64_path)
                except FileNotFoundError:
                    pass


def check_env_vars(config: Configuration):
    print(":: Checking if necessarry environment variables were set...")

    envfile = Path(".env")

    if envfile.exists():
        print(f":: Found {envfile} file. Reading environment variables from it...")
        with open(envfile) as f:
            for line in f.readlines():
                if line.strip()[0] != "#":
                    var, value = line.split("=")
                    config.environ[var.upper()] = value
                    print(f"::   -> New variable: {var}: {value}")
    else:
        print(
            f"- File {envfile} doesn't exists."
            " Relying on previously set environment variables..."
        )

    vcpkg_toolchain_file = config.environ["VCPKG_TOOLCHAIN_FILE"]

    openssl_dir = config.environ["OPENSSL_DIR"]
    if not openssl_dir:
        log(
            """
            :: [INFO] "OPENSSL_DIR" environment variable is not defined. Relying on meson to
                      properly find OpenSSL with standard `dependency('openssl')` command. If
                      meson does not find OpenSSL, consider setting it to your current OPENSSL
                      library directory. In this case, you may consider creating a .env file
                      containing:
                      
                          openssl_dir=<your OpenSSL directory>
            """
        )

    config.environ["PKG_CONFIG_LIBDIR"] = config.pkg_config_libdir


def setup_flags(config):
    log(
        """
        :: ------------------------------
        :: Setting up build flags...
        """
    )

    # Compilers

    config.environ["CC"] = "clang-cl"
    config.environ["CXX"] = "clang-cl"

    # Windows terminal specific options

    cflags = [
        "-fansi-escape-codes",
        "-Wno-implicit-fallthrough",
        *config.environ["CFLAGS"],
    ]

    if config.verbose:
        clags += ["-v"]

    config.environ["CFLAGS"] = " ".join(cflags)
    config.mesonflags += [
        f'-Dcmake_args=-DCMAKE_TOOLCHAIN_FILE={config.environ["VCPKG_TOOLCHAIN_FILE"].strip()}'
    ]

    openssl_dir = config.environ["OPENSSL_DIR"]
    if openssl_dir:
        _openssl_dir = openssl_dir.replace('"', "")
        config.mesonflags += [f'-Dopenssl_dir="{_openssl_dir}"']

    log(
        f"""
        :: ------------------------------
        :: Configuration:
            * C Compiler           : {config.environ["CC"]}
            * C++ Compiler         : {config.environ["CXX"]}
            * CFLAGS               : {config.environ["CFLAGS"]}
            * Vcpkg Toolchain File : {config.environ["VCPKG_TOOLCHAIN_FILE"]}
        :: ------------------------------
        :: Meson flags:
             {list_str(config.mesonflags)}
        :: ------------------------------
        :: Extra flags: 
             {list_str(config.mesonflags_extra)}
        :: ------------------------------
        """
    )


def generate_build(config):
    log(
        """
        :: ------------------------------
        :: Generating build...
        :: 
        """
    )

    config.environ = {**config.environ, **ensure_vcvars64()}

    run(
        ["meson", config.builddir, *config.mesonflags, *config.mesonflags_extra],
        env=config.environ,
        check=True,
    )


app = typer.Typer()


@app.command()
def configure(
    builddir: Path = typer.Option("build", help="Custom meson build directory"),
    verbose: bool = typer.Option(
        False, help="Verbose compilation (shows compile flags when building)"
    ),
    backtrace: bool = typer.Option(False, help="Shows backtrace when configure fails."),
    vcpkg_toolchain_file: Path = typer.Option(
        Path(""), help="Location of vcpkg's toolchain file."
    ),
    pkg_config_libdir: str = typer.Option(
        "", help="Set PKG_CONFIG_LIBDIR for meson call"
    ),
    mesonflags_extra: List[str] = typer.Option([], help="Extra arguments for meson"),
):
    def step(title: str, function: Callable, *args, show_backtrace=backtrace):
        try:
            function(*args)
        except Exception as e:
            log(
                f"""
                :: {title} failed.
                   Reason: {e}
                """
            )
            if show_backtrace:
                raise
            exit(1)

    mesonflags = [
        "-Dnls=false",
        "-Dsystemd=false",
        "-Dgstreamer=false",
        "-Ddbus=false",
        "-Daudio=false",
        "-Davahi=false",
        "-Dv4l2=false",
        "-Delua=false",
        "-Dx11=false",
        "-Dphysics=false",
        "-Dethumb=false",
        "-Demotion=false",
        "-Deeze=false",
        "-Dpulseaudio=false",
        "-Dharfbuzz=false",
        "-Dfribidi=false",
        "-Dfontconfig=false",
        "-Dedje-sound-and-video=false",
        "-Dlibmount=false",
        "-Devas-loaders-disabler=gst,pdf,ps,raw,svg,rsvg,xcf,bmp,dds,generic,gif,ico,jp2k,pmaps,psd,tga,tgv,tiff,wbmp,webp,xpm,json",
        "-Dopengl=none",
        "-Demotion-loaders-disabler=gstreamer1,libvlc,xine",
        "-Dbindings=",
        "--buildtype=debug",
        "--native-file",
        "native-file-windows.txt",
    ]

    environ = defaultdict(str, os.environ)

    config = Configuration(
        builddir,
        verbose,
        environ,
        mesonflags,
        mesonflags_extra,
        pkg_config_libdir if pkg_config_libdir else "thistotallydoesnotexist",
    )

    log(
        f"""
        :: ------------------------------
        :: EFL configure options:
           - Extra meson arguments: {config.mesonflags_extra}
           - Build Directory: {config.builddir} {"(default)" if builddir == "build" else ""}
           - Verbose build: {"ON" if config.verbose else "OFF"}
        """
    )

    step("Environment variables check", check_env_vars, config)
    step("Setup flags", setup_flags, config)
    step("Generate build", generate_build, config)

    log(":: ------------------------------")


@app.command()
def build(
    builddir: Path = typer.Option("build", help="Custom meson build directory"),
    ninjaflags: List[str] = typer.Argument(None, help="Extra arguments for ninja"),
):
    log(
        """
        :: ------------------------------
        :: Building...
        """
    )

    env = ensure_vcvars64()

    if not ninjaflags:
        ninjaflags = []

    result = run(["ninja", "-C", builddir, *ninjaflags], env=env)

    log(
        f"""
        :: ------------------------------
        :: Finished building EFL (Result: {result.returncode})
        """
    )

    exit_if_failed(result)


@app.command()
def test(
    builddir: Path = typer.Option("build", help="Custom meson build directory"),
    testsuite: str = typer.Argument(None, help="Run specific test suite"),
    mesonflags_extra: List[str] = typer.Argument(
        None, help="Extra arguments for meson"
    ),
):
    log(
        """
        :: ------------------------------
        :: Running tests...
        """
    )
    env = ensure_vcvars64()
    if testsuite:
        testsuite = [testsuite]
    else:
        testsuite = []
    result = run(
        [
            "meson",
            "test",
            "-C",
            builddir,
            "--print-errorlogs",
            *testsuite,
            *mesonflags_extra,
        ],
        env=env,
    )
    log(
        f"""
        :: ------------------------------
        :: Finished running tests (Result: {result.returncode})
        """
    )

    exit_if_failed(result)


@app.command()
def clean(builddir: Path = typer.Option("build", help="Custom meson build directory")):
    def remove_dir(path):
        log(f"::   Removing {path.resolve()}")
        while True:
            try:
                shutil.rmtree(path)
                return
            except PermissionError as e:
                # For some reason, some git objects after a subproject clone
                # are created with read-only flag, causing the PermissionError.
                # This forces writing permissions in order to delete
                path = Path(e.filename)
                log(f"Forcing removal of {path}")
                os.chmod(path, stat.S_IWRITE)
                Path(path).unlink()
            except FileNotFoundError:
                log(f"     > {d} does not exist")
                return

    log(
        """
        :: ------------------------------
        :: Cleaning
        """
    )

    remove_dir(Path("build"))

    subproject_dirs = (s for s in Path("subprojects").iterdir() if s.is_dir())
    for d in subproject_dirs:
        remove_dir(d)

    log(
        """
        :: ------------------------------
        :: Finished cleaning
        """
    )


if __name__ == "__main__":
    app(
        ["configure"]
        if not sys.argv[1:] or sys.argv[1].startswith("--")
        else sys.argv[1:]
    )
