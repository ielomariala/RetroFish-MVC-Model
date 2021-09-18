# RetroFish
This is the depository of Network project in C and Java. Welcome !

## Network Project: Fish simulation in an aquarium
This project consists of simmulating numerous fishes swimming an aquarium. The aquarium is controlled by a server who has a list of views and fishes. Every client can connect to a view, which literally a view or unique perspective of the aquarium. Thus, the server or the controller should deliver correct and fast informations to all clients, in order to have proper and coherant visualisation of the fishes in all the views. In order to achieve good results, a good understanding of TCP communication protocols and sockets is compulsory.

In brief, the project goal is to create a centralized fish aquarium. A controller controlls everything related to server, client connexions, aquarium and views. Moreover, a model contains informations about fishes, their dimensions, their patterns and mobilities. Furthermore, the client occupies a view and visualizes the correspondant fishes. Thus, as described, an MVC model is used, as the controlls links between the model and view. 


***

## Team members

- Benmendil Hamza 
- Boullit Mohamed Fayçal 
- Elomari Alaoui Ismail  
- Kini Walid
- Mechach Hamza

## Project Arborescence

*This deposit contains five principal folders :*

- aquariums : contains files desribing different aquariums such as aquarium1 which is the aquarium given to us by project supervisors, and aquarium2 which contains 50 views, used to test server performance and response speed.

- src : contains controller source files in C and client ones in Java:

	- controller : Subfolder containing server, parser, aquarium, mobility, fish, list and view source files, written in C. The controller folders has also a configuration file used to configurate server on startup.

	- client :  Subfolder containing client, console, fish and mainwindow classes, written in Java. JavaFx is used to build the application. The client folder has also a configuration file client.cfg, in order to configurate client on startup (2 different clients have 2 different configuration files, but the server treats other cases), and 2 subfolders:

          - img : Containing differents images for numerous fish types.

          - lib : Containing Javafx library used to simplify compiling and execution in the Makefile, if using Linux.

- build (created dynamically with "make") : contains all compiled class and .o files.

- test : Contains a basic client in C and 2 bash scripts stats.bash and manyclients.bash used to test server performance and speed, as we implemented mutltithreaded selection of clients.

- doc : Contains a Doxyfile used to generate documentation in html format.

*This deposit contains two important files :*

- Makefile : Contains rules for compiling the project, if you are running on Linux or WSL.

- README.md : This is the file you are reading right now :) . The markdown file describes the project.

## Requirements
In order to compile the client part of this project, some requirements need to be met:

First of all, you need to have JDK or JRE, as the client part is programmed in JAVA. 

Moreover, if you are using Linux, you only have to compile and execute the client part using Makefile rules 'make client' and 'make run_client' respectively. This is simple because JavaFX libraries are put in the src/client/lib folder.

However, if you are using Windows you will need to :

  - Download the javafx libraries from the official site : https://openjfx.io/ and put it in a certain file JAVAFX_FOLDER. 

  - Compile the client part with : javac -d build --module-path JAVAFX_FOLDER --add-modules javafx.controls,javafx.fxml src\client\*.java .

  - Execute it with: cd build && java --module-path JAVAFX_FOLDER --add-modules javafx.controls,javafx.fxml MainWindow ..\src\client\client.cfg .

## Build/Run
The commands explained here are working, if you will be using Linux or WSL.

- The command "make" compiles all the source files: controller + client.

- The command "make server" compiles all of the controller source files.

- The command "make client" compiles all of the client source files.

- The command "make run_server" executes the server. A configuration file is used to configurate server on startup, you can modify it accordingly. 

- The command "make run_client" executes the client. The mainwindow of the client will appear. You will find options in the header oh this window where you can open console to write commands to the server, you can also log out or log out and exit. If you log out, you can reconnect to another view, using hello in as N$id where $id is a number identifying an existing view in the used aquarium.

- The command "make doc" generates the documentation of the project.  

      - It generates numerous files in build/server_doc/html folder. You can open "index.html" file in any browser of your choice, you will find proper documentation of the controller part programmed in C. (This is exacltly what the command "make doc_server" does)

      - This command also generates numerous files in build/client_doc folder. You can open "overview-tree.html" file in any browser of your choice, you will find proper documentation of the client/view part programmed in Java. (This is exactly what the command "make doc_client" does)

- The command "make clean" cleans the deposit from any executables, and deletes build folder.

## Achievements
As for the achivements, we've managed to create a fully functionnal application with many features :

- The server is using only 5 threads and connecting 50 clients with impressive performances and response speed. This could only be achieved because of our client's unique way of continous fish demand.

- The server can be exited using 'q' or 'exit' command.

- Server master can modify numerous things: all basic commands described in the subject and more such as fish speed with the command "set speed_randomizer 5".

- Clients have all basic commands described in the subject. Furthermore, fishes' movements, changes of view or presence in many views are so fluid.

## Thanks
We thank all of ENSEIRB-MATMECA Network module supervisors for their ideas and help. 
All in all, we would like to express special thanks and gratitude for Leo Mendiboure, our supervisor.
