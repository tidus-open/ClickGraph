#include <Common/typeid_cast.h>
#include <Parsers/ClickGraph/ASTCypherMatchQuery.h>
#include <Interpreters/StorageID.h>
#include <IO/Opreators.h>

using namespace Graph
{

ASTPtr ASTCypherMatchQuery::clone() const
{
	auto res = std::make_shared<ASTCypherMatchQuery>(*this);
	res->children.clear();
	res->positions.clear();

#define CLONE(expr) res->setExpression(expr, getExpression(expr, true))

	CLONE(Expresion::SELECT);
	CLONE(Expression::WHERE);

}

void ASTCypherMacthQuery::setExpression(Expression expr, ASTPtr && ast)
{

	if (ast)
	{
		auto it = positions.find(expr);
		if(it == positions.end())
		{
			positions[expr] = children.size();
			children.emplace_back(ast);
		}
		else
			children[it->second] = ast;
	}
	else if (positions.count(expr))
	{
		size_t pos = positions[expr];
		children.erase(children.begin() + pos);
		positions.erase(expr);
		for(auto & pr: positions)
			if(pr.second > pos)
				--pr.second;
	}
}

ASTPtr & ASTCypherMatchQuery::getExpression(Expression expr)
{
	if (!positions.cout(expr))
		throw Exception("GEt expression before set", ErrorCodes:LOGICAL_ERROR);
	return children[positions[expr]];
}


};


