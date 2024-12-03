countOrdered :: [[Int]] -> Bool -> Int
countOrdered (x : tail) True
  | isAscendingDampened x = 1 + countOrdered tail True
  | isDescendingDampened x = 1 + countOrdered tail True
  | otherwise = 0 + countOrdered tail True

countOrdered (x : tail) False
  | isAscending x = 1 + countOrdered tail False
  | isDescending x = 1 + countOrdered tail False
  | otherwise = 0 + countOrdered tail False
countOrdered [] _ = 0


isOrdered :: Int -> Int -> Bool
isOrdered x y = x < y && x + 3 >= y


isAscending :: [Int] -> Bool
isAscending (x : y : tail) = isOrdered x y && isAscending (y : tail)
isAscending [_] = True
isAscending [] = True


isAscendingDampened :: [Int] -> Bool
isAscendingDampened (x : y : tail) =
    if isOrdered x y
        then isAscendingDampened (y : tail)
    -- problem here
    else isAscending (x : tail) || isAscending (y : tail)
isAscendingDampened [_] = True
isAscendingDampened [] = True


isDescending:: [Int] -> Bool
isDescending (x : y : tail) = isOrdered y x && isDescending (y : tail)
isDescending [_] = True
isDescending [] = True

isDescendingDampened:: [Int] -> Bool
isDescendingDampened (x : y : tail) =
    if isOrdered y x
        then isDescendingDampened (y : tail)
    -- problem here
    else isDescending (x : tail) || isDescending (y : tail)
isDescendingDampened [_] = True
isDescendingDampened [] = True


readInt :: [[Char]] -> [Int]
readInt = map read


main :: IO ()
main = do
    contents <- readFile "../bin/input.txt"
    let records = map (readInt . words) . lines $ contents
    putStrLn ("Safe reports: " ++ show (countOrdered records False) ++ "\nDampened reports: " ++ show(countOrdered records True))
