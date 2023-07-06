#pragma once

#include <unordered_map>

namespace pineBT
{
	class Blackboard 
	{
	public:
		using Key = short;

		union Value
		{
			bool asBool;
			int asInt;
			float asFloat;
		};

	public:
		Blackboard() = default;

		bool has(Key key) const
		{
			return map.find(key) != map.end();
		}

		template<typename RawValue>
		void set(Key key, RawValue value);

		template<>
		void set<bool>(Key key, bool value)
		{
			map[key] = { .asBool = value };
		}

		template<>
		void set<int>(Key key, int value)
		{
			map[key] = { .asInt = value };
		}

		template<>
		void set<float>(Key key, float value)
		{
			map[key] = { .asFloat = value };
		}

		void clear(Key key)
		{
			map.erase(key);
		}

		template<typename RawValue>
		void get(Key key, RawValue& value) const;

		template<>
		void get<bool>(Key key, bool& value) const
		{
			if (auto found = map.find(key); found != map.end())
				value = found->second.asBool;
		}

		template<>
		void get<int>(Key key, int& value) const
		{
			if (auto found = map.find(key); found != map.end())
				value = found->second.asInt;
		}

		template<>
		void get<float>(Key key, float& value) const
		{
			if (auto found = map.find(key); found != map.end())
				value = found->second.asFloat;
		}

	private:
		std::unordered_map<Key, Value> map;
	};
}
