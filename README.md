# **Suspension-Blocking**

### **Overview**
This project prevents debuggers from suspending your program's threads. It works by injecting a module into a Windows process and then resuming all threads in a loop using the `ResumeThread` function. 

This serves as a **proof of concept** for blocking thread suspension (not detecting, but actively preventing it as most people just detect it). After trying various methods without finding a solid solution, this is the best approach I've come up with so far.

> **Note**: This solution is extremely powerful **if you hide your logic correctly**. If you just paste and use lazy importer and skcrypt, it won't be very effective. as they will see what you are doing. You need to **properly randomize and obfuscate** your logic. It's also recommended to implement a watchdog for the thread and the CMD process.

---

## **How to Use:**

1. The project includes both a DLL and a console application as examples.
2. The **console project** shows how to use the DLL by injecting it into `cmd.exe`. 
   - In a real-world scenario, you would randomize this across multiple processes.
   - The loop then resumes all suspended threads within the target process.
3. **Important**: Ensure that you update the program name in the DLL project to match your target program, or it will not work. Once updated, don't forget to modify the corresponding byte array.

## tips:

1. Find your loader in the module using a rare identifier, instead of the program name.
2. Setup a watchdog/heart beat, this will stop people from killing your threads.
3. You can also generate a random interval when you start your program then increment a variable by that amount throughout your threads, then have a precalucated value and check it equals to that, if it doesnt they have jmp over something or killed your thread
4. randomize the processes you inject into, only injecting into CMD is not a good idea.
5. before you inject into the process, find the amount of threads it currently has, store that in a integer and increment it by one, then check and make sure it equals that amount of threads, if it doesnt they have killed the anti suspend thread
6. make sure the program your injecting into is compatible with all things above^, and ofc ensure the instance of the program your injecting into is always active

https://github.com/user-attachments/assets/2c0a4b1b-f4ac-42e4-b317-3b31f7a9ac34

I recommend using this as a base idea, and expanding and improving on it on your own. :)
