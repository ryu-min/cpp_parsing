#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include"tokenizer.h"
#include "parser.h"

using namespace std;

string readFile(const string& fileName) {
	ifstream f(fileName);
	stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

int main()
{
	cout << "simpleparser" << endl;
	string programText = readFile("/home/z1p1t/workspace/cpp_parsing /src/test.myc");
	
	Tokenizer tokenizer;
	vector<Token> tokens = tokenizer.parse(programText);
	
	cout << "================" << endl;
	cout << "=====tokens=====" << endl;
	cout << "================" << endl << endl;

    for (Token currToken : tokens)
	{
		currToken.debugPrint();
	}

    Parser parser;
    parser.parse(tokens);
    parser.debugPrint();
	//1:10
    return 0;
}
