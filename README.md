workwork
========

A simple tool to keep track of time spent working (in the shell).

concept:
--------

The three commands (see *usage* below) operate with a "state file". It stores four values:

* `total`: total time spent working
* `start`: timestamp when you began work
* `last`: timestamp when `workwork` was executed the last time
* `state`: the current state, either `+` (working) or `-` (not working)


usage:
------

`readytowork [statefile]`:
Begin working.

`jobsdone [statefile]`:
Stop working and add the interval between the `start` timestamp and now to
`total`.

`workwork [statefile]`:
Continue working, i.e. set the `last` timestamp to the current time.
It's designed to be executed in your shell prompt and will
print the current status as `+` if you're working and `-` if not (without a
newline). If the status is _working_ and the time between the current and last
execution of `workwork` is longer than ten minutes, the status will be set to
_not working_ and the interval between `start` and `last` will be added to
`total`. With this timeout behaviour it won't be a problem if you forget to run
`jobsdone`.

If `statefile` is omitted, `~/.workwork` will be used.
`readytowork` and `jobsdone` will print the `total` time (in hours).
