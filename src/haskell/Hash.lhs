\begin{code}
module Hash ( hash ) where

import Data.Char ( ord )

hash :: String -> String
hash s = [hashc 'a' s, hashc 'b' s, hashc 'c' s, hashc 'd' s]
  where hashc c "" = c
        hashc c (x:xs) = hashc (combine c x) xs
        combine c x = mix (ord c + ord x)

mix :: Int -> Char
mix n | n > 95 = mix (n `mod` 95)
mix 0 = 'V'
mix 1 = 'N'
mix 2 = '6'
mix 3 = 'd'
mix 4 = 'C'
mix 5 = 'i'
mix 6 = '1'
mix 7 = 'o'
mix 8 = 'B'
mix 9 = '3'
mix 10 = 'i'
mix 11 = 'b'
mix 12 = 'H'
mix 13 = 'L'
mix 14 = 'v'
mix 15 = 'o'
mix 16 = 'm'
mix 17 = 'M'
mix 18 = 'v'
mix 19 = '1'
mix 20 = '9'
mix 21 = '6'
mix 22 = 'e'
mix 23 = 'D'
mix 24 = 'A'
mix 25 = 'v'
mix 26 = '1'
mix 27 = 'L'
mix 28 = 'R'
mix 29 = 'U'
mix 30 = 'p'
mix 31 = 'Q'
mix 32 = '4'
mix 33 = 'h'
mix 34 = 't'
mix 35 = 'p'
mix 36 = 'e'
mix 37 = 'i'
mix 38 = 'I'
mix 39 = 'S'
mix 40 = 'D'
mix 41 = 'B'
mix 42 = 'U'
mix 43 = 'J'
mix 44 = 'U'
mix 45 = 'A'
mix 46 = 'n'
mix 47 = 'Z'
mix 48 = 'f'
mix 49 = 'O'
mix 50 = '7'
mix 51 = '4'
mix 52 = 'm'
mix 53 = 'b'
mix 54 = 'N'
mix 55 = '8'
mix 56 = 't'
mix 57 = 'e'
mix 58 = 's'
mix 59 = 'h'
mix 60 = '6'
mix 61 = 'M'
mix 62 = 'i'
mix 63 = '1'
mix 64 = 'v'
mix 65 = 'u'
mix 66 = 'G'
mix 67 = 'S'
mix 68 = '9'
mix 69 = 'k'
mix 70 = '2'
mix 71 = 'O'
mix 72 = 'B'
mix 73 = 'a'
mix 74 = 'D'
mix 75 = 'l'
mix 76 = 'N'
mix 77 = '3'
mix 78 = '7'
mix 79 = '9'
mix 80 = 'L'
mix 81 = 'F'
mix 82 = 'U'
mix 83 = '0'
mix 84 = 'a'
mix 85 = '3'
mix 86 = 'r'
mix 87 = 'm'
mix 88 = 'F'
mix 89 = 'M'
mix 90 = 'S'
mix 91 = 'T'
mix 92 = 'M'
mix 93 = 'U'
mix 94 = 'm'
mix 95 = 'C'
mix _ = 'x'
\end{code}