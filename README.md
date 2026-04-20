# Cullendula
A program to pick out the best shots of the vast amount of taken pictures per photo session.  
The name itself is a wordplay of the plant `Calendula` and the activity "to cull" (slang for sorting the photos).

## How to use?
Start it and then drag&drop a folder with the pictures or an example picture to the central area of the app. Cullendula figures out itself which path to use.  
It also creates automatically a new folder named "output" inside the given path.  
The first picture of the files is loaded automatically too.  
Switch between the images via the buttons at the botton of the app or use the arrow-keys (LEFT and RIGHT).  
The button "save" (or UP arrow-key) moves the current image to the output-folder.  
The button "trash" (or DOWN arrow-key) moves the current image to the trash-folder.  
When you are done, then close the app. The result (the best photos) are inside the output-folder :)  

![](Cullendula_current_state.png)

## Build
```
cmake -S . -B build
cmake --build build
./build/src/Cullendula
```

## Run the tests after building

The unit tests cover the `CullendulaUndoStack` behavior from the command line. They verify:

* initial empty stack state
* push/undo/redo semantics
* returned source/target path pairs
* clearing redo-history after a new push

You can run them in three supported CLI ways:

```
cmake --build build --target test

cmake --build build --target check

./build/tests/CullendulaTests
```

At the moment the test suite contains one test executable registered with CTest:

* `CullendulaUndoStackTest`

## Build information
This is version 0.5.5.

### Builds and runs with:
* Linux, cmake 4.1, GCC 15.2.1, Qt 5.15.17 (and QtCreator 17)
* not supported nor tested anymore:
  * Windows 7, Qt 5.5 and QtCreator 4.6 (todo: test)  
  * Win 10, Qt 5.15.1 and Qt 6.0 beta with MinGW 8.1 and QtCreator 4.13.2  

## History
* v0.1 was the basic release; working, but ugly
* v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality
* v0.3 added tooltips; fixed the "pumping center-label"-issue; added menus; fixed some resizing-issues with the image-label
* v0.4 added undo/redo-functionality with unit-test; added a nice violet icon for the executable and program
* v0.5 moved the buildsystem to cmake (from qmake)
* v0.5.4 fixed the undo-stack unit tests, clarified the CLI test workflow, and corrected the README
* v0.5.5 wired the existing icon into the Qt app runtime and platform-specific build hooks

## Open tasks
* show left and right (if possible) neighbour of the current image as smaller preview ... so that you have some preview of similar pictures follow
* show position and amount: like: "3/234 output: 7 trash: 10" - maybe in the status-bar? - refine this
* add an icon for the program
* make the used image-file-suffixes configureable
* DONE: add some version/help menu-entry
* important: add a file-existance_check before loading to QPixmap
* show the name + path of the current file
* add possibility to open the current file directly
