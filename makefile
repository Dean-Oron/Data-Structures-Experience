TEST = scheduler_test
NAME = thread_pool.cpp

C_FILES:= $(shell find ./src -name *.cpp)
INCLUDES:= -I ./include 
DEPS = ./include/*.hpp
LIB_PATH = /home/dean/Desktop/Dean/git/cpp/project/lib

CC = g++
CFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g 				# For Debbuging
LDFLAGS = -std=c++11 -Wno-unused-parameter -pedantic-errors -Wall -Wextra -DNDEBUG -O3 	# Not Debbug

# Link the test binary to shared objects
test:	libproject.so $(DEPS) ./test/$(TEST).cpp
	$(CC) -L./lib -Wl,-rpath=$(LIB_PATH) $(INCLUDES) $(LDFLAGS) -export-dynamic ./$@/$(TEST).cpp -o $(TEST) -lm -lproject -lboost_system -lboost_filesystem -lboost_chrono -lpthread -lboost_thread -lglut_utils -lm -lpoint -ldl
	mv $(TEST) ./$@/bin
	./$@/bin/$(TEST) 

debug:	./test/$(TEST).cpp libproject.so $(DEPS)
		@$(CC) -L./lib -Wl,-rpath=$(LIB_PATH) $(INCLUDES) $(CFLAGS) ./test/$(TEST).cpp -o main -lproject -lboost_system -lboost_chrono -lm -lpthread -lboost_thread -lglut_utils -lpoint -ldl
		@mv main ./test/debbuging 

libproject.so: $(C_FILES) $(DEPS) 
	$(CC) $(INCLUDES) $(CFLAGS) $(C_FILES) -fPIC -shared -o $@
	@mv $@ ./lib

single: 
	$(CC) -L./lib -Wl,-rpath=$(LIB_PATH) $(INCLUDES) $(CFLAGS) -export-dynamic ./test/$(TEST).cpp ./src/$(NAME) -o $(TEST) -lm -lproject -lboost_system -lboost_filesystem -lboost_chrono -lpthread -lboost_thread -lglut_utils -lm -lpoint -ldl
	mv $(TEST) ./test/bin
	./test/bin/$(TEST) 

run:	
	./test/bin/$(TEST)	

gt: ./test/bin/main
	clear
	@gdbtui ./test/bin/main
	
vlg: 
	@valgrind ./test/bin/$(TEST) 

.PHONY: clean

clean:
	 @rm -f -R *.o *.so *_test
