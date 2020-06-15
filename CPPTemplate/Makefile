TARGET=ProjectName
TEST=test_$(TARGET)

INCLUDEFLAGS=-Isrc -Iinclude -Iinclude/interfaces 
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++11 -lpthread -g $(INCLUDEFLAGS)
TESTFLAGS=-lgtest
CXX=g++

SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard include/*.h) \
        $(wildcard include/interfaces/*.h)

TEST_SOURCES=$(filter-out src/main.cpp, $(SOURCES)) \
			$(wildcard test/*.cpp)

TEST_HEADERS=$(HEADERS) \
			 $(wildcard test/*.h)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) Makefile
	@$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

$(TEST): $(TEST_SOURCES) $(TEST_HEADERS) Makefile
	@$(CXX) $(CXXFLAGS) -Itest $(TEST_SOURCES) -o $@ $(TESTFLAGS)

clean:
	@rm $(TARGET) $(TEST)

test: $(TEST)
	@./$(TEST) 2> /dev/null
