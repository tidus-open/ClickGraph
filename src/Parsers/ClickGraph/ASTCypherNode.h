#pragma once

#include <Parsers/IAST.h>
#include <Parsers/ClickGraph/ASTCypherMapList.h>
#include <Parsers/ClickGraph/ASTCypherAliasAndLabel.h>
#include <Parsers/ClickGraph/ASTCypherEntity.h>

namespace DB
{

class ASTCypherNode : public ASTCypherEntity
{
public:
	String getID(char delimiter) const override { return "ASTCypherNode" + (delimiter + String("node"));}
	
	ASTPtr clone() const override { return std::make_shared<ASTCypherNode>(*this);}

};

};

