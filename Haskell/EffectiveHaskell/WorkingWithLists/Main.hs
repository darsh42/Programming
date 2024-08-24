module Main where

countdown :: Integer -> [Integer]
countdown num 
    | num == 0 = []
    | otherwise = num : countdown (num - 1)

originalIsBalanced s = 0 == originalIsBalanced' 0 s
    where originalIsBalanced' count s
            | null s = count
            | head s == '(' = originalIsBalanced' (count + 1) (tail s)
            | head s == ')' = originalIsBalanced' (count - 1) (tail s)
            | otherwise       = originalIsBalanced' count (tail s)

myReduce func carryValue list = if null list
                                then carryValue
                                else myReduce func (func carryValue (head list)) (tail list)

refactorIsBalanced s = 0 == myReduce balanced 0 s
    where balanced count value 
            | value == '(' = count + 1
            | value == ')' = count - 1
            | otherwise    = count

-- refactorIsBalanced s = 

main = print "hello world"
