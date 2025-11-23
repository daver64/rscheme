; Example 4: Conditional Expressions
; Using if to make decisions

(define age 25)

; Basic if expression
(display "Age check: ")
(display (if (> age 18) "Adult" "Minor"))
(newline)

; Comparison operators
(define a 10)
(define b 20)

(display "a = b? ")
(display (if (= a b) "yes" "no"))
(newline)

(display "a < b? ")
(display (if (< a b) "yes" "no"))
(newline)

(display "a > b? ")
(display (if (> a b) "yes" "no"))
(newline)

; Nested conditionals
(define score 85)
(display "Grade: ")
(display 
  (if (>= score 90)
      "A"
      (if (>= score 80)
          "B"
          (if (>= score 70)
              "C"
              "F"))))
(newline)
