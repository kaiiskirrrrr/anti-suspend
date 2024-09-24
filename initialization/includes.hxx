#include <Windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <tlhelp32.h>
#include <winbase.h>
#include <TlHelp32.h>
#include <iostream>
#include <xiosbase>
#include <fstream>  
#include <string>   
#include <thread>
#include <chrono>

#include "injection.hxx"
#include "tools/bytes.hxx"
#include "tools/lazy.hxx"
#include "tools/process.hxx"