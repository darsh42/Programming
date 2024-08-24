factorial :: Integer -> Integer
factorial num 
    | num == 0 = 1
    | otherwise = num * factorial (num - 1)

fibonacci :: Integer -> Integer
fibonacci num = compute num 0 1
    where compute count prev curr 
            | count == 1 = curr
            | otherwise  = compute (count - 1) curr (prev + curr)

mycurry func a b = func (a, b)

myuncurry func x = func (fst x) (snd x)
