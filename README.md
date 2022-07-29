# Bonzo
A small language that allows you to define rules for creating regular expressions.

The language persists only of rules, functions and builtin-functions and allows you
to easily create regexes.

## Setup
This was tested only on Ubuntu 20.04.

### Prerequisites
* *nix machine
* CMake >=3.16
* Make
* C++20 compiler

### Downloading
To download prerequisites on Ubuntu:
```
sudo apt install build-essential cmake gcc-11
```

Clone this repository.

### Compiling

Enter the created directory and run:
```
cmake . && make
```

The executable, called `bonzo`, will be set in the base directory.

### Running
There are some example files in the `examples/` directory.

To run the hello-world file:
```
./bonzo examples/hello_world.bz
```

## Todo
* [*] Generate regex
* [] Our own matching engine
* [] More regex syntaxes
* [] Better errors (show position in runtime errors)
* [] Better argument handling
