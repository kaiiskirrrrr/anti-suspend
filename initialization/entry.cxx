#ifndef ENTRY_CXX
#define ENTRY_CXX

#include "includes.hxx"

/*
    [ please read: ]

    this entire concept relies on the fact your logic for anti suspension is fully hidden, if it's not then it's very easy to bypass this method. 
    if it is properly hidden, this method is extremely powerful. if its not, its useless
*/

auto main() -> std::int32_t
{
	if (!c_resume_injection->start())
	{
		std::cout << "[X] failed to load module \n";
		std::cin.get();
		goto exit;
	}

	while (true)
	{
		std::cout << "[->] program is running..  \n";

		std::this_thread::sleep_for(std::chrono::milliseconds(1700));
	}

exit:
	return 0;
}


#endif // ! ENTRY_CXX
