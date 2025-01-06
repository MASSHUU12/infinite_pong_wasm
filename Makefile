default: wasm

wasm: main.c
	clang --target=wasm32 -O3 -flto -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--lto-O3 -Wl,-z,stack-size=8388608 -o main.wasm main.c

wat: main.wasm
	wasm2wat main.wasm > main.wat

serve: main.wasm
	python3 -m http.server
