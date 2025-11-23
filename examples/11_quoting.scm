; Example 11: Quoting and Symbols
; Understanding quote and symbols

; Quote prevents evaluation
(display "Evaluated expression: ")
(display (+ 1 2 3))
(newline)

(display "Quoted expression: ")
(display (quote (+ 1 2 3)))
(newline)

; Shorthand with '
(display "Quoted list: ")
(display '(a b c d))
(newline)

; Symbols
(define sym 'hello)
(display "Symbol: ")
(display sym)
(newline)

; Type checking
(display "Is 'hello a symbol? ")
(display (if (symbol? 'hello) "yes" "no"))
(newline)

(display "Is 123 a symbol? ")
(display (if (symbol? 123) "yes" "no"))
(newline)

; Quoted lists preserve structure
(define data '(name John age 30 city Boston))
(display "Data structure: ")
(display data)
(newline)

; Building code as data
(define code '(define (double x) (* x 2)))
(display "Code as data: ")
(display code)
(newline)

; Nested quotes
(display "Nested: ")
(display '(a (b c) d))
(newline)
