#lang racket

;receives input n in which a 2^n by 2^n courtyard is established and tiled; r is row of statue and c is column of statue

(define (main-function n r c) (tiling (initialize n r c 1) (expt 2 n) 1))

;creates the courtyard, with statue located at row r and column c; s is number of statue and n is the expotent of two that defines the dimensions of the courtyard.
(define (initialize n r c s) (change-index '() r c s (all-rows (expt 2 n) 0)))

;creates one row of length n, filled in by zeros.
(define (one-row n) (cond ((equal? n 0) '()) (else (cons 0 (one-row (- n 1))))))

;creates a two by two grid with dimensions n; a should always be 0
(define (all-rows n a) (cond ((equal? n 0) '()) (else (cons (one-row (+ n a)) (all-rows (- n 1) (+ a 1))))))

;r is the row, c is the column, s is value to change to, L is a two dimensional list and accum is '()
(define (change-index accum r c s L) (cond ((equal? 0 r) (cond ((equal? accum '()) (cons (change '() c s (car L)) (cdr L))) (else (append (reverse accum) (cons (change '() c s (car L)) (cdr L)))))) (else (change-index (cons (car L) accum) (- r 1) c s (cdr L)))))

;changes index b to the value of s in the row L; accum is always '()
(define (change accum b s L) (cond ((equal? 0 b) (cond ((equal? accum '()) (cons s (cdr L))) (else (append (reverse accum) (cons s (cdr L)))))) (else (change (cons (car L) accum) (- b 1) s (cdr L)))))

;determines what quadrant the statue is in. L is the courtyard, n is its dimensions, and s is the value of the statue
(define (statue L n s) (cond ((< (car (index-statue L 0 s)) (/ n 2)) (cond ((< (car (cdr (index-statue L 0 s))) (/ n 2)) 2) (else 1))) (else (cond ((< (car (cdr (index-statue L 0 s))) (/ n 2)) 3) (else 4)))))

;determines if s is in row r.
(define (in-row? s r) (cond ((equal? r '()) #f) ((equal? (car r) 0) (in-row? s (cdr r))) (else #t)))

;determines column of an element in a row, assuming the element is in the row, a always begins as zero; l is the row, s is the value of the statue.
(define (index s l a) (cond ((equal? (car l) 0) (index s (cdr l) (+ a 1))) (else a)))

;determines row and column of the statue, returned as a list; r is inputted as 0; L is the courtyard; s is value of the statue.
(define (index-statue L r s) (cond ((in-row? s (car L)) (list r (index s (car L) 0))) (else (index-statue (cdr L) (+ r 1) s))))

;changes multiple index's; L is a two by two array; Li is a list of indicies; s is the number of the new indicies.
(define (change-mult-index L Li s) (cond ((equal? Li '()) L) (else (change-mult-index (change-index '() (car (car Li)) (car (cdr (car Li))) s L) (cdr Li) s))))


;places tile in proper spot; n is length of row, s is number of staue, i is number of tile; L is the courtyard.
(define (place-tile L n s i) (cond ((equal? (statue L n s) 1) (change-mult-index L (list (list (- (/ n 2) 1) (- (/ n 2) 1)) (list (/ n 2) (/ n 2)) (list (/ n 2) (- (/ n 2) 1))) i))
                                 ((equal? (statue L n s) 2) (change-mult-index L (list (list (- (/ n 2) 1) (/ n 2)) (list (/ n 2) (/ n 2)) (list (/ n 2) (- (/ n 2) 1))) i))
                                 ((equal? (statue L n s) 3) (change-mult-index L (list (list (- (/ n 2) 1) (- (/ n 2) 1)) (list (- (/ n 2) 1) (/ n 2)) (list (/ n 2) (/ n 2))) i))
                                 (else (change-mult-index L (list (list (- (/ n 2) 1) (- (/ n 2) 1)) (list (- (/ n 2) 1) (/ n 2)) (list (/ n 2) (- (/ n 2) 1))) i))))

;splits a row in half; L is the row, n is length of row; ar is always 0
(define (split-row1 L n ar) (cond ((equal? ar (- (/ n 2) 1)) (list (car L))) (else (cons (car L) (split-row1 (cdr L) n (+ ar 1))))))

;second half of row; L is the row, n is length of the row; ar is 0.
(define (split-row2 L n ar) (cond ((equal? ar (- (/ n 2) 1)) (cdr L)) (else (split-row2 (cdr L) n (+ ar 1)))))

;returns only quadrant 1 of a grid L; n is length of row and ac always begin as zero
(define (quad-1 L n ac) (cond ((equal? ac (- (/ n 2) 1)) (list (split-row2 (car L) n 0))) (else (cons (split-row2 (car L) n 0) (quad-1 (cdr L) n (+ ac 1))))))

;returns only quadrant 2
(define (quad-2 L n ac) (cond ((equal? ac (- (/ n 2) 1)) (list (split-row1 (car L) n 0))) (else (cons (split-row1 (car L) n 0) (quad-2 (cdr L) n (+ ac 1))))))

;returns only quadrant 3
(define (quad-3 L n ac counter) (cond ((equal? ac (/ n 2)) (cond ((equal? counter (- n 1)) (list (split-row1 (car L) n 0)))
                                                                       (else (cons (split-row1 (car L) n 0) (quad-3 (cdr L) n ac (+ counter 1))))))
                                      (else (quad-3 (cdr L) n (+ ac 1) (+ counter 1)))))

;return only quadrant 4
(define (quad-4 L n ac counter) (cond ((equal? ac (/ n 2)) (cond ((equal? counter (- n 1)) (list (split-row2 (car L) n 0)))
                                                                       (else (cons (split-row2 (car L) n 0) (quad-4 (cdr L) n ac (+ counter 1))))))
                                      (else (quad-4 (cdr L) n (+ ac 1) (+ counter 1)))))

;return a two by two array by combining quadrants; n is the length of each of the quadrants.
(define (combine q1 q2 q3 q4 n) (append (combine-top2 q1 q2 n) (combine-top2 q4 q3 n)))

;combines q1 and q2, also works for q3 and q4, but q4 has to go in the q1 argument. n is the length of one quadrant.
(define (combine-top2 q1 q2 n) (cond ((eq? (- n 1) 0) (list (append (car q2) (car q1)))) (else (cons (append (car q2) (car q1)) (combine-top2 (cdr q1) (cdr q2) (- n 1))))))

;tiles the courtyard, recursive step; L is the courtyard; n is dimensions of courtyard; s is value of the statue or tile.
(define (tiling L n s) (cond ((equal? n 2) (place-tile L n s (+ s 1))) ((equal? n 4) (combine (tiling (quad-1 (place-tile L n s (+ s 1)) n 0) (/ n 2) (+ s 2)) (tiling (quad-2 (place-tile L n s (+ s 1)) n 0) (/ n 2) (+ s 1))
                                                                                      (tiling (quad-3 (place-tile L n s (+ s 1)) n 0 0) (/ n 2) (+ s 2)) (tiling (quad-4 (place-tile L n s (+ s 1)) n 0 0) (/ n 2) (+ s 1)) (/ n 2)))
                                                                                           (else (combine (tiling (quad-1 (place-tile L n s (+ s 1)) n 0) (/ n 2) (+ s 1)) (tiling (quad-2 (place-tile L n s (+ s 1)) n 0) (/ n 2) (+ s 1))
                                                                                      (tiling (quad-3 (place-tile L n s (+ s 1)) n 0 0) (/ n 2) (+ s 1)) (tiling (quad-4 (place-tile L n s (+ s 1)) n 0 0) (/ n 2) (+ s 1)) (/ n 2)))))
