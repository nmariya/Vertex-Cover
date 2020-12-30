# Inter-Process Communication

IPC.cpp is the driver program that uses the Inter-Process Communication (IPC) to link the output of the random input generator (rgen.cpp) to the input of the python script and the output of the python script to the input of the Shortest_path.cpp so that we create a pipeline, where each code communicates with th4e other.

Follow the CMake instruction sequence to execute the code:

cd a3 && mkdir build && cd build && cmake ../ && make install
