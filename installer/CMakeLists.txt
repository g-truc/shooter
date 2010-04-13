set(NAME "shoot(r)-installer")

file(GLOB SOURCE_FILES *.nsi)

source_group(
	"NSIS Files" 
	FILES 
	${SOURCE_FILES})

add_custom_target(
	${NAME}
	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/)

