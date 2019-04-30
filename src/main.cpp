#include <iostream>
#include <fstream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"

void DFS(const std::string &path, CFileSystemTree::CEntry &entry){
    std::vector< std::tuple< std::string, bool> > Entries;
    std::string PathWithSlash = path;
    if(PathWithSlash.back() != '/'){
        PathWithSlash += "/";
    }
    DirectoryListing::GetListing(path, Entries);
    for(auto &Entry : Entries){
        if(std::get<1>(Entry)){
            std::string DirName = std::get<0>(Entry);
            if((DirName != ".")and(DirName != "..")){
                entry.AddChild(DirName);
                DFS(PathWithSlash + DirName, *entry.Find(DirName));
            }
        }
        else{
            entry.AddChild(std::get<0>(Entry));
        }
    }
}

//void DFS(const std::string &path, CFileSystemTree::CEntry &entry)
//{
//    std::vector< std::tuple< std::string, bool> > Entries;
//    std::string PathWithSlash = path;
//    if(PathWithSlash.back() != '/'){
//        PathWithSlash += "/";
//    }
//    DirectoryListing::GetListing(path, Entries);
//    for(auto &Entry : Entries){
//        std::ifstream fileInput;
//        std::string fileContents;
//        char c;
//
//        if(std::get<1>(Entry)){
//            std::string DirName = std::get<0>(Entry);
//            if((DirName != ".")and(DirName != "..")){
//                entry.AddChild(DirName);
//
//                fileInput.open(PathWithSlash);
//
//                while(fileInput >> c){
//                    fileContents.push_back(c);
//                }
//
//                entry.SetData(fileContents);
//                DFS(PathWithSlash + DirName, *entry.Find(DirName));
//            }
//        }
//        else{
//            entry.AddChild(std::get<0>(Entry));
//        }
//    }
//}

int main(int argc, char *argv[])
{
    std::cout << "???" << std::endl;
    bool loop = true;
    CFileSystemTree treeStruct;
    CFileSystemTree::CEntry dirNode = treeStruct.Root();
    std::vector<std::string> dirPath;
    std::string objName, userInput, rootNodeName;

    //DFS(".", dirNode);
    dirPath.push_back(treeStruct.Root().Name());

    while(loop){
        std::cout << "> ";
        std::getline(std::cin, userInput);
        std::vector<std::string> inputVector;
        CFileSystemTree::CEntryIterator i = treeStruct.Root().begin();

        inputVector = StringUtils::Split(userInput);

        if(userInput == "exit"){
            loop = false;
        }

        else if(userInput == "pwd"){
            std::string dirCurrent = StringUtils::Join("/",dirPath);
            std::cout << dirCurrent << std::endl;
        }

        else if(userInput.substr(0,2) == "cd"){
            if(userInput.size() < 2){
                std::vector<std::string> newPath = StringUtils::Split(userInput.substr(3));
                for(int n = 0; n < newPath.size() - 1; n++){
                    if(newPath[n] == i -> Name()){
                        i = i -> begin();
                    }
                }
            }
            else{
                dirNode = treeStruct.Root();
            }

        }

        else if(userInput.substr(0,2) == "ls"){
            if(userInput.size() < 2){
                CFileSystemTree::CEntry dirCurrent = dirNode;
                std::vector<std::string> newPath = StringUtils::Split(userInput.substr(3));
                for(int n = 0; n < newPath.size() - 1; n++){
                    if(newPath[n] == i -> Name()){
                        i = i -> begin();
                    }
                }
                while (i -> ChildCount()){
                    std::cout << i -> Name() << std::endl;
                }
                i = dirNode.begin();
            }
            else{
                while (i -> ChildCount()){
                    std::cout << i -> Name() << std::endl;
                }
            }
        }

        // else if(userInput.substr(0,3) == "cat"){
        //   std::vector<char> v;
        //   while(i -> Name() != userInput.substr(4)) i++;
        //   for(auto a : i -> GetData(v)){
        //         std::cout << a;
        //     }
        //
        //     std::cout << std::endl;
        // }

        else if(userInput.substr(0,5) == "mkdir"){
            if(userInput.size() < 5){
                dirNode.AddChild(userInput.substr(6), true);
            }
            else{
                std::cout << "Error mkdir: missing parameter" << std::endl;
            }
        }

        else if(userInput.substr(0,2) == "rm"){
            if(userInput.size() < 2){
                dirNode.RemoveChild(userInput.substr(3));
            }
            else{
                std::cout << "Error rm: missing parameter" << std::endl;
            }
        }

        else if (userInput.substr(0,2) == "mv"){
            std::string stringFileName, stringDest;
            if(userInput.size() < 2){
                std::vector<std::string> v = StringUtils::Split(userInput.substr(3));
                stringDest = v.back();
                stringFileName = v[v.size() - 2];

                stringDest += stringFileName;
                dirNode.AddChild(stringDest, true);
                dirNode.RemoveChild(stringFileName);
            }
            else{
                std::cout << "Error mv: missing parameter" << std::endl;
            }

        }

        else if (userInput.substr(0,2) == "cp"){
            std::string stringFileName, stringDest;
            if(userInput.size() < 2){
                std::vector<std::string> v = StringUtils::Split(userInput.substr(3));
                stringDest = v.back();
                stringFileName = v[v.size() - 2];
                stringDest += stringFileName;
                dirNode.AddChild(stringDest, true);
            }
            else{
                std::cout << "Error cp: missing parameter" << std::endl;
            }
        }

        else if(userInput.substr(0,4) == "tree"){
            if(userInput.size() < 4){
                std::vector<std::string> newPath = StringUtils::Split(userInput.substr(3));
                for(int n = 0; n < newPath.size() - 1; n++){
                    if(newPath[n] == i -> Name()){
                        i = i -> begin();
                    }
                }
                std::string(treeStruct);
            }
            else{
                std::string(treeStruct);
            }
        }

        else{
            std::cout << "Unknown command: " << userInput[0] << std::endl;
        }
    }
}
