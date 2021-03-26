# Interchange File Format

This library provides an API for working with the Interchange File
Format, a hierarchical, structured container format. While you are
unlikely to find files already in this format, you may find the
library useful to save data from your application. The documentation
(in [docs/](https://msharov.github.io/iff)) gives an example of saving
the player object in an RPG game.

Download from github
[release](https://github.com/msharov/iff/releases/latest) page.
C++14 support is needed, using gcc 4.6+, and the
[uSTL library](https://github.com/msharov/ustl/releases/latest).

```sh
$ ./configure --prefix=/usr && make check && make install
```

If uSTL is a static library, you will need to also link with -lsupc++.
Use `pkg-config --libs --static iff` to get the full link command.

Read [documentation in docs/](https://msharov.github.io/iff).
Report bugs at [project](https://github.com/msharov/iff)
[bugtracker](https://github.com/msharov/iff/issues).
