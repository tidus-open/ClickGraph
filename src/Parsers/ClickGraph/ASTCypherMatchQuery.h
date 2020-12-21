namespace Graph
{

class ASTCypherMatchQuery : public IAST
{

public:

	enum class Expression : uint8_t
	{
		MATCH,
		WHERE
	};

	String getID(char) const override { return "CypherMatchQuery";}

	ASTPtr clone() const override;

	ASTPtr & refMatch() { return getExpression(Expression::MACTH);}
	ASTPtr & refWhere() { return getExpression(Expression::WHERE);}

	const ASTPtr match() 	const { return getExpression(Expression::MATCH);}
	const ASTPtr where()	const { return getExpression(Expression::WHERE);}

	void setExpression(Expression expr, ASTPtr && ast);

	ASTPtr getExpression(Expression expr, bool clone = false) const
	{
		auto it = positions.find(expr);
		if(it != positions.end())
			return clone ? children[it->second]->clone() : children[it->second];
		return {};
	}


protected:

	void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;l

private:

	std::unordered_map<Expression, size_t> positions;

	ASTPtr & getExpression(Expression expr);

};
};
