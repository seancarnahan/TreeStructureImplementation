#include "FileSystemTree.h"
#include "Path.h"
#include "StringUtils.h"
#include <iostream>
#include <memory>

/**
 * Tree should have a root CEntry
 */
struct CFileSystemTree::SImplementation{
  //Root of the filesystem
  CEntry root;

  //Parent of the root - used for completeness
  //dummy version of CEntry just for parent of root
  std::shared_ptr<CEntry> rootParent;

};

/**
 * CEntry should have a path
 */
struct CFileSystemTree::CEntry::SImplementation{
  //std::weak_ptr<CEntry> sharedSelf;
  std::unique_ptr<CPath> path;
  std::weak_ptr<CEntry> parent;
  std::vector< std::shared_ptr<CEntry> > children;
  bool isValid = true;
};

struct CFileSystemTree::CEntryIterator::SImplementation{
    // You implementation here
};

struct CFileSystemTree::CConstEntryIterator::SImplementation{
    // You implementation here
};

/**
 * Default constructor for CEntry
 */
CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
  DImplementation->path = std::make_unique<CPath>();
}

/**
 * Param constructor for CEntry. All copy logic should be in the operator overload but not sure
 * how to do that. So will dup for now
 */
CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
  //DImplementation->path = entry.DImplementation->path;
}

CFileSystemTree::CEntry::~CEntry(){
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
    // You code here
  if (this != &entry) {
    //assign things here
    //DImplementation->path = entry.DImplementation->path;
  }
  return *this;
}


bool CFileSystemTree::CEntry::Valid() const{
  return DImplementation->isValid;
}

//return the base name
std::string CFileSystemTree::CEntry::Name() const{
  return DImplementation->path->ToString();

}

/**
 * Return the full path associated with this entry as a string
 */
std::string CFileSystemTree::CEntry::FullPath() const{
  //TODO
  return DImplementation->path->ToString();
}

/**
  this should be the printible version of this child with padding
 */

 std::string CFileSystemTree::CEntry::ToString(const std::string padding) const{
   std::string buffer = DImplementation->path->ToString() + "\n";
   int childCounter = DImplementation->children.size();
   if (childCounter > 0) {
     for (auto child : DImplementation->children) {
       childCounter--;
       if (padding.length() > 0) {
 	       buffer += padding;
       }
       if (childCounter > 0) {
 	       buffer += "|--";
 	       buffer += child->ToString(padding + "|  ");
       } else {
 	       buffer += "`--";
 	       buffer += child->ToString(padding + "   ");
       }
     }
   }
   return buffer;
}


std::string CFileSystemTree::CEntry::ToString() const{
  return StringUtils::RStrip(this->ToString(""));
}

/**
 * This should be ToString()
 */
CFileSystemTree::CEntry::operator std::string() const{
  return this->ToString();
}

bool CFileSystemTree::CEntry::Rename(const std::string &name){
    // You code here
  return true;
}

/**
 * Return the number of children (one level down)
 */
size_t CFileSystemTree::CEntry::ChildCount() const{
    // You code here
  return DImplementation->children.size();
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){

  return true;
}


std::vector<std::string> splitPath (const std::string path) {
  std::vector<std::string> subpaths;
  std::string buffer = "";
  for (int i = 0; i < path.length(); i++) {
    if ('/' == path[i]) {
      if (buffer.length() > 0) {
	subpaths.push_back(buffer);
	buffer = "";
      }
    } else {
      buffer += path[i];
    }
  }
  if (buffer.length() > 0) {
    subpaths.push_back(buffer);
  }
  return subpaths;
}

//aaaa to bbbb -1
//aaaa to aaaa 0
//bbbb to aaaa 1
bool centryCompare(std::shared_ptr<CFileSystemTree::CEntry> centry1, std::shared_ptr<CFileSystemTree::CEntry> centry2) {
  //sort the vector
  if (centry1->Name().compare(centry2->Name()) <0 ) {
    return true;
  } else {
    return false;
  }
}



/**
 * Add child or path to child using recursion
 */
bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall){

  bool addedEntry = false;
  //std::cout << "  *************** Adding path [" << path << "] to entry [" << this->FullPath() << "] with [" << DImplementation->children.size() << "] children\n";
  std::vector<std::string> subpaths = splitPath(path);
  if (subpaths.size() > 0) {

    //look through all the children and
    //check if the first node already exists
    std::shared_ptr<CEntry> childEntry = NULL;
    for (auto child : DImplementation->children) {
      if (child->DImplementation->path->ToString() == subpaths[0]) {
	       childEntry = child;
      }
    }

    //if it does not yet exist create it
    if (childEntry == NULL) {
      //std::cout << "  *************** Creating new child for path [" << subpaths[0] << "] for node [" << this->FullPath() << "]\n";

      //create childEntry for foo
      childEntry = std::make_shared<CEntry>();
      childEntry->DImplementation->path   = std::make_unique<CPath>(CPath(subpaths[0]));

      //Still need to figure out how to set the parent here
      //Need to set the parent of the child I just created to myself
      //childEntry->DImplementation->parent = DImplementation->sharedSelf;

      //then simply add it to children
      DImplementation->children.push_back(childEntry);
      addedEntry = true;
    } else {
      //std::cout << "  *************** Using existing child for path [" << subpaths[0] << "] for node [" << this->FullPath() << "]\n";
    }

    //create new string with remaining subpaths and recursively add those children
    //if after adding to path and we still have more in the vector
    if (1 < subpaths.size()) {
      //create a new string with remaining
      std::string buffer = "";
      for (int j = 1; j < subpaths.size(); j++) {
	       buffer += "/"+subpaths[j];
      }

      //call addChild on my childEntry (new or existing) with the new path
      bool addedChildEntry = childEntry->AddChild(buffer, true);
      if (addedChildEntry) {
	       addedEntry = true;
      }
    }

  }
  if (addedEntry) {
    std::sort(DImplementation->children.begin(), DImplementation->children.end(),centryCompare);
  }
  return addedEntry;

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

/**
 * Return the parent of this CEntry by locking the weak_ptr
 */
CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent(){
  if (auto parent = DImplementation->parent.lock()) {
    return *parent;
  } else {
    std::cout << "  *************** COULD NOT GET LOCK ON PARENT\n";
    return *this;
  }
}

/**
 * Return the parent of this CEntry
 */
const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
  //return DImplementation->parent;
  return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
    // You code here
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
    // You code here
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
    // You code here
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
    // You code here
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
    // You code here
  return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
    // You code here
  return true;
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
    // You code here
  return true;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
    // You code here
  return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int){
    // You code here
  return *this;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
    // You code here
  return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int){
    // You code here
  return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CEntry> entryPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *entryPtr;
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
    // You code here
  CFileSystemTree::CEntry *newEntry = new CFileSystemTree::CEntry();
  return newEntry;
}

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
    // You code here
  return *this;
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
    // You code here
  return true;
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
    // You code here
  return true;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
    // You code here
  return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
    // You code here
  return *this;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
    // You code here
  return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
    // You code here
  return *this;
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CEntry> entryPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *entryPtr;

}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
    // You code here
  CFileSystemTree::CEntry *newEntry = new CFileSystemTree::CEntry();
  return newEntry;

}

/**
 * Default Constructor. Need to create a dummy parent for the root CEntry
 */
CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){

  DImplementation->rootParent = std::make_shared<CEntry>();
  DImplementation->root.DImplementation->parent = DImplementation->rootParent;
  if (std::shared_ptr<CEntry> lock = DImplementation->root.DImplementation->parent.lock()) {
    lock->DImplementation->isValid = false;
  } else {
    std::cout << "  *************** ERROR: UNABLE TO SET ISVALID TO FALSE\n";
  }

}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::~CFileSystemTree(){
    // You code here
}

CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
    // You code here
  return *this;
}

/**
 * Return the root CEntry of this tree
 */
CFileSystemTree::CEntry &CFileSystemTree::Root(){
  return DImplementation->root;
}

/**
 * Return the root CEntry of this tree
 */
const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
  return DImplementation->root;
}

std::string CFileSystemTree::ToString() const{
  return DImplementation->root.ToString();

}

CFileSystemTree::operator std::string() const{
    // You code here
  return this->ToString();
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path){
    // You code here
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;

}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;

}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
    // You code here
  std::unique_ptr<CFileSystemTree::CEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CEntryIterator>();
  return *iterPtr;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CConstEntryIterator> iterPtr = std::make_unique<CFileSystemTree::CConstEntryIterator>();
  return *iterPtr;
}
