#include <memory>
#include <Interperters/ExpressionActions.h>
#include <Interperters/ExpressionAnalyzer.h>
#include <Interpreters/Context.h>
#include <Interpreters/IInterpreter.h>

#include <Interpreter/StorageID.h>

#include <Parsers/IAST_fwd.h>
#include <Parsers/ClickGraph/ASTCypherMatchQuery.h>

#include <Columns/FilterDescription.h>

using namespace DB;

namespace Graph
{

class Context;
class QueryPlan;

class InterpreterCypherMatchQuery : public IInterpreter
{

public:

	BlockIO execute() override;

	virtual void buildQueryPlan(QueryPlan & query_plan) override;

	virtual void ignoreWithTotals() override;

	static void addEmptySourceToQueryPlan(QueryPlan & query_plan, const Block & sourec_header, const SelectQueryInfo & query_info);
	

	InterpreterCypherMatchQuery(
		const ASTPtr & query_ptr_,
		const Context & context_,
		const BlockInputStreamPtr & input_,
		std::optional<Pipe> input_pipe,
		const StoragePtr & storage_,
		const Names & required_result_column_names = {},
		const StorageMetadataPtr & metadata_snapshot_ = nullptr);


	virtual void ingoreWitTotals() = 0;
	
	virtual ~InterpreterCypherMatchQuery() override = default;

	Block getSampleBlock() { return result_header; }

	size_t getMaxStreams() const { return max_streams; }	

private:

	ASTCypherMatchQuery & getCypherMatchQuery() { return query_ptr->as<ASTCypherMatchQuery &>(); }

	Block getSampleBlockImpl();
	
	void executeImpl(QueryPlan & query_plan, const BlockInputStreamPtr & prepared_input, std::optional<Pipe> prepared_pipe);

	void executeFetchColumns(
		QueryProcessingStage::Enum processing_stage,
		QueryPlan & query_plan,
		const PrewhereDAGInfoPtr & prewhere_info,
		const NameSet & columns_to_remove_after_prewhere);

	void executeWhere(QueryPlan & query_plan, const ActionsDAGPtr & expression, bool remove_filter);
	void executeAggreation(QueryPlan & query_plan, const ActionsDAGPtr & expression, bool overflow_row, bool final, InputOrderInfoPtr group_by_info);
	static void executeExpression(QueryPlan & query_plan, const ActionDAGPtr & expression, const std::string & description);
	void executeOrder(QueryPlan & query_plan, InputOrderInfoPtr sorting_info);
	void executeLimitBy(QueryPlan & query_plan);
	void executeLimit(QueryPlan & query_plan);
	void executeOffset(QueryPlan & query_plan);
	static void executeProjection(QueryPlan & query_plan, const ActionsDATPtr & expression);
	void executeDistinct(QueryPlan & query_plan, bool before_order, Names columns, bool pre_distinct);
	void executeExtremes(QueryPlan & query_plan);
	
	String generateFilterActions(
		ActionsDATPtr & actions, const ASTPtr & row_policy_filter, const Names & prerequisite_columns = {}) const;
	
	enum class Modificator
	{
		ROLLUP = 0,
		CUBE = 1
	};

	void executeRollupOrCube(QueryPlan & query_plan, Modificator modificator);

	void initSettings();

	TreeRewriteerResultPtr syntax_analyzer_result;
	std::unique_ptr<SelectQueryExpressionAnalyzer> query_analyzer;
	SelectQueryInfo query_info;

	ExpressionAnalysisResult analysis_result;
	FilterInfoPtr filter_info;

	QueryProcessingStage::Enum from_stage = QueryProcessionStage::FetchColumns;

	Names required_columns;
	
	Block source_header;

	std::unique_ptr<InterpreterCypherMatchQuery> interpreter_subquery;

	StoragePtr storage;
	StorageID table_id = StorageID::createEmpty();
	TableLockHolder table_lock;

	BlockInputStreamPtr input;
	std::optional<Pipe> input_pipe;

	Poco::Logger *log;
	StorageMetadataPtr metadata_snapshot;

	
	ASTPtr query_ptr;
	std::shared_ptr<Context> context;
	Block result_header;
	size_t max_streams = 1;

};




};
