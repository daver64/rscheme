;; Test specific R5RS compliance issues
(display "Testing missing R5RS functions:")
(newline)

;; Test list-ref
(display "list-ref test: ")
(display (list-ref '(a b c d e) 2))
(display " (should be c)")
(newline)

;; Test append  
(display "append test: ")
(display (append '(1 2) '(3 4)))
(display " (should be (1 2 3 4))")
(newline)

;; Test reverse
(display "reverse test: ")
(display (reverse '(1 2 3 4)))
(display " (should be (4 3 2 1))")
(newline)

;; Test procedure? on lambda
(display "procedure? on lambda: ")
(display (procedure? (lambda (x) (* x x))))
(display " (should be #t)")
(newline)