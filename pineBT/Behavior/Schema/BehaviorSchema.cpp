#include "BehaviorSchema.h"

using namespace pineBT;
using Options = BehaviorSchema::Options;
using Builder = BehaviorSchema::Options::Builder;

Builder& Builder::boolean(const Options::Key& key)
{
	options.booleans.push_back(key);
	return *this;
}

Builder& Builder::number(const Options::Key& key) {
	options.numbers.push_back(key);
	return *this;
}

Builder& Builder::enumeration(const Options::Key& key, const std::vector<std::string>& cases)
{
	options.enums[key] = cases;
	return *this;
}

Options& Builder::end() {
	return options;
}

Builder Options::build() { return Builder(*this); }

Options Options::merge(const Options& other) const
{
	Options merged(*this);
	merged.booleans.insert(merged.booleans.end(), other.booleans.begin(), other.booleans.end());
	merged.numbers.insert(merged.numbers.end(), other.numbers.begin(), other.numbers.end());
	merged.enums.insert(other.enums.begin(), other.enums.end());
	return merged;
}

BehaviorSchema BehaviorSchema::inherit(const std::string& name, const BehaviorSchema& base, const Options& options)
{
	return {
		name,
		base.hierarchy,
		base.options.merge(options)
	};
}

void BehaviorSchema::print() {
	printf("[%s]\n", name.c_str());
	for (auto&& option : options.booleans) printf(" - %s : BOOL\n", option.c_str());
	for (auto&& option : options.numbers) printf(" - %s : NUMBER\n", option.c_str());
	for (auto&& option : options.enums)
	{
		printf(" - %s : ENUM [ ", option.first.c_str());
		for (auto&& value : option.second) printf("%s ", value.c_str());
		printf("]\n");
	}
}
