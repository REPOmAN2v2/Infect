Infection
=========

Epidemic simulator. The original idea, ruleset and gameplay elements were imagined by [Ziddykins](https://github.com/Ziddykins/Infect) but the code itself was developped independently at first. Some balance changes by [ilovecock](https://github.com/ilovecock/Infect) were added later on. Being written in C, this version is noticeably faster and much less resource intensive.

![alt text](http://i.imgur.com/rJhKu9a.png?1 "A few weeks into the apocalypse")

##Play

Right now, the game isn't really interactive and is mostly a simulation. You can affect the game and try different scenarios using the menu at the beginning of the game or by using a few different command line arguments: 

- ```-h``` or ```--help```: print the help
- ```-v``` or ```--version```: print the version information
- ```--slow```: refresh the screen every 10 days
- ```--fast```: refresh the screen every 5 days
- ```--fastest```: refresh the screen every day
- ```--debug```: refresh step-by-step by pressing ```n``` on the keyboard
- ```-m``` or ```--map```: specify the path to a custom map to load it. Incompatible with the following flags.
- ```-x``` and/or ```-y```: specify the dimensions of the map
- ```-d```, ```-i```, ```-n```,```-s```, ```-w```: specify the number of doctors, infected, nurses, soldiers and/or wood

The default arguments are X = 79, Y = 20, --fast. If you don't specify the x/y dimensions, 16 doctors, 16 infected, 32 soldiers and 80 nurses are spread randomly on the map and you get 790 wood logs. They scale with the dimensions automatically. Note that you can only load a map using the command line, not via the menu, right now.

Press q to quit the current game after it has ended.

##Issues

- Either infected who should be dying don't die sometimes or they appear out of thin air. This results in accurate counters but a disproportionate amount of infected on the screen and needs further testing. It might affect other units too, but I haven't noticed.
- The unit logic might not be acting as it should be, so that needs to be tested and reviewed in the future. 

##Version

1.1

##Installation

- Install ncurses using you package manager or Cygwin on Windows
- Run ```make``` to get a release build or ```make DEBUG=1``` to get a debug build, which logs to infect.log

##Technologies

* C
* Ncurses