# Control Surface
An Arduino library for MIDI control surfaces (input and output).  
## UNDER CONSTRUCTION
Should not be considered stable. API is bound to change in the near future. 
Master might not even compile on some boards.

## Documentation
The automatically generated Doxygen documentation for this library can be found 
[here](https://tttapa.github.io/Control-Surface/Doc/Doxygen/index.html). 
(incomplete)

## Building and running the tests

### 1. Clone Google Test

```sh
cd
git clone https://github.com/google/googletest.git
```

If you already have the `googletest` repository locally, or if you wish to use
a different path, you can edit this in `Control-Surface/build/CMakeLists.txt.in`.

### 2. Run CMake
```sh
cd Control-Surface # edit this to your installation of the Control-Surface repo.
cd build
cmake .
```

You have to run CMake each time you add files or change dependencies. Other than
that, Make will do most of it for you: it will automatically recompile files 
that have changed, and all new tests are automatically detected as well.

### 3. Build all libraries and the test executable
```sh
make -j$(nproc)
```

### 4. Run the tests
```sh
make test
```

### 3 + 4. Build and run in Visual Studio Code
If you're using Visual Studio Code, you can use `CTRL+SHIFT+B` to perform the
last two steps. The build process is defined in the `.vscode/tasks.json` file,
and it will automatically parse the compiler output to display compilation 
warnings and errors in the _Problems_ tab.