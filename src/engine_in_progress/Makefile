CXX = g++
CXXFLAGS = -Wall -Wwrite-strings -Wshadow -Werror -pedantic-errors -fno-stack-protector -g
LIBFLAGS = -lSDL2-2.0 -lSDL2_image -lSDL2_ttf

all: engine_bst engine_linked_list engine_bst_enemies
	@rm -f *.o
	@rm -f a.out

clean:
	rm -f *.o
	@rm -f a.out

engine_bst_enemies: engine_bst_woods_enemies.o
	$(CXX) engine_bst_woods_enemies.o -o engine_bst_enemies $(LIBFLAGS)

engine_bst_woods_enemies.o: engine_bst_woods_enemies.cpp engine_bst.h
	$(CXX) engine_bst_woods_enemies.cpp -c $(CXXFLAGS) $(LIBFLAGS)

engine_bst: engine_bst.o
	$(CXX) engine_bst.o -o engine_bst $(LIBFLAGS)

general_engine_bst.o: engine_bst.cpp engine_bst.h
	$(CXX) engine_bst.cpp -c $(CXXFLAGS) $(LIBFLAGS)

engine_linked_list: engine_linkedList.o
	$(CXX) engine_linkedList.o -o linked_list_engine_test $(LIBFLAGS)

general_engine_linkedList.o: engine_linkedList.cpp engine_linkedList.h
	$(CXX) -c engine_linkedList.cpp $(CXXFLAGS) $(LIBFLAGS)

# HELLO? Why was this not working... :
#engine_array: engine_array.o
#	$(CXX) engine_array.o -o engine_test $(LIBFLAGS)

#engine_array.o: engine_array.cpp engine_array.h
#	$(CXX) -c engine_array.cpp $(CXXFLAGS) $(LIBFLAGS) 

