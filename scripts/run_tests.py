from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"


def find_compiler() -> str:
    for name in ("gcc", "clang", "cc"):
        path = shutil.which(name)
        if path:
            return path
    raise SystemExit("未找到 gcc/clang/cc，请先安装 C 编译器后再运行测试。")


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


def main() -> int:
    compiler = find_compiler()
    BUILD.mkdir(exist_ok=True)

    test_exe = BUILD / exe_name("test_avl")
    bench_exe = BUILD / exe_name("benchmark_avl")

    compile_program(compiler, test_exe, ROOT / "tests" / "test_avl.c")
    compile_program(compiler, bench_exe, ROOT / "benchmark" / "benchmark_avl.c")

    run([str(test_exe)])
    run([str(bench_exe), "50000"])

    print("\n测试完成：正确性测试已通过，性能数据已输出。")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

