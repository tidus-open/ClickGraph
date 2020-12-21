#pragma once

#include <Parsers/IParserBase.h>

namespace DB
{

class ParserCypherMapList : public IParserBase
{
	const char * getName() const override { return "CypherMapList"; }
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;
};

};
