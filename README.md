## Memory Game

This is a school project. Be gentle — I'm learning as I go!

### Game Modes

* **Normal Mode (Meme)**
  Classic memory game, but with Brazilian memes to make it more fun.

* **Random Mode (Aleatório)**
  Randomly selects a theme from `assets/temas.txt` using `random.cpp`.
  Then, `feeder.py` grabs images for that theme and places them in `assets/random`.

* **Choose Your Theme (Escolha)**
  Same logic as Random Mode, but you pick the theme yourself.

* **Speedrun Mode**
  Tracks your time and gives you a little surprise if you finish under 10 seconds.

### System Requirements

Tested on **Fedora Linux**, but it should work on any modern Linux distro.

### Dependencies

#### C++

Well... it's a C++ project.

#### SFML

Handles graphics and window rendering.

#### Python 3

Used to run `feeder.py`:

* `requests` and `duckduckgo_search` for fetching images online.

#### ImageMagick

Used to resize images to `100x100px` via the `mogrify` command.


