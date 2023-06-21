#include "BehaviorTreePrinter.h"

using namespace pineBT;

void BehaviorTreePrinter::print() {
	std::stack<Behavior*> behaviors;
	behaviors.push(tree.getRoot());
	int indent = 0;
	const int spacing = 2;
	Behavior* END_INDENT = nullptr;

	while (!behaviors.empty()) {
		Behavior* behavior = behaviors.top();
		behaviors.pop();

		if (behavior == END_INDENT) // NULL is the end indent 
		{
			indent -= spacing;
			continue;
		}

		printf("%*s - %s\n", indent, "", behavior->toString().c_str());

		if (auto composite = dynamic_cast<Composite*>(behavior); composite)
		{
			behaviors.push(END_INDENT);
			auto children = composite->getChildren();
			for (auto child = children.rbegin(); child != children.rend(); child++)
				behaviors.push(*child);
			indent += spacing;
		}
		else if (auto decorator = dynamic_cast<Decorator*>(behavior); decorator)
		{
			behaviors.push(END_INDENT);
			behaviors.push(decorator->getChild());
			indent += spacing;
		}
	}
}
