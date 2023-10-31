# Gnilk Logger

Dependency: https://fmt.dev/latest/syntax.html  (src in src/ext/fmt - included)


This the new Logger library. This replaces the 'logger' which I have been used for almost 20 years. 
I want more or less the same interface but a more predictable modern design around it.

This version supports two types of formatting, the {fmt} way and the old printf - both ways through {fmt} library


The interesting stuff is in the following classes
* Log
* LogEvent
* LogManager

The rest is more or less just sugar on the coating (containers, ipc, and other point-less-crap)...

Besides making it a bit modern one of my main goals was this:
```c
void LogWriter::Debug(const char *format, ...) {
    // REDUCE THIS CODE AS MUCH AS POSSIBLE!!!!
}
```
Plus have it flexible, portable and somewhat nice...

# Examples:

## Single In-Process Logger
The most simple version, this will work...
```c++
    auto logger = Logger::GetLogger("test");
    logger->Debug("This is some debug output with printf syntax: %d, %s", 4, "world");
```

What you really ought to be doing, in order to be nice is:
```c++
    Logger::Initialize();
    // Get the logger as a reference instead of a pointer
    auto logger = Logger::GetLog("test");
    // Use the fmt functions (all uses it, but this skips a beat)
    logger->Dbg("This is with fmt {}", 4);
    // Flush the IPC queue before leaving...
    LogManager::Instance().Consume();
```




## TODO List
```text
! Split the logmanager in a 'client' / 'server' part
! Serialization of event's should not be in the Event class but rather in the IPC handling
! Add an in-proc Queue for handing of things to the new LogServer (i.e. replace SendToSinks with a thread)
! LogCache, cache up lines and when a sink attach send them all (default/base impl) to the sink
! Review the Reset function -> it's nice for unit-testing but perhaps not needed...
! LogSink/LogCache, add ability to just have the last X number of cached entries...
! Enable/Disable sink's
! LogLevel for Log's - not just sinks (assume a Sink is configured for everything, but from modules XYZ I just want Errors)
- Add git workflow
 
```