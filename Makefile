# Load environment variables
ifneq (,$(wildcard ./.env))
    include .env
    export
endif
# Command shortcuts
DOCKER_COMPOSE_CMD := sudo docker-compose
DOCKER_CMD := sudo docker
APT_GET_CMD := sudo apt-get
APT_CMD := sudo apt
PYTHON_CMD := sudo python3 -m pip
CMAKE_CMD := cmake
MAKE_CMD := make

# Package lists
BASIC_PACKAGES := gcc g++ $(CMAKE_CMD) $(MAKE_CMD) doxygen graphviz clang-format git llvm pkg-config curl zip unzip tar python3-dev python3-pip clang-tidy meson ninja-build
TEST_PACKAGES := gcovr lcov
DEVELOPMENT_PACKAGES := odb gcovr lcov libsqlite3-dev libargon2-dev libmysqlclient-dev libboost-all-dev
PYTHON_PACKAGES := pre-commit cmake-format

# PostgreSQL settings
PG_IMAGE := postgres:latest
PG_CONTAINER := postgres-api
PG_PORT := 5432
PG_USER := $(PG_USER)
PG_PASSWORD := $(PG_PASSWORD)
PG_DATA_VOLUME := postgres-data

# Directories
BUILD_DIR := build

.PHONY: all prepare tests basic_install dependencies install_pre_commit pre_commit docker clean-docker dependency_graph odb_schema

# Default target
all: prepare basic_install dependencies install_pre_commit tests

# Prepare the build directory
rebuild:
	@echo "Preparing build directory..."
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

run:
	cd build && cmake .. && make
	./build/app/ccfolio-api

# Install basic system packages
basic_install:
	@echo "Installing basic system packages..."
	$(APT_CMD) update && $(APT_CMD) upgrade -y
	$(APT_GET_CMD) install -y $(BASIC_PACKAGES)

# Install test-related packages
tests:
	@echo "Installing test-related packages..."
	$(APT_GET_CMD) install $(TEST_PACKAGES)

# Install development dependencies
dependencies:
	@echo "Installing development dependencies..."
	$(APT_CMD) update
	$(APT_GET_CMD) install -y $(DEVELOPMENT_PACKAGES)
install_pre_commit:
	@echo "Installing and setting up Python-based tools..."
	$(PYTHON_CMD) install $(PYTHON_PACKAGES)
	pre-commit install
	pre-commit install-hooks

# Run pre-commit on all files
pre_commit:
	@echo "Running pre-commit on all files..."
	pre-commit run --all-files

# Build and run using Docker
docker:
	@echo "Building and running using Docker..."
	cd $(BUILD_DIR) && $(CMAKE_CMD) .. && $(MAKE_CMD)
	$(DOCKER_COMPOSE_CMD) up -d

# Clean up Docker containers and images
clean-docker:
	@echo "Cleaning up Docker containers and images..."
	$(DOCKER_CMD) rm -f $($(DOCKER_CMD) ps -a -q)
	$(DOCKER_CMD) rmi -f $($(DOCKER_CMD) images -q)

# Generate dependency graph
dependency_graph:
	@echo "Generating dependency graph..."
	cd $(BUILD_DIR) && $(CMAKE_CMD) .. --graphviz=graph.dot && dot -Tpng graph.dot -o graphImage.png

# Generate ODB schema
odb_schema:
	@echo "Generating ODB schema..."
	odb --std c++11 --database pgsql --generate-query --generate-schema -o ./app/Entities/odb/ ./app/Entities/User.h

db-up:
	docker run -d --name $(PG_CONTAINER) -e POSTGRES_USER=$(PG_USER) -e POSTGRES_PASSWORD=$(PG_PASSWORD) -p $(PG_PORT):5432 -v $(PG_DATA_VOLUME):/var/lib/postgresql/data $(PG_IMAGE)

db-down:
	docker stop $(PG_CONTAINER) && docker rm $(PG_CONTAINER)

db-shell:
	docker exec -it $(PG_CONTAINER) psql -U $(PG_USER)
