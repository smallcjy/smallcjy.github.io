---
title: 编译原理--C.F.G
date: 2024-06-22 16:28:41
tags: 编译原理
---

# 1 Context-free grammar

CFG上下文无关文法，特殊的语法结构，和正则表达式类似，包含可迭代的规则，是正则表达式的超集。

定义：G=(VT, VN, P, S)
- VT是terminals的集合，终止符，the basic symbols from which strings are formed. Terminals are tokens
- VN是nonterminals的集合，非终止符，names for structures that denote sets of strings
- P是productions的集合，语法规则，A->a，where A∈ VN and α∈ (VN∪VT) *.
- S is a start symbol , S∈VN

# 2 Derivation/reduction
derivation推导的作用

CFG规则决定strings of token的合法性是通过derivation或者reduction的。derivation的过程就是将一个非终止符替换成该production的右边的过程的重复过程。

CFA的language：
Let G be a context-free grammar with start symbol S. Then the language of G is:

L(G) = { s∈VT* | there exists S =》 * s of G}



# 3 leftmost derivation 
最左推导：在推导的每一步里优先推导最左边的非终止符，对应对一颗parse tree进行前序遍历。
# 4 rightmost derivation
最右推导：在推导的每一步里优先推导最右边的非终止符，它对应于解析树的后序遍历的逆过程
# 5 sentential form and sentence
什么是sentence？S is the start symbol of G, if S =》 * α, α ∈ (VN∪VT) *, α is a sentential form of G。但如果a只有终止符构成，则a是G的sentence。注意sentence form和sentence的区别。
# 6 parse tree
语法树。表示token串的结构的工具。如何定义一个parse tree？
- The root node is labeled with the start symbol S
- Each leaf node is labeled with a terminal or with ε
- Each nonleaf node is labeled with a nonterminal
- If a node with label A ∈ VN has n children with labels X1,X2,..,Xn(which may be terminals or nonterminals), then A -》 X1X2…Xn ∈P。一个产生式可以对应一个内部节点的分叉。

如何输出一个parse tree呢？叶子节点的自左向右遍历就是原始输入
# 7 Abstract syntas tree
parse tree很多对于编译器生成可执行代码的多余的信息，因此需要抽象语法树来简化。AST是parse tree的压缩，叶子节点是操作对象，内部节点是操作
# 8 grammars with left recursion and/or left factor
左递归文法以及如何提取左公因子消除左递归

A -> Aa|b1|b2 转换为
- A -> b1A'|b2A'
- A' -> aA'|e

# 9 ambiguous grammar
歧义：某些文法会对用一个string产生不同的parse tree，也就是最左推导和最右推导产生的语法树是不一样的。

如何处理歧义：
- Disambiguating rule ：设置规则确认哪一个parse tree是正确的
- Rewriting the grammar：重写文法消除歧义，比如添加优先级、关联性
