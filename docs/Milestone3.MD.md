
This week - ⚙️ Features added
New positional features to allow swapping in battle, and being forcefully moved during combat.

Floor functionality which is just chaining rooms together randomly

Town/Shop Functionality, which for this allows the recruitment of new characters, allows for stress reduction, and inventory management/shop to restock.

New Character Types, using my cookie cutter style, i made new character types, corresponding to the other classes in darkest dungeon, IE: Hellion, arbalest, musketeer, man at arms

Main campaign option which is just the refactored function code from the test cases, and new menu's with some additional function to ensure smooth experience.


- 🧱 Updates to your system design
  
  Complete overhaul of most menu's to add color contrast to make them easier to read, and making sure you don't have to scroll when opening a new screen.
  
  
  - 🧹 Refactoring improvements
     my refactors this week, have been to how position is handled, another to the massive Battle.cpp break up, turning that massive file which had outlived its purpose and bloated way too much. in its new configuration, it handles the same functions, but now in numerous smaller files, which are easier to parse through, and make changes in then the enormous file it was previously. 
     
     The overhaul to position was necessary to add being knocked back in combat, its now handled in its own class and in a new way to allow for modification more easily.

- 💭 What you changed and why
  i made all these changed because files were beginning to get bloated, and for a system so integral to gameplay like the position function, it made sense to take it and make it its own class rather than being handled in battle, which makes it now easier to apply skill/attack rules, change and modify position, and modify the function of the files if i need to in the future rather than having to pour through a giant file and hope it doesn't mess anything up