# Kittens

Software for live mixing and playback of software instruments.

Compile with cmake like anything else. Deps on arch:

`yay -S sdl2 zenity boost`

on debian it needs libboost-system-dev for whatever reason. i don't make the rules.

```
$ cd ..
$ mkdir build
$ cd build
$ cmake ../kittens/
$ make -j16
```

for example ^

MIT license.

# Disclaimer

This code is bad. I'm not a C++ programmer. If you see this code by some absurd chance, I'm open to 
architectural suggestions as long as you aren't a dick or anything. Ultimately this is code for
my own personal use so: "if it works it works".

However, I'm always open to learn.

### [🎶Oh, the redhead said you shred the Trello...🎶](https://trello.com/b/l1v4R3fo/kittens-trello)
