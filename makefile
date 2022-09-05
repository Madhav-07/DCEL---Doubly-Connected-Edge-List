CC=g++
TARGET=assignment3

$(TARGET): $(TARGET).cpp function.cpp
	$(CC) $(TARGET).cpp function.cpp -o $(TARGET)

clean:
	rm $(TARGET)