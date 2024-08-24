module Fibs where

fib n
    | n == 0    = 0
    | n == 1    = 1
    | otherwise = fib (n - 1) + fib (n - 2)

fib2 n = compute 0 0 1
    where compute count prev curr
            | count == n - 1 = curr
            | otherwise      = compute (count + 1) curr (prev + curr)

fibs = map fib2 [1..]

fibs2 prev curr = curr : fibs2 curr (prev + curr)

fibs3 = 0 : 1 : helper fibs3
    where helper l@(a:b:_) = a+b: helper (tail l)
