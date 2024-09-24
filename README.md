# suspension-blocking

this project will block debuggers from suspending your programs threads, it does this by injecting a module into a Windows process and from there resumes all ur threads in a loop. This is more of a proof of concept about using ResumeThread, i have been trying to block suspension (not detect, block) for a while, and i have never found a solid solution for it, this is the best i have been able to think of. 

> **Note**: this solution is extremely powerful if you hide your logic correctly, if you just use lazy importer and skcrypt its not gonna do much, you need to properly hide randomize the logic to this. You should also implement a watchdog for the thread and CMD.

## How to use:
- included is a dll project and a console project on how to use it, the console project will have a example of how to use it. it injects the dll into cmd, (in a real world scenario you would randomize this accross multiple proceses) and that then runs the loop 
- ensure you change the program name in the dll project to your programs name, or it will not work, after doing that update the byte array.
