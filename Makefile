CC=gcc
CFLAGS=-Wall -Wextra -pthread -g

CONTROLLER_FOLDER=src/controller
BUILD_FOLDER=build
CLIENT_FOLDER=src/client
DOC_FOLDER_SERVER=build/server_doc/
DOC_FOLDER_CLIENT=build/client_doc/
PATH_TO_FX=src/client/lib

SRC		= $(wildcard src/controller/*.c)
_OBJS	= $(patsubst src/controller/%.c, %.o, $(SRC))
OBJS	= $(addprefix $(BUILD_FOLDER)/, $(_OBJS))


all: build server client

server: $(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ -lm

.PHONY: client
client:
	javac -d ${BUILD_FOLDER} --module-path ${PATH_TO_FX} --add-modules javafx.controls,javafx.fxml ${CLIENT_FOLDER}/*.java

run_client:
	cd ${BUILD_FOLDER} && java --module-path ../${PATH_TO_FX} --add-modules javafx.controls,javafx.fxml MainWindow ../${CLIENT_FOLDER}/client.cfg

run_server:
	./server ${CONTROLLER_FOLDER}/controller.cfg

build/%.o: ${CONTROLLER_FOLDER}/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

build : 
	if test -d ${BUILD_FOLDER}; then echo Folder already exists ;else mkdir ${BUILD_FOLDER}; fi

doc_server : build
	if test -d ${DOC_FOLDER_SERVER} ; then echo Folder already exist; else mkdir ${DOC_FOLDER_SERVER}; fi
	cd ${DOC_FOLDER_SERVER} && doxygen ../../doc/Doxyfile
		
doc_client: build
	if test -d ${DOC_FOLDER_CLIENT} ; then echo Folder already exist; else mkdir ${DOC_FOLDER_CLIENT}; fi
	javadoc -d ${DOC_FOLDER_CLIENT} --module-path ${PATH_TO_FX} --add-modules javafx.controls,javafx.fxml src/client/*.java

doc: doc_server doc_client

clean :
	rm -rf ${BUILD_FOLDER} server *.log