#include "Path.h"
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include "StringUtils.h"

struct CPath::SImplementation{
    std::string pathName;
};

CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
}

CPath::CPath(const CPath &path) : DImplementation(std::make_unique< SImplementation >()){
    DImplementation -> pathName = path.DImplementation -> pathName;
}

CPath::CPath(const std::string &path): DImplementation(std::make_unique< SImplementation >()) {
    DImplementation -> pathName = path;
}

CPath::~CPath(){
}

CPath &CPath::operator=(const CPath &path){
    if (this != &path){
      DImplementation -> pathName = path.DImplementation -> pathName;
    }

    return *this;
}

CPath CPath::operator+(const CPath &path) const{
    DImplementation -> pathName += path.DImplementation -> pathName;
    
    return DImplementation -> pathName;
}

bool CPath::operator==(const CPath &path) const{
  bool val = true;
  if (*this != path){
    val = false;
  }
  return val;
}

bool CPath::operator!=(const CPath &path) const{
    return !operator==(path);
}

CPath CPath::Directory() const{
    CPath newNode;
    std::vector <std::string> v = StringUtils::Split("/", DImplementation -> pathName);
    std::string s;
    v.pop_back();
    s = StringUtils::Join("/", v);
    
    return newNode.DImplementation -> pathName = s;
}

std::string CPath::BaseName() const{
    std::vector <std::string> v = StringUtils::Split("/", DImplementation -> pathName);
    
    return v[v.size() - 1];
}

std::string CPath::Extension() const{
    std::vector <std::string> v = StringUtils::Split("/", DImplementation -> pathName);
    std::string s = v[v.size() - 1];
    int i = s.find('.');
    
    return s.substr(i, s.size() - i);
}

bool CPath::IsAbsolute() const{
  bool val = false;
    if (DImplementation -> pathName[0] != '.'){
      val = true;
    }

    return val;
}

std::string CPath::ToString() const{
    CPath newNode;
    return newNode.DImplementation -> pathName;
}

CPath::operator std::string() const{
    CPath newNode;
    return newNode.DImplementation -> pathName;
}

CPath CPath::AbsolutePath() const{
    CPath newNode;
    std::string s = DImplementation -> pathName;
    s.insert(0, "HOME");
    
    return newNode.DImplementation -> pathName = s;
    
}

CPath CPath::CommonPath(const CPath &path) const{
    CPath newNode;
    std::vector <std::string> v1 = StringUtils::Split(DImplementation -> pathName, "/");
    std::vector <std::string> v2 = StringUtils::Split(path.DImplementation -> pathName, "/");
    std::string s;
    
    for (int i = 0; i != v1.size(); i++){
        if (v1[i] == v2[i]){
            s.append(v1[i]);
            s.append("/");
        }
    }
    
    return newNode.DImplementation -> pathName = s;
}

CPath CPath::NormalizePath() const{
    CPath newNode;
    std::string newString, s = DImplementation -> pathName;
    
    for (int i = 0; i < s.length(); i++){
        
        if (s[i] == '.'){
            if (s[i + 1] == '.' || s[i + 1] == '/'){
                s.erase(i);
            }
        }
    }
    
    return newNode.DImplementation -> pathName = newString;
}

CPath CPath::RelativePathTo(const CPath &path) const{
    CPath newNode;
    std::vector <std::string> v1 = StringUtils::Split(DImplementation -> pathName, "/");
    std::vector <std::string> v2 = StringUtils::Split(path.DImplementation -> pathName, "/");
    std::vector <std::string> v3;
    std::string s1, s2 = "../";
    int save;
    bool cond = true;
    
    while(cond){
        for (int i = 0; i > v1.size(); i++){
            if (v1[i] != v2[i]){
                save = i;
                cond = false;
                break;
            }
        }
    }
    
    for (int i = save; i >= 0; i--){
        v3[i] = v1[i];
    }
    
    s1 = StringUtils::Join("/", v3);
    s2 += s1;
    
    return newNode.DImplementation -> pathName = s2;
}

// Helper function provided to get the current working directory
std::string CPath::GetCWD(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome(){
    return std::getenv("HOME");
}

CPath CPath::AbsolutePath(const CPath &path){
    std::string s = path.DImplementation -> pathName;
    std::string newString = std::getenv("HOME");
    newString += s;
    
    return path.DImplementation -> pathName = newString;
}

CPath CPath::CurrentPath(){
    return GetCWD();
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
    CPath newNode;
    std::vector <std::string> v;
    std::string s;
    int save = 0;
    
    for(int i = 0; i < std::min(path1.DImplementation -> pathName.length(), path2.DImplementation -> pathName.length()); i++){
        if (path1.DImplementation -> pathName[i] != path2.DImplementation -> pathName[i]){
            save = i - 1;
            break;
        }
    }
    
    v = StringUtils::Split(path1.DImplementation -> pathName, "/");
    while(path1.DImplementation -> pathName.length() != save){
        v.pop_back();
    }
    
    s = StringUtils::Join("/", v);
    
    return newNode.DImplementation -> pathName = s;
}

CPath CPath::ExpandUserPath(const CPath &path){
    CPath newNode;
    std::string str1 = GetHome() + "/";;
    str1 += path.DImplementation -> pathName;
    return newNode.DImplementation -> pathName = str1;
}

CPath CPath::HomePath(){
    return GetHome();
}

CPath CPath::NormalizePath(const CPath &path){
    std::string newString, s = path.DImplementation -> pathName;
    
    for (int i = 0; i < s.length(); i++){
        
        if (s[i] == '.'){
            if (s[i + 1] == '.' || s[i + 1] == '/'){
                s.erase(i);
            }
        }
    }
    
    return path.DImplementation -> pathName = newString;
}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath){
    CPath newNode;
    std::string s;
    
    s = path.DImplementation -> pathName + startpath.DImplementation -> pathName;
    
    return newNode.DImplementation -> pathName = s;
}
