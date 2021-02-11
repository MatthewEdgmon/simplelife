TARGET = simplelife

all: $(TARGET)

simplelife: main.c
	gcc -Wall main.c -o simplelife

clean:
	rm ./$(TARGET)