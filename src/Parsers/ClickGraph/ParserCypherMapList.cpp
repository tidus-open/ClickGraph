#include <Parsers/ClickGraph/ParserCypherMapList.h>
#include <Parsers/ClickGraph/ASTCypherMapList.h>
#include <Parsers/ExpressionElementParsers.h>
#include <Parsers/ASTLiteral.h>

namespace DB
{

bool ParserCypherMapList::parseImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
	ParserLiteral literal;
	auto map_list = std::make_shared<ASTCypherMapList>();
	if(pos->type != TokenType::OpeningCurlyBrace)
		return false;
	
	++pos;
	
	while(true)
	{
		
		if(pos->type!= TokenType::BareWord)
		{
			expected.add(pos, "substitution name (identifier)");
			return false;
		}

		String prop_name(pos->begin, pos->end);
		++pos;

		if(pos->type != TokenType::Colon)
		{
			expected.add(pos, "colon between name and value");
			return false;
		}
		
		++pos;

		if(pos->type == TokenType::Number || pos->type == TokenType::StringLiteral)
		{
			ASTPtr value;
			if(!literal.parse(pos, value, expected))
				return false;

			map_list->addMap(prop_name, value->as<ASTLiteral &>());
		}else 
		{
			expected.add(pos, "Number or String");
			return false;
		}

		if(pos->type == TokenType::Comma)
		{
			++pos;
			continue;
		}else if(pos->type == TokenType::ClosingCurlyBrace) {
			++pos;
			break;
		}else {

			expected.add(pos, ", or } ");
			return false;
		}
	}
	
	node = map_list;
	return true;
}

};
