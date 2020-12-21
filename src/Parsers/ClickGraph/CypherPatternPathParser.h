#pragma once

#include <Parsers/IParserBase.h>

namespace DB
{

class ParserCypherMap : public IParserBase
{
	const char * getName() const override { return "CypherMap";}
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;

};



};
