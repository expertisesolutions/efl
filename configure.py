import sys
import subprocess
import shlex
import venv
from pathlib import Path
from typing import List, Union


def run_command(
    command: Union[str, List[str]], *args, **kwargs
) -> subprocess.CompletedProcess:
    """Wrapper for subprocess.run to support passing the command as a string."""
    split_command = shlex.split(command) if isinstance(command, str) else command
    try:
        return subprocess.run(split_command, check=True, *args, **kwargs)
    except subprocess.CalledProcessError as e:
        print(f":: Failed to execute command {command}")
        sys.exit(e.returncode)


def create_venv(venv_dir: Path):
    if venv_dir.is_dir():
        print(":: Venv (.venv) already exists. Skipping...")
        return

    venv.create(venv_dir, with_pip=True)
    print(":: Venv created successfully.")


def main(args: List[str]):
    venv_dir = Path.cwd() / ".venv"

    create_venv(venv_dir)

    venv_python = venv_dir / "Scripts" / "python.exe"

    print(":: Ensuring typer is installed")
    run_command([venv_python, "-m", "pip", "install", "typer"])
    print(":: Configuring project...")
    run_command([venv_python, "-m", "generate_build", *args])
    print(":: Finished configuring project.")


if __name__ == "__main__":
    main(sys.argv[1:])
