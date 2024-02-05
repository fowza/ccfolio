all: prepare

tests:
	sudo apt-get install gcovr lcov

install:
	sudo apt-get install -y gcc g++ cmake make doxygen git llvm pkg-config curl zip unzip tar python3-dev clang-format clang-tidy meson ninja-build

install_pre_commit:
	sudo pip install pre-commit
	sudo pre-commit install
	sudo pre-commit install-hooks

pre_commit:
	pre-commit run --all-files

prepare:
	rm -rf build
	mkdir build

docker:
	cd build && cmake .. && make
	sudo docker-compose up -d

clean-docker:
	sudo docker rm -f $(shell sudo docker ps -a -q)
	sudo docker rmi -f $(shell sudo docker images -q)

dependency:
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graphImage.png

odb_schema:
	odb --std c++11 --database sqlite --generate-query --generate-schema -o ./src/Domain/Entities/odb/ ./src/Domain/Entities/User.h
