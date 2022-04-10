# output binary name
NAME=tetris

# output directory name
DIRECTORY=build

# log prefix
PREFIX=[tetris]

# create the output director and compile to source code into it
all: directory compile

# script to compile the project
compile:
	@echo "$(PREFIX) Compiling source code"
	@g++ -fdiagnostics-color=always -g src/*.cpp -o ${DIRECTORY}/${NAME} -lncursesw
	@echo "$(PREFIX) Saved binary file to '${CURDIR}/${DIRECTORY}/${NAME}'"

# script to create the output directory
directory:
	@if [ ! -d ${DIRECTORY} ]; then \
		echo "$(PREFIX) Creating output directory"; \
		mkdir -p $(DIRECTORY); \
		echo "$(PREFIX) Created output directory '${CURDIR}/${DIRECTORY}'"; \
	else echo "$(PREFIX) Found output directory at '${CURDIR}/${DIRECTORY}'"; \
	fi