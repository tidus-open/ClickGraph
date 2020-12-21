#pragma once

#include <Parsers/IParserBase.h>

namespace DB
{

class ParserCypherAliasAndLabel : public IParserBase
{

	const char * getName() const override { return "CypherAliasAndLabel"; }
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;

};

};
