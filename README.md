# ABOUT

This is my university project for discrete math course.
It is public only in case anyone wants to take a look at it,
after time where i got my grade and forgot it exist.

# WARNING

This is **UNTESTED** software. I do not garantuee it even compiles anywhere besides Linux.
Thus if you are a teacher from my KNUBA, consider creating discussion or issue.

Also program may crash in very edge-case scenarios, because i didn't had time to fully test it.

# COMPILATION

Before anything you need to install [Raylib] (https://www.raylib.com/) on your platform, follow the official instructions for your platform.

Project has Makefile. To build the project you only need to run it

**IF YOU RUNNING NON LINUX SYSTEM EDIT OUT ALL X11 RELATED LIBRARIES FROM A MAKEFILE, OTHERWISE CODE WILL NOT COMPILE**

This will just build the project
```
  $ make build
```

This will build it and Run it, assuming your file with graph data is called exactly `data`.
```
  $ make run
```

