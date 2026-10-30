#include <iostream>
#include <string>
using namespace std;
#include <filesystem>
namespace fs = filesystem;
int main() {
    string directory, oldExt, newExt;
    cout << "Enter directory:\n";
    getline(cin, directory);
    if (fs::exists(directory) && fs::is_directory(directory)) {cout << "Operating at:" << "" << directory << "\n";}
    else {
        cout << "The directory either does not exist or cannot be found\n";
        return 1;
    }
    cout << "Enter old extension:\n";
    cin >> oldExt;
    if (oldExt[0] != '.') {oldExt = '.' + oldExt;}
    cout << "Enter new extension:\n";
    cin >> newExt;
    if (newExt[0] != '.') {newExt = '.' + newExt;}
    for(auto& entry: fs::recursive_directory_iterator(directory)) {
        auto file = entry.path();
        if (file.extension() != oldExt) {continue;}
        auto newFile = file;
        newFile = newFile.replace_extension(newExt);
        fs::rename(file, newFile);
        cout << file << "-->" << newFile << "\n";
    }
    cout << "Success\n";
    return 0;
}
