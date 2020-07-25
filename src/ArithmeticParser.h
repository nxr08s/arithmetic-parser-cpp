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