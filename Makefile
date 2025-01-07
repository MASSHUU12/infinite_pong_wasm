.PHONY: all build clean serve

CC = clang
OUTPUT_FILE = main.wasm
CFLAGS = --target=wasm32 -Oz -s -flto -nostdlib -fno-builtin-memset -Wall -Wextra
LDFLAGS = -Wl,--no-entry -Wl,--export-all -Wl,--lto-O3 -Wl,-z,stack-size=8388608 -Wl,--allow-undefined

all: build

build: $(OUTPUT_FILE)

$(OUTPUT_FILE): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

main.wat: $(OUTPUT_FILE)
	wasm2wat $< > $@

serve: $(OUTPUT_FILE)
	python3 -m http.server

clean:
	$(RM) *.wasm *.wat
