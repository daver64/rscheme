; Example 12: Type Predicates
; Testing types of values

; Number predicates
(display "number? 42: ")
(display (if (number? 42) "yes" "no"))
(newline)

(display "number? 'hello: ")
(display (if (number? 'hello) "yes" "no"))
(newline)

; Boolean predicates
(display "boolean? #t: ")
(display (if (boolean? #t) "yes" "no"))
(newline)

(display "boolean? 0: ")
(display (if (boolean? 0) "yes" "no"))
(newline)

; String predicates
(display "string? \"hello\": ")
(display (if (string? "hello") "yes" "no"))
(newline)

; Symbol predicates
(display "symbol? 'foo: ")
(display (if (symbol? 'foo) "yes" "no"))
(newline)

; List predicates
(display "pair? (1 2 3): ")
(display (if (pair? (list 1 2 3)) "yes" "no"))
(newline)

(display "null? (): ")
(display (if (null? (list)) "yes" "no"))
(newline)

(display "null? (1 2): ")
(display (if (null? (list 1 2)) "yes" "no"))
(newline)

; Procedure predicates
(define (my-func x) (* x 2))

(display "procedure? my-func: ")
(display (if (procedure? my-func) "yes" "no"))
(newline)

(display "procedure? 42: ")
(display (if (procedure? 42) "yes" "no"))
(newline)

; Custom type checking
(define (safe-divide a b)
  (if (and (number? a) (number? b) (not (= b 0)))
      (/ a b)
      'error))

(display "Safe divide 10/2: ")
(display (safe-divide 10 2))
(newline)

(display "Safe divide 10/0: ")
(display (safe-divide 10 0))
(newline)
