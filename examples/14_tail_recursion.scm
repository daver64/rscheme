; Example 14: Tail Recursion
; Efficient recursion using tail calls

; Non-tail recursive factorial (grows stack)
(define (factorial-normal n)
  (if (<= n 1)
      1
      (* n (factorial-normal (- n 1)))))

; Tail recursive factorial (constant stack)
(define (factorial-tail n)
  (define (helper n acc)
    (if (<= n 1)
        acc
        (helper (- n 1) (* n acc))))
  (helper n 1))

(display "Normal factorial of 10: ")
(display (factorial-normal 10))
(newline)

(display "Tail recursive factorial of 10: ")
(display (factorial-tail 10))
(newline)

; Tail recursive sum
(define (sum-tail lst)
  (define (helper lst acc)
    (if (null? lst)
        acc
        (helper (cdr lst) (+ acc (car lst)))))
  (helper lst 0))

(display "Sum (1 2 3 4 5): ")
(display (sum-tail (list 1 2 3 4 5)))
(newline)

; Tail recursive list length
(define (length-tail lst)
  (define (helper lst acc)
    (if (null? lst)
        acc
        (helper (cdr lst) (+ acc 1))))
  (helper lst 0))

(display "Length of (a b c d e f): ")
(display (length-tail (list 'a 'b 'c 'd 'e 'f)))
(newline)

; Tail recursive reverse
(define (reverse-tail lst)
  (define (helper lst acc)
    (if (null? lst)
        acc
        (helper (cdr lst) (cons (car lst) acc))))
  (helper lst (list)))

(display "Reverse (1 2 3 4 5): ")
(display (reverse-tail (list 1 2 3 4 5)))
(newline)

; Tail recursive range
(define (range-tail start end)
  (define (helper n acc)
    (if (> n end)
        (reverse-tail acc)
        (helper (+ n 1) (cons n acc))))
  (helper start (list)))

(display "Range 1 to 10: ")
(display (range-tail 1 10))
(newline)
