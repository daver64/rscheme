;; Test medium priority fixes

;; Test eqv?
(display "Testing eqv?: ")
(display (eqv? 5 5))
(display " ")
(display (eqv? 5 6))
(display " ")
(display (eqv? #t #t))
(newline)

;; Test list?
(display "Testing list?: ")
(display (list? (quote (1 2 3))))
(display " ")
(display (list? 5))
(display " ")
(display (list? (quote ())))
(newline)

;; Test proper list printing
(display "List printing: ")
(display (quote (1 2 3 4 5)))
(newline)

(display "Nested list: ")
(display (quote ((1 2) (3 4) (5 6))))
(newline)

;; Test improved append with null checks
(display "Append test: ")
(display (append (quote (1 2)) (quote (3 4)) (quote (5 6))))
(newline)

;; Test improved reverse with null checks
(display "Reverse test: ")
(display (reverse (quote (a b c d e))))
(newline)
