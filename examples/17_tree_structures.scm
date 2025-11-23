; Example 17: Tree Structures
; Binary trees, traversal, and tree operations

; === TREE REPRESENTATION ===
; A tree is either:
; - '() (empty tree/leaf)
; - (value left right) (node with value and two subtrees)

; Create a tree node
(define (make-tree value left right)
  (list value left right))

; Tree accessors
(define (tree-value tree) (car tree))
(define (tree-left tree) (car (cdr tree)))
(define (tree-right tree) (car (cdr (cdr tree))))
(define (tree-empty? tree) (null? tree))

; Example tree:
;       5
;      / \
;     3   8
;    / \   \
;   1   4   9
(define sample-tree
  (make-tree 5
    (make-tree 3
      (make-tree 1 (list) (list))
      (make-tree 4 (list) (list)))
    (make-tree 8
      (list)
      (make-tree 9 (list) (list)))))

(display "Sample tree created: ")
(display sample-tree)
(newline)

; === TREE TRAVERSAL ===

; In-order traversal (left, root, right)
(define (tree-inorder tree)
  (if (tree-empty? tree)
      (list)
      (append (tree-inorder (tree-left tree))
              (cons (tree-value tree)
                    (tree-inorder (tree-right tree))))))

(display "In-order traversal: ")
(display (tree-inorder sample-tree))
(newline)

; Pre-order traversal (root, left, right)
(define (tree-preorder tree)
  (if (tree-empty? tree)
      (list)
      (cons (tree-value tree)
            (append (tree-preorder (tree-left tree))
                    (tree-preorder (tree-right tree))))))

(display "Pre-order traversal: ")
(display (tree-preorder sample-tree))
(newline)

; Post-order traversal (left, right, root)
(define (tree-postorder tree)
  (if (tree-empty? tree)
      (list)
      (append (tree-postorder (tree-left tree))
              (append (tree-postorder (tree-right tree))
                      (list (tree-value tree))))))

(display "Post-order traversal: ")
(display (tree-postorder sample-tree))
(newline)

; === TREE OPERATIONS ===

; Tree size (number of nodes)
(define (tree-size tree)
  (if (tree-empty? tree)
      0
      (+ 1
         (tree-size (tree-left tree))
         (tree-size (tree-right tree)))))

(display "Tree size: ")
(display (tree-size sample-tree))
(newline)

; Tree height (longest path from root to leaf)
(define (tree-height tree)
  (if (tree-empty? tree)
      0
      (+ 1 (max (tree-height (tree-left tree))
                (tree-height (tree-right tree))))))

(define (max a b) (if (> a b) a b))

(display "Tree height: ")
(display (tree-height sample-tree))
(newline)

; Search for value in tree
(define (tree-contains? tree value)
  (if (tree-empty? tree)
      #f
      (if (= (tree-value tree) value)
          #t
          (if (tree-contains? (tree-left tree) value)
              #t
              (tree-contains? (tree-right tree) value)))))

(display "Contains 4? ")
(display (if (tree-contains? sample-tree 4) "yes" "no"))
(newline)

(display "Contains 7? ")
(display (if (tree-contains? sample-tree 7) "yes" "no"))
(newline)

; === BINARY SEARCH TREE ===

; Insert into BST (maintains sorted property)
(define (bst-insert tree value)
  (if (tree-empty? tree)
      (make-tree value (list) (list))
      (if (< value (tree-value tree))
          (make-tree (tree-value tree)
                     (bst-insert (tree-left tree) value)
                     (tree-right tree))
          (make-tree (tree-value tree)
                     (tree-left tree)
                     (bst-insert (tree-right tree) value)))))

; Build BST from list
(define (list-to-bst lst)
  (define (helper lst tree)
    (if (null? lst)
        tree
        (helper (cdr lst) (bst-insert tree (car lst)))))
  (helper lst (list)))

(define bst (list-to-bst (list 5 3 8 1 4 9 2)))

(display "BST from (5 3 8 1 4 9 2), in-order: ")
(display (tree-inorder bst))
(newline)

; Search in BST (more efficient)
(define (bst-search tree value)
  (if (tree-empty? tree)
      #f
      (if (= (tree-value tree) value)
          #t
          (if (< value (tree-value tree))
              (bst-search (tree-left tree) value)
              (bst-search (tree-right tree) value)))))

(display "BST search for 4: ")
(display (if (bst-search bst 4) "found" "not found"))
(newline)

; Find minimum value in BST
(define (bst-min tree)
  (if (tree-empty? (tree-left tree))
      (tree-value tree)
      (bst-min (tree-left tree))))

(display "BST minimum: ")
(display (bst-min bst))
(newline)

; Find maximum value in BST
(define (bst-max tree)
  (if (tree-empty? (tree-right tree))
      (tree-value tree)
      (bst-max (tree-right tree))))

(display "BST maximum: ")
(display (bst-max bst))
(newline)

; === TREE TRANSFORMATIONS ===

; Map function over tree (apply to all values)
(define (tree-map f tree)
  (if (tree-empty? tree)
      (list)
      (make-tree (f (tree-value tree))
                 (tree-map f (tree-left tree))
                 (tree-map f (tree-right tree)))))

(define (double x) (* x 2))
(define doubled-tree (tree-map double sample-tree))

(display "Tree with doubled values: ")
(display (tree-inorder doubled-tree))
(newline)

; Mirror/flip tree (swap left and right)
(define (tree-mirror tree)
  (if (tree-empty? tree)
      (list)
      (make-tree (tree-value tree)
                 (tree-mirror (tree-right tree))
                 (tree-mirror (tree-left tree)))))

(display "Mirrored tree in-order: ")
(display (tree-inorder (tree-mirror sample-tree)))
(newline)
