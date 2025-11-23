; Example 7: Recursion
; Defining recursive functions

; Factorial function
(define (factorial n)
  (if (<= n 1)
      1
      (* n (factorial (- n 1)))))

(display "Factorial of 5: ")
(display (factorial 5))
(newline)

(display "Factorial of 10: ")
(display (factorial 10))
(newline)

; Fibonacci sequence
(define (fibonacci n)
  (if (<= n 1)
      n
      (+ (fibonacci (- n 1))
         (fibonacci (- n 2)))))

(display "Fibonacci of 8: ")
(display (fibonacci 8))
(newline)

; Sum of list elements
(define (sum-list lst)
  (if (null? lst)
      0
      (+ (car lst) (sum-list (cdr lst)))))

(define numbers (list 1 2 3 4 5))
(display "Sum of ")
(display numbers)
(display ": ")
(display (sum-list numbers))
(newline)

; Count elements in a list
(define (count lst)
  (if (null? lst)
      0
      (+ 1 (count (cdr lst)))))

(display "Count: ")
(display (count numbers))
(newline)

; Find maximum in a list
(define (max-list lst)
  (if (null? (cdr lst))
      (car lst)
      (let ((rest-max (max-list (cdr lst))))
        (if (> (car lst) rest-max)
            (car lst)
            rest-max))))

(display "Maximum: ")
(display (max-list (list 3 7 2 9 1)))
(newline)
