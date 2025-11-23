; Example 9: Let Bindings and Scope
; Local variables and lexical scoping

; Basic let
(define x 100)

(display "Outer x: ")
(display x)
(newline)

(let ((x 10)
      (y 20))
  (display "Inside let, x: ")
  (display x)
  (newline)
  (display "Inside let, y: ")
  (display y)
  (newline)
  (display "Sum inside let: ")
  (display (+ x y))
  (newline))

(display "After let, x: ")
(display x)
(newline)

; Nested let
(let ((a 5))
  (let ((b 10))
    (let ((c (+ a b)))
      (display "Nested result: ")
      (display c)
      (newline))))

; Let for computing intermediate values
(define (quadratic a b c x)
  (let ((ax2 (* a (* x x)))
        (bx (* b x)))
    (+ ax2 (+ bx c))))

(display "Quadratic 2x^2 + 3x + 1 at x=4: ")
(display (quadratic 2 3 1 4))
(newline)

; Using inner define for recursive helper
(define (factorial-let n)
  (define (helper n acc)
    (if (<= n 1)
        acc
        (helper (- n 1) (* n acc))))
  (helper n 1))

(display "Factorial via let: ")
(display (factorial-let 6))
(newline)
