#include <Columns/IColumn.h>
#include <Columns/ColumnsNumber.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/IDataType.h>
#include <Core/Block.h>
#include <Columns/ColumnString.h>
#include <DataTypes/DataTypeString.h>

using namespace DB;

int main()
{
	ColumnPtr col1 = ColumnUInt64::create({0UL, 1UL, 2UL, 3UL, 4UL, 5UL, 6UL, 7UL, 8UL, 9UL});
	ColumnPtr col2 = ColumnUInt32::create({0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U});

	Block block = {ColumnWithTypeAndName(col1, std::make_shared<DataTypeUInt64>(), "id"),
			ColumnWithTypeAndName(col2, std::make_shared<DataTypeUInt32>(), "count")
			};

	std::cout << block.dumpStructure() << std::endl;
	std::cout << block.dumpIndex() << std::endl;

	
	return 0;
}
