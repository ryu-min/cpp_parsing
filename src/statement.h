#pragma  once

#include <string>
#include <vector>

#include "type.h"



class Statement {
public:
	enum class StatementKind {
		VARIABLE_DECLARATION,
		FUNCTION_CALL
	};
	
public:
	std::string name;
	Type type;
	std::vector<Statement> statements;
	StatementKind kind = StatementKind::FUNCTION_CALL;

	void debugPrint() const;
};

