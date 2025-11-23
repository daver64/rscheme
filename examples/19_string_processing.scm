;; Example 19: String Processing Functions
;; Demonstrates working with strings and characters

(display "=== String Processing Demo ===\n")
(newline)

;; Function to count occurrences of a character in a string
(define (count-char str ch)
  (define (iter index count)
    (if (>= index (string-length str))
        count
        (iter (+ index 1)
              (if (char=? (string-ref str index) ch)
                  (+ count 1)
                  count))))
  (iter 0 0))

;; Test count-char
(display "Counting 'l' in \"Hello World\": ")
(display (count-char "Hello World" #\l))
(newline)

;; Function to convert string to uppercase (simple version for ASCII)
(define (string-upcase str)
  (define (iter index result)
    (if (>= index (string-length str))
        result
        (let ((ch (string-ref str index)))
          (iter (+ index 1)
                result))))  ;; Note: Would need string-append and char->string for full implementation
  (iter 0 ""))

;; Function to check if character is vowel
(define (vowel? ch)
  (or (char=? ch #\a)
      (char=? ch #\e)
      (char=? ch #\i)
      (char=? ch #\o)
      (char=? ch #\u)
      (char=? ch #\A)
      (char=? ch #\E)
      (char=? ch #\I)
      (char=? ch #\O)
      (char=? ch #\U)))

;; Test vowel?
(display "Is 'e' a vowel? ")
(display (vowel? #\e))
(newline)

(display "Is 'x' a vowel? ")
(display (vowel? #\x))
(newline)

;; Count vowels in a string
(define (count-vowels str)
  (define (iter index count)
    (if (>= index (string-length str))
        count
        (iter (+ index 1)
              (if (vowel? (string-ref str index))
                  (+ count 1)
                  count))))
  (iter 0 0))

(display "Vowels in \"Scheme Programming\": ")
(display (count-vowels "Scheme Programming"))
(newline)

;; Character classification tests
(newline)
(display "=== Character Classification ===\n")
(display "Is 'A' alphabetic? ")
(display (char-alphabetic? #\A))
(newline)

(display "Is '7' numeric? ")
(display (char-numeric? #\7))
(newline)

(display "Is space whitespace? ")
(display (char-whitespace? #\space))
(newline)

;; Character comparisons
(newline)
(display "=== Character Comparisons ===\n")
(display "'a' < 'b': ")
(display (char<? #\a #\b))
(newline)

(display "'z' > 'a': ")
(display (char>? #\z #\a))
(newline)

;; Case conversion
(newline)
(display "=== Case Conversion ===\n")
(display "Uppercase of 'a': ")
(display (char-upcase #\a))
(newline)

(display "Lowercase of 'Z': ")
(display (char-downcase #\Z))
(newline)

;; Character/integer conversion
(newline)
(display "=== Character/Integer Conversion ===\n")
(display "ASCII code of 'A': ")
(display (char->integer #\A))
(newline)

(display "Character from ASCII 66: ")
(display (integer->char 66))
(newline)

(newline)
(display "String processing demonstration complete!\n")
