add_executable(
	${TARGET_EXE_NAME}
	"pineBTcli.h"
	"pineBTcli.cpp"
 )

 target_link_libraries(
	${TARGET_EXE_NAME}
	PUBLIC
	${TARGET_LIB_NAME}
 )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${TARGET_EXE_NAME} PROPERTY CXX_STANDARD 20)
endif()
