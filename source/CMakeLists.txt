set(NAME "shoot(r)")

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INLINE_FILES *.inl)
file(GLOB HEADER_FILES *.hpp)
file(GLOB INSTALL_FILES ../installer/*.nsi)

source_group(
	"Inlines Files" 
	FILES 
	${INLINE_FILES})

source_group(
	"NSIS Files" 
	FILES 
	${INSTALL_FILES})

add_executable(
	${NAME}
	${SOURCE_FILES} 
	${INLINE_FILES}
	${HEADER_FILES}
	${INSTALL_FILES})

add_dependencies(
	${NAME} 
	${TARGET_DATA_COPY})

add_dependencies(
	${NAME} 
	${TARGET_INSTALLER_COPY})

target_link_libraries(
	${NAME}
	${BINARY_FILES})
