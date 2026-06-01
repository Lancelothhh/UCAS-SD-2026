from __future__ import annotations

import argparse
import os
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"


def find_compiler() -> str:
    for name in ("gcc", "clang", "cc"):
        path = shutil.which(name)
        if path:
            return path
    raise SystemExit("No C compiler found. Please install gcc, clang, or cc first.")


def exe_name(name: str) -> str:
    return name + (".exe" if os.name == "nt" else "")


def run(command: list[str]) -> None:
    print("$ " + " ".join(str(part) for part in command))
    subprocess.run(command, cwd=ROOT, check=True)


def compile_program(compiler: str, output: Path, source: Path) -> None:
    command = [
        compiler,
        "-std=c11",
        "-O2",
        "-Wall",
        "-Wextra",
        "-I",
        str(ROOT / "src"),
        str(ROOT / "src" / "avl_tree.c"),
        str(source),
        "-o",
        str(output),
    ]
    run(command)


def run_member_a(compiler: str) -> None:
    test_core = BUILD / exe_name("test_core")
    compile_program(compiler, test_core, ROOT / "tests" / "test_core.c")
    run([str(test_core)])


def run_member_b(compiler: str) -> None:
    test_extra = BUILD / exe_name("test_extra")
    benchmark = BUILD / exe_name("benchmark_avl")

    compile_program(compiler, test_extra, ROOT / "tests" / "test_extra.c")
    compile_program(compiler, benchmark, ROOT / "benchmark" / "benchmark_avl.c")

    run([str(test_extra)])
    run([str(benchmark), "50000"])


def run_all(compiler: str) -> None:
    test_avl = BUILD / exe_name("test_avl")

    run_member_a(compiler)
    run_member_b(compiler)
    compile_program(compiler, test_avl, ROOT / "tests" / "test_avl.c")
    run([str(test_avl)])


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Build and run AVL tests for member A, member B, or the whole project."
    )
    parser.add_argument(
        "target",
        nargs="?",
        default="all",
        choices=("a", "b", "all"),
        help="a: core algorithm tests, b: extra/display/performance tests, all: all tests",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    compiler = find_compiler()
    BUILD.mkdir(exist_ok=True)

    if args.target == "a":
        run_member_a(compiler)
        print("\nMember A tests completed.")
    elif args.target == "b":
        run_member_b(compiler)
        print("\nMember B tests completed.")
    else:
        run_all(compiler)
        print("\nAll tests completed.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

