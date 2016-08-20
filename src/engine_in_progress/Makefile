CXX = g++
CXXFLAGS = -ggdb -O0 -Wall -Wwrite-strings -Wshadow -Werror\
	   -pedantic-errors -fno-stack-protector
LIBFLAGS = -lSDL2-2.0 -lSDL2_image -lSDL2_ttf
EXECUTABLES = engine_bst engine_linked_list engine_bst_enemies \
	      engine_linked_list_spaces engine_heap

all: $(EXECUTABLES)
	@rm -f *.o
	@rm -f a.out

clean:
	@rm -f *.o
	@rm -f a.out
	@rm -f *.exe

engine_heap: engine_heap.hpp engine_heap.o
	@$(CXX) -g engine_heap.cpp -o engine_heap.exe $(LIBFLAGS)

engine_heap.o: engine_heap.cpp
	@$(CXX) -c engine_heap.cpp $(CXXFLAGS) $(LIBFLAGS)

engine_bst_enemies: engine_bst_woods_enemies.o
	@$(CXX) -g engine_bst_woods_enemies.o -o engine_bst_enemies.exe $(LIBFLAGS)

engine_bst_woods_enemies.o: engine_bst_woods_enemies.cpp engine_bst.h
	@$(CXX) -c engine_bst_woods_enemies.cpp $(CXXFLAGS) $(LIBFLAGS)

engine_bst: engine_bst.o
	@$(CXX) -g engine_bst.o -o engine_bst.exe $(LIBFLAGS)

engine_bst.o: engine_bst.cpp engine_bst.h
	@$(CXX) -c engine_bst.cpp $(CXXFLAGS) $(LIBFLAGS)

engine_linked_list: engine_linkedList.o
	@$(CXX) -g engine_linkedList.o -o linked_list_engine_test.exe $(LIBFLAGS)

engine_linkedList.o: engine_linkedList.cpp engine_linkedList.h
	@$(CXX) -c engine_linkedList.cpp $(CXXFLAGS) $(LIBFLAGS)

engine_linked_list_spaces: engine_ll_spaces.o
	@$(CXX) -g engine_linkedList.o -o ll_engine_spaces.exe $(LIBFLAGS)

engine_ll_spaces.o: engine_ll_spaces.cpp engine_linkedList.h
	@$(CXX) -c engine_ll_spaces.cpp $(CXXFLAGS) $(LIBFLAGS)

#
# NOTE: the following was not compiling correctly.
# We probably don't want the array implementation anyway,
# but we might want to debug it for good measure later on:
#engine_array: engine_array.o
#	$(CXX) engine_array.o -o engine_test $(LIBFLAGS)
#
#engine_array.o: engine_array.cpp engine_array.h
#	$(CXX) -c engine_array.cpp $(CXXFLAGS) $(LIBFLAGS) 
#
