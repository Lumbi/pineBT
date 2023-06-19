# CMakeList.txt : CMake project for matsuBT, include source and define
# project specific logic here.
#

add_executable (
	${PROJECT_NAME}
	"matsuBT.cpp"
	"BehaviorTree.cpp"
	"Behavior/Composite/Selector.cpp"
	"Behavior/Composite/Sequence.cpp"
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
