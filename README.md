# Werckmeister's Music Protoyping Tool

## What it wants to be
* a fast and easy way to create sheet music
* that tool in your toolchain, between the initial idea and the details work with your favourite Sequencer/DAW
* a sequencer which can be used without a Graphical User Interface
* a tool which relies on human readable files


## What it doesn't want to be
* a replacement for your favourite Sequencer/DAW
* a score writer
* a programming language

## Editor
There is an editor for sheet files: https://github.com/SambaGodschynski/werckmeister-editor


## Syntax
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notaion syntax. 	So a note is written by letter followed by a number for its duration. If a note has no duration, the last given duration will be used.
e.g.:
```
[ -- a track
{ -- a voice
    c4 d e f | g1
}
{ -- a  further voice
    b1~ | b1
}
]
```

 
# How to build 
## Dependencies
- boost library
- rtmidi

## preparations
### Mac
 - install homebrew
 - `brew install boost` (assuming that is also installing icu4c to /usr/local/opt/icu4c
## prerequisites
if your boost library is not located in a common path (e.g. "/usr/local/lib") make sure that the 
environment variable "BOOST_ROOT" is set. (see https://cmake.org/cmake/help/v3.15/module/FindBoost.html)
## run cmake
- make a `build` directory
- cd into it
### *Nix
- run `cmake ..`
- run `make`
### Windows
- run `cmake --help' 
- look for your "generator" e.g. "Visual Studio 12 2013"
- run `cmake .. -G "Visual Studio 12 2013"`
- open the generated sln file with Visual Studio
- Start Building
## after build
there should be at least 3 files:
- `sheetp` -> a cli "*.sheet"-file compiler
- `sheetp` -> a cli "*.sheet"-file player
- `rehearse` -> run the unit tests

# Compiler
run `./sheetc asheetfile.sheet` to compile a sheet file into a MIDI file.

# Player
run `./sheetp asheetfile.sheet`.

There are several optional arguments:
- `--watch` -> on the fly change detection/recompile
- `--begin` -> start from a specific quarter position
- `--loop` -> play as loop
- `--list` -> lists your detected MIDI devices

