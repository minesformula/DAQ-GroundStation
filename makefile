TARGET = visualizer
SRC_FILES = main.cpp dataSend.cpp 

CXX = g++
CFLAGS = -g -std=c++17 -L/usr/local/lib -lInfluxDB
OBJECTS = $(SRC_FILES:.cpp=.o)
ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	$(DEL) $(TARGET) $(OBJECTS)

# DEPENDENCIES
main.o: main.cpp dataSend.cpp