# CLANG_FORMAT変数にclang-formatを設定
CLANG_FORMAT ?= clang-format

# srcは以下の拡張子を持つファイルを対象とする
SRC := $(shell find src -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \))

# formatはファイル名ではないことを指定
.PHONY: format build run

# フォーマットの適用
format:
	$(CLANG_FORMAT) -i $(SRC)

# アプリケーションのビルド
build:
	cmake --preset=default
	cmake -S . -B build
	cmake --build build
	ln -sf build/compile_commands.json compile_commands.json

# アプリケーションの実行
run:
	./build/Helloworld

