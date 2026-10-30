#include <iostream>
#include <string>
#include <system_error>
using namespace std;
#include <filesystem>
namespace fs = filesystem;

int directoryCheckValidity(const string& directory) {
    if (!fs::exists(directory)) {
        cout << "The directory either does not exist or cannot be found.\n";
        cout << "HINT: Remove any ending or trailing quotes from the path.\n";
        return 1;
    }
    if (!fs::is_directory(directory)) {
        cout << directory << " Is not a directory/folder.\n";
        return 1;
    }
    cout << "Directory set\n";
    return 0;
};

int main() {
    string inDirectory, outDirectory, extension;

    cout << "Information: This renames file and moves them on failure for speed and cross disk transfer. This works non-recursively for security.\n";
    cout << "Subdirectories will NOT be tranferred.\n";
    cout << "To transfer files in all subdirectories, run it in each directory one-by-one or edit the code.\n";
    cout << "Remove any strings in path.\n";

    cout << "Enter directory you want to move items from: ";
    getline(cin, inDirectory);
    if (directoryCheckValidity(inDirectory) == 1) {return 1;}

    cout << "\nEnter directory you want to move items to: ";
    getline(cin, outDirectory);
    if (directoryCheckValidity(outDirectory) == 1) {return 1;}

    cout << "Please enter the extension: ";
    cin >> extension;
    //Adds a trailing dot in case the user forgets
    if (extension[0] != '.') {
        extension = "." + extension;
    }

    for(auto& entry: fs::directory_iterator(inDirectory)) {
        auto file = entry.path();
        error_code error;
        if (!file.has_extension()||!fs::is_regular_file(file)) {continue;}
        const auto fileName = file.filename().string();
        const auto fileLength = fileName.length();
        const auto extensionLength = extension.length();

        // Catches if the file extention is exact, in case of multiple extensions.
        if (fileLength < extensionLength || fileName.substr(fileLength - extensionLength) != extension) {continue;}

        //Makes sure there is enough space to safely transfer the file.
        if (fs::file_size(file) > fs::space(outDirectory).available) {error = make_error_code(errc::no_space_on_device);}
        else {
            fs::rename(file, outDirectory / file.filename(), error);
            if (error) {
                fs::copy(file, outDirectory, error);
                if (!error) {fs::remove(file, error);}
            }
        }
        if (error) {cout << "Failed to move: " << file.filename() << ": " << error.message() << "\n";}
        else {cout << "Moved: " << fileName << " from: " << inDirectory << " to: " << outDirectory << "\n";}
    }
    cout << "Script terminated\n";
    return 0;
}
