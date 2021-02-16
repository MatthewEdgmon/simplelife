TARGET = simplelife

all: $(TARGET)

simplelife: simplelife.c
	gcc -g -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined simplelife.c -o simplelife

clean:
	rm ./$(TARGET)