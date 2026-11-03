.PHONY: all build run tokens check clean compile-commands format check-format lint check-style
all: build
build:
	@./utils/build.sh
run:
	@./utils/run.sh $(ARGS)
tokens:
	@./utils/run.sh --tokens $(ARGS)
check:
	@./utils/check.sh
clean:
	@./utils/bazel.sh clean
	@python3 ./utils/clean.py
compile-commands:
	@python3 ./utils/compile_commands.py
format:
	@python3 ./utils/style.py format
check-format:
	@python3 ./utils/style.py check-format
lint:
	@python3 ./utils/style.py lint
check-style:
	@python3 ./utils/style.py check
