#include <string>
#include <iostream>
#include <exception>

#include "ArithmeticParser.h"

namespace ArithParser {
	ParseException::ParseException(const char* msg, unsigned int pos) : runtime_error(msg), errPos(pos) {}

	unsigned int ParseException::pos() const { return errPos; }

	Parser::States Parser::changeState(States currentState, char ch) {
		// this function represents transition table (which shown in description)
		switch (currentState) {
		case States::InNumber:
			if (isdigit(ch) || ch == '.') return States::InNumber;
			switch (ch) {
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':	return States::InOperator;
			case ')':	return States::InCloseBracket;
			case 'E':
			case 'e':	return States::InELetterState;
			}
			break;
		case States::InOperator:
			if (isdigit(ch) || ch == '.') return States::InNumber;
			switch (ch) {
			case '-':	return States::InUnary;
			case '(':	return States::InOperator;
			}
			break;
		case States::InUnary:
			if (isdigit(ch) || ch == '.')	return States::InNumber;
			if (ch == '(')	return States::InOperator;
		case States::InCloseBracket:
			switch (ch) {
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':	return States::InOperator;
			case ')':	return States::InCloseBracket;
			}
		case States::InELetterState:
			if (isdigit(ch) || ch == '-')	return States::InNumber;
		}
		return States::Error;
	}

	Parser::Parser(const char* src, unsigned int length) :
		srcString(src),
		srcLength((length) ? (length) : (-1))
	{	
		parse();
		calculate();
	};

	int Parser::priority(char oper) {
		switch (oper) {
		case '-':
		case '+':	return 0;
		case '*':
		case '/':	return 1;
		case '^':	return 2;
		case '~':	return 3;
		}
		return INT32_MAX;
	}

	void Parser::parse() {
		States	prevState, state = States::InOperator;
		std::string digits;

		int brackets = 0;
		unsigned int pos = 0;

		_locale_t lc = _create_locale(LC_NUMERIC, "C");

		while (pos < srcLength && srcString[pos] != '\0') {
			if (srcString[pos] < 0)
				throw ParseException("incorrect expression", pos);
			if (isspace(srcString[pos])) {
				pos++;
				continue;
			}
			if (srcString[pos] == '(')	brackets++;
			if (srcString[pos] == ')')	brackets--;
			if (brackets < 0)
				throw ParseException("close bracket without open bracket", pos);

			prevState = state;
			state = changeState(state, srcString[pos]);

			switch (state) {
			case States::Error:
				throw ParseException("incorrect expression", pos);
			case States::InELetterState:
			case States::InNumber:
				digits.push_back(srcString[pos]);
				break;
			case States::InUnary:
				appendNumber(0);
				appendOperator('~');
				break;
			case States::InOperator:
			case States::InCloseBracket:
				if (prevState == States::InNumber) {
					appendNumber(_atof_l(digits.c_str(), lc));
					digits.clear();
				}
				appendOperator(srcString[pos]);
			}
			pos++;
		}
		// to not lose number in the end
		if (state == States::InNumber)
			appendNumber(_atof_l(digits.c_str(), lc));

		// if expression ends with operator char (include unary minus) or missing close bracket
		if (state == States::InOperator || state == States::InUnary || state == States::InELetterState || brackets)
			throw ParseException("incomplete expression", pos - 1);
	}

	void Parser::appendOperator(char ch) {
		Token token;
		token.type = TokenType::Operator;
		token.op = ch;
		tokens.push_back(token);
	}

	void Parser::appendNumber(double number) {
		Token token;
		token.type = TokenType::Number;
		token.number = number;
		tokens.push_back(token);
	}

	void Parser::calculate() {
#ifdef _DEBUG
		std::cout << "Tokens:" << std::endl;
		for (Token& tk : tokens) {
			if (tk.type == TokenType::Number)
				std::cout << "\t" << "N " << tk.number << std::endl;
			else
				std::cout << "\t" << "O " << tk.op << std::endl;
		}
		std::cout << std::endl << "Calculations:" << std::endl;
#endif // _DEBUG

		for (Token& token : tokens) {
			if (token.type == TokenType::Number)	
				numStack.push(token.number);
			else {
				switch (token.op) {
				case '(':
				case '~':
					opStack.push(token.op);
					break;;
				case ')':
					// execute all op's after '(' and then remove it from opStack
					while (opStack.top() != '(')
						exec();
					opStack.pop();
					break;
				default:
					// execute all operators in stack with higher priority than current until '(' operator or until
					//	opStack become empty
					if (!opStack.empty() && priority(token.op) <= priority(opStack.top()))
						while (!opStack.empty() && priority(token.op) <= priority(opStack.top()) && opStack.top() != '(')
							exec();
					opStack.push(token.op);
				}
			}
		}
		// execute all remaining op's
		while (!opStack.empty())
			exec();
	}

	void Parser::exec() {
		double value1, value2, returnValue;

		value2 = numStack.top();
		numStack.pop();

		value1 = numStack.top();
		numStack.pop();

		switch (opStack.top()) {
		case '-':
		case '~':
			returnValue = value1 - value2;
			break;
		case '+':
			returnValue = value1 + value2;
			break;
		case '*':
			returnValue = value1 * value2;
			break;
		case '/':
			if (!value2)
				throw ParseException("division by zero", 0);
			returnValue = value1 / value2;
			break;
		case '^':
			returnValue = pow(value1, value2);
		}
#ifdef _DEBUG
		std::cout << '\t' << value1 << ' ' << opStack.top() << ' ' << value2 << " = " << returnValue << std::endl;
#endif // _DEBUG
		opStack.pop();
		numStack.push(returnValue);
	}

	double Parser::result() const { return numStack.top(); }
}