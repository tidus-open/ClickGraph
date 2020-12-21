#pragma once

#include <Parsers/IAST.h>
#include <Parsers/ClickGraph/ASTCypherEntity.h>

namespace DB
{

class ASTCypherEdge : public ASTCypherEntity 
{
public:
	String getID(char delimiter) const override { return "ASTCypherEdge" + (delimiter + String("Edge"));}
	
	ASTPtr clone() const override { return std::make_shared<ASTCypherEdge>(*this);}

};

};
