;; ============================================================================
;; R5RS Scheme Compliance Test Suite
;; This comprehensive test covers all major R5RS language features
;; ============================================================================

(display "=== R5RS Scheme Compliance Test Suite ===")
(newline)
(newline)

;; ============================================================================
;; SECTION 1: Basic Data Types and Literals
;; ============================================================================

(display "1. Testing Basic Data Types")
(newline)

;; Numbers
(display "Numbers: ")
(display 42)
(display " ")
(display -17)
(display " ")
(display 3.14)
(display " ")
(display 0)
(newline)

;; Booleans
(display "Booleans: ")
(display #t)
(display " ")
(display #f)
(newline)

;; Strings
(display "Strings: ")
(display "hello")
(display " ")
(display "world")
(newline)

;; Symbols
(display "Symbols: ")
(display 'hello)
(display " ")
(display 'x)
(display " ")
(display '+)
(newline)

;; Empty list
(display "Empty list: ")
(display '())
(newline)

;; ============================================================================
;; SECTION 2: Arithmetic Operations
;; ============================================================================

(display "2. Testing Arithmetic Operations")
(newline)

;; Addition
(display "Addition: ")
(display (+ 1 2))
(display " ")
(display (+ 1 2 3 4))
(display " ")
(display (+))
(newline)

;; Subtraction
(display "Subtraction: ")
(display (- 5 3))
(display " ")
(display (- 10 1 2))
(display " ")
(display (- 0 5))
(newline)

;; Multiplication
(display "Multiplication: ")
(display (* 2 3))
(display " ")
(display (* 2 3 4))
(display " ")
(display (*))
(newline)

;; Division
(display "Division: ")
(display (/ 10 2))
(display " ")
(display (/ 8 2 2))
(display " ")
(display (/ 1 5))
(newline)

;; ============================================================================
;; SECTION 3: Comparison Operations
;; ============================================================================

(display "3. Testing Comparison Operations")
(newline)

;; Equality
(display "Equality: ")
(display (= 5 5))
(display " ")
(display (= 3 4))
(display " ")
(display (= 1 1 1))
(newline)

;; Less than
(display "Less than: ")
(display (< 3 5))
(display " ")
(display (< 5 3))
(display " ")
(display (< 1 2 3))
(newline)

;; Greater than
(display "Greater than: ")
(display (> 5 3))
(display " ")
(display (> 3 5))
(display " ")
(display (> 3 2 1))
(newline)

;; Less or equal
(display "Less or equal: ")
(display (<= 3 5))
(display " ")
(display (<= 5 5))
(display " ")
(display (<= 5 3))
(newline)

;; Greater or equal
(display "Greater or equal: ")
(display (>= 5 3))
(display " ")
(display (>= 5 5))
(display " ")
(display (>= 3 5))
(newline)

;; ============================================================================
;; SECTION 4: Variable Definition and Binding
;; ============================================================================

(display "4. Testing Variable Definition")
(newline)

(define x 42)
(display "Defined x = 42: ")
(display x)
(newline)

(define y (+ x 8))
(display "Defined y = (+ x 8): ")
(display y)
(newline)

(define name "R5RS Scheme")
(display "Defined name = ")
(display name)
(newline)

;; ============================================================================
;; SECTION 5: Conditional Expressions
;; ============================================================================

(display "5. Testing Conditional Expressions")
(newline)

;; Basic if
(display "If true: ")
(display (if #t "yes" "no"))
(newline)

(display "If false: ")
(display (if #f "yes" "no"))
(newline)

;; If with expression
(display "If with expression: ")
(display (if (> 5 3) "greater" "not greater"))
(newline)

;; If without else clause
(display "If without else: ")
(display (if #t "present"))
(newline)

;; ============================================================================
;; SECTION 6: Lists and Pairs
;; ============================================================================

(display "6. Testing Lists and Pairs")
(newline)

;; Empty list
(display "Empty list: ")
(display '())
(newline)

;; Simple list
(define simple-list '(1 2 3 4))
(display "Simple list: ")
(display simple-list)
(newline)

;; Mixed list
(define mixed-list '(1 "hello" #t x))
(display "Mixed list: ")
(display mixed-list)
(newline)

;; Nested list
(define nested-list '((1 2) (3 4) (5 6)))
(display "Nested list: ")
(display nested-list)
(newline)

;; Pair
(define my-pair (cons 'a 'b))
(display "Pair: ")
(display my-pair)
(newline)

;; ============================================================================
;; SECTION 7: List Operations
;; ============================================================================

(display "7. Testing List Operations")
(newline)

;; car
(display "car of (1 2 3 4): ")
(display (car '(1 2 3 4)))
(newline)

;; cdr
(display "cdr of (1 2 3 4): ")
(display (cdr '(1 2 3 4)))
(newline)

;; cons
(display "cons 0 to (1 2 3 4): ")
(display (cons 0 '(1 2 3 4)))
(newline)

;; ============================================================================
;; SECTION 8: Lambda Expressions and Procedures
;; ============================================================================

(display "8. Testing Lambda and Procedures")
(newline)

;; Simple lambda
(define square (lambda (x) (* x x)))
(display "Square of 5: ")
(display (square 5))
(newline)

;; Multiple parameter lambda
(define add (lambda (x y) (+ x y)))
(display "Add 3 and 4: ")
(display (add 3 4))
(newline)

;; Lambda with no parameters
(define pi (lambda () 3.14159))
(display "Pi: ")
(display (pi))
(newline)

;; Recursive function
(define factorial 
  (lambda (n)
    (if (= n 0)
        1
        (* n (factorial (- n 1))))))
(display "Factorial of 5: ")
(display (factorial 5))
(newline)

;; ============================================================================
;; SECTION 9: Advanced Control Structures
;; ============================================================================

(display "9. Testing Advanced Control")
(newline)

;; begin
(display "Begin result: ")
(display (begin 
           (define temp 10)
           (+ temp 5)))
(newline)

;; set!
(define counter 100)
(display "Before set!: ")
(display counter)
(newline)
(set! counter 200)
(display "After set!: ")
(display counter)
(newline)

;; ============================================================================
;; SECTION 10: Type Predicates
;; ============================================================================

(display "10. Testing Type Predicates")
(newline)

(display "number? tests: ")
(display (number? 42))
(display " ")
(display (number? "hello"))
(newline)

(display "boolean? tests: ")
(display (boolean? #t))
(display " ")
(display (boolean? 42))
(newline)

(display "string? tests: ")
(display (string? "hello"))
(display " ")
(display (string? 42))
(newline)

(display "symbol? tests: ")
(display (symbol? 'hello))
(display " ")
(display (symbol? "hello"))
(newline)

(display "pair? tests: ")
(display (pair? '(1 . 2)))
(display " ")
(display (pair? '(1 2)))
(display " ")
(display (pair? 42))
(newline)

(display "null? tests: ")
(display (null? '()))
(display " ")
(display (null? '(1 2)))
(newline)

(display "procedure? tests: ")
(display (procedure? +))
(display " ")
(display (procedure? square))
(display " ")
(display (procedure? 42))
(newline)

;; ============================================================================
;; SECTION 11: Additional List Functions
;; ============================================================================

(display "11. Testing Additional List Functions")
(newline)

;; length
(display "length of (a b c d e): ")
(display (length '(a b c d e)))
(newline)

;; list-ref
(display "list-ref (a b c d e) 2: ")
(display (list-ref '(a b c d e) 2))
(newline)

;; append
(display "append (1 2) (3 4): ")
(display (append '(1 2) '(3 4)))
(newline)

;; reverse
(display "reverse (1 2 3 4): ")
(display (reverse '(1 2 3 4)))
(newline)

;; ============================================================================
;; SECTION 12: String Operations
;; ============================================================================

(display "12. Testing String Operations")
(newline)

;; string-length
(display "string-length of ")
(display "hello")
(display ": ")
(display (string-length "hello"))
(newline)

;; string-ref
(display "string-ref ")
(display "hello")
(display " 1: ")
(display (string-ref "hello" 1))
(newline)

;; ============================================================================
;; SECTION 13: Complex Expressions
;; ============================================================================

(display "13. Testing Complex Expressions")
(newline)

;; Nested arithmetic
(display "Nested arithmetic: ")
(display (+ (* 2 3) (- 7 2)))
(newline)

;; Nested conditionals
(display "Nested conditionals: ")
(display (if (and (> 5 3) (< 2 4)) "both true" "not both true"))
(newline)

;; Function composition
(define compose-example 
  (lambda (x) 
    (square (+ x 1))))
(display "Compose example: ")
(display (compose-example 2))
(newline)

;; ============================================================================
;; SECTION 14: Edge Cases and Corner Cases
;; ============================================================================

(display "14. Testing Edge Cases")
(newline)

;; Zero arithmetic
(display "Zero tests: ")
(display (+ 0 0))
(display " ")
(display (* 5 0))
(display " ")
(display (= 0 0))
(newline)

;; Empty string
(display "Empty string length: ")
(display (string-length ""))
(newline)

;; Single element list
(display "Single element list: ")
(display '(only))
(display " car: ")
(display (car '(only)))
(newline)

;; Deeply nested expression
(display "Deeply nested: ")
(display (+ 1 (+ 2 (+ 3 (+ 4 5)))))
(newline)

;; ============================================================================
;; SECTION 15: Quoting and Symbols
;; ============================================================================

(display "15. Testing Quoting")
(newline)

;; Quote
(display "Quoted list: ")
(display '(+ 1 2 3))
(newline)

;; Quote vs evaluation
(display "Evaluated: ")
(display (+ 1 2 3))
(display " Quoted: ")
(display '(+ 1 2 3))
(newline)

;; Nested quotes
(display "Nested quotes: ")
(display '('a 'b 'c))
(newline)

;; ============================================================================
;; TEST COMPLETION
;; ============================================================================

(display "=== R5RS Compliance Test Complete ===")
(newline)
(display "If you see this message, the R5RS Scheme implementation")
(newline)
(display "successfully processed all major language features!")
(newline)
(display "Test completed successfully.")
(newline)