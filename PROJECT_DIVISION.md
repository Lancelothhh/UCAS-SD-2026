# 二人项目分工建议

## 成员 A：AVL 树核心算法

负责目录：`student_a/`

建议任务：

1. 阅读 `src/avl_tree.c` 中的旋转、插入、删除逻辑。
2. 独立整理 AVL 树核心说明文档，包括 LL、RR、LR、RL 四种失衡情况。
3. 重点维护和讲解：
   - `avl_insert`
   - `avl_delete`
   - `avl_search`
   - `avl_is_balanced_bst`
4. 在报告中说明删除算法：
   - 删除非叶子节点时，用左子树最大值或右子树最小值替代。
   - 删除动作最终传递到叶子或单分支节点。
   - 回溯时更新高度并做旋转调整。

## 成员 B：交互演示、扩展功能与测试

负责目录：`student_b/`

建议任务：

1. 阅读 `src/main.c` 中的菜单交互和显示逻辑。
2. 重点维护和讲解：
   - `avl_print_tree`
   - `avl_print_inorder`
   - `avl_merge`
   - `avl_split`
3. 完善测试与性能验证：
   - `tests/test_avl.c`
   - `benchmark/benchmark_avl.c`
   - `scripts/run_tests.py`
4. 在报告中说明测试数据如何设计，以及性能测试结果如何证明 AVL 树高度稳定。

## 协作方式

- 成员 A 修改核心算法后，运行 `python scripts/run_tests.py` 验证正确性。
- 成员 B 修改交互或测试后，也运行同一条命令验证。
- 两人分别在 `student_a/` 和 `student_b/` 中保存自己的说明、草稿和实验结果。
- 最终提交前，保证 `tests/test_avl.c` 通过，且性能测试能够输出插入、查找、删除耗时。

