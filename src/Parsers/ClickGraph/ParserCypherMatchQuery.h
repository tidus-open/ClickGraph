#pragma once

#include <Parsers/IParserBase.h>


namespace DB
{

class ParserCypherMatchQuery : public IParserBase
{

protected:
	const char * getName() const override { return "Cypher Match Query";}
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;

};

};
