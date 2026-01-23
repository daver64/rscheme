;; Comprehensive test of all bug fixes

;; Test improved list printing
(display "List printing test: ")
(display (quote (1 2 3 4 5)))
(newline)

;; Test nested lists
(display "Nested lists: ")
(display (quote ((a b) (c d) (e f))))
(newline)

;; Test eqv? (medium priority fix)
(display "eqv? test: ")
(display (eqv? 42 42))
(display " ")
(display (eqv? "hello" "hello"))
(newline)

;; Test list? (medium priority fix)
(display "list? test: ")
(display (list? (quote (1 2 3))))
(display " ")
(display (list? 42))
(newline)

;; Test append with null checks
(display "append test: ")
(display (append (quote (1 2)) (quote (3 4)) (quote (5 6))))
(newline)

;; Test reverse with null checks
(display "reverse test: ")
(display (reverse (quote (a b c d e))))
(newline)

;; Test improved error messages (try invalid if)
;; This should produce a better error message if uncommented:
;; (if)

;; Test car/cdr with null checks
(display "car/cdr test: ")
(display (car (quote (1 2 3))))
(display " ")
(display (cdr (quote (1 2 3))))
(newline)

;; Test recursive functions (memory management)
(define factorial 
  (lambda (n)
    (if (= n 0)
        1
        (* n (factorial (- n 1))))))

(display "factorial 5: ")
(display (factorial 5))
(newline)

;; Test proper cleanup with let expressions
(display "let test: ")
(let ((x 10) (y 20))
  (display (+ x y)))
(newline)

(display "All tests completed successfully!")
(newline)
