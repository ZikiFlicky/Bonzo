# Bonzo
A small language that allows you to define rules for creating regular expressions and for searching
for patterns in text.

The language persists only of rules, functions and builtin-functions and allows you
to match text or generate regexes in an easy and readable way.

## Setup
This was tested only on my Ubuntu 20.04 machine.

### Prerequisites
* *nix machine
* CMake >=3.16
* GNU Make
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

To run the hello-world example file:
```
./bonzo examples/hello_world.bz
```

## Todo
- [x] Generate regex
- [X] Our own matching engine
- [X] Better argument handling
- [ ] Show matches in an interactive way (like `less`)
- [ ] More regex syntaxes
- [ ] Better errors (show position in runtime errors)
