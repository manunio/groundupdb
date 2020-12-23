//
// Created by maxx on 20/12/20.
//

#ifndef EXTQUERY_H
#define EXTQUERY_H

#include "query.h"

namespace groundupdbext
{
	using namespace groundupdb;

	class DefaultQueryResult : public IQueryResult
	{
	public:
		DefaultQueryResult();
		explicit DefaultQueryResult(std::unique_ptr<std::unordered_set<std::string>> recordKeys);
		virtual ~DefaultQueryResult() = default;

		const std::unique_ptr<std::unordered_set<std::string>> recordKeys();
	private:
		std::unique_ptr<std::unordered_set<std::string>> m_recordKeys;
	};
}

#endif //EXTQUERY_H
