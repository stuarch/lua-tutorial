CC = gcc
INCLUDE = /usr/include/lua5.1 -llua5.1
TARGET = example

main: $(TARGET).c
	$(CC) -Wall -fPIC -I $(INCLUDE) $(TARGET).c -shared -o $(TARGET).so
