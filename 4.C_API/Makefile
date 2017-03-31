CC = gcc
INCLUDE = `pkg-config --cflags --libs lua51`
TARGET = example

main: $(TARGET).c
	$(CC) -Wall -fPIC -I $(INCLUDE) $(TARGET).c -shared -o $(TARGET).so
