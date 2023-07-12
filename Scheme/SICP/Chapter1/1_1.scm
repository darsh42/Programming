(define (square x)
  (* x x))

(define (sum-of-squares a b)
  (+ (square a) (square b)))

(define (f a)
  (sum-of-squares (+ a 1) (+ a 2)))

(f 5)
