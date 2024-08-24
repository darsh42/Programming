module Exercises where
import Prelude hiding (reverse, foldl, foldr, zip, zipWith)

foldl func carry list
    | null list = carry
    | otherwise = foldl func (func carry (head list)) (tail list)

foldr func carry list
    | null list = carry
    | otherwise = func (head list) (foldr func carry (tail list))

reversel = foldl (\c v -> v : c) []

reverser = foldr (\v c -> c ++ [v]) []

zipWith f as bs = [f (as !! idx) (bs !! idx) | idx <- [1..len-1]]
    where len = min (length as) (length bs)

zip = zipWith (,)
