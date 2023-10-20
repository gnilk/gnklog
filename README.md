# Gnilk Logger

Dependecy: https://fmt.dev/latest/syntax.html  (src in src/ext/fmt - included)
wef


This the new Logger library. This replaces the 'logger' which I have been used for almost 20 years. 
I want more or less the same interface but a more predictable modern design around it.

This version supports two types of formatting, the {fmt} way and the old printf - both ways through {fmt} library


The interesting stuff is in the following classes
* Log
* LogEvent
* LogManager

The rest is more less just sugar on the coating (containers, ipc, and other point-less-crap)...

Besides making it a bit modern one of my main goals was this:
```c
void LogWriter::Debug(const char *format, ...) {
    // REDUCE THIS CODE AS MUCH AS POSSIBLE!!!!
}
```

Plus have it flexible, portable and somewhat nice...
