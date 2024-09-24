# **Suspension-Blocking**

### **Overview**
This project prevents debuggers from suspending your program's threads. It works by injecting a module into a Windows process and then resuming all threads in a loop using the `ResumeThread` function. 

This serves as a **proof of concept** for blocking thread suspension (not detecting, but actively preventing it as most people just detect it). After trying various methods without finding a solid solution, this is the best approach I've come up with so far.

> **Note**: This solution is extremely powerful **if you hide your logic correctly**. If you just paste and use lazy importer and skcrypt, it won't be very effective. as they will see what you are doing. You need to **properly randomize and obfuscate** your logic. It's also recommended to implement a watchdog for the thread and the CMD process.

---

## **How to Use:**

1. The project includes both a DLL and a console application as examples.
2. The **console project** demonstrates how to use the DLL by injecting it into `cmd.exe`. 
   - In a real-world scenario, you would randomize this across multiple processes.
   - The loop then resumes all suspended threads within the target process.
3. **Important**: Ensure that you update the program name in the DLL project to match your target program, or it will not work. Once updated, don't forget to modify the corresponding byte array.

<video width="720" height="480" controls>
  <source src="https://github.com/kaiiskirrrrr/suspension-blocking/raw/main/2024-09-24%2017-27-50.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

I recommend using this as a base idea, and expanding and improving on it on your own. :)
