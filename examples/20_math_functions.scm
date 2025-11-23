; Example 20: Mathematical Functions
; GCD, LCM, prime numbers, and number theory

(display "=== GREATEST COMMON DIVISOR (GCD) ===")
(newline)

; Euclidean algorithm for GCD
(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (modulo a b))))

(display "GCD(48, 18): ")
(display (gcd 48 18))
(newline)

(display "GCD(100, 35): ")
(display (gcd 100 35))
(newline)

(display "GCD(17, 19): ")
(display (gcd 17 19))
(newline)

; === LEAST COMMON MULTIPLE (LCM) ===

(display "=== LEAST COMMON MULTIPLE (LCM) ===")
(newline)

(define (lcm a b)
  (/ (* a b) (gcd a b)))

(display "LCM(12, 18): ")
(display (lcm 12 18))
(newline)

(display "LCM(21, 6): ")
(display (lcm 21 6))
(newline)

; === PRIME NUMBERS ===

(display "=== PRIME NUMBERS ===")
(newline)

; Check if number is prime
(define (prime? n)
  (define (test-divisor d)
    (if (> (* d d) n)
        #t
        (if (= (modulo n d) 0)
            #f
            (test-divisor (+ d 1)))))
  (if (< n 2)
      #f
      (test-divisor 2)))

(display "Is 17 prime? ")
(display (if (prime? 17) "yes" "no"))
(newline)

(display "Is 18 prime? ")
(display (if (prime? 18) "yes" "no"))
(newline)

(display "Is 97 prime? ")
(display (if (prime? 97) "yes" "no"))
(newline)

; Generate list of primes up to n
(define (primes-up-to n)
  (define (helper i acc)
    (if (> i n)
        (reverse acc)
        (if (prime? i)
            (helper (+ i 1) (cons i acc))
            (helper (+ i 1) acc))))
  (helper 2 (list)))

(display "Primes up to 30: ")
(display (primes-up-to 30))
(newline)

; === FACTORIAL ===

(display "=== FACTORIAL ===")
(newline)

(define (factorial n)
  (if (<= n 1)
      1
      (* n (factorial (- n 1)))))

(display "Factorial(6): ")
(display (factorial 6))
(newline)

(display "Factorial(10): ")
(display (factorial 10))
(newline)

; === FIBONACCI ===

(display "=== FIBONACCI NUMBERS ===")
(newline)

; Efficient Fibonacci with tail recursion
(define (fibonacci n)
  (define (fib-iter a b count)
    (if (= count 0)
        a
        (fib-iter b (+ a b) (- count 1))))
  (fib-iter 0 1 n))

(display "Fibonacci(10): ")
(display (fibonacci 10))
(newline)

(display "Fibonacci(15): ")
(display (fibonacci 15))
(newline)

; First n Fibonacci numbers
(define (fibonacci-sequence n)
  (define (helper i acc)
    (if (= i n)
        (reverse acc)
        (helper (+ i 1) (cons (fibonacci i) acc))))
  (helper 0 (list)))

(display "First 10 Fibonacci: ")
(display (fibonacci-sequence 10))
(newline)

; === EXPONENTIATION ===

(display "=== EXPONENTIATION ===")
(newline)

; Simple exponentiation
(define (power base exp)
  (if (= exp 0)
      1
      (* base (power base (- exp 1)))))

(display "2^10: ")
(display (power 2 10))
(newline)

(display "3^5: ")
(display (power 3 5))
(newline)

; === DIVISIBILITY AND FACTORS ===

(display "=== DIVISORS ===")
(newline)

; Find all divisors of n
(define (divisors n)
  (define (helper d acc)
    (if (> d n)
        (reverse acc)
        (if (= (modulo n d) 0)
            (helper (+ d 1) (cons d acc))
            (helper (+ d 1) acc))))
  (helper 1 (list)))

(display "Divisors of 24: ")
(display (divisors 24))
(newline)

(display "Divisors of 36: ")
(display (divisors 36))
(newline)

; Sum of divisors
(define (sum-divisors n)
  (define (sum-list lst)
    (if (null? lst)
        0
        (+ (car lst) (sum-list (cdr lst)))))
  (sum-list (divisors n)))

(display "Sum of divisors of 12: ")
(display (sum-divisors 12))
(newline)

; === PERFECT NUMBERS ===

(display "=== PERFECT NUMBERS ===")
(newline)

; A perfect number equals the sum of its proper divisors
(define (perfect? n)
  (= n (- (sum-divisors n) n)))

(display "Is 6 perfect? ")
(display (if (perfect? 6) "yes" "no"))
(newline)

(display "Is 28 perfect? ")
(display (if (perfect? 28) "yes" "no"))
(newline)

(display "Is 12 perfect? ")
(display (if (perfect? 12) "yes" "no"))
(newline)

; === BINOMIAL COEFFICIENT ===

(display "=== BINOMIAL COEFFICIENT ===")
(newline)

; C(n,k) = n! / (k! * (n-k)!)
(define (binomial n k)
  (/ (factorial n)
     (* (factorial k) (factorial (- n k)))))

(display "C(5,2): ")
(display (binomial 5 2))
(newline)

(display "C(10,3): ")
(display (binomial 10 3))
(newline)

; === ABSOLUTE VALUE AND SIGN ===

(display "=== ABSOLUTE VALUE ===")
(newline)

(define (abs x)
  (if (< x 0) (- x) x))

(display "abs(-15): ")
(display (abs -15))
(newline)

(display "abs(20): ")
(display (abs 20))
(newline)

; === INTEGER SQUARE ROOT ===

(display "=== INTEGER SQUARE ROOT ===")
(newline)

; Find integer square root using Newton's method
(define (isqrt n)
  (define (good-enough? guess)
    (<= (abs (- (* guess guess) n)) 1))
  (define (improve guess)
    (/ (+ guess (/ n guess)) 2))
  (define (sqrt-iter guess)
    (if (good-enough? guess)
        (/ guess 1)  ; Convert to integer
        (sqrt-iter (improve guess))))
  (if (<= n 0)
      0
      (sqrt-iter 1)))

(display "isqrt(25): ")
(display (isqrt 25))
(newline)

(display "isqrt(50): ")
(display (isqrt 50))
(newline)

(display "isqrt(100): ")
(display (isqrt 100))
(newline)
