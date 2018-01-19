# C++ Utilities

## Content

* Geo: geometry primitives (point, interval, and box)
* Prettyprint: pretty printing for C++ STL containers
* Log: logging utilities (timer, memory checker and python-style print)

## How to use?
Simple in general.
Check out some examples in `example_main.cpp`.

You can compile it by (in Linux):
```
$ g++ example_main.cpp utils/log.cpp -o example
```

Or (in Windows):
```
$ g++ example_main.cpp utils/log.cpp -lpsapi -o example
```

Note: geo and prettyprint is header only.