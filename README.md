# Name-TBD
  good stuffs

##Updates already commited##
*   Added CPP files for the engine-in-progress.
*   Several files added to model different data structures (most notably the BST, the linked list, and the heap) to store the entity structures.
*   Made the basic framework for updating the player's health and displaying it on screen each frame.
*   The heap implementation is used for all recent updates, e.g.:
*   The program now can load any arbitrary background, as demonstrated by moving the player far, far to the right (off screen) in engine heap.cpp. The background processing is managed by a separate function, the shownewBackground() function, to allow for code reuse.
*   The program is capable of updating the player's HP in real time. The health bar updates in real time as this changes.

##Current Architecture##
*   Note that, in this current architecture of the game engine, much of the processing uses files. Thus, to change which backgrounds exist and what entities they contain by default, one must only change the appropriate files. (No recompilation neccesary :D). So that makes it quite simple and effective.

##Things currently in development:##
*   Getting the inventory to load and render everything in the bag of items - i.e., the displayInventory() function
*   Getting the user interaction to work in more cases (e.g., talking to another character, etc.)
*   Figuring out the frame/sample rate and the timestep of the program, to make sure it renders quickly (avoiding temporal aliasing and other issues). Still working on researching this issue.
