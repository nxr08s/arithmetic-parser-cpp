/*
@. Parser can perform next operations:
	1. Addition			+			
	2. Substraction		-		
	3. Division			/	note: division by zero cause throw 'ParseException' exception with
								pos = 0 (no matter where division perfroms)
	4. Multiplication	*	
	5. Exponention		^		
	6. Unary minus		-	note: in parser's internals, unary minus represented as '~' character;
								allowed only 1 unary minus in series (e.g. 2--1 allowed, 2---1 not allowed)
	7. Brackets			()

@. All numbers represented as 'double', even if values are integers. As numbers can be used 
	all digits(0-9), deciminal point(.) and letter e(lower and upper case). NOTE, that error in number
	(e.g. 12.5.54 ; 1.5e-4e.64) will be igored, but during parsing part of number from errro pos and to end will be 
	cutted off (in previus examples .54 and e.64).

@. Parser using custom exception class 'ParseException' which inherited by 'std::runtime_error' exception, 
	and contains position where error occured (except division by zero, it always will be 0).

@. Supposed, that 'ArithParser::Parser' object will be always created in 'try-catch' block to handle 
	possible errors.

@. To divide source string by tokens, next transition table applies(it also uses to verify input string):

		Legend:	Err	- error state
				Opr	- in operator state
				Num	- in number state
				Brt	- in close bracket state
				Unr	- in unary(minus) state
				Enm - in E(e) literal(in number) state
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

#include <stack>
#include <vector>

namespace ArithParser {

	class std::runtime_error;
	class ParseException : public std::runtime_error {
		unsigned int errPos = 0;
	public:
		ParseException(const char*, unsigned int);
		unsigned int pos() const;
	};

	class Parser {
		enum class States { Error, InOperator, InUnary, InNumber, InCloseBracket, InELetterState };
		enum class TokenType { Number, Operator };
		struct Token {
			TokenType type;
			double number;
			char op;
		};

		std::vector<Token> tokens;
		std::stack<char> opStack;
		std::stack<double> numStack;

		const char* srcString;
		const unsigned int srcLength;

		States changeState(States, char);
		int priority(char);
		void parse();
		void calculate();
		void exec();
		void appendOperator(char);
		void appendNumber(double);

	public:
		Parser(const char*, unsigned int = 0);
		double result() const;
	};
}