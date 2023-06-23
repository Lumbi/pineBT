#pragma once

#include <string>
#include <unordered_map>

namespace pineBT
{
	struct BehaviorSchema
	{
		enum class Hierarchy { 
			NONE, 
			ONE, 
			MANY 
		};

		struct Options
		{
			using Key = std::string;

			std::vector<Options::Key> booleans;
			std::vector<Options::Key> numbers;
			std::unordered_map<Options::Key, std::vector<std::string>> enums;

			struct Builder
			{
				Builder(Options& options) : options(options) {}

				Builder& boolean(const Options::Key& key);
				Builder& number(const Options::Key& key);
				Builder& enumeration(const Options::Key& key, const std::vector<std::string>& cases);

				Options& end();

			private:
				Options& options;
			};

			Builder build();

			Options merge(const Options& other) const;
		};

	public:
		std::string name;
		Hierarchy hierarchy = Hierarchy::NONE;
		Options options;

	public:
		static BehaviorSchema inherit(
			const std::string& name, 
			const BehaviorSchema& base, 
			const Options& options = Options()
		);

		void print();
	};
}
