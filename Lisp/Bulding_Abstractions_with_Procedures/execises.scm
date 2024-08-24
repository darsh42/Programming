; Exercise 1.1
10          ; 10
(+ 5 3 4)   ; 12
(- 9 1)     ; 8


; Exercise 1.02
(define (eval) (/
              (+ 5 4
                 (- 2
                    (+ 6
                       (/ 4 5)
                       )
                    )
                 )
              (* 3
                 (- 6 2) (- 2 7)
                 )
              )
  )

; Exercise 1.03
(define (square x)
  (* x x))

(define (sum-square a b)
  (+ (square a) (square b)))

(define (comp a b)
  (if (> a b)
      a
      b))

(define (is-equal a b)
  (if (= a b)
      #t
      #f))

(define (largest-two-sum-square x y z)
  (cond ((= (comp x y) (comp x z))
         (sum-square (comp y z) (comp x y)))
        ((= (comp y x) (comp y z))
         (sum-square (comp x z) (comp y x)))
        ((= (comp z x) (comp z y))
         (sum-square (comp z x) (comp x y)))
        )
  )

; Exercise 1.04
;
; Explination:
; The if statement determines the sign applied
; to the operation based on the value of b,
; this reduces the number of comparisons required
; to compute the function
(define (a-plus-abs-b a b)
  ((if (> b 0) + -) a b))


; Exercise 1.05
;
; Explain:
; Appliative order evaluation evaluates all expressions
; and applies the called procedure onto the resulting
; expressions.
;                             |-> 0  |-> evaluates to endless loop
; i.e (test 0 (p)) -> test(   0  ,   p() )
;
; Normal order evaluation will reduce the expression into
; most basic primetives possible and then evaluate them
;
; i.e (test 0 (p)) -> (if (= 0 0)
;                          0      <- enters this statement
;                          (p))   <- not reached
;
; Therefor an Applicative order eval will never terminate
; whilst a Normal order eval will resolve to 0

(define (p) (p))

(define (test x y)
  (if (= x 0)
      0
      y))

; Newton Raphson Method to calculate sqare roots
; x -> square in question
; guess the current root estimate

(define (sqrt-iter guess x)
  (if (good-enough? guess x)
      guess
      (sqrt-iter (improve guess x) x)))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2))

; will find the difference between x and guess
; if it is less than 0.00001 then it returns true
(define (good-enough? guess x)
  (< (abs (- (square guess) x)) 0.001))

(define (abs a)
  ((if (> a 0) + -) a))

(define (square x)
  (* x x))





(define (-sqrt x)
  (sqrt-iter 1.0 x))

; Exercise 1.6
;
; When calling a function the program needs to set up
; a stack frame for each function call, this allows it
; to save the current program state and execute the fun-
; ction, the drawback of this is that the number of function
; calls that can be made is determined by the stack size (or
; language), the effect of using new-if is that the limit is hit
; thereby aborting execution
;

(define (new-if predicate then-clause else-clause)
  (cond (predicate then-clause)
        (else else-clause)))

(define (new-if-sqrt-iter guess x)
  (new-if (good-enough? guess x)
          guess
          (new-if-sqrt-iter (improve guess x) x)))

; Exercise 1.7
;
; On the definition of (-sqrt):
; The function will fail on small values:
;   This is because the precision in the absolute difference
;   between the square of the guess and x might be enough to
;   satisfy the condition specified in (good-enough?)
; The function will fail on large values:
;   This is because the precision in the absolute difference
;   between the square of the guess and x will be too high,
;   this means that either the function will be terminated by
;   the system or the resulting value will be far to precise

(define (abs-diff x y)
  (abs (- x y)))

(define (better-good-enough? guess prev-guess x)
  (< (/ (abs-diff (square guess) x) (abs-diff (square prev-guess) x)) 0.01))

(define (better-sqrt-iter guess prev-guess x)
  (if (better-good-enough? guess prev-guess x)
      guess
      (better-sqrt-iter (improve guess x) guess x)))

(define (better-sqrt x)
  (better-sqrt-iter 1.0 1.1 x))


; Exercise 1.8
;
; Cube root function, utilizes Newton's formula to calculate next
; guess

(define (cube-root x)
  (cube-root-iter 1.0 1.1 x))

(define (cube-root-iter guess prev-guess x)
  (if (cube-good-enough? guess prev-guess x)
      guess
      (cube-root-iter (cube-improve guess x) guess x)))

(define (cube-good-enough? guess prev-guess x)
  (< (/ (abs-diff (cube guess) x) (abs-diff (cube prev-guess) x)) 0.01))

(define (cube-improve guess x)
  (/ (+ (/ x (square guess)) (* 2 guess))
     3))

(define (cube x)
  (* x x x))

; Re-factored cube and square-root function
; taking advantage of block structure

(define (better-cube-root x)
  (define (root-iter guess prev-guess)
    (if (good-enough? guess prev-guess)
        guess
        (root-iter (improve guess) guess)))

  (define (good-enough? guess prev-guess)
    (< (/ (abs-diff (cube guess) x) (abs-diff (cube prev-guess) x)) 0.01))

  (define (improve guess)
    (/ (+ (/ x (square guess)) (* 2 guess))
      3))

  (define (cube a)
    (* a a a))

  (root-iter 1.0 1.1))

(define (better-sqrt x)

  (define (sqrt-iter guess prev-guess )
    (if (good-enough? guess prev-guess )
        guess
        (sqrt-iter (improve guess) guess)))

  (define (good-enough? guess prev-guess)
    (< (/ (abs-diff (square guess) x) (abs-diff (square prev-guess) x)) 0.01))

  (define (improve guess)
    (average guess (/ x guess)))

  (define (average a b)
    (/ (+ a b) 2))

  (better-sqrt-iter 1.0 1.1 x))


; Section 1.2
;
; Procedures and the Processes they generate

; Basic factorials

(define (rec-factorial n)
  (if (= n 1)
      n
      (* n (rec-factorial (- n 1)))))

(define (iter-factorial n)
  (define (iter counter product)
    (if (> counter n)
        product
        (iter (+ counter 1)
              (* product counter))))
  (iter 1 1))


; Exercise 1.9
;
; procedure 1 is recursive, it expands and contracts:
; procedure 2 is iterative, it does not expand and contract:
;

; Exercise 1.10
;

(define (A x y)
  (cond ((= y 0) 0)
        ((= x 0) (* 2 y))
        ((= y 1) 2)
        (else (A (- x 1)
                 (A x (- y 1))))))

(define (f n) (A 0 n))
(define (g n) (A 1 n))
(define (h n) (A 2 n))
(define (k n) (* 5 n n))


; Fibonacci numbers
;

(define (rec-fib n)
  (cond ((= n 0) 0)
        ((= n 1) 1)
        (else (+ (rec-fib (- n 1))
                 (rec-fib (- n 2))))))

(define (iter-fib n)
  (define (iter a b count)
    (if (= count 0)
        b
        (iter (+ a b) a (- count 1))))
  (iter 1 0 n))

(define (currency-change amount)
  (define (cc amount kinds-of-coins)
    (cond ((= amount 0) 1)
          ((or (< amount 0) (= kinds-of-coins 0)) 0)
           (else (+ (cc amount
                        (- kinds-of-coins 1))
                    (cc (- amount
                           (first-denomination kinds-of-coins))
                        kinds-of-coins)))))

  (define (first-denomination kinds-of-coins)
    (cond ((= kinds-of-coins 1) 1)
          ((= kinds-of-coins 2) 5)
          ((= kinds-of-coins 3) 10)
          ((= kinds-of-coins 4) 25)
          ((= kinds-of-coins 5) 50)))
  (cc amount 5))

; Exercise 1.11
;

(define (rec-f n)
  (cond ((< n 3) n)
        (else
         (+ (rec-f (- n 1))
            (* 2 (rec-f (- n 2)))
            (* 3 (rec-f (- n 3)))))))

(define (iter-f count)
  (define (iter a b c count)
    (if (< count 3) a
        (iter (+ a b c)
              (* 2 a)
              (* 3 (/ b 2))
              (- count 1))))
  (iter 2 2 0 count))

(define (pasc depth)
  (define (recur a b n)
    (if (= n 0)
        (+ a b)
        (recur (+ a b)
               (recur a b (- n 1))
               (- n 1))))
  (recur 1 0 depth))

(define (pasc2 depth)
  (define (iter a b)
    (if (= depth 0) (+ a b)
        ((- depth 1) (+ (iter (- a 1)
                              (- b 1))
                        (iter (- a 1)
                              (- b 1))))))
  (iter 1 0))

 (define (pascal-item m n)
    (cond ((= n 1) 1)
          ((= n m) 1)
          (else (+ (pascal-item (- m 1) (- n 1))
                   (pascal-item (- m 1) n)))))

; Exercise 1.15
;
; The space complexity of the sin function is O(1) as the
; number of values kept in check at any given time is
; constant
;
; The time complexity of the function is O(log(n)) this can
; be deduced from the number of invocations of funtion (p),
; when the angle is 12.15 (p) is invoked 5 times, as 12.15
; needed to be divided by (pow 3 5) to be less than 0.1.
; From this we can re-arrange (< (/ 12.15 (pow 3 5) 0.1)) to
; (ceiling (/ (log (/ 12.15 0.1)) (log 3))) == 5 (ceiling is next
; highest integer)
;

(define (sin angle)
  (define (cube x) (* x x x))
  (define (p x) (- (* 3 x) (* 4 (cube x))))
  (if (not (> (abs angle) 0.1))
      angle
      (p (sin (/ angle 3.0)))))

; Computing exponentials
;
;

(define (rec-exp a b)
  (if (= b 1) a
      (* a (rec-exp a (- b 1)))))

(define (iter-exp product count)
  (define (iter count)
    (if (= count 1)
        product
        (* product (iter (- count 1)))))
  (iter count))

(define (fast-exp m n)
  (define (is-even? a)
    (= (remainder a 2) 0))
  (define (exp m n)
    (cond ((= n 0) 1)
          ((is-even? n) (square (exp m (/ n 2))))
          (else (* m (exp m (- n 1))))))
  (exp m n))

; Exercise 1.16
;
; Faster exponent funtion taking into account the idea
; of b^n = (b ^ (/ n 2)) ^ 2,

(define (fast-iter-exp a b n)
  (define (is-even? x)
    (= (remainder x 2) 0))
  (cond ((= n 0) a)
        ((is-even? n) (fast-iter-exp a (square b) (/ n 2)))
        (else (fast-iter-exp (* a b) b (- n 1)))))

(define (multiply a b)
  (define (is-even? x)
    (= (remainder x 2) 0))
  (define (double x) (+ x x))
  (define (halve x) (/ x 2))
  (cond ((= b 0) 0)
        ((is-even? b) (double (multiply a (halve b))))
        (else (+ a (multiply a (- b 1))))))

; Exercise 1.18
;
; The difference between the mutliply and iter-multiply is that
; in the else clause the value that needed to be added onto the
; multiply call was made into a function status flag that removed
; the recursion and made it into iteration
;

(define (iter-multiply a b)
  (define (double x) (+ x x))
  (define (halve x) (/ x 2))
  (define (even? x) (= (remainder x 2) 0))

  (define (iter a b accumulator)
    (cond ((= b 0) accumulator)
          ((even? b) (iter (double a) (halve b) accumulator))
          (else (iter a (- b 1) (+ accumulator a)))))

  (iter a b 0))

; Exercise 1.19

(define (fast-fib n)
  (define (even? x)
    (< (remainder x 2) 0))
  (define (fib-iter a b p q count)
    (cond ((= count 0) b)
          ((even? count)
           (fib-iter a
                     b
                     (+ (square q) (square p))
                     (+ (square q) (+ 2 p q))
                     (/ count 2)))
          (else (fib-iter (+ (* b q ) (* a q) (* a p))
                          (+ (* a q) (* b p))
                          0
                          1
                          (- count 1)))))
  (fib-iter 1 0 0 1 n))

; Euclid's algorithm
;
(define (iter-GCD a b)
  (if (= b 0) a
      (iter-GCD b (remainder a b))))

; Fermat's little theorem
;


(define (expmod base exp m)
  (define (even? x) (= (remainder x 2) 0))
  (cond ((= exp 0) 1)
        ((even? exp)
         (remainder (square (expmod base (/ exp 2) m))
                    m))
        (else (remainder (* base (expmod base (- exp 1) m))
                         m))))

(define (ferm-little n)
  (define (try-it? a)
    (= (expmod a n n) a))
  (try-it? (+ 1 (random (- n 1)))))


(define (fast-prime? n times)
  (cond ((= times 0) true)
        ((ferm-little n) (fast-prime? n (- times 1)))
        (else false)))

; Exercise 1.21
;

(define (smallest-divisor n)
  (define (divides? a b)
    (= (remainder b a) 0))
  (define (find-divisor n test-divisor)
    (cond ((> (square test-divisor) n) n)
          ((divides? test-divisor n) test-divisor)
          (else (find-divisor n (+ test-divisor 1)))))
  (find-divisor n 2))

; Exercise 1.22
;
; lower limit |         3 smallest primes
; =======================================
; 1000        |          1009, 1013, 1019
; 10000       |       10007, 10009, 10037
; 100000      |    100003, 100019, 100043
; 1000000     | 1000003, 1000033, 1000037
;

(define (timed-prime-test n)
  (start-prime-test n (runtime)))

(define (start-prime-test n start-time)
  (define (prime? n) (= n (smallest-divisor n)))
  (if (prime? n)
      (report-prime n (- (runtime) start-time))))

(define (report-prime n elapsed-time)
  (newline)
  (display n)
  (display " *** ")
  (display elapsed-time))

(define (search-for-primes lower upper)
  (define (odd? x) (= (remainder x 2) 1))
  (define (iter n)
    (cond ((<= n upper) (timed-prime-test n) (iter (+ n 2)))))
  (iter (if (odd? lower) lower (+ lower 1))))

; Exercise 1.23
;

(define (prime? n)
  (= n (fast-smallest-divisor n)))

(define (fast-smallest-divisor n)
  (define (next x) (if (= x 2) 3 (+ x 2)))
  (define (divides? a b) (= (remainder b a) 0))
  (define (find-divisor n test-divisor)
    (cond ((> (square test-divisor) n) n)
          ((divides? test-divisor n) test-divisor)
          (else (find-divisor n (next test-divisor)))))
  (find-divisor n 2))


(define (fast-search-for-primes lower upper)
  (define (timed-prime-test n)
    (start-prime-test n (runtime)))

  (define (start-prime-test n start-time)
    (define (prime? n) (= n (fast-smallest-divisor n)))
    (if (prime? n)
        (report-prime n (- (runtime) start-time))))

  (define (report-prime n elapsed-time)
    (newline)
    (display n)
    (display " *** ")
    (display elapsed-time))
  (define (odd? x) (= (remainder x 2) 1))
  (define (iter n)
    (cond ((<= n upper) (timed-prime-test n) (iter (+ n 2)))))
  (iter (if (odd? lower) lower (+ lower 1))))

; Exercise 1.24
;
; Search for primes procedure using fermat's little
; theorem, compare the time taken between 2 formats

(define (fastest-search-for-primes lower upper)
  (define (timed-prime-test n)
    (start-prime-test n (runtime)))

  (define (start-prime-test n start-time)
    (define (prime? n) (ferm-little n))
    (if (prime? n)
        (report-prime n (- (runtime) start-time))))

  (define (report-prime n elapsed-time)
    (newline)
    (display n)
    (display " *** ")
    (display elapsed-time))
  (define (odd? x) (= (remainder x 2) 1))
  (define (iter n)
    (cond ((<= n upper) (timed-prime-test n) (iter (+ n 2)))))
  (iter (if (odd? lower) lower (+ lower 1))))

; Exercise 1.25



; Exercise 1.26
; Louis has transformed the procedure from a O(log n) into a O(n)
; procedure by having the interpreter compute the value of half the
; exponent twice, this is because the square funtion mearly multiplies
; the passed argument by it-self, as the interpreter uses application
; evaluation the square funtion just gets a number.
;


; Exercise 1.28
;
; #TODO
;
; Raben-Miller test, will determine weather a number is a prime without
; being fooled by the Carmicheal numbers

(define (modified-expmod base exp m)
  (define (even? x) (= (remainder x 2) 0))

  (define (non-trivial-root? a)
    (if ((and (not (= a 1))
              (not (= a (- exp 1)))
              (= (remainder (square a) exp) 1)))
        0
        a))

  (cond ((= exp 0) 1)
        ((even? exp) (non-trivial-root? (square (expmod base (/ exp 2) m))))
        (else (if (= (* base (expmod base (- exp 1) m))
                     (remainder 1 exp))
                  a))))

(define (raben-miller n)
  (define (try-it? a)
    (= (modified-expmod a n n) a))
  (try-it? (+ 1 (random (- n 1)))))


(define (miller-fast-prime? n times)
  (cond ((= times 0) true)
        ((raben-miller n) (fast-prime? n (- times 1)))
        (else false)))


; Section 1.3
;
; HIGHER ORDER PROCEDURES
;

; basic summation
;

(define (sum-numbers a b)
  (if (> a b)
      0
      (+ a (sum-numbers (+ a 1) b))))

(define (pi-sum a b)
  (if (> a b)
      0
      (+ (/ 1.0 (* a (+ a 2)) )
         (pi-sum (+ a 4) b )  )))

; INPUTS:
;     Func -> f(x) to be summed
;     increment -> how next element is retrieved
;     lower -> lower bound/ current number summed
;     upper -> upper bound/ termination condition
;
; OUTPUT:
;     sum of f(x) from lower to upper, incremented by increment

(define (sum func increment lower upper)
  (if (> lower upper)
      0
      (+ (func lower)
         (sum func increment (increment lower) upper))))

(define (pi lower upper)
  (define (func-pi a) (/ 1.0 (* a (+ a 2))))
  (define (increment-pi a) (+ a 4))
  (* 8 (sum func-pi increment-pi lower upper)))

(define (integral func dx lower upper)
  (define (increment x) (+ x dx))
  (* (sum func increment (+ lower (/ dx 2.0)) upper)
     dx))

; Exercise 1.29
;
; Simpson's Rule

(define (simpson func n lower upper)
  (define (h) (/ (- upper lower) n))
  (define (increment x) (+ x (* 2 (h))))
  (define (func-area x)
    (+ (func (- x  (h)))
       (* 4 (func x))
       (func (+ x (h)))))
  (* (/ (h) 3) (sum func-area increment lower upper)))


; Exercise 1.30
;
; Iterative version of sum function

(define (iter-sum func increment lower upper)
  (define (iter a result)
    (if (> a upper)
        result
        (iter (increment a) (+ result (func a)))))
  (iter lower 0))

(define (simpson-v2 func n lower upper)
  (define (h) (/ (- upper lower) n))
  (define (increment x) (+ x (* 2 (h))))
  (define (func-area x)
    (+ (func (- x  (h)))
       (* 4 (func x))
       (func (+ x (h)))))
  (* (/ (h) 3) (iter-sum func-area increment lower upper)))

; Exercise 1.31
;
; a) higher order procedure to multiply all numbers in a given
;    range
;
; b) higher order procedure to multiply all numbers in a given
;    range iterativly

(define (inc x) (+ x 1))

(define (rec-product func increment lower upper)
  (if (> lower upper)
      1
      (* (func lower)
         (rec-product func
                      increment
                      (increment lower)
                      upper))))

(define (iter-product func increment lower upper)
  (define (iter a result)
    (if (> a upper)
        result
        (iter (increment a) (* result (func a)))))
  (iter lower 1))

(define (factorial-v2 lower upper)
  (define (func x) x)
  (define (increment x) (+ x 1))
  (iter-product func increment lower upper))

(define (pi-v2 accuracy)
  (define (func x) (/ (* x (+ x 2.0))
                      (square (+ x 1.0))))
  (define (increment x) (+ x 2.0))
  (* 4 (iter-product func increment 2 accuracy)))

; Exercise 1.32
;

(define (rec-accumulate combiner null-value term a next b)
  (if (> a b)
      null-value
      (combiner (term a)
                (accumulate combiner
                            null-value
                            term
                            (next a)
                            next
                            b))))

(define (rec-acc-sum term a next b)
  (rec-accumulate + 0 term a next b))

(define (rec-acc-product term a next b)
  (rec-accumulate * 1 term a next b))

(define (iter-accumulate combiner null-value term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (combiner (term a) result))))
  (iter a null-value))

(define (iter-acc-sum term a next b)
  (iter-accumulate + 0 term a next b))

(define (iter-acc-product term a next b)
  (iter-accumulate * 1 term a next b))

; Exercise 1.33
;
; Filtered version of accumulator function
;

(define (rec-filter-accumulate filter? combiner null-value term a next b)
  (cond ((> a b) null-value)
        ((filter? a) (combiner (term a) (rec-filter-accumulate filter? combiner null-value term (next a) next b)))
        (else (rec-filter-accumulate filter? combiner null-value term (next a) next b))))


(define (rec-filter-acc-sum filter? term a next b)
  (rec-filter-accumulate filter? + 0 term a next b))

(define (rec-filter-acc-product filter? term a next b)
  (rec-filter-accumulate filter? * 1 term a next b))


(define (iter-filter-accumulate filter? combiner null-value term a next b)
  (define (iter a result)
   (cond ((> a b) result)
         ((filter? a) (iter (next a) (combiner (term a) result)))
         (else (iter (next a) result))))
  (iter a null-value))

(define (iter-filter-acc-sum filter? term a next b)
  (iter-filter-accumulate filter? + 0 term a next b))

(define (iter-filter-acc-product filter? term a next b)
  (iter-filter-accumulate filter? * 1 term a next b))

(define (sum-of-primes n)
  (define (term x) x)
  (define (next x) (+ x 1))
  (iter-filter-acc-sum prime? term 0 next n))

(define (less-than-relativly-prime n)
  (define (term x) x)
  (define (next x) (+ x 1))
  (define (filter? x) (= (iter-GCD x n) 1))
  (iter-filter-acc-product filter? term 1 next n))

; Exercise 1.34
;
; The interpreter will evaluate the function call as follows:
;   (f f)
;     |
;   (f 2) --> (2 2)
;              ^
;              |
;      2 is not an object

(define (f g)
  (g 2))

; root program
;

(define (search f neg-point pos-point)
  (let ((midpoint (average pos-point neg-point)))
    (if (close-enough? neg-point pos-point)
        midpoint
        (let ((test-value (f midpoint)))
          (cond ((positive? test-value)
                 (search f neg-point midpoint))
                ((negetive? test-value)
                 (search f midpoint pos-point))
                (else midpoint))))))

(define (close-enough? x y) (< (abs (- x y)) 0.001))
(define (positive? x) (> x 0))
(define (negetive? x) (< x 0))

(define (half-interval-method f a b)
  (let ((a-val (f a))
        (b-val (f b)))
    (cond ((and (positive? a-val) (negetive? b-val))
           (search f b a))
          ((and (positive? b-val) (negetive? a-val))
           (search f a b))
          (else
           (error "Function has positive values for both" a b)))))

(define (fixed-point f first-guess)
  (define tolerance 0.00001)
  (define (close-enough? x y) (< (abs (- x y)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (if (close-enough? guess next)
          next
          (try next))))
  (try first-guess))


(define (fixed-point-sqrt x)
  (fixed-point (lambda (y) (average y (/ x y))) 1.0))

; Exercise 1.35
;
; defining golden ratio

(define golden-ratio
  (fixed-point (lambda (x) (+ 1 (/ 1 x))) 1.0))

; Exercise 1.36
;
; Fixed point will display all the guesses

(define (fixed-point-print-guess f first-guess)
  (define (pretty-print x)
    (display x)
    (newline))
  (define tolerance 0.00001)
  (define (close-enough? x y) (< (abs (- x y)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (pretty-print next)
      (if (close-enough? guess next)
          next
          (try next))))
  (try first-guess))

(define (fixed-point-print-sqrt x)
  (fixed-point-print-guess (lambda (y) (average y (/ x y))) 1.0))

(define (ex1.36)
  (fixed-point-print-guess (lambda (x) (/ (log 1000) (log x))) 1.1))

; Exercise 1.37
;
; procedure to calculate continued fractions

(define (cont-frac-iter n d k)
   (define (iter x result)
     (if (<= x 0)
         result
         (iter (- x 1) (/ (n x) (+ (d x) result)))))
   (iter k 0))

(define (cont-frac-rec n d k)
  (if (= k 0)
      (/ (n k) (d k))
      (/ (n k) (+ (d k) (cont-frac-rec n d (- k 1))))))

; Exercise 1.38
;
; calculate eulers constant using his infinite fraction

(define (euler)
  (define (d i)
    (let ((term (- i 2)))
      (if (= (remainder term 3) 0)
          (* 2 (+ 1 (/ term 3)))
          1)))
(* 1.0 (+ 2 (cont-frac-iter (lambda (i) 1) d 1000))))

; Exercise 1.39
;
;

(define (tan x)
  (define (d a) (- a 2))
  (define (n a) (fast-iter-exp 1 x a))
  (* 1.0 (cont-frac-iter n d 99)))


; Find root of nth polynomials
;
; using average damping and fast-iter-exp
;
; after completing the procedure I found that when n > 3
; the procedure fails to compute and execution never finishes,
; after reading exercise 1.45 I understand that the damped
; procedure needs to further be dampend to converge to fixed-point
;

(define (average-damp f)
  (lambda (x) (average x (f x))))


(define (nth-poly-roots n x)
  (fixed-point (average-damp (lambda (y) (/ x (fast-iter-exp 1 y (- n 1)))))
               1.0))


(define (deriv f dx)
  (lambda (x) (/ (- (f (+ x dx))
                    (f x))
                 dx)))

(define (newtons-method g guess)
  (define (newton-transform x) (lambda (x) (- x (/ (g x) ((deriv g 0.00001) x)))))
  (fixed-point (newton-transform g) guess))

(define (newton-sqrt x)
  (newtons-method (lambda (y) (- (square y) x))
                  1.0))

(define (fixed-point-of-transform g transform guess)
  (fixed-point (transform g) guess))

; Exercise 1.40
;
; approx cubic polynomial with a b c

(define (cubic a b c)
  (lambda (x) (+ (cube x)
                 (* a (square x))
                 (* b x)
                 c)))

; Exercise 1.41
;
; procedure double will apply any procedure with a single
; argument twice

(define (double f)
  (lambda (x) (f (f x))))

(define (inc x)
  (+ x 1))

; Exercise 1.42
;
; function that composes 2 functions

(define (compose f g)
  (lambda (x) (f (g x))))

; Exercise 1.43
;
; procedure that applies a repeated application till n

(define (repeat f n)
  (define (iter k result)
    (if (= k n)
        result
        (iter (+ k 1)
              (compose f result))))
  (iter 1 f))

; Exercise 1.44
;
; Smoothing function, average of f(x - dx), f(x), f(x + dx)
; implement a n-fold smooth function

(define dx 0.0001)

(define (smooth f)
  (lambda (x)
    (/ (+ (f (- x dx))
          (f x)
          (f(+ x dx)))
       3)))

(define (n-fold-smooth f x n)
  (((repeat smooth n) f)  x))

; Exercise 1.45
;
; create a n-th polynomial solver function



(define (nth-poly-roots n x)
  (fixed-point (average-damp (lambda (y) (/ x (fast-iter-exp 1 y (- n 1)))))
               1.0))
(define (nth-roots n guess)
  (define (equ a)
    (lambda (b) (/ b y)))
  (fixed-point (repeat equ n) guess))
