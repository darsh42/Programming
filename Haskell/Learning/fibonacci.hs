import System.Environment

fibonacci :: Integer -> Integer
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = fibonacci (n - 1) + fibonacci (n - 2)

main :: IO ()
main = do 
       args <- getArgs
       print (fibonacci (read (head args) :: Integer))
