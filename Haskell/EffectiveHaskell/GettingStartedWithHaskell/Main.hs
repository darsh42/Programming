module Main where

greeting :: String -> String -> String
greeting salutation person = salutation <> " " <> person

makeGreeting = (<>) . (<> " ")

fizzbuzz num 
    | num == 0          = show num
    | num `rem` 15 == 0 = "fizzbuzz" <> " " <> fizzbuzz (num - 1)
    | num `rem`  5 == 0 = "buzz" <> " "<> fizzbuzz (num - 1)
    | num `rem`  3 == 0 = "fizz" <> " "<> fizzbuzz (num - 1)
    | otherwise         = show num <> " " <> fizzbuzz (num - 1)
    

main = print (makeGreeting "Hello" "Bhuvvi")

