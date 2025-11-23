; Example 3: Variables and Definitions
; Using define to create variables

; Define simple values
(define x 42)
(define y 10)
(define name "Alice")

(display "x = ")
(display x)
(newline)

(display "y = ")
(display y)
(newline)

(display "name = ")
(display name)
(newline)

; Use variables in expressions
(define sum (+ x y))
(display "x + y = ")
(display sum)
(newline)

; Variables can reference other variables
(define doubled (* x 2))
(display "x * 2 = ")
(display doubled)
(newline)
