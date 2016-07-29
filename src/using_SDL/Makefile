CXX = g++
CXXFLAGS = -Wall -Wwrite-strings -Wshadow -Werror -pedantic-errors -fstack-protector -g
LIBFLAGS = -lSDL2-2.0 -lSDL2_image -lSDL2_ttf

all: movement_test font_test linked_list_engine_test
	@rm -f *.o
	@rm -f a.out

clean:
	rm -f *.o
	@rm -f a.out

linked_list_engine_test: general_engine_with_linkedList.o
	$(CXX) general_engine_with_linkedList.o -o linked_list_engine_test $(LIBFLAGS)

general_engine_with_linkedList.o: general_engine_with_linkedList.cpp general_engine_linkedList.h
	$(CXX) -c general_engine_with_linkedList.cpp $(CXXFLAGS) $(LIBFLAGS)

font_test: font_test.o
	$(CXX) font_test.o -o font_test $(LIBFLAGS)

font_tets.o: font_test.cpp
	$(CXX) -c font_test.cpp $(CXX FLAGS) $(LIBFLAGS)

graphics_test: test.o
	$(CXX) test.o -o graphics_test $(LIBFLAGS)

test.o: test.cpp
	$(CXX) -c test.cpp $(CXXFLAGS) $(LIBFLAGS)

movement_test: movement.o
	$(CXX) movement.o -o movement_test $(LIBFLAGS)

movement.o: movement.cpp
	$(CXX) -c movement.cpp $(CXXFLAGS) $(LIBFLAGS)

engine_test: general_engine.o
	$(CXX) general_engine.o -o engine_test $(LIBFLAGS)

general_engine.o: general_engine.cpp general_engine.h
	$(CXX) -c general_engine.cpp $(CXXFLAGS) $(LIBFLAGS) 

