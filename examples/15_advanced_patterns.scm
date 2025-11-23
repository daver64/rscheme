; Example 15: Advanced Patterns
; Complex programming patterns in Scheme

; Currying - converting multi-argument function to chain of single-argument functions
(define (curry2 f)
  (lambda (x)
    (lambda (y)
      (f x y))))

(define curried-add (curry2 +))
(define add5 (curried-add 5))

(display "Curried add 5 + 3: ")
(display (add5 3))
(newline)

; Memoization - caching function results (simplified without assoc)
(define (make-memo f)
  (let ((cache-keys (list))
        (cache-values (list)))
    (lambda (n)
      (define (find-in-cache keys vals)
        (if (null? keys)
            #f
            (if (= (car keys) n)
                (car vals)
                (find-in-cache (cdr keys) (cdr vals)))))
      (let ((cached (find-in-cache cache-keys cache-values)))
        (if cached
            (begin
              (display "(cached) ")
              cached)
            (let ((result (f n)))
              (set! cache-keys (cons n cache-keys))
              (set! cache-values (cons result cache-values))
              result))))))

(define slow-fib
  (lambda (n)
    (if (<= n 1)
        n
        (+ (slow-fib (- n 1))
           (slow-fib (- n 2))))))

(define fast-fib (make-memo slow-fib))

(display "Fibonacci 10 (with memo): ")
(display (fast-fib 10))
(newline)

(display "Fibonacci 10 again: ")
(display (fast-fib 10))
(newline)

; Fixed-point combinator (Y combinator)
(define Y
  (lambda (f)
    ((lambda (x) (f (lambda (y) ((x x) y))))
     (lambda (x) (f (lambda (y) ((x x) y)))))))

; Using Y combinator for factorial
(define factorial-y
  (Y (lambda (f)
       (lambda (n)
         (if (<= n 1)
             1
             (* n (f (- n 1))))))))

(display "Factorial via Y combinator: ")
(display (factorial-y 5))
(newline)

; Church numerals (representing numbers as functions)
(define zero (lambda (f) (lambda (x) x)))
(define one (lambda (f) (lambda (x) (f x))))
(define two (lambda (f) (lambda (x) (f (f x)))))

(define (church-to-int n)
  ((n (lambda (x) (+ x 1))) 0))

(display "Church zero: ")
(display (church-to-int zero))
(newline)

(display "Church one: ")
(display (church-to-int one))
(newline)

(display "Church two: ")
(display (church-to-int two))
(newline)

; Continuation-passing style (CPS)
(define (factorial-cps n k)
  (if (<= n 1)
      (k 1)
      (factorial-cps (- n 1)
                     (lambda (v) (k (* n v))))))

(display "Factorial CPS of 5: ")
(factorial-cps 5 (lambda (result) (display result) (newline)))

; List comprehension pattern
(define (list-comp expr pred lst)
  (define (helper lst acc)
    (if (null? lst)
        (reverse acc)
        (if (pred (car lst))
            (helper (cdr lst) (cons (expr (car lst)) acc))
            (helper (cdr lst) acc))))
  (helper lst (list)))

(define (square x) (* x x))
(define (even? n) (= (- n (* 2 (/ n 2))) 0))

(display "Squares of even numbers from (1 2 3 4 5 6): ")
(display (list-comp square even? (list 1 2 3 4 5 6)))
(newline)
