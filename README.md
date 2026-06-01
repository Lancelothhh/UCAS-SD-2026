# AVL 平衡二叉树动态查找表

这是一个数据结构大作业项目，主题是“平衡二叉树操作的演示”。项目使用 C 语言实现 AVL 平衡二叉树，并把代码、分工、参考实现、测试和性能验证分开存放，方便两名成员并行开发。

## 作业要求对应关系

| 作业要求 | 本项目中的实现 |
| --- | --- |
| 动态查找表 | 使用 AVL 平衡二叉树维护关键字集合 |
| 查找 | `avl_search` |
| 插入 | `avl_insert` |
| 删除 | `avl_delete` |
| 插入/删除后更新显示 | `src/main.c` 菜单程序会输出当前树形结构 |
| 四入表或图形化显示 | 使用旋转后的横向树形文本显示 |
| 选作：合并两棵 AVL 树 | `avl_merge` |
| 选作：按 x 分裂 AVL 树 | `avl_split` |

## 目录结构

```text
.
├── src/
│   ├── avl_tree.h              # AVL 树接口声明
│   ├── avl_tree.c              # 工程版核心实现
│   └── main.c                  # 菜单交互演示程序
├── reference/
│   └── avl_reference_full.c    # 单文件 C 语言参考完整版
├── tests/
│   ├── test_core.c             # 成员 A 核心算法测试
│   ├── test_extra.c            # 成员 B 扩展功能测试
│   └── test_avl.c              # 全体综合正确性测试
├── benchmark/
│   └── benchmark_avl.c         # 性能测试
├── scripts/
│   └── run_tests.py            # A / B / 全体三种测试入口
├── student_a/                  # 成员 A 工作区
├── student_b/                  # 成员 B 工作区
├── PROJECT_DIVISION.md         # 详细二人分工
├── CMakeLists.txt
└── .gitignore
```

## 一键测试

需要先安装 C 编译器，例如 `gcc` 或 `clang`。

成员 A 只验证核心算法：

```bash
python scripts/run_tests.py a
```

成员 B 验证扩展功能和性能测试：

```bash
python scripts/run_tests.py b
```

最终提交前跑全体测试：

```bash
python scripts/run_tests.py all
```

不写参数时默认等同于 `all`。

## 三种测试的区别

| 测试入口 | 主要用途 | 覆盖内容 |
| --- | --- | --- |
| `a` | 成员 A 排查核心 AVL 算法 | 初始化、销毁、插入、查找、删除、高度、规模、平衡性校验、随机操作 |
| `b` | 成员 B 排查扩展和展示相关内容 | 中序显示、树形显示、合并、分裂、性能测试 |
| `all` | 最终验收 | A 测试 + B 测试 + 综合测试 + 性能测试 |

注意：成员 B 的合并、分裂和性能测试需要基础插入、查找等函数可用，因为扩展功能建立在核心 AVL 树之上。

## 运行交互演示程序

使用 `gcc` 编译：

```bash
mkdir build
gcc -std=c11 -O2 -Wall -Wextra -I src src/avl_tree.c src/main.c -o build/avl_demo
```

Windows PowerShell 下运行：

```powershell
.\build\avl_demo.exe
```

程序菜单支持：

- 插入关键字
- 删除关键字
- 查找关键字
- 中序遍历显示
- 树形显示
- 合并另一棵 AVL 树
- 按关键字 `x` 分裂 AVL 树

## 单文件参考代码

`reference/avl_reference_full.c` 是完整 C 语言参考版，所有函数和菜单都放在一个文件中，适合阅读、备份或单独提交参考。

编译方式：

```bash
gcc -std=c11 -O2 -Wall -Wextra reference/avl_reference_full.c -o build/avl_reference_full
```

Windows PowerShell 下运行：

```powershell
.\build\avl_reference_full.exe
```

## CMake 编译

如果已安装 CMake，也可以使用：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 开发建议

1. 成员 A 优先保证 `python scripts/run_tests.py a` 通过。
2. 成员 B 在 A 的基础函数可用后，保证 `python scripts/run_tests.py b` 通过。
3. 最终提交前运行 `python scripts/run_tests.py all`。
4. 如果测试失败，先看终端输出中失败的是 `test_core`、`test_extra`、`test_avl` 还是 `benchmark_avl`，再定位对应成员负责的函数。

