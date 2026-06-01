# 平衡二叉树动态查找表

本项目用于数据结构大作业：利用 AVL 平衡二叉树实现动态查找表，支持查找、插入、删除，并提供合并、分裂两项扩展操作示例。

## 项目结构

```text
.
├── src/                    # 工程版代码
│   ├── avl_tree.h
│   ├── avl_tree.c
│   └── main.c
├── reference/              # 可直接参考的单文件完整版
│   └── avl_reference_full.c
├── tests/                  # 正确性测试
│   └── test_avl.c
├── benchmark/              # 性能测试
│   └── benchmark_avl.c
├── scripts/                # 自动编译和测试脚本
│   └── run_tests.py
├── student_a/              # 成员 A 工作区
├── student_b/              # 成员 B 工作区
├── PROJECT_DIVISION.md     # 二人分工建议
├── CMakeLists.txt
└── .gitignore
```

## 快速运行

如果电脑已安装 `gcc` 或 `clang`，可直接运行：

```bash
python scripts/run_tests.py
```

脚本会自动编译并运行：

- 正确性测试：插入、查找、删除、合并、分裂、随机操作
- 性能测试：大规模插入、查找、删除耗时

## 运行交互演示程序

使用 `gcc`：

```bash
gcc -std=c11 -O2 -Wall -Wextra -I src src/avl_tree.c src/main.c -o build/avl_demo
./build/avl_demo
```

Windows 下如果生成的是 `avl_demo.exe`，运行：

```powershell
.\build\avl_demo.exe
```

## 参考代码

`reference/avl_reference_full.c` 是单文件 C 语言完整版，适合阅读、提交参考或单独编译：

```bash
gcc -std=c11 -O2 -Wall -Wextra reference/avl_reference_full.c -o build/avl_reference
./build/avl_reference
```

## 已实现功能

- AVL 树初始化与销毁
- 查找关键字
- 插入关键字，自动旋转保持平衡
- 删除关键字，自动旋转保持平衡
- 中序遍历显示
- 树形结构显示
- 合并两棵 AVL 树
- 按关键字 `x` 分裂为 `<= x` 和 `> x` 两棵 AVL 树
- 正确性测试和性能测试

## 编译说明

也可以使用 CMake：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

