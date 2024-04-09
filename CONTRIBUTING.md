# Contributing
We always want people to contribute, but make sure to follow these things:
- No confusing names (like make function names be called something easy to understand).
- Put things in the correct paths:
  - kernel/corelib/
     - Put kernel libraries here.
  - kernel/entry/
     - Avoid putting any thing in here.
  - kernel/
     - Here are the important things placed, like nighterm, serial, filesystem. These will be moved later on when i do a refactor.
  - kernel/system/
     -  Put nearly evertyhing in here in respective folders, drivers: Drivers for thing, keyboard, speaker, etc.
  - arch/
     - Put arch specific code here
