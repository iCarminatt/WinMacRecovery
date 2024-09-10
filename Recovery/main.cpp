#include <iostream>
#include <Windows.h>

int main() {
    DWORD drivesMask = GetLogicalDrives();
    for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter) {
        if (drivesMask & (1 << (driveLetter - 'A'))) {
            std::cout << "Drive " << driveLetter << " is available\n";
        }
    }
    //identify drives avaliable OK.
    //format and partition selected drive
    //a way to navegate through file explorer
    //import the image(iso)
    //write image to drive
    //performe cleanup at the end
    return 0;
}
/*
int driveType() 
{

}
*/