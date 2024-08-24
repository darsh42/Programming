euclid :: Int -> Int -> Int
euclid m n
    | m >= n = if (mod n m) == 0
               then m
               else euclid m (mod n m)
    | m < n  = if (mod m n) == 0
               then n
               else euclid n (mod m n)
