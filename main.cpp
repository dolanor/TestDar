#include <iostream>
#include <string>
#include <string.h>
#include <dar/libdar.hpp>
#include <libgen.h>

using namespace std;
using namespace libdar;

// our own callback functions.
// for the illustration of what theses 'context' arguments
// can be used for we will imagine the situation where
// multiple windows or multiple threads may each one use
// libdar, but all share the same callback functions.

typedef class t_window_type t_win;

// this is an arbitrary type that here we will say
// points to a graphical window object wrapped in a C++
// class.
// Note that the method show() wait_for_click() and so on
// attributed to the t_win class are absolutely
// imaginary. Any link to an existing class is a pure
// coincidence...

void warning_callback(const std::string & //x
			, void * //context
			)
{
}

bool answer_callback(const std::string & //x
		, void * //context
		)
{
	return true;
}

std::string string_callback(const std::string & //x
		, bool //echo
		, void * //context
		)
{

	return std::string("callbackString");
}

int fcukingfunc()
{
	return 4;
}
// So now each window can have its user_interaction object based on the same
// user_interaction_callback object pointing to the same functions.
// user_interaction_callback objects can be shared among different window objects


int testArchive(std::string & archiveDirPath, std::string & archiveName)
{
	libdar::U_I maj, med, min;
	libdar::U_16 exception;
	std::string msg;

	// first we MUST call get_version()

	libdar::get_version_noexcept(maj, med, min, exception, msg);

	if (exception != LIBDAR_NOEXCEPT)
	{
		std::cout << msg << endl;
		return -1;
	}

	if (maj != LIBDAR_COMPILE_TIME_MAJOR || med
			< libdar::LIBDAR_COMPILE_TIME_MEDIUM)
	{
		std::cout << "we are linking against wrong libdar" << std::endl;
		return -1;
	}

	libdar::user_interaction_callback dialog =
			libdar::user_interaction_callback(&warning_callback,
					&answer_callback, &string_callback, (void *) fcukingfunc());


	try
	{
		libdar::archive *myArch = new archive(dialog, archiveDirPath, // where is the archive
				archiveName, // slice name
				"dar", // dar's archive extensions
				libdar::crypto_blowfish, "z4,r5s!", //password
				10240, // theses three previous are for encryptions
				"", // not used as we didn't gave "-" as
				"", // slice name
				"", // no command executed for now
				false); // and in case of error the
		// cause of the error
		//
	} catch (...)
	{
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	string archiveDirPath(argv[1]);
	string archiveName(argv[2]);

	return testArchive(archiveDirPath, archiveName);
	//return 0;
}
