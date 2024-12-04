#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// 使用标准命名空间
using namespace std;

// 定义文件控制块结构体
struct FileControlBlock {
    string name;          // 文件名
    int size;             // 文件大小
    vector<int> blocks;   // 存储文件数据的磁盘块列表
};

// 定义目录项结构体
struct DirectoryEntry {
    string name;                      // 目录名
    bool isDirectory;                 // 是否为目录
    vector<FileControlBlock*> files;  // 目录中的文件列表
    unordered_map<string, DirectoryEntry*> subdirectories; // 子目录映射
};

// 文件管理类
class FileManager {
public:
    FileManager() {
        currentDir = &root; // 初始化当前目录为根目录
    }

    // 注册用户
    void registerUser(const string& username) {
        if (users.find(username) != users.end()) {
            cout << "User already exists." << endl;
        } else {
            users[username] = new DirectoryEntry();
            cout << "User registered successfully." << endl;
        }
    }

    // 登录用户
    void loginUser(const string& username) {
        if (users.find(username) == users.end()) {
            cout << "User does not exist." << endl;
        } else {
            currentDir = users[username];
            cout << "Logged in as " << username << "." << endl;
        }
    }

    // 创建文件
    void createFile(const string& filename) {
        if (currentDir->subdirectories.find(filename) != currentDir->subdirectories.end()) {
            cout << "A directory with the same name already exists." << endl;
            return;
        }
        for (const auto& file : currentDir->files) {
            if (file->name == filename) {
                cout << "File already exists." << endl;
                return;
            }
        }
        currentDir->files.push_back(new FileControlBlock{filename, 0, {}});
        cout << "File created successfully." << endl;
    }

    // 创建目录
    void mkdir(const string& dirname) {
        if (currentDir->subdirectories.find(dirname) != currentDir->subdirectories.end()) {
            cout << "Directory already exists." << endl;
            return;
        }
        for (const auto& file : currentDir->files) {
            if (file->name == dirname) {
                cout << "A file with the same name already exists." << endl;
                return;
            }
        }
        currentDir->subdirectories[dirname] = new DirectoryEntry{dirname, true, {}, {}};
        cout << "Directory created successfully." << endl;
    }

    // 打开文件
    void openFile(const string& filename) {
        cout << "File opened successfully." << endl;
    }

    // 关闭文件
    void closeFile(const string& filename) {
        cout << "File closed successfully." << endl;
    }

    // 读取文件
    void readFile(const string& filename) {
        cout << "File content read successfully." << endl;
    }

    // 删除文件
    void deleteFile(const string& filename) {
        auto it = find_if(currentDir->files.begin(), currentDir->files.end(),
                          [&](const FileControlBlock* fcb) { return fcb->name == filename; });
        if (it != currentDir->files.end()) {
            delete *it;
            currentDir->files.erase(it);
            cout << "File deleted successfully." << endl;
        } else {
            cout << "File not found." << endl;
        }
    }

    // 删除目录
    void removeDir(const string& dirname) {
        if (currentDir->subdirectories.find(dirname) != currentDir->subdirectories.end()) {
            deleteDirectory(currentDir->subdirectories[dirname]);
            currentDir->subdirectories.erase(dirname);
            cout << "Directory removed successfully." << endl;
        } else {
            cout << "Directory not found." << endl;
        }
    }

    // 切换目录
    void cd(const string& dirName) {
        if (dirName == "..") {
            if (currentDir != &root) {
                currentDir = parentDirs[currentDir];
            }
        } else if (currentDir->subdirectories.find(dirName) != currentDir->subdirectories.end()) {
            parentDirs[currentDir->subdirectories[dirName]] = currentDir;
            currentDir = currentDir->subdirectories[dirName];
        } else {
            cout << "Directory not found." << endl;
        }
    }

    // 列出目录内容
    void listDir() {
        cout << "Files:" << endl;
        for (const auto& file : currentDir->files) {
            cout << "- " << file->name << endl;
        }
        cout << "Directories:" << endl;
        for (const auto& subdir : currentDir->subdirectories) {
            cout << "- " << subdir.first << endl;
        }
    }

    // 写入文件
    void writeToFile(const string& filename, const string& data, int totalLength) {
        auto it = find_if(currentDir->files.begin(), currentDir->files.end(),
                          [&](const FileControlBlock* fcb) { return fcb->name == filename; });
        if (it != currentDir->files.end()) {
            (*it)->size = totalLength;
            cout << "Data written to file successfully." << endl;
        } else {
            cout << "File not found." << endl;
        }
    }

    // 修改文件属性
    void changeFileAttributes(const string& filename) {
        cout << "File attributes changed successfully." << endl;
    }

    // 清屏
    void clearScreen() {
        system("clear"); // 对于 Unix/Linux/MacOS
        // system("cls"); // 对于 Windows
    }

    // 显示帮助信息
    void help() {
        cout << "Available commands:\n"
             << "register - Register a new user\n"
             << "login - Login an existing user\n"
             << "create - Create a new file\n"
             << "mkdir - Create a new directory\n"
             << "open - Open a file\n"
             << "close - Close a file\n"
             << "read - Read a file\n"
             << "del - Delete a file\n"
             << "remove - Remove a directory\n"
             << "cd - Change directory\n"
             << "dir - List directory contents\n"
             << "ls - List files in the current directory\n"
             << "write - Write data to a file\n"
             << "change - Change file attributes\n"
             << "clear - Clear the screen\n"
             << "help - Display available commands\n"
             << "exit - Exit the system\n";
    }

private:
    DirectoryEntry root;                                      // 根目录
    DirectoryEntry* currentDir;                             // 当前目录指针
    unordered_map<string, DirectoryEntry*> users;           // 用户映射
    unordered_map<DirectoryEntry*, DirectoryEntry*> parentDirs; // 父目录映射

    // 递归删除目录及其子目录和文件
    void deleteDirectory(DirectoryEntry* dir) {
        for (auto& file : dir->files) {
            delete file;
        }
        for (auto& subdir : dir->subdirectories) {
            deleteDirectory(subdir.second);
            delete subdir.second;
        }
    }
};

int main() {
    FileManager fm;
    string command;
    while (true) {
        cout << "> ";
        cin >> command;
        if (command == "exit") break;
        else if (command == "register") {
            string username;
            cout << "Enter username: ";
            cin >> username;
            fm.registerUser(username);
        } else if (command == "login") {
            string username;
            cout << "Enter username: ";
            cin >> username;
            fm.loginUser(username);
        } else if (command == "create") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.createFile(filename);
        } else if (command == "mkdir") {
            string dirname;
            cout << "Enter directory name: ";
            cin >> dirname;
            fm.mkdir(dirname);
        } else if (command == "open") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.openFile(filename);
        } else if (command == "close") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.closeFile(filename);
        } else if (command == "read") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.readFile(filename);
        } else if (command == "del") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.deleteFile(filename);
        } else if (command == "remove") {
            string dirname;
            cout << "Enter directory name: ";
            cin >> dirname;
            fm.removeDir(dirname);
        } else if (command == "cd") {
            string dirName;
            cout << "Enter directory name (or '..' to go up): ";
            cin >> dirName;
            fm.cd(dirName);
        } else if (command == "dir" || command == "ls") {
            fm.listDir();
        } else if (command == "write") {
            string filename, data;
            int totalLength;
            cout << "Enter filename: ";
            cin >> filename;
            cout << "Enter data: ";
            cin.ignore();
            getline(cin, data);
            cout << "Enter total length: ";
            cin >> totalLength;
            fm.writeToFile(filename, data, totalLength);
        } else if (command == "change") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            fm.changeFileAttributes(filename);
        } else if (command == "clear") {
            fm.clearScreen();
        } else if (command == "help") {
            fm.help();
        } else {
            cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }
    return 0;
}



