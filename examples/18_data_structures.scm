; Example 18: Data Structures
; Implementing stacks, queues, and sets in pure Scheme

; === STACK (LIFO - Last In, First Out) ===

(display "=== STACK ===")
(newline)

; Stack operations using lists
(define (make-stack) (list))
(define (stack-empty? s) (null? s))
(define (stack-push s item) (cons item s))
(define (stack-pop s) (cdr s))
(define (stack-peek s) (car s))

(define my-stack (make-stack))
(set! my-stack (stack-push my-stack 10))
(set! my-stack (stack-push my-stack 20))
(set! my-stack (stack-push my-stack 30))

(display "Stack after pushing 10, 20, 30: ")
(display my-stack)
(newline)

(display "Peek: ")
(display (stack-peek my-stack))
(newline)

(set! my-stack (stack-pop my-stack))
(display "After pop: ")
(display my-stack)
(newline)

; === QUEUE (FIFO - First In, First Out) ===

(display "=== QUEUE ===")
(newline)

; Queue represented as a pair of lists (front, rear)
(define (make-queue) (cons (list) (list)))
(define (queue-empty? q) (null? (car q)))

(define (queue-enqueue q item)
  (cons (car q) (cons item (cdr q))))

(define (queue-dequeue q)
  (if (null? (cdr (car q)))
      ; Need to reverse rear to front
      (if (null? (cdr q))
          q
          (cons (cdr (reverse (cdr q))) (list)))
      ; Just pop from front
      (cons (cdr (car q)) (cdr q))))

(define (queue-front q)
  (if (null? (car q))
      (if (null? (cdr q))
          'empty
          (car (reverse (cdr q))))
      (car (car q))))

; Simpler queue using just a list (less efficient)
(define (simple-queue-enqueue q item)
  (append q (list item)))

(define (simple-queue-dequeue q)
  (cdr q))

(define (simple-queue-front q)
  (car q))

(define my-queue (list))
(set! my-queue (simple-queue-enqueue my-queue 1))
(set! my-queue (simple-queue-enqueue my-queue 2))
(set! my-queue (simple-queue-enqueue my-queue 3))

(display "Queue after enqueuing 1, 2, 3: ")
(display my-queue)
(newline)

(display "Front: ")
(display (simple-queue-front my-queue))
(newline)

(set! my-queue (simple-queue-dequeue my-queue))
(display "After dequeue: ")
(display my-queue)
(newline)

; === SET (Unique Elements) ===

(display "=== SET ===")
(newline)

; Set operations
(define (make-set) (list))

(define (set-contains? s item)
  (if (null? s)
      #f
      (if (= (car s) item)
          #t
          (set-contains? (cdr s) item))))

(define (set-add s item)
  (if (set-contains? s item)
      s
      (cons item s)))

(define (set-remove s item)
  (if (null? s)
      (list)
      (if (= (car s) item)
          (cdr s)
          (cons (car s) (set-remove (cdr s) item)))))

(define (set-union s1 s2)
  (if (null? s1)
      s2
      (set-union (cdr s1) (set-add s2 (car s1)))))

(define (set-intersection s1 s2)
  (if (null? s1)
      (list)
      (if (set-contains? s2 (car s1))
          (cons (car s1) (set-intersection (cdr s1) s2))
          (set-intersection (cdr s1) s2))))

(define (set-difference s1 s2)
  (if (null? s1)
      (list)
      (if (set-contains? s2 (car s1))
          (set-difference (cdr s1) s2)
          (cons (car s1) (set-difference (cdr s1) s2)))))

(define set1 (make-set))
(set! set1 (set-add set1 1))
(set! set1 (set-add set1 2))
(set! set1 (set-add set1 3))

(define set2 (make-set))
(set! set2 (set-add set2 2))
(set! set2 (set-add set2 3))
(set! set2 (set-add set2 4))

(display "Set1: ")
(display set1)
(newline)

(display "Set2: ")
(display set2)
(newline)

(display "Contains 2? ")
(display (if (set-contains? set1 2) "yes" "no"))
(newline)

(display "Union: ")
(display (set-union set1 set2))
(newline)

(display "Intersection: ")
(display (set-intersection set1 set2))
(newline)

(display "Difference (set1 - set2): ")
(display (set-difference set1 set2))
(newline)

; === PRIORITY QUEUE (Simple implementation) ===

(display "=== PRIORITY QUEUE ===")
(newline)

; Items are pairs (priority . value)
(define (make-pqueue) (list))

(define (pqueue-insert pq priority value)
  (define (insert-sorted lst)
    (if (null? lst)
        (list (cons priority value))
        (if (< priority (car (car lst)))
            (cons (cons priority value) lst)
            (cons (car lst) (insert-sorted (cdr lst))))))
  (insert-sorted pq))

(define (pqueue-extract-min pq)
  (if (null? pq)
      'empty
      (cdr pq)))

(define (pqueue-peek pq)
  (if (null? pq)
      'empty
      (cdr (car pq))))

(define my-pqueue (make-pqueue))
(set! my-pqueue (pqueue-insert my-pqueue 3 'low))
(set! my-pqueue (pqueue-insert my-pqueue 1 'high))
(set! my-pqueue (pqueue-insert my-pqueue 2 'medium))

(display "Priority queue: ")
(display my-pqueue)
(newline)

(display "Peek (highest priority): ")
(display (pqueue-peek my-pqueue))
(newline)

; === ASSOCIATION LIST (Key-Value Store) ===

(display "=== ASSOCIATION LIST ===")
(newline)

(define (make-alist) (list))

(define (alist-get alist key)
  (if (null? alist)
      'not-found
      (if (= (car (car alist)) key)
          (cdr (car alist))
          (alist-get (cdr alist) key))))

(define (alist-put alist key value)
  (cons (cons key value) alist))

(define my-alist (make-alist))
(set! my-alist (alist-put my-alist 1 'one))
(set! my-alist (alist-put my-alist 2 'two))
(set! my-alist (alist-put my-alist 3 'three))

(display "Association list: ")
(display my-alist)
(newline)

(display "Get key 2: ")
(display (alist-get my-alist 2))
(newline)

(display "Get key 5: ")
(display (alist-get my-alist 5))
(newline)
