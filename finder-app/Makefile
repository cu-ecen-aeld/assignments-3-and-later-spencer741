CC=$(if $(CROSS_COMPILE), $(CROSS_COMPILE)gcc,gcc)

CFLAGS=-Wall -Wextra

TARGET=writer

SOURCES=writer.c

all: $(TARGET)

$(TARGET): $(SOURCES) 
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
