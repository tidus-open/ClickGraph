#pragma once

#include <Parsers/IAST.h>

namespace DB
{

class ASTCypherAliasAndLabel : public IAST
{
public:
	String alias;
	String label;

	ASTCypherAliasAndLabel() = default;

	String getID(char delimiter) const override { return "ASTCypherAliasAndLabel" + (delimiter + alias + delimiter + label);}
	
	ASTPtr clone() const override { return std::make_shared<ASTCypherAliasAndLabel>(*this);}
};

};
