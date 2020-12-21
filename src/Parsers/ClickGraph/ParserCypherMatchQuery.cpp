#include <memory>
#include <Pasers/ClickGraph/ASTCypherMatchQuery.h>
#include <Parser/IParserBase.h>
#include <Parser/CommonParsers.h>
#include <Parsers/ExpressionElementParsers.h>
#include <Parsers/ExpressionListParsers.h>
#include <Parsers/ParserSetQuery.h>
#include <Parsers/ParserSampleRatio.h>
#include <Parsers/ClickGraph/ParserCypherMatchQuery.h>
#include <Parsers/ParserWithElement.h>


namespace DB
{

namespace ErrorCodes
{
	extern const int TOP_AND_LIMIT_TOGETHER;

}

bool ParserCypherMatchQuery::parserImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
	auto cypher_match_query = std::make_shared<ASTCypherMatchQuery>();
	node = cypher_match_query;

	ParserKeyword s_match("MATCH");
	ParserKeyword s_where("WHERE");
	ParserKeyword s_return("RETURN");

	ParserNotEmptyExpressionList exp_list_for_return_clause(true);
	ParserExpressionWithOptionalAlias exp_elem(false);

	ASTPtr where_expression;
	ASTPtr return_expression;
	
	//match [pattern] [where expr list] return expr list
	{
		if(!s_match.ignore(pos, expected))
			return false;
	
		if(s_where.ignore(pos, expected))
		{
			if(!exp_elem.ignore(pos, where_expression, expected))
				return false;
		}

		if(s_return.ignore(pos, expected))
		{
			if(!exp_list_for_return_clause.parse(pos, return_expression, expected))
				return false;
		}

	}

	cypher_match_query->setExpression(ASTCypherMatchQuery::Expression::WHERE, std::move(where_expression));
	cypher_match_query->setExpreesion(AStCypherMatchQuery::Expreesion::RETURN, std::move(return_expression));
	return true;

}

};
