# Pretty dumb arithmetic parser

### Operations
**Parser can perform following operations:**
- Addition `+`
- Substraction `-`
- Division `/`
> note: division by zero cause throw `ParseException` exception with pos = 0 (no matter where division perfroms)
- Multiplication `*`
- Exponention `^`	
- Unary minus `-`
> note: in parser's internals, unary minus represented as `~` character; allowed only 1 unary minus in series (e.g. 2--1 allowed, 2---1 not allowed)
- Brackets `()`

### Numbers
All numbers represented as `double`. As numbers can be used all digits (0-9), deciminal point(.) and letter e(lower and upper case). Note, that error in number (e.g. 12.5.54 ; 1.5e-4e.64) will be igored, but during parsing part of number from error pos and to the end will be cutted off (in previus examples .54 and e.64).

### Exceptions
Parser using custom exception class `ParseException` which inherited by `std::runtime_error` exception class, and contains position where error occured (except division by zero, it always will be 0).

### Parsing arithmetic expression
To divide source string by tokens, next transition table applies(it also uses to verify input string):

|      State	  |     +    |     -    |     *    |     /    |     ^    |     (    |    )    |  num . |    E e    |
| --------------- | -------- | -------- | -------- | -------- | -------- | -------- | ------- | ------ | --------- |
| **in Operator** | Error    | Unary    | Error    | Error    | Error    | Operator | Error   | Number | Error     |
| **in Number**   | Operator | Operator | Operator | Operator | Operator | Error    | Bracket | Number | E literal |
| **in E literal**| Error    | Number   | Error    | Error    | Error    | Error    | Error   | Number | Error     |
| **in Unary** 	  | Error    | Error    | Error    | Error    | Error    | Operator | Error   | Number | Error     |	
| **in Bracket**  | Operator | Operator | Operator | Operator | Operator | Error    | Bracket | Error  | Error     |

### Usage of this class
If `_DEBUG` defined (in Visual Studio, in `Debug` configuration it always defined automatically) intermediate steps will be outputed in `stdout`.
*Supposed*, that `ArithParser::Parser` object will be always created in `try-catch` block to handle possible errors.

**Example of using this class:**
```c++
#include <iostream>
#include "ArithParser.h"
...
const char* expr = "-(5.6 * (4 ^ 0.5 * 1e3) --(-5E-1)) + 16^0.25";
double result = 0;
try{
	ArithParser::Parser pr(expr);
	result = pr.result();
}catch( ArithParser::ParseException& e){
	std::cout << e.what() << ' ' << e.pos() << std::endl;
}
...
```

## Issues:
- [ ] Unexpected crushes (related to series of unary `-`)

## Todo:
- [ ] 1e-5 interpreted as `1*10^-5` (test perfomance on each variant)
- [ ] Make wrapper-function to Parser class to use w/o exceptions
