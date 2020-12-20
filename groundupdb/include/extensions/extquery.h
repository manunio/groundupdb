//
// Created by maxx on 20/12/20.
//

#ifndef EXTQUERY_H
#define EXTQUERY_H

#include "query.h"

namespace groundupdbext
{
	using namespace groundupdb;
	using Set = std::unique_ptr<std::unordered_set<std::string>>;

	class DefaultQueryResult : public IQueryResult
	{
	public:
		DefaultQueryResult();
		DefaultQueryResult(Set recordKeys);
		virtual ~DefaultQueryResult() = default;

		const Set recordKeys();
	private:
		Set m_recordKeys;
	};
}

#endif //EXTQUERY_H
