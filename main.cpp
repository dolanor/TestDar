#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <dar/libdar.hpp>
#include <libgen.h>


#include <stdio.h>
#include <sys/stat.h>


#include <ctime>

using namespace std;
using namespace libdar;

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

void truncateFile(const std::string &filePath, const long long newSize)
{
	off_t newSizeOffset;
        newSizeOffset = (off_t)newSize;

        truncate(filePath.c_str(), newSize);
}

intmax_t fileSize(const std::string &filePath)
{
	struct stat file_stat;

	stat(filePath.c_str(), &file_stat);
	return (intmax_t)file_stat.st_size;
}


int main(int argc, char* argv[])
{
	std::string archiveDirPath(argv[1]);
	std::string archiveName(argv[2]);
	std::string archiveExt(argv[3]);
	std::string archivePath(argv[1]);
	archivePath += "/" + archiveName + archiveExt;


	cout << "archiveDirPath : " << archiveDirPath << endl;
	cout << "archiveName : " << archiveName << endl;
	cout << "archivePath : " << archivePath << endl;

	intmax_t archiveBeginSize = fileSize(archivePath);
	int archiveOK = -1;
	cout << "taille archive : " << archiveBeginSize << " octets\n";

	//time_t currentTime = time(0);
	//time_t beginTime = currentTime;

	const int finishingBytes = 1449710000;

	int currentByte = archiveBeginSize;

	//int bytesToProcess = currentByte - finishingBytes;
	int i = 0;
	for(currentByte = archiveBeginSize; currentByte > finishingBytes; --currentByte)
	{

		archiveOK = testArchive(archiveDirPath, archiveName);


		if(archiveOK == 0)
		{
			if(i != 0)
				++currentByte;
			cout << "Archive truncated to the good size ! (" << currentByte << ")" << endl;
			return 0;
		}
		else
		{
			truncateFile(archivePath, currentByte);
			/*if(currentByte % 500 == 0)
			{
				currentTime = time(0);
				time_t elapsedTime = difftime(currentTime, beginTime);
				bytesToProcess = currentByte - finishingBytes;
				time_t timeToFinish = (time_t)((int)elapsedTime * bytesToProcess / i);
				tm *dt = gmtime(&timeToFinish);
				printf("Estimation restante : %02d:%02d:%02d\n", dt->tm_hour, dt->tm_min, dt->tm_sec);
			}*/
		}
		++i;
	}
	return -1; // sortie sans que l'archive soit OK
}
