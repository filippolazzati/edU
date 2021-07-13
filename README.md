# edU

Implementation of a simple text editor, with specific spatial-temporal constraints. An [**ed**](https://www.gnu.org/software/ed/) text editor with multiple undo/redo.

## Language

C

## Usage

Just compile it through, for instance, the gcc compiler:
```
gcc main.c -o main
```
and after run it.

## Commands

edU has a command line interface and provides 6 different commands:
* c -> change
* d -> delete
* p -> print
* u -> undo
* r -> redo
* q -> quit

The first three commands have the same syntax `(add1,add2)x`, where add1 is the first line to consider and add2 is the last line.
The syntax for u and r is `(num)y`, where num represents the number of undo/redo to perform.
The q command is written standalone.
