; basic numberic operations on rational data
;
; (make-rat) -> creates a rational number
; (numer)    -> retrieves numerator part of rational number
; (denom)    -> retrieves denominator part of rational number

; Rational number primitive operations
;

(define (add-rat x y)
  (make-rat (+ (* (numer x) (denom y))
               (* (numer y) (denom x)))
            (* (denom x) (denom y))))

(define (sub-rat x y)
  (make-rat (- (* (numer x) (denom y))
               (* (numer y) (denom x)))
            (* (denom x) (denom y))))

(define (mul-rat x y)
  (make-rat (* (numer x) (numer y))
            (* (denom x) (denom y))))

(define (div-rat x y)
  (make-rat (* (numer x) (denom y))
            (* (denom x) (numer y))))

(define (equal-rat? x y)
  (= (* (numer x) (denom y))
     (* (denom x) (numer y))))

; setter and getter

(define (gdc a b)
  (if (= b 0) a
      (iter-GCD b (remainder a b))))

(define (make-rat n d)
  (let ((g ((if (> d 0) - +) (gdc n d))))
    (cons (/ n g) (/ d g))))

(define (numer x) (car x))

(define (denom x) (cdr x))

(define (print-rat rat)
  (newline)
  (display (numer rat))
  (display "/")
  (display (denom rat)))

(define (one-half)
  (make-rat 1 2))

(define (one-third)
  (make-rat 1 3))

; Exercise 2.2
;
; Make a coordinate representation system

(define (make-point x y) (cons x y))

(define (x-point p) (car p))
(define (y-point p) (cdr p))

(define (point-equal? p1 p2)
  (and (= (- (abs (x-point p1)) (abs (x-point p2))) 0)
       (= (- (abs (y-point p1) (abs (y-point p2)))) 0)))

(define (make-segment start-p end-p) (cons start-p end-p))

(define (start-segment segment) (car segment))
(define (end-segment segment) (cdr segment))

(define (midpoint-segment segment)
  (define (average x y) (/ (+ x y) 2))
  (make-point (average (x-point (start-segment segment))
                       (x-point (end-segment segment)))
              (average (y-point (start-segment segment))
                       (y-point (end-segment segment)))))

(define (common-points segment-1 segment-2)
  (cond ((point-equal? (start-segment segment-1) (start-segment segment-2)) (start-segment segment-1))
        ((point-equal? (end-segment segment-1) (end-segment segment-2)) (end-segment segment-1))
        ((point-equal? (start-segment segment-1) (end-segment segment-2)) (start-segement segment-1))
        ((point-equal? (end-segment segment-1) (start-segment segment-2)) (end-segment segment-1))
        (else (display "no common points"))))

(define (print-point p)
  (newline)
  (display "(")
  (display (x-point p))
  (display ",")
  (display (y-point p))
  (display ")"))

(define (square x) (* x x))

(define (sqrt x)
  (define (improve guess x)
    (average guess (/ x guess)))

  (define (average x y)
    (/ (+ x y) 2))

  (define (abs-diff x y)
    (abs (- x y)))

  (define (better-good-enough? guess prev-guess x)
    (< (/ (abs-diff (square guess) x) (abs-diff (square prev-guess) x)) 0.01))

  (define (better-sqrt-iter guess prev-guess x)
    (if (better-good-enough? guess prev-guess x)
        guess
        (better-sqrt-iter (improve guess x) guess x)))
  (better-sqrt-iter 1.0 1.1 x))


(define (length-segment segment)
  (define (point-diff p1 p2)
    (cons (- (x-point p1) (x-point p2))
          (- (y-point p1) (y-point p2))))
  (define (point-diff-x d) (car d))
  (define (point-diff-y d) (cdr d))
  (let ((seg-diff (point-diff (start-segment segment)
                              (end-segment segment))))
    (sqrt (+ (square (point-diff-x seg-diff))
             (square (point-diff-y seg-diff))))))

; Exercise 2.3
;
; Make rectangle representation in coordinate system

(define (make-rect side-1 side-2 side-3 side-4)
  (define (equal-length? segment-1 segment-2)
    (if (= (length-segment segment-1)
           (length-segment segment-2))
        #t
        #f))
  (define (connected? segment-1 segment-2)
    (cond ((point-equal? (start-segment segment-1) (start-segment segment-2)))
          ((point-equal? (end-segment segment-1) (end-segment segment-2)))
          ((point-equal? (start-segment segment-1) (end-segment segment-2)))
          ((point-equal? (end-segment segment-1) (start-segment segment-2)))
          (else false)))
  (cond ((and (equal-length? side-1 side-2)
              (equal-length? side-3 side-4))
         (cond ((and (connected? side-1 side-3)
                     (connected? side-2 side-4)
                     (connected? side-1 side-4)
                     (connected? side-2 side-3))
                (cons (cons (common-points side-1 side-3)
                            (common-points side-3 side-2))
                      (cons (common-points side-2 side-4)
                            (common-points side-4 side-1))))))
        ((and (equal-length? side-1 side-3)
              (equal-length? side-2 side-4))
         (cond ((and (connected? side-1 side-2)
                     (connected? side-3 side-4)
                     (connected? side-1 side-4)
                     (connected? side-2 side-3))
                (cons (cons (common-points side-1 side-2)
                            (common-points side-2 side-3))
                      (cons (common-points side-3 side-4)
                            (common-points side-4 side-1))))))
        ((and (equal-length? side-1 side-4)
              (equal-length? side-3 side-2))
         (cond ((and (connected? side-1 side-3)
                     (connected? side-2 side-4)
                     (connected? side-1 side-2)
                     (connected? side-4 side-3))
                (cons (cons (common-points side-1 side-3)
                            (common-points side-3 side-4))
                      (cons (common-points side-4 side-2)
                            (common-points side-2 side-1))))))
        (else (display "points do not satify requirements to create rectangle"))))

(define (make-rect-segment seg-1 seg-2)
  (make-rect seg-1 seg-2))

(define (make-rect-point p1 p2 p3 p4)
  (make-rect (make-segment p1 p2)
             (make-segment p3 p4)))

(define (rect-seg-1 x)
  (car x))

(define (rect-seg-2 x)
  (cdr x))


(make-rect (make-segment (make-point 0 0)
                         (make-point 0 7))
           (make-segment (make-point 0 7)
                         (make-point 7 7))
           (make-segment (make-point 7 7)
                         (make-point 7 0))
           (make-segment (make-point 7 0)
                         (make-point 0 0)))

;
; Creating cons, car and cdr as pure procedures
;
; This is accomplished by the proc-cons procedure creating
; a unique procedure where depending on the argument specified
; the procedure will return the x or y values from the proc-cons
; procedure

(define (proc-cons x y)
  (define (dispatch m)
    (cond ((= m 0) x)
          ((= m 1) y)
          (else (error "Argument not 0 or 1 ---- ARGUMENT" m))))
  dispatch)

(define (proc-car z) (z 0))

(define (proc-cdr z) (z 1))

; Exercise 2.4
;

(define (cons-proc x y)
  (lambda (m) (m x y)))

(define (car-proc z)
  (z (lambda (p q) p)))

(define (cdr-proc z)
  (z (lambda (p q) q)))

; Exercise 2.5
;

(define (exp b n)
  (define (iter a result)
    (if (= a n)
        result
        (iter (+ a 1)
              (* result b))))
  (iter 0 1))

(define (ret-power x)
  (define (even? x) (= (remainder x 2) 0))
  (let ((div (if (even? x) 2 3)))
    (define (iter a result)
      (if (= (remainder result div) 0)
          (iter (+ a 1) (/ result div))
          a))
    (iter 0 x)))

(define (2.5-cons a b)
  (cons (exp 2 a) (exp 3 b)))

(define (2.5-car z)
  (ret-power (car z)))

(define (2.5-cdr z)
  (ret-power (cdr z)))

; Exercise 2.6
;
; using church numerals to express number without using numbers

(define (inc x)
  (+ x 1))

(define zero (lambda (f) (lambda (x) x)))
(define one (lambda (f) (lambda (x) (f x))))
(define two (lambda (f) (lambda (x) (f (f x)))))
(define three (lambda (f) (lambda (x) (f (f (f x))))))
(define four (lambda (f) (lambda (x) (f (f (f (f x)))))))

(define (add-one n)
  (lambda (f) (lambda (x) (f ((n f) x)))) )


(define (add a b)
  (lambda (f) (lambda (x) ((a f) ((b f) x)))))

(define (mult a b)
  (lambda (f) (lambda (x) ((a (b f)) x))))

; Extended exercise 2.1.4
;
; Create a arithmetic system that uses intervals as its number primatives

; pre-defined methods
(define (add-interval x y)
  (make-interval (+ (lower-bound x) (lower-bound y))
                 (+ (upper-bound x) (upper-bound y))))

(define (mult-interval-orig x y)
  (let ((p1 (* (lower-bound x) (lower-bound y)))
        (p2 (* (lower-bound x) (upper-bound y)))
        (p3 (* (upper-bound x) (lower-bound y)))
        (p4 (* (upper-bound x) (upper-bound y))))
    (make-interval (min p1 p2 p3 p4)
                   (max p1 p2 p3 p4))))

(define (div-interval-orig x y)
  (mult-interval x
                 (make-interval (/ 1.0 (lower-bound y))
                                (/ 1.0 (upper-bound y)))))

(define (print-interval x)
  (newline)
  (display "[")
  (display (lower-bound x))
  (display ", ")
  (display (upper-bound x))
  (display "]"))

; Exercise 2.7
(define (make-interval x y)
  (if (< x y)
      (cons x y)
      (cons y x)))

(define (lower-bound x) (car x))
(define (upper-bound x) (cdr x))

; Exercise 2.8

(define (sub-interval x y)
  (add-interval x
                (make-interval (- (lower-bound y))
                               (- (upper-bound y)))))

; Exercise 2.9
;
; Have proved that multiplication give different width compare to expectation
; it soley depends on the numbers representing the upper or lower bound

(define (width x)
  (/ (- (upper-bound x)
        (lower-bound x))
     2))

; Exercise 2.10
;
; Modifieing interval division to handle zero intervals

(define (div-interval x y)
  (if (= 0 (* (lower-bound y) (upper-bound y)))
      (error "Division error (interval spans 0)" y)
      (mult-interval x
                     (make-interval (/ 1.0 (lower-bound y))
                                    (/ 1.0 (upper-bound y))))))
; Exercise 2.11
;
;
; patt  |  min  |  max
; a  b
; ++ ++ | al bl | ah bh
; ++ -+ | ah bl | ah bh
; ++ -- | ah bl | al bh
; -+ ++ | al bh | ah bh
; -+ -+ | trouble case
; -+ -- | ah bl | al bl
; -- ++ | al bh | ah bl
; -- -+ | al bh | al bl
; -- -- | ah bh | al bl

(define (mult-interval x y)
  (define (pos? x) (>= x 0))
  (define (neg? x) (not (pos? x)))
  (define (greater x y) (if (> x y) x y))
  (define (less x y) (not (greater x y)))
  (let ((al (lower-bound x))
        (ah (upper-bound x))
        (bl (lower-bound y))
        (bh (upper-bound y)))
    (cond ((and (pos? al) (pos? ah) (pos? bl) (pos? bh))
           (make-interval (* al bl) (* ah bh)))
          ((and (pos? al) (pos? ah) (neg? bl) (pos? bh))
           (make-interval (* al bl) (* ah bh)))
          ((and (pos? al) (pos? ah) (neg? bl) (neg? bh))
           (make-interval (* ah bl) (* al bh)))
          ((and (neg? al) (pos? ah) (pos? bl) (pos? bh))
           (make-interval (* al bh) (* ah bh)))
          ((and (neg? al) (pos? ah) (neg? bl) (pos? bh))
           (make-interval (* (less al bl) (greater ah bh))
                          (greater (* ah bh) (* al bl))))
          ((and (neg? al) (pos? ah) (neg? bl) (neg? bh))
           (make-interval (* al bl) (* ah bh)))
          ((and (neg? al) (neg? ah) (pos? bl) (pos? bh))
           (make-interval (* al bl) (* ah bh)))
          ((and (neg? al) (neg? ah) (neg? bl) (pos? bh))
           (make-interval (* al bl) (* ah bh)))
          ((and (neg? al) (neg? ah) (neg? bl) (neg? bh))
           (make-interval (* al bl) (* ah bh))))))

; Exercise 2.12
;
; alternate constructors and selectors

(define (make-center-percent c p)
  (cons (- c (/ (* c p) 100.0))
        (+ c (/ (* c p) 100.0))))

(define (center x)
  (/ (+ (lower-bound x) (upper-bound x)) 2))

(define (percent x)
  (let ((c (center x)))
    (/ (* (- (upper-bound x) c) 100.0) c)))

; Exercise 2.13
;
; No Scheme just maths, the problem can be defined as:
; [(x - p1*x), x, (x + p1*x)] * [(y - p2*y), y, (y + p2*y)]
; therefor the upper/lower bounds multiply to:
; lower:
;       (x - p1*x) * (y - p2y) = xy - xp1y - xp2y + xyp1p2
;                              = xy * (1 + p1p2 - p1 -p2)
;       As percentage tolerances are small p1p2 can be ignored
;                              = xy * (1 - p1 - p2)
; upper:
;       (x + p1*x) * (y + p2y) = xy + xp1y + xp2y + xyp1p2
;                              = xy * (1 + p1p2 + p1 +p2)
;       As percentage tolerances are small p1p2 can be ignored
;                              = xy * (1 + p1 + p2)
;
; Therefor the percentage tolerances are additive
;



; Exercise 2.14
;
; Investigating complaint made by Lem.E Fixit

(define (par1 r1 r2)
  (div-interval (mult-interval r1 r2)
                (add-interval r1 r2)))

(define (par2 r1 r2)
  (let((one (make-interval 1 1)))
    (div-interval one
                  (add-interval (div-interval one r1)
                                (div-interval one r2)))))

;

(define (last-ref list n)
  (if (= n 0)
      (car list)
      (last-ref (cdr list) (- n 1))))

(define (last-ref-iter items n)
  (define (iter list a)
    (if (null? list)
        nil
        (if (= a n)
            (car list)
            (iter (cdr list)
                  (+ a 1)))))
  (iter items 0))

(define (length list)
  (if (null? list)
      0
      (+ 1 (length (cdr list)))))

; Exercise 2.17
;
; Create procedure that returnes last item from non-empty list

(define (last-item list)
  (if (null? (cdr list))
      (car list)
      (last-item (cdr list))))

; Exercise 2.18
;
; Reverse list order

(define (append l-1 l-2)
  (if (null? l-1)
      l-2
      (cons (car l-1)
            (append (cdr l-1) l-2))))
(define (sub-list list index)
  (if (= index 0)
      (car list)
      (cons (car list) (sub-list (cdr list)
                                 (- index 1)))))

(define (reverse-list list)
  (let ((l (length list))
        (offset-max (round (/ 2 (length list)))))
    (define (switch offset)
      (let ((tmp) (sub-list list offset))
        (append (sub-list list offset) (sub-list list (- length offset)))
        (append (sub-list list (- length offset)) tmp)))
    (define (iter offset)
      (cond ((= l 0) (error "list has 0 length"))
            ((= offset offset-max) (switch offset) list)
            ((< offset offset-max) (switch offset) (iter (+ offset 1)))
            (else (error "some issue has occured"))))
    (iter 0)))
