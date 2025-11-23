; Example 10: Mutation with set!
; Modifying variables (imperative style)

; Basic set!
(define counter 0)

(display "Initial counter: ")
(display counter)
(newline)

(set! counter 10)
(display "After set!: ")
(display counter)
(newline)

(set! counter (+ counter 5))
(display "After increment: ")
(display counter)
(newline)

; Counter function with state
(define make-counter
  (lambda ()
    (let ((count 0))
      (lambda ()
        (set! count (+ count 1))
        count))))

(define my-counter (make-counter))

(display "Counter call 1: ")
(display (my-counter))
(newline)

(display "Counter call 2: ")
(display (my-counter))
(newline)

(display "Counter call 3: ")
(display (my-counter))
(newline)

; Bank account simulation
(define (make-account balance)
  (lambda (amount)
    (set! balance (+ balance amount))
    balance))

(define account (make-account 100))

(display "Initial deposit 100: ")
(display (account 0))
(newline)

(display "Deposit 50: ")
(display (account 50))
(newline)

(display "Withdraw 30: ")
(display (account -30))
(newline)
