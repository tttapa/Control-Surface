## Building and running the tests

### 1. Clone Google Test

```sh
cd
git clone https://github.com/google/googletest.git
```

By default, the build script will look in your home directory for the google 
test repository.

If you already have the `googletest` repository locally, or if you wish to use a
different path, you can edit this in `Control-Surface/build/CMakeLists.txt.in`.

### 2. Run CMake
```sh
cd Control-Surface # edit this to your installation of the Control-Surface repo.
mkdir build && cd build
cmake ..
```

Normally, you don't have to run CMake if you make changes. Make will will
automatically recompile files that have changed, and all new tests in existing
files are automatically detected as well.  
If you add new implementation (`cpp`) files, Make will not be able to discover
these automatically, so you'll have to run CMake again.

### 3. Build all libraries and the test executable
```sh
make -j$(nproc)
```

### 4. Run the tests
```sh
make test
```

To get the full output, run
```sh
make check
```

### 3 + 4. Build and run in Visual Studio Code
If you're using Visual Studio Code, you can use `CTRL+SHIFT+B` to perform the
last two steps. The build process is defined in the `.vscode/tasks.json` file,
and it will automatically parse the compiler output to display compilation 
warnings and errors in the _Problems_ tab.

### 5. Compile the Arduino examples
You need to install Go and the 
[Arduino Builder](https://github.com/arduino/arduino-builder).
```sh
make arduino
```