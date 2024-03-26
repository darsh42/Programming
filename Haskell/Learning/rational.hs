add_rat :: [Integer] -> [Integer] -> [Integer]
add_rat x y = (((head x) * (last y)) + 
               ((head y) * (last x)) :
               ((last x) * (last y)))

sub_rat :: [Integer] -> [Integer] -> [Integer]
sub_rat x y = (((head x) * (last y)) -
               ((head y) * (last x)) :
               ((last x) * (last y)))

main :: IO ()
main = do
       let rat_one = (1:2)
       let rat_two = (1:3)
       print (add_rat rat_one rat_two)
