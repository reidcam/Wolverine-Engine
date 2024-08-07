#include "Engine.h"
#include "FileUtils.h"

using namespace std;

/**
 * The main function where the working directory is set and then the engine is ran
 */
int main(int argc, char* argv[]) {
    FileUtils::SetWorkingDirectory();
    Game();
	return 0;
}
