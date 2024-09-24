this project will block debuggers from suspending your program and its threads, it does this by injecting a module into a windows process and from there resumes all ur threads in a loop.

> **Note**: this solution is extremely powerful if you hide your logic correctly, if you just use lazy importer and skcrypt its not gonna do much, you need to properly hide randomize the logic to this. You should also implement a watchdog for the thread and CMD.

This is more of a proof of concept about using ResumeThread, i have been trying to block suspension (not detect, block) for a while, and i have never found a solid solution for it, this is the best i have been able to think of. 

**How to use:**
included is a dll project and a console project on how to use it, the console project will inject the dll into cmd, (in a real world scenario you would randomize this accross multiple proceses), if u want to modify the dll

note: ensure you change the program name in the dll project to your programs name, or it will not work, after doing that update the byte array.
