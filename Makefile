CC = g++
SOURCE  = handler.cpp

all:
	$(CC) $(SOURCE) -o stankin_analyze

clean:
	rm stankin_analyze