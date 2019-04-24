#include "FileSystemTree.h"
#include <gtest/gtest.h>
//structures
struct CFileSystemTree::SImplementation{
    std::string obj = "/";

};

struct CFileSystemTree::CEntry::SImplementation{
    //std::weak_ptr parentPoint= nullptr;
    //std::vector<int> v;

};

struct CFileSystemTree::CEntryIterator::SImplementation{
    // You implementation here
};

struct CFileSystemTree::CConstEntryIterator::SImplementation{
    // You implementation here
};



//constructors
CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntry::~CEntry(){
    // You code here
}


//this is the pointer to the current instance
CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
    if (this != &entry) {
      //assign things here
    }
    return *this;
}




bool CFileSystemTree::CEntry::Valid() const{
    // You code here
    return true;
}

std::string CFileSystemTree::CEntry::Name() const{
    // You code here
    return "";
}

std::string CFileSystemTree::CEntry::FullPath() const{
    // You code here
    return "";
}

//this method and the below method are going to do the same thing
//the below operator just calls this method
std::string CFileSystemTree::CEntry::ToString() const{
  return "";
}






//not overloading the operator -> instead you are overloading to string
CFileSystemTree::CEntry::operator std::string() const{
    // You code here
    return this->ToString();
}

bool CFileSystemTree::CEntry::Rename(const std::string &name){
    // You code here
    return true;
}

size_t CFileSystemTree::CEntry::ChildCount() const{
    // You code here
    size_t foo = 1;
    return foo;
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){
    // You code here
    return true;
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall){
    // You code here
    return true;
}

bool CFileSystemTree::CEntry::RemoveChild(const std::string &path){
    // You code here
    return true;
}

bool CFileSystemTree::CEntry::SetData(const std::vector< char > &data){
    // You code here
    return true;
}

bool CFileSystemTree::CEntry::GetData(std::vector< char > &data) const{
    // You code here
    return true;
}


CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent(){
    return *this;

}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
    // You code here
    std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
    return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntryIterator::~CEntryIterator(){
    // You code here
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator  &iter){
    return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
    return true;
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
    return true;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int){
    return *this;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int){
    return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
  std::unique_ptr<CFileSystemTree::CEntry> iterPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *iterPtr;
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
  CFileSystemTree::CEntry *foo = new CFileSystemTree::CEntry();
  return foo;
}

//constructors for CConstEntryIterator
CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()){

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator(){
    // You code here
}



CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter){
    return *this;
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
    return true;
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
    return true;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
    return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
    return *this;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
    return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
    return *this;
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
  std::unique_ptr<CFileSystemTree::CEntry> iterPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *iterPtr;
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
  CFileSystemTree::CEntry *foo = new CFileSystemTree::CEntry();
  return foo;
}

//constructors for CFileSystemTree
CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::~CFileSystemTree(){
    // You code here
}


CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
    return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::Root(){
  std::unique_ptr<CFileSystemTree::CEntry> iterPtr = std::make_unique<CFileSystemTree::CEntry>();
  
  return *iterPtr;
}

const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
  std::unique_ptr<CFileSystemTree::CEntry> iterPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *iterPtr;
}

std::string CFileSystemTree::ToString() const{
    return "";
}

CFileSystemTree::operator std::string() const{
    return this->ToString();
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path){
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}
