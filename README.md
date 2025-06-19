# MoriNoKomichi
A simple yet effective todo app with a cute aesthetic. Named this way as this app will help you find your 'path' to success in a 'forest' of tasks!

## Features
- Add tasks
- Add subtasks (or todo items) to the tasks
- 4 sections to add your tasks to
- A cute interface!

## Installing
To install, use one of the exe installers (they are self sufficient, so you don't need qt installed to use it) in the releases page.
### Compiling and running from source
Ensure `cmake` is installed. Then, run the following:
```bash
mkdir ./build
cd ./build
cmake ../
make
```
And then in the `./build` folder, you should see an executable you can run to run the program! Additionally, a .exe file is provided in the Releases.

If this does not work, ensure QT is installed. If it still doesn't work, try building it with QT creator.

## Some language used (mainly for developers)
If it's too complex (which it probably won't be, but you never know) here are some important words:
- `section`: one of the 4 sections tasks are placed in (coloured for your convenience, but for developer reference here are the numbers:)
```
Section numbers:
0 | 1
--+--
2 | 3
```
- `task`: a task (the large papery rectangles in the app). These contain `subtasks`
- `sub-task` (also referred to as `todo`): the label+checkboxes that tasks are comprised of.

## Why I have it structured like this
So you have tasks; e.g. 'finish assignment', 'housework', etc. which you can categorise into the sections (which you might decide to be importance, whether it's for the house or for work, etc.).
Then, you have subtasks (or todos); which are the things you need to do to finish the task (e.g. with 'finish assignment' you might have 'research', 'write slides', 'peer review', etc.)

## Documentation
More docs are in [docs/DOCS.md](docs/DOCS.md).
