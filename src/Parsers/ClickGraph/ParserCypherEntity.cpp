#include <Parsers/ClickGraph/ParserCypherEntity.h>
#include <Parsers/ClickGraph/ParserCypherMapList.h>
#include <Parsers/ClickGraph/ParserCypherAliasAndLabel.h>
#include <Parsers/ClickGraph/ASTCypherMapList.h>
#include <Parsers/ClickGraph/ASTCypherAliasAndLabel.h>
#include <Parsers/ClickGraph/ASTCypherNode.h>

namespace DB
{

bool ParserCypherEntity::parseImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
	ParserCypherMapList map_list_parser;
	ParserCypherAliasAndLabel alias_and_label_parser;
	
	ASTPtr map_list;
	ASTPtr alias_and_label;

	auto parseMapList = [&]() -> bool {
		
		if(pos->type != TokenType::OpeningCurlyBrace) 
		{
			expected.add(pos, " { ");
			return false;
		}

		if(!map_list_parser.parse(pos, map_list, expected))
			return false;
		cypher_entity->addMapList(map_list);
		return true;
		
	};
	
	if(pos->type != opening_type)
		return false;

	++pos;
	
	if(pos->type == TokenType::BareWord || pos->type == TokenType::Colon)	
	{
		if(!alias_and_label_parser.parse(pos, alias_and_label, expected))
			return false;

		cypher_entity->addAliasAndLabel(alias_and_label);
		
		if(pos->type == TokenType::OpeningCurlyBrace){
			if(!parseMapList())
				return false;
		}
	
	} else if(pos->type == TokenType::OpeningCurlyBrace)
	{
		cypher_entity->addAnonAliasAndAllLabel();	
		
		if(!parseMapList())
			return false;
	} else if (pos->type == TokenType::ClosingRoundBracket)
	{
		cypher_entity->addAnonAliasAndAllLabel();	
	}else	
	{
		expected.add(pos, " alias:label or list of { key:value }");
		return false;
	}
	
	if(pos->type != closing_type)
	{
		String err = (getTokenName(closing_type) + String("or list of {key:value}"));
		expected.add(pos, err.c_str());
		return false;
	}
	
	++pos;

	node = cypher_entity;
	
	return true;

}

};
