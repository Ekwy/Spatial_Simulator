/*
General guidelines:
	-Never have more than one consecutive blank lines
	-Comments in this styleguide aren't counted as blank lines
	-4 character wide hard tabs, that is the character "\t" instead of a bunch of spaces
*/

#ifndef CODESTYLEGUIDE_H
#define CODESTYLEGUIDE_H
//NAMEOFCLASS_EXT

#include <standard libA>
#include <standard libB>
#include <standard libC>
#include "local libA"
#include "local libB"
#include "local libC"
//Standard libraries first, then local libraries, generally in order of use
//Include all required libraries for each file
//No blank line between the includes

#define CONST_A 42
#define CONST_B 11
// All constants should be splitting words with an underscore and be with full uppercases

template <typename T>	//No blank lines after template
class CodeStyleguide {	//Class name should be the same as the file name (Always use an uppercase for the first letter)
// Always use the private, protected, public order
private:
	//Var names start lowercase, every next word start with uppercase (see camelCase)
	bool active;
	//Bool names should be an adjective clearly yes/no, for example "red" is a little too ambiguous
	bool isRed;
	//Otherwise add "is", "has" or something similar to make it obvious
	int numApple;
	//Same for the rest, it should be relatively obvious how the var works by its name, add "num" to signify "number of apples"
	//For organization purposes, groups of variables could be separated by a blank line

protected:	//A blank line before new access specifiers
	CodeStyleguide* codeStyleguide;	//Pointer asterisk on the side of the type
	void (*onClick)();

public:
	CodeStyleguide(int argA, bool argB, bool isRed) : isRed(isRed), active(argB) {	//Prefer member initialization list over this->varA = varB
		if (argA > 4)
			numApple = argA + argB ? 22 : 13;
	}	//Blank line between functions

	~CodeStyleguide() {	//Space before {}

	}

	void setNumApple(int numApple) {	//Function names should use a verb lowercased to start, uppercase at every starting words
		this->numApple = numApple;	//Same as constructor, use same variable name for immediate assignation
	}

	float doMath(int argA, int argB, int argC, int argD) {	//Spaces after ",", not before
		argA += (argB + argC) / argD;	//Adequate spacing around math operators

		while (argB <= argD)	//Space after while, for, switch, if, etc statement
			argB++;	//Don't use {} for one liners, unless it becomes very complex (quinta-nested-for-switch style)

		return (float)argA;	//No space after casting
	}

	std::string analyseAnswer(int answer) {
		std::string str = "";

		switch (answer) {	//If more than 2 if (var == something), use switch
			case 0:	//Space between case and value, no space after value
				return "Meh...";	//Don't use break if not necessary
			case 42:	//New line between cases
				str = "Nice";	//Indent after case
				break;
			case 111:
				str += "Very ";
			case 11:
				str += "Good!";
				break;
			default:;	//Same here, no need of break at default
			//Only blank default if can't not be one of the cases, this is not a good example for it
		}

		return str;
	}

	void incrementNumApple() {
		numApple++;	//If alone, increment/decrement with ++ after the variable
	}

	virtual unsigned int getType() {
		return CONST_A;
	}

	virtual bool draw(int interpolation) = 0;
};	//Blank line after class ending

#endif
