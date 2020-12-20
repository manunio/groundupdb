//
// Created by maxx on 20/12/20.
//

#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <unordered_set>

namespace groundupdb
{
	class Query
	{
	public:
		Query() = default;
		virtual ~Query() = default;
	private:
	};

	using Set = std::unique_ptr<std::unordered_set<std::string>>;

	class IQueryResult
	{
	public:
		IQueryResult() = default;
		virtual ~IQueryResult() = default;

		virtual const Set recordKeys() = 0;
	private:
	};

	class BucketQuery : public Query
	{
	public:
		BucketQuery(std::string& bucket);
		virtual ~BucketQuery();

		virtual std::string bucket() const;
	private:
		class Impl;
		std::unique_ptr<Impl> mImpl;
	};
}

#endif //QUERY_H
