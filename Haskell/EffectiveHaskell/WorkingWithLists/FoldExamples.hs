module FoldExamples where
import Prelude hiding (foldl, foldr, map, filter)

foldl func carry list
    | null list = carry
    | otherwise = foldl func (func carry (head list)) (tail list)

foldr func carry list
    | null list = carry
    | otherwise = func (head list) (foldr func carry (tail list))

foldrMatch _ _ [] = []
foldrMatch f c (x:xs) = f x (foldr f c xs)

mapFold op = foldrMatch func []
    where func value carry = op value : carry

map func list
    | null list = []
    | otherwise = func (head list) : map func (tail list)

filterFold op = foldr func []
    where func value carry 
            | op value  = value : carry
            | otherwise = carry

filter op list
    | null list      = []
    | op (head list) = head list : filter op (tail list)
    | otherwise      = filter op (tail list)

filterMatch _ [] = []
filterMatch op l@(x:xs)
    | op x      = x : filter op (tail l)
    | otherwise = filter op xs

combine xs ys
    | null xs || null ys = []
    | otherwise          = (head xs, head ys) : combine (tail xs) (tail ys)
