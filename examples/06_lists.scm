; Example 6: Lists and Pairs
; Working with Scheme's fundamental data structure

; Create lists
(define my-list (list 1 2 3 4 5))
(define mixed (list 1 "hello" #t 3.14))

(display "First list: ")
(display my-list)
(newline)

(display "Mixed list: ")
(display mixed)
(newline)

; List operations
(display "First element (car): ")
(display (car my-list))
(newline)

(display "Rest of list (cdr): ")
(display (cdr my-list))
(newline)

; Building lists with cons
(define new-list (cons 0 my-list))
(display "Prepend 0: ")
(display new-list)
(newline)

; List length
(display "Length: ")
(display (length my-list))
(newline)

; Access by index
(display "Element at index 2: ")
(display (list-ref my-list 2))
(newline)

; Append lists
(define combined (append (list 1 2) (list 3 4)))
(display "Appended: ")
(display combined)
(newline)

; Reverse a list
(display "Reversed: ")
(display (reverse my-list))
(newline)

; Empty list
(display "Empty? ")
(display (if (null? (list)) "yes" "no"))
(newline)
