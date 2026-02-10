CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = build/main
SRC = src/*.c


build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFAGS)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

# odpowiedzalny za generowanie .json działanie clangd 
bear:
	bear -- make build

