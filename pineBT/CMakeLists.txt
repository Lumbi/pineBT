# CMakeList.txt : CMake project for pineBT, include source and define
# project specific logic here.
#

add_executable (
	${PROJECT_NAME}
	"pineBT.h"
	"pineBT.cpp"
	"Behavior.cpp"
	"BehaviorTree.cpp"
	"Behavior/Composite/Composite.cpp"
	"Behavior/Composite/Selector.cpp"
	"Behavior/Composite/Sequence.cpp"
	"Behavior/Composite/Parallel.cpp"
	"Behavior/Composite/Monitor.cpp"
	"Behavior/Decorator/Decorator.cpp"
	"Behavior/Decorator/Condition.cpp"
	"Behavior/Task/Task.cpp"
	"Behavior/Mock/MockTask.cpp"
	"Behavior/Mock/MockCondition.cpp"
	"Behavior/Schema/BehaviorSchema.cpp"
	"Behavior/Schema/BehaviorSchemaLibrary.cpp"
	"Builder/BehaviorTreeBuilder.cpp"
	"Builder/BehaviorTreeBuilderContext.cpp"
	"Builder/ParallelBuilder.cpp"
	"Printer/BehaviorTreePrinter.cpp"
	"Query/BehaviorTreeQuery.cpp"
	"Serialization/JSONSerialization.cpp"
 )

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/lib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
