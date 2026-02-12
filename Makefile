CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = build/main
SRC = src/*.c
ALL = $(wildcard build/*.ch8)


build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: build
	@printf "Wpisz ROM: "
	@read ROM; ./$(TARGET) "$$ROM"

run_all: build
	for f in $(ALL); do \
		./$(TARGET) "$$f"; \
	done	

clean:
	rm -f $(TARGET)

# odpowiedzalny za generowanie .json działanie clangd 
bear:
	bear -- make build

