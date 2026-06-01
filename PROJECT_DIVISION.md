# 二人项目分工

本项目建议按“核心算法”和“展示扩展”分工。这样做的好处是职责清楚：成员 A 负责 AVL 树是否正确平衡，成员 B 负责项目是否完整、好演示、好验证。

## 成员 A：AVL 树核心算法

负责目录：`student_a/`

负责文件：

- `src/avl_tree.h`
- `src/avl_tree.c` 中的核心结构和基础操作
- `tests/test_core.c`

具体函数分工：

| 函数或模块 | 职责 |
| --- | --- |
| `AVLNode` | 设计树节点，保存关键字、高度、左右孩子 |
| `AVLTree` | 设计树对象，保存根节点和节点总数 |
| `avl_init` | 初始化空树 |
| `avl_destroy` | 释放整棵树，避免内存泄漏 |
| `avl_insert` | 插入关键字，拒绝重复关键字 |
| `avl_delete` | 删除关键字，重点处理非叶子节点删除 |
| `avl_search` | 查找关键字是否存在 |
| `avl_height` | 返回当前 AVL 树高度 |
| `avl_size` | 返回当前节点总数 |
| `avl_is_balanced_bst` | 检查二叉搜索树性质、高度记录和平衡因子 |
| `rotate_left` | 左旋，用于 RR 或 RL 调整 |
| `rotate_right` | 右旋，用于 LL 或 LR 调整 |
| `rebalance` | 统一处理 LL、RR、LR、RL 四种失衡情况 |
| `min_value_node` | 删除双孩子节点时寻找右子树最小节点 |

成员 A 的验收命令：

```bash
python scripts/run_tests.py a
```

成员 A 报告中应重点说明：

- 为什么 AVL 树的查找、插入、删除时间复杂度是 `O(log n)`。
- 插入后如何根据平衡因子判断 LL、RR、LR、RL。
- 删除非叶子节点时，如何用后继节点替代，并继续向下删除。
- 删除后为什么也必须回溯更新高度并重新平衡。

## 成员 B：展示、扩展功能、测试与性能验证

负责目录：`student_b/`

负责文件：

- `src/main.c`
- `src/avl_tree.c` 中的显示和扩展函数
- `tests/test_extra.c`
- `tests/test_avl.c`
- `benchmark/benchmark_avl.c`
- `scripts/run_tests.py`
- `README.md`

具体函数分工：

| 函数或模块 | 职责 |
| --- | --- |
| `avl_print_inorder` | 用中序遍历输出有序关键字序列 |
| `avl_print_tree` | 以横向树形结构显示 AVL 树 |
| `avl_merge` | 将另一棵 AVL 树中的关键字合并到目标树 |
| `avl_split` | 按关键字 `x` 分裂为 `<= x` 和 `> x` 两棵树 |
| `print_menu` | 输出交互菜单 |
| `read_int` | 统一读取用户输入 |
| `show_status` | 插入或删除后显示节点数、高度和树形结构 |
| `merge_from_input` | 从用户输入构造第二棵树并合并 |
| `split_by_input` | 从用户输入读取 `x` 并展示分裂结果 |
| `benchmark_avl.c` | 生成大量测试数据，统计插入、查找、删除耗时 |
| `scripts/run_tests.py` | 提供 A、B、全体三种测试入口 |

成员 B 的验收命令：

```bash
python scripts/run_tests.py b
```

成员 B 报告中应重点说明：

- 菜单如何覆盖查找、插入、删除三种基本功能。
- 为什么中序遍历能验证二叉搜索树的有序性。
- 合并操作如何避免重复关键字。
- 分裂操作如何保证两棵结果树仍然是 AVL 树。
- 性能测试中数据规模、计时方式和结果含义。

## 附加内容 1：合并两棵 AVL 树

作业选作内容：

> 合并两棵平衡二叉树。

本项目对应函数：

- `avl_merge`
- `merge_rec`

实现思路：

1. 中序遍历源树。
2. 将源树每个关键字插入目标树。
3. 插入过程复用 `avl_insert`，因此每一步都会自动保持 AVL 平衡。
4. 如果关键字重复，`avl_insert` 会拒绝重复插入。

测试位置：

- `tests/test_extra.c`
- `tests/test_avl.c`

## 附加内容 2：按 x 分裂 AVL 树

作业选作内容：

> 把一棵平衡二叉树分裂为两棵平衡二叉树，使得一棵树中的所有关键字都小于或等于 x，另一棵树中的任一关键字都大于 x。

本项目对应函数：

- `avl_split`
- `split_rec`

实现思路：

1. 中序遍历原树。
2. 若当前关键字 `<= x`，插入 `less_equal` 树。
3. 若当前关键字 `> x`，插入 `greater` 树。
4. 两棵结果树都通过 `avl_insert` 建立，因此自然保持 AVL 平衡。

测试位置：

- `tests/test_extra.c`
- `tests/test_avl.c`

## 协作流程

1. 成员 A 先完成核心 AVL 操作，并运行 `python scripts/run_tests.py a`。
2. 成员 B 在核心操作可用后完成展示、合并、分裂和测试，并运行 `python scripts/run_tests.py b`。
3. 两人合并代码后运行 `python scripts/run_tests.py all`。
4. 最终提交时，把各自说明、截图、性能结果分别放入 `student_a/` 和 `student_b/`。

