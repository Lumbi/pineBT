#pragma once

#include <vector>
#include <memory>
#include <string>
#include <format>

#include "Behavior/Schema/BehaviorSchema.h"

namespace pineBT
{
	class Behavior
	{
	public:
		using ID = uint32_t;

		enum class Result
		{
			RUNNING,
			FAILURE,
			SUCCESS,
			ABORTED,
			INVALID
		};

		struct Option;

	public:
		Behavior() = default;

		~Behavior() {}

		virtual void configure(const Option&) = 0;

		Result run();

		Result getResult() const { return result; }

		bool isCompleted() const;

		virtual void abort();

		virtual std::string toString() const { return "Behavior"; }

		virtual void addChild(Behavior*) = 0;

	public:
		ID id = 0;

	protected:
		virtual void onEnter() {}

		virtual Result update() = 0;

		virtual void onExit(Result) {}

	private:
		Result result = Result::INVALID;

	public:
		struct Option
		{
			using Key = std::string;
			Key key;

			struct Value
			{
				enum class Type 
				{
					BOOLEAN, 
					NUMBER, 
					CASE
				} tag;

				union 
				{
					bool asBoolean;
					float asNumber;
					int asCase;
				};
			};
			Value value;

			template<Value::Type valueType, typename ValueType>
			static Option from(const Key& key, ValueType value);

			template<>
			static Option from<Value::Type::BOOLEAN, bool>(const Key& key, bool value)
			{
				Option option { key };
				option.value.tag = Value::Type::BOOLEAN;
				option.value.asBoolean = value;
				return option;
			}

			template<>
			static Option from<Value::Type::NUMBER, float>(const Key& key, float value)
			{
				Option option { key };
				option.value.tag = Value::Type::NUMBER;
				option.value.asNumber = value;
				return option;
			}

			template<>
			static Option from<Value::Type::CASE, int>(const Key& key, int value)
			{
				Option option{ key };
				option.value.tag = Value::Type::CASE;
				option.value.asCase = value;
				return option;
			}
		};
	};
	
	std::string name(Behavior::Result);
}
