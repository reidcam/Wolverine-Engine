#include "Engine.h"
#include "FileUtils.h"

using namespace std;

int main(int argc, char* argv[]) {
    FileUtils::SetWorkingDirectory();
    Game();
	return 0;
}
