# Test makefile

Includes = "Template_Library/container/stack.h" \
		"Template_Library/container/vector.h" \
		"Template_Library/container/list.h" \
		"Template_Library/range.h"



Testing.exe : Testing/main.cpp $(Includes)
	$(CPP) Testing/main.cpp