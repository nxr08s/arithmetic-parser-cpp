#include <iostream>
#include <string>

#include "ArithmeticParser.h"

int main() {

	std::string readedLine;
#ifdef _DEBUG
	std::cout << std::endl << "\t\t\/!\\ /!\\ Warning: compiled in DEBUG configuration! /!\\ /!\\" << std::endl << std::endl;
#endif // _DEBUG
	std::cout << "Type 'help' to get basic info" << std::endl;
	for(;;){
		std::cout << ">>> ";
		std::getline(std::cin, readedLine);
		if (readedLine == "exit") {
			return 0;
		}
		if (readedLine == "clear") {
			system("cls");
			continue;
		}
		if (readedLine == "help") {
			std::cout << "\t1. clear - clear the screen" << std::endl
				<< "\t2. help - display this message" << std::endl
				<< "\t3. exit - close the program" << std::endl
				<< "\t4. Available op's: +, -, *, /, ^" << std::endl;
#ifdef _DEBUG
			std::cout << "\t(!debug!). intermediate steps are displaying" << std::endl;
#endif // _DEBUG
			continue;
		}
		try {
			ArithParser::Parser parser(readedLine.c_str(), readedLine.size());
			std::cout << "= " << parser.result() << std::endl;
		}
		catch (ArithParser::ParseException& e) {
			std::cout << "error: " << e.what() << '(' << e.pos() << ')' << std::endl;
		}
	}
}