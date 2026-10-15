*This project has been created as part of the 42 curriculum by jbayonne*

# Ready, Set, Bool!

This C++ project was developed for the 42 **Ready, Set, Boole!** subject. It
progressively connects low-level bit manipulation with propositional logic,
Boolean algebra, and set theory.

The original subject is available in [en.subject.pdf](./en.subject.pdf).

## Project overview

The project covers:

- binary addition, multiplication, and Gray-code conversion using bitwise
  operations;
- evaluation of Boolean formulas written in Reverse Polish Notation (RPN);
- construction and evaluation of logical expression trees;
- truth tables and satisfiability checking;
- Negation Normal Form (NNF) and Conjunctive Normal Form (CNF);
- powersets and set operations;
- Z-order / Z-curve encoding and decoding through bit interleaving.

The formula language uses the following operators:

| Symbol | Meaning | Set-theory equivalent |
| --- | --- | --- |
| `!` | negation | complement |
| `&` | conjunction | intersection |
| `|` | disjunction | union |
| `^` | exclusive disjunction | symmetric difference |
| `>` | implication | logical implication |
| `=` | equivalence | logical equivalence |

Formulas use postfix notation. For example, `AB&` represents `A ∧ B`, while
`AB|C&` represents `(A ∨ B) ∧ C`. Postfix notation makes formulas easy to
parse with a stack and avoids precedence and parenthesis handling.

## Logical tree architecture

From Exercise 03 onward, formulas are represented by `LogicNode`. Exercise 03
starts with a binary expression tree. Later exercises generalize the structure
into an n-ary tree: associative operators can group several operands under the
same node.

The same tree model supports several types of leaves:

- Boolean constants for proposition evaluation;
- named variables for truth tables, NNF, CNF, and SAT;
- variables associated with integer sets for set evaluation.

For direct Boolean evaluation, the `compute` operation handles `^`, `>` and `=`
as exclusive disjunction, implication, and equivalence. Before normalization,
`transformTree` rewrites these operators using the basic logical operators and
removes them from the tree. In particular:

```text
A ^ B  ⇔  (¬A ∨ ¬B) ∧ (A ∨ B)
A > B  ⇔  ¬A ∨ B
A = B  ⇔  (A > B) ∧ (B > A)
```

The resulting tree can then be processed by negation and distribution rules.

The general transformation pipeline is:

```text
postfix formula
      ↓
tree construction
      ↓
remove > and = with transformTree
      ↓
Negation Normal Form
      ↓
tree reduction / distribution
      ↓
Boolean or set evaluation
```

## A small compiler-like project

This project can be viewed as a small compiler-oriented project for logical
expressions. It does not generate machine code, but it implements several core
ideas found in a compiler:

- reading a formal input language written in postfix notation;
- building an abstract syntax tree with `LogicNode`;
- transforming the tree while preserving the meaning of the expression;
- normalizing the representation into NNF or CNF;
- evaluating the final tree in different domains, such as Boolean values or
  integer sets.

The project is therefore closer to a **logical expression interpreter with AST
transformations** than to a complete compiler.

### Example: tree structure

The postfix formula below illustrates the n-ary tree representation:

```text
ABC&&CDE||J!>|
```

Reading it from left to right gives the equivalent infix expression:

```text
(A ∧ B ∧ C) ∨ ((C ∨ D ∨ E) ⇒ ¬J)
```

Its tree can be schematized as follows. The `&` and `|` nodes are shown with
three branches because the implementation groups associative operations:

```text
                                |
                              /   \
                             /     \
                            &       >
                         /  |  \   / \
                        A   B   C  |   !
                                  /|\   \
                                 C D E   J
```

The same expression could also be represented as a strictly binary tree by
introducing parentheses, but the n-ary representation avoids unnecessary nested
nodes for associative operators.

## Exercise-by-exercise analysis

### Exercise 00 — Adder

Adds two natural numbers without using the arithmetic addition operator. The
implementation reproduces a binary adder: XOR produces the result bit, while
AND and OR propagate the carry.

### Exercise 01 — Multiplier

Builds multiplication from addition, shifts, and the bits of the multiplier.
Each active bit selects a shifted partial product, which is then accumulated.

### Exercise 02 — Gray code

Converts an integer to Gray code. Consecutive Gray-code values differ by only one
bit. This exercise reuses the bitwise operations introduced in the first two
exercises.

### Exercise 03 — Boolean evaluation

Parses and evaluates a Boolean formula in postfix notation. Constants become
leaves, operators become tree nodes, and the resulting tree is evaluated
recursively. This is the foundation for the later logic exercises.

### Exercise 04 — Truth table

Extracts the distinct variables from a formula, enumerates every assignment, and
prints the resulting truth table. Gray-code ordering is used to enumerate the
assignments.

### Exercise 05 — Negation Normal Form

Converts a formula to NNF. The result only uses `!`, `&`, and `|`, with each
negation placed directly before a variable. This requires double-negation
elimination, De Morgan's laws, and the rewriting of implication and equivalence.

### Exercise 06 — Conjunctive Normal Form

Converts a formula to CNF: a conjunction of disjunctions, with negations directly
attached to variables. Distribution is the key transformation, and the output
may grow exponentially as allowed by the subject.

### Exercise 07 — SAT

Checks whether at least one assignment of the variables makes the formula true.
The implementation explores the possible assignments and can stop as soon as a
satisfying assignment is found.

### Exercise 08 — Powerset

Builds the powerset `P(A)` of an integer set. For an input containing `n`
elements, the result contains `2^n` subsets.

### Exercise 09 — Set evaluation

Transposes Boolean logic into set theory. Variables represent input sets, and
logical operations become union, intersection, symmetric difference, and
complement.

The universal set for complement operations is the union of all input sets. The
formula is parsed into a tree, variables are associated with their sets, and the
tree is normalized before evaluation. The vector overload of `compute` performs
the set operations, while `solve` returns the resulting set.

### Exercise 10 — Curve

This non-mandatory exercise maps a pair of `uint16_t` coordinates to a value in
`[0, 1]`. The implementation interleaves the bits of `x` and `y` to create a
single integer code, then normalizes that code into the target interval.

### Exercise 11 — Inverse function

This non-mandatory exercise reverses the previous mapping. It extracts the
interleaved bits and reconstructs the original `x` and `y` coordinates.

## Z-curve and Morton order

The mapping used in Exercises 10 and 11 is a discrete **Z-order curve**, also
called a **Morton-order encoding**. It is based on bit interleaving rather than
on a continuous geometric drawing of a curve.

For two coordinates written in binary:

```text
x = x15 x14 ... x1 x0
y = y15 y14 ... y1 y0
```

the encoded Morton value interleaves their bits:

```text
Z(x, y) = y15 x15 y14 x14 ... y1 x1 y0 x0
```

The exact ordering depends on the chosen bit convention; the important property
is that bits from both coordinates are alternated in one integer. Dividing this
integer by the maximum 32-bit value maps it to `[0, 1]`.

This encoding preserves spatial locality reasonably well: points that are close
in the two-dimensional grid tend to have close Morton codes, although the order
also contains jumps when the traversal moves between Z-shaped subregions. The
inverse function recovers the coordinates by taking alternating bits back from
the encoded value.

## Repository layout

```text
.
├── exo00/                 bitwise addition
├── exo01/                 bitwise multiplication
├── exo02/                 Gray code
├── exo03/                 Boolean evaluation and logic tree
├── exo04/                 truth table
├── exo05/                 Negation Normal Form
├── exo06/                 Conjunctive Normal Form
├── exo07/                 satisfiability
├── exo08/                 powerset
├── exo09/                 set evaluation
├── exo10/                 Z-curve encoding
├── exo11/                 inverse Z-curve mapping
└── en.subject.pdf          42 subject
```

Exercises 03–07 and 09 share the `LogicTree`, `Variable`, `Sequence`, and
`Math` components. Exercises 00–02, 08, 10, and 11 are standalone programs.

## Compilation and execution

The exercises with a Makefile can be compiled independently:

```bash
make -C exo03
make -C exo04
make -C exo05
make -C exo06
make -C exo07
make -C exo09
```

The Makefiles produce, respectively, `bool`, `thruth`, `NNF`, `CNF`, `SAT`, and
`evalSet` in their exercise directories.

The standalone exercises can be compiled directly, for example:

```bash
c++ -Wall -Wextra -Werror exo00/main.cpp -o exo00/adder
```

Example invocations:

```bash
./exo03/bool '101|&'
./exo04/thruth 'AB&C|'
./exo05/NNF 'AB&!'
./exo06/CNF 'ABCD&|&'
./exo07/SAT 'AA!&'
./exo08/powerset '1 2 3'
./exo09/evalSet 'AB&' '0 1 2' '0 3 4'
```

## Main concepts

- binary representation and bitwise operations;
- carry propagation and shifts;
- Gray code;
- propositional logic and Boolean algebra;
- Reverse Polish Notation;
- stacks and syntax trees;
- binary and n-ary trees;
- De Morgan's laws, implication, and equivalence;
- NNF, CNF, and distribution;
- truth tables and satisfiability;
- union, intersection, complement, and symmetric difference;
- powersets and cardinality `2^n`;
- Z-order encoding and bit manipulation.

## Cleaning generated files

```bash
make -C exo03 fclean
make -C exo04 fclean
make -C exo05 fclean
make -C exo06 fclean
make -C exo07 fclean
make -C exo09 fclean
```
