; Example 16: List Algorithms
; Sorting, searching, and list utilities

; === SEARCHING ===

; Linear search
(define (find pred lst)
  (if (null? lst)
      #f
      (if (pred (car lst))
          (car lst)
          (find pred (cdr lst)))))

(display "Find even in (1 3 5 8 9): ")
(define (even? n) (= (- n (* 2 (/ n 2))) 0))
(display (find even? (list 1 3 5 8 9)))
(newline)

; Find index of element
(define (index-of x lst)
  (define (helper lst i)
    (if (null? lst)
        -1
        (if (= (car lst) x)
            i
            (helper (cdr lst) (+ i 1)))))
  (helper lst 0))

(display "Index of 7 in (3 5 7 9): ")
(display (index-of 7 (list 3 5 7 9)))
(newline)

; === SORTING ===

; Bubble sort (simple but inefficient)
(define (bubble-sort lst)
  (define (bubble-pass lst)
    (if (null? (cdr lst))
        lst
        (if (> (car lst) (car (cdr lst)))
            (cons (car (cdr lst))
                  (bubble-pass (cons (car lst) (cdr (cdr lst)))))
            (cons (car lst) (bubble-pass (cdr lst))))))
  (define (sorted? lst)
    (if (null? (cdr lst))
        #t
        (if (> (car lst) (car (cdr lst)))
            #f
            (sorted? (cdr lst)))))
  (if (sorted? lst)
      lst
      (bubble-sort (bubble-pass lst))))

(display "Bubble sort (5 2 8 1 9): ")
(display (bubble-sort (list 5 2 8 1 9)))
(newline)

; Insertion sort
(define (insertion-sort lst)
  (define (insert x sorted)
    (if (null? sorted)
        (list x)
        (if (< x (car sorted))
            (cons x sorted)
            (cons (car sorted) (insert x (cdr sorted))))))
  (define (helper unsorted sorted)
    (if (null? unsorted)
        sorted
        (helper (cdr unsorted) (insert (car unsorted) sorted))))
  (helper lst (list)))

(display "Insertion sort (7 3 9 1 4): ")
(display (insertion-sort (list 7 3 9 1 4)))
(newline)

; Merge sort (efficient divide-and-conquer)
(define (merge-sort lst)
  (define (merge left right)
    (if (null? left)
        right
        (if (null? right)
            left
            (if (< (car left) (car right))
                (cons (car left) (merge (cdr left) right))
                (cons (car right) (merge left (cdr right)))))))
  (define (split lst)
    (define (helper lst left right toggle)
      (if (null? lst)
          (cons left right)
          (if toggle
              (helper (cdr lst) (cons (car lst) left) right #f)
              (helper (cdr lst) left (cons (car lst) right) #t))))
    (helper lst (list) (list) #t))
  (if (null? lst)
      (list)
      (if (null? (cdr lst))
          lst
          (let ((halves (split lst)))
            (merge (merge-sort (car halves))
                   (merge-sort (cdr halves)))))))

(display "Merge sort (9 4 7 2 1 8 3): ")
(display (merge-sort (list 9 4 7 2 1 8 3)))
(newline)

; Quick sort
(define (quick-sort lst)
  (if (null? lst)
      (list)
      (let ((pivot (car lst))
            (rest (cdr lst)))
        (define (partition lst less equal greater)
          (if (null? lst)
              (cons less (cons equal greater))
              (if (< (car lst) pivot)
                  (partition (cdr lst) (cons (car lst) less) equal greater)
                  (if (= (car lst) pivot)
                      (partition (cdr lst) less (cons (car lst) equal) greater)
                      (partition (cdr lst) less equal (cons (car lst) greater))))))
        (let ((parts (partition rest (list) (list pivot) (list))))
          (let ((less (car parts))
                (equal (car (cdr parts)))
                (greater (cdr (cdr parts))))
            (append (quick-sort less)
                    (append equal (quick-sort greater))))))))

(display "Quick sort (6 2 9 4 1 8 3): ")
(display (quick-sort (list 6 2 9 4 1 8 3)))
(newline)

; === LIST UTILITIES ===

; Remove duplicates
(define (unique lst)
  (define (contains? x lst)
    (if (null? lst)
        #f
        (if (= x (car lst))
            #t
            (contains? x (cdr lst)))))
  (define (helper lst acc)
    (if (null? lst)
        (reverse acc)
        (if (contains? (car lst) acc)
            (helper (cdr lst) acc)
            (helper (cdr lst) (cons (car lst) acc)))))
  (helper lst (list)))

(display "Remove duplicates (1 2 2 3 1 4 3): ")
(display (unique (list 1 2 2 3 1 4 3)))
(newline)

; Zip two lists together
(define (zip lst1 lst2)
  (if (null? lst1)
      (list)
      (if (null? lst2)
          (list)
          (cons (cons (car lst1) (car lst2))
                (zip (cdr lst1) (cdr lst2))))))

(display "Zip (1 2 3) with (a b c): ")
(display (zip (list 1 2 3) (list 'a 'b 'c)))
(newline)

; Take first n elements
(define (take n lst)
  (if (= n 0)
      (list)
      (if (null? lst)
          (list)
          (cons (car lst) (take (- n 1) (cdr lst))))))

(display "Take 3 from (1 2 3 4 5): ")
(display (take 3 (list 1 2 3 4 5)))
(newline)

; Drop first n elements
(define (drop n lst)
  (if (= n 0)
      lst
      (if (null? lst)
          (list)
          (drop (- n 1) (cdr lst)))))

(display "Drop 2 from (1 2 3 4 5): ")
(display (drop 2 (list 1 2 3 4 5)))
(newline)

; Flatten nested lists
(define (flatten lst)
  (if (null? lst)
      (list)
      (if (pair? (car lst))
          (append (flatten (car lst)) (flatten (cdr lst)))
          (cons (car lst) (flatten (cdr lst))))))

(display "Flatten ((1 2) (3 (4 5)) 6): ")
(display (flatten (list (list 1 2) (list 3 (list 4 5)) 6)))
(newline)
