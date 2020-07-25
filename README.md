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
<table>
	<tr>
		<th>State</th>
		<th>+</th>
		<th>-</th>
		<th>*</th>
		<th>/</th>
		<th>^</th>
		<th>(</th>
		<th>)</th>
		<th>num .</th>
		<th>E e</th>
	</tr>
	<tr>
		<td><b>in Operator</b></td>
		<td>Error</td>
		<td>Unary</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Operator</td>
		<td>Error</td>
		<td>Number</td>
		<td>Error</td>
	</tr>
	<tr>
		<td><b>in Number</b></td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Error</td>
		<td>Bracket</td>
		<td>Number</td>
		<td>E literal</td>
	</tr>
	<tr>
		<td><b>in E literal</b></td>
		<td>Error</td>
		<td>Number</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Number</td>
		<td>Error</td>
	</tr>
	<tr>
		<td><b>in Unary</b></td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Error</td>
		<td>Operator</td>
		<td>Error</td>
		<td>Number</td>
		<td>Error</td>
	</tr>
	<tr>
		<td><b>in Bracket</b></td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Operator</td>
		<td>Error</td>
		<td>Bracket</td>
		<td>Error</td>
		<td>Error</td>
	</tr>
</table>

<p><b>If '_DEBUG' defined</b> (in Visual Studio, in 'Debug' configuration it always defined automatically) intermediate steps will be outputed in 'stdout'. </p>

<p><b>Example of using this class:</b></p>

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

<h3>Issues:</h3>
<ul>
	<li>Unexpected crushes (related to series of unary -)</li>
</ul>

<h3>Todo:</h3>
<ul>
	<li>1e-5 interpreted as (1*10^-5) (test perfomance on each variant)</li>
	<li>Make wrapper-function to Parser class to use w/o exceptions</li>
</ul>
