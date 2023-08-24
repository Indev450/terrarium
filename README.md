Terrarium
=========

2d sandbox game engine with modding support
-------------------------------------------

Controls
--------
| Button              | Action                                             |
|---------------------|----------------------------------------------------|
| a, d                | Move                                               |
| space               | Jump                                               |
| Mouse wheel or b, n | scroll items in hotbar, or scroll crafting recipes |
| i                   | Open inventory                                     |
| e                   | Interact with world                                |
| escape              | Open pause menu (WIP)                              |
| =, -                | Zoom in/out                                        |
| o                   | Toggle debug info                                  |

Dependencies
------------

* Lua 5.4
* SFML 2.6

Building
--------

Install dependencies with your package manager

Clone this repository:

    git clone https://git.phreedom.club/indev/terrarium
    cd terrarium

Create build directory and run build:

    mkdir build && cd build
    cmake ..
    cmake --build . -j$(nproc)
    cd ..

After build finished, you can run the engine from repo root:

    ./build/terrarium <save_name>

It will create `./saves` directory and `./saves/<save_name>/` directory with
save files
