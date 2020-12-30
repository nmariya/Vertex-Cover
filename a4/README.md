# Vertex-Cover using CNF-SAT solver

You will need to checkout minisat to the main directory of the project.
You can do this using

```
git clone https://github.com/agurfinkel/minisat
```

You might need to modify `CMakeLists.txt` if you are adding additional
source files for your code and/or tests.

Here, we solve the minimal Vertex Cover problem for the input graph. My approch is based on a polynomial-time reduction to CNF-SAT solver.

As mentioned before you should use aCMakeLists.txt le to build your project.  We will build your project using the following sequence:

cd a4 && mkdir build && cd build && cmake .
