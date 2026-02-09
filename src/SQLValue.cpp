#include "SqlValue.h"

namespace database
{
	SqlValue::SqlValue(size_t value) :
		value(static_cast<int64_t>(value))
	{

	}

	const SqlValue::ValueType& SqlValue::operator *() const
	{
		return value;
	}

	const SqlValue::ValueType* SqlValue::operator ->() const
	{
		return &value;
	}
}
