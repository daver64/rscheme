# RScheme Examples

This directory contains a progressive tutorial series for learning Scheme, from basic concepts to advanced techniques.

## Tutorial Order

### Beginner Level

1. **01_hello_world.scm** - Your first Scheme program
2. **02_basic_arithmetic.scm** - Numbers and arithmetic operations
3. **03_variables.scm** - Defining and using variables
4. **04_conditionals.scm** - Making decisions with if
5. **05_functions.scm** - Creating functions with lambda and define
6. **06_lists.scm** - Working with lists and pairs

### Intermediate Level

7. **07_recursion.scm** - Recursive function definitions
8. **08_higher_order.scm** - Functions as first-class values
9. **09_let_and_scope.scm** - Local bindings and lexical scope
10. **10_mutation.scm** - Mutable state with set!
11. **11_quoting.scm** - Quote, symbols, and code as data
12. **12_predicates.scm** - Type checking and predicates

### Advanced Level

13. **13_closures.scm** - Closures and captured environments
14. **14_tail_recursion.scm** - Efficient recursion with tail calls
15. **15_advanced_patterns.scm** - Currying, memoization, Y combinator, Church numerals, CPS

### Practical Applications

16. **16_list_algorithms.scm** - Sorting (bubble, insertion, merge, quick), searching, list utilities
17. **17_tree_structures.scm** - Binary trees, BST, tree traversal (inorder, preorder, postorder)
18. **18_data_structures.scm** - Stacks, queues, sets, priority queues, association lists
20. **20_math_functions.scm** - GCD, LCM, prime numbers, factorials, Fibonacci, binomial coefficients

## Running Examples

### Interpreter Mode

```bash
./rscheme examples/01_hello_world.scm
```

### Compilation Mode

```bash
./rscheme -c examples/01_hello_world.scm -o hello
./hello
```

### Interactive Exploration

You can also load these examples in the REPL:

```bash
./rscheme
```

Then paste the code from any example file to experiment interactively.

## Learning Path

- **Complete Beginners**: Start with examples 1-6 to understand basic Scheme syntax and operations
- **Programmers New to Functional Programming**: Focus on examples 7-12 to understand recursion and functional patterns
- **Advanced Users**: Jump to examples 13-15 for advanced functional programming techniques

## Additional Resources

- See `r5rs_compliance_test.scm` in the root directory for comprehensive language feature tests
- Check the main `README.md` for compiler and interpreter usage details
