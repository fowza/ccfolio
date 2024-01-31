all: prepare

install_tests:
	sudo apt-get install gcovr lcov

install:
	sudo apt-get install gcc g++ cmake make doxygen git llvm pkg-config curl zip unzip tar python3-dev clang-format clang-tidy

install_pre_commit:
	sudo pip install pre-commit
	sudo pre-commit install
	sudo pre-commit install-hooks

prepare:
	rm -rf build
	mkdir build