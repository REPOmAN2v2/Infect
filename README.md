Infection
=========

Epidemic simulator. The original idea, ruleset and gameplay elements were imagined by [Ziddykins](https://github.com/Ziddykins/Infect) but the code itself was developped independently at first. Being written in C, this version is noticeably faster and much less resource intensive.

![alt text](http://i.imgur.com/rJhKu9a.png?1 "A few weeks into the apocalypse")

##Play

Right now, the game isn't really interactive and is simply a simulation. You can either launch the game and use the default arguments or use the flags -x and -y to specify the game width and heigth, -m or --map to specify the path to a file containing a map template, --slow, --fast or --fastest to specify the simulation speed. -x/-y and -m are not compatible.

The default arguments are X = 79, Y = 20, --fast as well as 10 doctors, 3 infected and 1 soldier spread randomly on the map.

Press q to quit the current game after it has ended.

##Issues

The game isn't balanced right now and the outcome seems different from the original perl version. 

##Version

0.9

##Installation

- Install ncurses using you package manager or Cygwin on Windows
- Run ```make```

##Technologies

* C
* Ncurses