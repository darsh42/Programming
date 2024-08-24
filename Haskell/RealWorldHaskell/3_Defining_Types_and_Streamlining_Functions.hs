import Data.Ord
import Data.List

-- Data and Types
type CustomerID = Int
type ReviewBody = String
type CardHolder = String
type CardNumber = Integer
type Address    = [String]

data BookInfo = Book Int String [String]
                deriving (Show)

data BookReview = BookReview BookInfo CustomerID String

data BetterReview = BetterReview BookInfo CustomerID ReviewBody

type BookRecord = (BookInfo, BookReview)

data BillingInfo =   CreditCard CardHolder CardNumber Address
                   | CashOnDelivery
                   | Invoice CustomerID
                   deriving (Show)

data Customer = Customer {
    customerID :: CustomerID,
    customerName :: String,
    customerAddress :: Address
} deriving (Show)

data List a = Cons a (List a)
              | Nil
              deriving (Show)

data MyMaybe a = JustM a
             | Nothin
             deriving (Show)

data Tree a = Node a (Tree a) (Tree a)
            | Empty
              deriving (Show)


-- Definitions
myinfo = Book 12151841841 "Don Quixote" 
         ["Miguel Cervantes"]


-- Functions
fromList :: [a] -> (List a)
fromList (x:xs) = Cons x (fromList xs)
fromList     [] = Nil

fromCons :: (List a) -> [a]
fromCons (Cons x xs) = (x: (fromCons xs))
fromCons         Nil = []

mySecond :: [a] -> a
mySecond xs = if null (tail xs)
              then error "list is too short"
              else head (tail xs)

safeSecond :: [a] -> Maybe a
safeSecond [] = Nothing
safeSecond xs = if null (tail xs)
                    then Nothing
                    else Just (head (tail xs))

tidySecond :: [a] -> Maybe a
tidySecond (_:x:_) = Just x
tidySecond      _  = Nothing

unwrap :: Maybe a -> a
unwrap (Just a) = a

secondAdd :: [Int] -> [Int] -> Maybe Int
secondAdd [] [] = Nothing
secondAdd xs ys = if (tidySecond xs) == Nothing || (tidySecond ys) == Nothing
                  then Nothing
                  else Just ((unwrap (tidySecond xs)) + (unwrap (tidySecond ys)))

lend :: Int -> Int -> Maybe Int
lend amount balance = let reserve = 100
                          newbalance = balance - amount
                      in if balance < reserve
                         then Nothing
                         else Just newbalance

lend2 :: Integer -> Integer -> Maybe Integer
lend2 amount balance = if balance < reserve
                       then Nothing
                       else Just newbalance
    where reserve    = 100
          newbalance = balance - amount

pluralise :: String -> [Integer] -> [String]
pluralise word counts = map plural counts
    where plural 0 = "no " ++ word ++ "s"
          plural 1 = "one " ++ word
          plural n = show n ++ " " ++ word ++ "s"


-- Exercise 1 & Exercise 2
mylength :: [a] -> Integer
mylength (x:xs)
        | (null xs) = 1
        | otherwise = 1 + mylength xs

-- Exercise 3
mymean :: [Double] -> Double
mymean xs = (sum xs) / (fromIntegral (length xs))

-- Exercise 4
palindrome :: [a] -> [a]
palindrome (x:xs) = [x] ++ (palindrome xs) ++ [x]
palindrome []     = []

-- Exercise 5
ispalindrome :: (Eq a) => [a] -> Bool
ispalindrome xs = xs == reverse xs

-- Exercise 6
sortLength :: [[a]] -> [[a]]
sortLength xs = sortBy (comparing length) xs

-- Exercise 7
myIntersperse :: a -> [[a]] -> [a]
myIntersperse s []       = []
myIntersperse s (x:[])   = x
myIntersperse s (x:y:ys) = x ++ [s] ++ y ++ (myIntersperse s ys)

-- Exercise 8
treeHeight :: Tree a -> Integer
treeHeight (Node _ l r) = let right = treeHeight r
                              left  = treeHeight l
                          in if right > left
                             then right + 1
                             else left  + 1
treeHeight Empty        = 0

-- Exercise 9
data Direction = TurnRight | TurnLeft | Straight deriving (Show)

-- Exercise 10
computeDirection :: [[Double]] -> Direction
computeDirection (a:b:c:_) | (cross a b c)  < 0 = TurnRight
                           | (cross a b c)  > 0 = TurnLeft
                           | (cross a b c) == 0 = Straight
    where cross :: [Double] -> [Double] -> [Double] -> Double
          cross (x1:y1:_) (x2:y2:_) (x3:y3:_) = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)

-- Exercise 11
computeDirections :: [[Double]] -> [Direction]
computeDirections (a:b:c:rs) = (computeDirection [a, b, c]):(computeDirections (b:c:rs))




