# EpicRoll
Some ugly code to test if the ["Epic Roll" dice game](https://www.amazon.com/dp/B01M6CZ32M) is fair.  Currently implements about half of the game:
- No retreating is allowed
- No cards are used
- If you have >= 4 life you fight again.

>     gcc -Wall -Werror -o epicroll epicroll.c && ./epicroll
>     
>     1000000 games played
>     Wizard won  295732(29.57%)
>     Warrior won 442047(44.20%)
>     Elf won     262221(26.22%)

