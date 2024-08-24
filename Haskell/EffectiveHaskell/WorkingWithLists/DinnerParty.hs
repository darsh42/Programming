module DinnerParty where

checkGuestList list name = name `elem` list

-- function is called: dinnerParty checkGuestList guests
--  filter (isAttending . fst) -> the first element of the list is passed into isAttending, this makes a new function
--                                the filter function then applies the function to each element only selecting the ones from the 
--                                the guest list
--  foldr ((+) . snd) 0        -> foldr is called with the function which selects the second element of the list and adds them to the carry
dinnerParty isAttending = foldr ((+) . snd) 0 . filter (isAttending . fst)

