; Example 8: Higher-Order Functions
; Functions that take or return other functions

; Map - apply a function to each element
(define (map f lst)
  (if (null? lst)
      (list)
      (cons (f (car lst))
            (map f (cdr lst)))))

(define (square x) (* x x))

(display "Map square over (1 2 3 4): ")
(display (map square (list 1 2 3 4)))
(newline)

; Filter - keep elements that satisfy a predicate
(define (filter pred lst)
  (if (null? lst)
      (list)
      (if (pred (car lst))
          (cons (car lst) (filter pred (cdr lst)))
          (filter pred (cdr lst)))))

(define (even? n) (= (- n (* 2 (/ n 2))) 0))

(display "Filter even from (1 2 3 4 5 6): ")
(display (filter even? (list 1 2 3 4 5 6)))
(newline)

; Reduce/fold - combine elements
(define (fold f init lst)
  (if (null? lst)
      init
      (f (car lst) (fold f init (cdr lst)))))

(display "Sum via fold: ")
(display (fold + 0 (list 1 2 3 4 5)))
(newline)

; Function composition
(define (compose f g)
  (lambda (x) (f (g x))))

(define (add1 x) (+ x 1))
(define add1-then-square (compose square add1))

(display "Add 1 then square 4: ")
(display (add1-then-square 4))
(newline)

; Apply a function n times
(define (apply-n f n x)
  (if (<= n 0)
      x
      (apply-n f (- n 1) (f x))))

(display "Add 1 five times to 10: ")
(display (apply-n add1 5 10))
(newline)
