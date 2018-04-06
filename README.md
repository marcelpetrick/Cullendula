# Cullendula
Small program to pick out the best shots of the vast amount of taken pictures per session (excursion).
The name itself is a wordplay of the plant Calendula and the activity "to cull" (slang for sorting the photos).


# How to use?
Start it and then drag&drop a folder with the pictures or an example picture to the central area of the app. Cullendula figures out itself which path to use.
It also creates automatically a new folder named "output" inside the given path.
The first picture of the files is loaded automatically too.
Switch between the images via the buttons at the botton of the app or use the arrow-keys (LEFT and RIGHT).
The button "save" (or UP arrow-key) moves the current image to the output-folder.
The button "trash" (or DOWN arrow-key) moves the current image to the trash-folder.
When you are done, then close the app. The result (the best photos) are inside the output-folder :)

This is version 0.1.
Builds and runs with: Linux, Gcc, Qt 5.10 and QtCreator 4.6-beta
Builds and runs with: Windows 7, Qt 5.5 and QtCreator 4.6

note bene: if you don't like, THEN don't use OR help to fix and expand it :)

# History
v0.1 was the basic release; working, but ugly
v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality

# Open tasks
- show left and right (if possible) neighbour of the current image as smaller preview ... so that you have some preview of similar pictures follow
- show position and amount: like: "3/234 output: 7 trash: 10" - maybe in the status-bar? - refine this
- add an icon for the program
- make the used image-file-suffixes configureable
- add some version/help menu-entry
- Important: add a file-existance_check before loading to QPixmap
