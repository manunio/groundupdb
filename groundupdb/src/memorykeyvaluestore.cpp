//
// Created by maxx on 18/12/20.
//
#include "extensions/extdatabase.h"
#include "extensions/highwayhash.h"

#include <unordered_map>

namespace groundupdbext
{
	class MemoryKeyValueStore::Impl
	{
	public:
		Impl();
		explicit Impl(std::unique_ptr<KeyValueStore>& toCache);

		std::unordered_map<std::string, std::string, HighwayHash> m_KeyValueStore;
		std::unordered_map<std::string, std::unordered_set<std::string>, HighwayHash> m_listStore;
		std::optional<std::unique_ptr<KeyValueStore>> m_cachedStore;
	private:

	};

	MemoryKeyValueStore::Impl::Impl()
		: m_KeyValueStore(), m_listStore(), m_cachedStore()
	{
		;
	}

	MemoryKeyValueStore::Impl::Impl(std::unique_ptr<KeyValueStore>& toCache)
		: m_KeyValueStore(), m_listStore(), m_cachedStore(toCache.release())
	{
		;
	}

	MemoryKeyValueStore::MemoryKeyValueStore()
		: mImpl(std::make_unique<MemoryKeyValueStore::Impl>())
	{
		;
	}

	MemoryKeyValueStore::MemoryKeyValueStore(std::unique_ptr<KeyValueStore>& toCache)
		: mImpl(std::make_unique<MemoryKeyValueStore::Impl>(toCache))
	{
		mImpl->m_cachedStore->get()->loadKeysInfo([this](std::string key, std::string value)
		{
			mImpl->m_KeyValueStore.insert({ key, value });
		});
	}

	MemoryKeyValueStore::~MemoryKeyValueStore()
	{
		;
	}

	// Key-Value use cases
	void MemoryKeyValueStore::setKeyValue(std::string key, std::string value)
	{
		// Also write to our in-memory unordered map
		// Note: insert on unordered_map does NOT perform an insert if the key already exists
		mImpl->m_KeyValueStore.erase(key);
		mImpl->m_KeyValueStore.insert({ key, value });
		if (mImpl->m_cachedStore)
			mImpl->m_cachedStore->get()->setKeyValue(key, value);
	}

	std::string MemoryKeyValueStore::getKeyValue(std::string key)
	{
		// Only ever read from our in memory map!
		const auto& v = mImpl->m_KeyValueStore.find(key);
		if (v == mImpl->m_KeyValueStore.end())
			return ""; // DANGEROUS! Should be 'not found'. TODO: error handling.
		return v->second;
	}

	void MemoryKeyValueStore::loadKeysInfo(std::function<void(std::string, std::string)> callback)
	{
		for (auto& element: mImpl->m_KeyValueStore)
		{
			callback(element.first, element.second);
		}
	}

	void MemoryKeyValueStore::clear()
	{
		mImpl->m_KeyValueStore.clear();
		if (mImpl->m_cachedStore)
			mImpl->m_cachedStore->get()->clear();
	}

	void MemoryKeyValueStore::setKeyValue(std::string key, std::string value, std::string bucket)
	{
		setKeyValue(key, value);
		if (mImpl->m_cachedStore)
			mImpl->m_cachedStore->get()->setKeyValue(key, value, bucket);
		// Note: Bucket indexing implemented at DATABASE level, not kv store level
	}

	void MemoryKeyValueStore::setKeyValue(std::string key, std::unordered_set<std::string> value)
	{
		mImpl->m_listStore.erase(key);
		mImpl->m_listStore.insert({ key, value });
		if (mImpl->m_cachedStore)
			mImpl->m_cachedStore->get()->setKeyValue(key, value);
	}

	std::unique_ptr<std::unordered_set<std::string>> MemoryKeyValueStore::getKeyValueSet(std::string key)
	{
		const auto& v = mImpl->m_listStore.find(key);
		if (v == mImpl->m_listStore.end())
		{
			// try underlying store first
			if (mImpl->m_cachedStore)
				return mImpl->m_cachedStore->get()->getKeyValueSet(key);
			return std::make_unique<std::unordered_set<std::string>>(); //copy ctor
		}
		return std::make_unique<std::unordered_set<std::string>>(v->second); //copy ctor

	}

}