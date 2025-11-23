; Example 13: Closures and Lexical Scoping
; Functions that capture their environment

; Simple closure
(define (make-multiplier factor)
  (lambda (x) (* x factor)))

(define times3 (make-multiplier 3))
(define times5 (make-multiplier 5))

(display "3 * 7 = ")
(display (times3 7))
(newline)

(display "5 * 7 = ")
(display (times5 7))
(newline)

; Closure with multiple captured variables
(define (make-linear a b)
  (lambda (x) (+ (* a x) b)))

(define line1 (make-linear 2 3))  ; 2x + 3
(define line2 (make-linear -1 10)) ; -x + 10

(display "Line 2x+3 at x=5: ")
(display (line1 5))
(newline)

(display "Line -x+10 at x=5: ")
(display (line2 5))
(newline)

; Counter with increment
(define (make-resettable-counter)
  (let ((count 0))
    (lambda ()
      (set! count (+ count 1))
      count)))

(define counter (make-resettable-counter))

(display "Increment: ")
(display (counter))
(newline)

(display "Increment: ")
(display (counter))
(newline)

(display "Increment: ")
(display (counter))
(newline)

; Calculator with memory
(define (make-calculator initial)
  (let ((memory initial))
    (lambda (amount)
      (set! memory (+ memory amount))
      memory)))

(define calc (make-calculator 0))

(display "Add 10: ")
(display (calc 10))
(newline)

(display "Add 5: ")
(display (calc 5))
(newline)

(display "Subtract 3: ")
(display (calc -3))
(newline)

(display "Current value: ")
(display (calc 0))
(newline)
