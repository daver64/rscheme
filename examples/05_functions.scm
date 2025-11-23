; Example 5: Functions (Lambda and Define)
; Creating and using functions

; Define a simple function
(define (square x)
  (* x x))

(display "Square of 5: ")
(display (square 5))
(newline)

; Function with multiple parameters
(define (add a b)
  (+ a b))

(display "3 + 4 = ")
(display (add 3 4))
(newline)

; Using lambda directly
(define double (lambda (x) (* x 2)))

(display "Double of 7: ")
(display (double 7))
(newline)

; Function that returns a function
(define (make-adder n)
  (lambda (x) (+ x n)))

(define add10 (make-adder 10))
(display "Add 10 to 5: ")
(display (add10 5))
(newline)

; More complex function
(define (absolute x)
  (if (< x 0)
      (- x)
      x))

(display "Absolute of -15: ")
(display (absolute -15))
(newline)
