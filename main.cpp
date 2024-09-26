#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <Windows.h>

struct DriveInfo {
    char letter;
    UINT type;
    ULONGLONG totalBytes;
};

constexpr std::array<const char*, 7> DRIVE_TYPES = {
    "Unknown", "Invalid root path", "Removable drive", "Fixed drive",
    "Remote (network) drive", "CD-ROM drive", "RAM disk"
};

void formatSize(ULONGLONG bytes, char* buffer, size_t bufferSize) {
    constexpr std::array<const char*, 5> suffixes = {"B", "KB", "MB", "GB", "TB"};
    int suffixIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && suffixIndex < suffixes.size() - 1) {
        size /= 1024;
        ++suffixIndex;
    }

    snprintf(buffer, bufferSize, "%.2f %s", size, suffixes[suffixIndex]);
}

void printDriveInfo(const DriveInfo& drive) {
    char sizeBuffer[32];
    formatSize(drive.totalBytes, sizeBuffer, sizeof(sizeBuffer));
    std::cout << "Drive " << drive.letter << " (" 
              << DRIVE_TYPES[std::min(drive.type, static_cast<UINT>(DRIVE_TYPES.size() - 1))] 
              << "): Total size: " << sizeBuffer << '\n';
}

std::vector<DriveInfo> getAvailableDrives() {
    std::vector<DriveInfo> drives;
    DWORD drivesMask = GetLogicalDrives();
    char rootPath[] = "A:\\";

    for (int i = 0; i < 26; ++i) {
        if (drivesMask & (1 << i)) {
            rootPath[0] = 'A' + i;
            UINT driveType = GetDriveTypeA(rootPath);
            ULARGE_INTEGER totalBytes = {0};
            GetDiskFreeSpaceExA(rootPath, nullptr, &totalBytes, nullptr);
            drives.push_back({static_cast<char>('A' + i), driveType, totalBytes.QuadPart});
        }
    }
    return drives;
}

void printAllDrivesInfo(const std::vector<DriveInfo>& drives) {
    std::cout << "\n--- All Drives Information ---\n";
    for (const auto& drive : drives) {
        printDriveInfo(drive);
    }
    std::cout << "-----------------------------\n";
}

void performOperationsOnDrive(const DriveInfo& selectedDrive) {
    while (true) {
        std::cout << "\nSelected Drive: " << selectedDrive.letter << "\n"
                  << "Options:\n"
                  << "1. View drive information\n"
                  << "2. [Placeholder for future operation]\n"
                  << "3. [Placeholder for future operation]\n"
                  << "4. Return to main menu\n"
                  << "Enter your choice: ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "1") {
            printDriveInfo(selectedDrive);
        } else if (input == "2" || input == "3") {
            std::cout << "This operation is not yet implemented.\n";
        } else if (input == "4") {
            break;
        } else {
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}

int main() {
    auto drives = getAvailableDrives();

    auto start = std::chrono::high_resolution_clock::now();
    printAllDrivesInfo(drives);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Total execution time: " 
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    while (true) {
        std::cout << "\nAvailable drives: ";
        for (const auto& drive : drives) {
            std::cout << drive.letter << " ";
        }
        std::cout << "\nEnter a drive letter to select (or 'q' to quit): ";
        
        std::string input;
        std::getline(std::cin, input);

        if (input == "q" || input == "Q") {
            break;
        }

        auto it = std::find_if(drives.begin(), drives.end(), 
                               [&input](const DriveInfo& d) { return d.letter == toupper(input[0]); });
        if (input.length() == 1 && it != drives.end()) {
            performOperationsOnDrive(*it);
        } else {
            std::cout << "Invalid input. Please enter a valid drive letter.\n";
        }
    }

    std::cout << "\nFinal drive information:\n";
    start = std::chrono::high_resolution_clock::now();
    printAllDrivesInfo(drives);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Total execution time: " 
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    return 0;
}