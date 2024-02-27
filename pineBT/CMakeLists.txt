set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(
	${TARGET_LIB_NAME}
	SHARED
	"Behavior.cpp"
	"BehaviorTree.cpp"
	"Behavior/Composite/Composite.cpp"
	"Behavior/Composite/Selector.cpp"
	"Behavior/Composite/Sequence.cpp"
	"Behavior/Composite/Parallel.cpp"
	"Behavior/Composite/Monitor.cpp"
	"Behavior/Decorator/Decorator.cpp"
	"Behavior/Decorator/Condition.cpp"
	"Behavior/Decorator/BlackboardCondition.cpp"
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
	"API.cpp"
)

target_include_directories(
	${TARGET_LIB_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
	${TARGET_LIB_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/lib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${TARGET_LIB_NAME} PROPERTY CXX_STANDARD 20)
endif()

if(MSVC)
  target_compile_options(${TARGET_LIB_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
