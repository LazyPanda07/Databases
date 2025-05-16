#include "SQLValue.h"

namespace database
{
	SQLValue::SQLValue(const ValueType& value) :
		value(value)
	{

	}

	SQLValue& SQLValue::operator = (const ValueType& value)
	{
		this->value = value;

		return *this;
	}

	SQLValue& SQLValue::operator = (ValueType&& value) noexcept
	{
		this->value = std::move(value);

		return *this;
	}

	const SQLValue::ValueType& SQLValue::operator *() const
	{
		return value;
	}
}
