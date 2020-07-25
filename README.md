<h1>Pretty stupid arithmetic parser</h1>
<strong>Parser can perform next operations:</strong>
<ul>
	<li>Addition <b><q>+</q></b></li>
	<li>Substraction <b><q>-</q></b></li>
	<li>Division <b><q>/</q></b> <em><br>note: division by zero cause throw 'ParseException' exception with pos = 0 (no matter where division perfroms)</em></li>
	<li>Multiplication <b><q>*</q></b></li>
	<li>Exponention <b><q>^</b></q></li>	
	<li>Unary minus <b><q>-</b></q>	<em><br>note: in parser's internals, unary minus represented as '~' character; allowed only 1 unary minus in series (e.g. 2--1 allowed, 2---1 not allowed)</em></li>
	<li>Brackets <b><q>()</q></b></li>
</ul>
<p><b>All numbers represented as 'double'.</b> As numbers can be used all digits (0-9), deciminal point(.) and letter e(lower and upper case). Note, that error in number (e.g. 12.5.54 ; 1.5e-4e.64) will be igored, but during parsing part of number from error pos and to the end will be cutted off (in previus examples .54 and e.64).</p>

<p><b>Parser using custom exception class 'ParseException'</b> which inherited by 'std::runtime_error' exception, and contains position where error occured (except division by zero, it always will be 0).</p>

<p><b>Supposed,</b> that 'ArithParser::Parser' object will be always created in 'try-catch' block to handle possible errors.</p>

<strong>To divide source string by tokens, next transition table applies(it also uses to verify input string):</strong>
<br>
<b>Legend:</b>
<ul>
	<li>Err - error state</li>
	<li>Opr - in operator state</li>
	<li>Num - in number state</li>
	<li>Brt - in close bracket state</li>
	<li>Unr - in unary(minus) state</li>
	<li>Enm - in E(e) literal(in number) state</li>
</ul>
		 ___________________________________________________________________________________
		|	State	|	+	|	-	|	*	|	/	|	^	|	(	|	)	|  num .|	Ee	|
		|___________|_______|_______|_______|_______|_______|_______|_______|_______|_______|
		|	Opr		|	Err	|	Unr	|	Err	|	Err	|	Err	|	Opr	|	Err	|	Num	|	Err	|
		|	Num		|	Opr	|	Opr	|	Opr	|	Opr	|	Opr	|	Err	|	Brt	|	Num	|	Enm	|
		|	Enm		|	Err	|	Num	|	Err	|	Err	|	Err	|	Err	|	Err	|	Num	|	Err	|
		|	Unr		|	Err	|	Err	|	Err	|	Err	|	Err	|	Opr	|	Err	|	Num	|	Err	|	
		|	Brt		|	Opr	|	Opr	|	Opr	|	Opr	|	Opr	|	Err	|	Brt	|	Err	|	Err	|
		|___________|_______|_______|_______|_______|_______|_______|_______|_______|_______|
		
@. If '_DEBUG' defined (in Visual Studio, in 'Debug' configuration it always defined automatically)
	intermediate steps will be outputed in 'stdout'.

@. Example of using this class:
	
	...
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
		
@. Issues:

@. Todo:
	@ 1e-5 interpreted as (1*10^-5) (test perfomance on each variant)
	@ Make wrapper-function to Parser class to use w/o exceptions
*/	
