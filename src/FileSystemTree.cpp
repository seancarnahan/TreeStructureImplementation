#include "FileSystemTree.h"
#include "Path.h"
#include "StringUtils.h"
#include <iostream>
#include <memory>

/**
 * Struct definition for the Tree
 */
struct CFileSystemTree::SImplementation{

  //Root of the filesystem
  std::shared_ptr<CEntry> root;

  //Parent of the root - used for completeness
  std::shared_ptr<CEntry> rootParent;

  //Will create default iter backed by a zero-length vector
  //This is used in comparison operators;
  CEntryIterator NOT_FOUND_ITER;

};

/**
 * Struct definition for Entry
 */
struct CFileSystemTree::CEntry::SImplementation{

  //path to this entry
  std::unique_ptr<CPath> path;

  //ptr to this entry to use as a parent for its children
  std::weak_ptr<CEntry> sharedSelf;

  //ptr to the parent of this entry
  std::weak_ptr<CEntry> parent;

  //child entries
  std::vector< std::shared_ptr<CEntry> > children;

  //whether this is a valid entry
  bool isValid = true;

  //the data for this entry (directories cannot have data)
  std::vector< char > data;

};


/**
 * Struct definition for Entry Iterator
 */
struct CFileSystemTree::CEntryIterator::SImplementation{

  //the entries in this iterator
  std::vector< std::shared_ptr<CEntry> > children;

  //the current index of this iterator, set to -1 (invalid) by default)
  int currentChildIdx = -1;

};

/**
 * TODO: Not Implemented
 */
struct CFileSystemTree::CConstEntryIterator::SImplementation{
};

/**
 * Default constructor for CEntry
 */
CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
  DImplementation->path = std::make_unique<CPath>();
}

/**
 * Param constructor for CEntry used in = operator assignment with a pointer.
 */
CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
  if (entry.Valid()) {

    // assign valid to true
    DImplementation->isValid = true;

    //assign these values from the given entry
    DImplementation->sharedSelf = entry.DImplementation->sharedSelf;
    DImplementation->parent = entry.DImplementation->parent;
    DImplementation->children = entry.DImplementation->children;

    //need to make a unique ptr for the path of the given entry
    DImplementation->path = std::make_unique<CPath> (CPath(entry.Name()));

  } else {
    //set valid to false
    DImplementation->isValid = false;
  }
}

/**
 * TODO: Need to clean up vectors and pointers
 */
CFileSystemTree::CEntry::~CEntry(){
}

/**
 * Equal sign operator used when assigning variabled without a pointer. Should look similar
 * to param constructor
 */
CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
  if (this != &entry) {
    if (entry.Valid()) {

      // assign valid to true
      DImplementation->isValid = true;

      //assign these values from the given entry
      DImplementation->sharedSelf = entry.DImplementation->sharedSelf;
      DImplementation->parent = entry.DImplementation->parent;
      DImplementation->children = entry.DImplementation->children;

      //need to make a unique ptr for the path of the given entry
      DImplementation->path = std::make_unique<CPath> (CPath(entry.Name()));

    } else {
      //set valid to false
      DImplementation->isValid = false;
    }
  }
  return *this;
}


/**
 * Returns whether entry is valid. For example the parent of the root
 * entry is invalid
 */
bool CFileSystemTree::CEntry::Valid() const{
  return DImplementation->isValid;
}


/**
 * Added this method to make it easier to determine if entry is the root
 * TODO: We should identify the root entry by whether or not its parent is
 * valid but that has issues. For now we will use the path name
 */
bool CFileSystemTree::CEntry::isRoot() const{
  return (FullPath() == "/");
}

/**
 * Return the base name of the entry
 */
std::string CFileSystemTree::CEntry::Name() const{
  return DImplementation->path->ToString();
}

/**
 * Return the full path associated with this entry as a string
 */
std::string CFileSystemTree::CEntry::FullPath() const{
  if (Name() != "/") {



    //Recursively add the Name() of each parent as prefix to
    //current path using the / separator
    std::string buffer = "/" + Name();
    CEntry parent = this->Parent();
    while(parent.Name() != "/") {
      buffer = "/" + parent.Name() + buffer;
      parent = parent.Parent();
    }
    return buffer;

  } else {

    //return the name if this is the root entry
    return Name();
  }
}



/**
 * Added this method so that entries can be printed with recursion.
 * This should return a printable version of this entry with padding
 */
std::string CFileSystemTree::CEntry::ToString(const std::string padding) const{
  std::string buffer = DImplementation->path->ToString() + "\n";

  //use a counter so that we know to use | or `
  int childCounter = DImplementation->children.size();
  if (childCounter > 0) {
    for (auto child : DImplementation->children) {

      childCounter--;

      //add the padding to the buffer
      if (padding.length() > 0) {
	buffer += padding;
      }

      //recursively call this method on each child and add more padding
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


/**
 * This should be printable version of this child. Real work is done
 * in ToString(padding) method. Use RStrip to strip the last newline
 */
std::string CFileSystemTree::CEntry::ToString() const{
  return StringUtils::RStrip(this->ToString(""));
}

/**
 * This should be ToString()
 */
CFileSystemTree::CEntry::operator std::string() const{
  return this->ToString();
}

/**
 * Helper method to compare the names of two entries
 */
bool centryCompare (std::shared_ptr<CFileSystemTree::CEntry> centry1,
		    std::shared_ptr<CFileSystemTree::CEntry> centry2) {
  //compare the names
  if (centry1->Name().compare(centry2->Name()) < 0) {
    return true;
  }
  return false;
}

/**
 * Rename this entry
 * moves things around
 */
bool CFileSystemTree::CEntry::Rename(const std::string &name){

  //return false if any siblings have the same name
  for(auto sibling : Parent().DImplementation->children) {
    if (sibling->Name() == name) {
      return false;
    }
  }

  //reset the path by using a new unique ptr
  DImplementation->path = std::make_unique<CPath>(CPath(name));

  //re-sort siblings after changing the name
  std::sort(Parent().DImplementation->children.begin(),
  Parent().DImplementation->children.end(),
  centryCompare);

  return true;
}

/**
 * Return the number of children (one level down)
 */
size_t CFileSystemTree::CEntry::ChildCount() const{
  return DImplementation->children.size();
}

/**
 * This method moves a entry, with all of its properties,
 * to be child of a different parent and gives the entry a new name
 * add child adds a child, set child moves an existing child to another position
 */
bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){

  //return false if this entry is NOT a directory (use data to determine)
  if (DImplementation->data.size() > 0) {
    return false;
  }

  //return false if this entry already has a child by that name
  for(auto child: DImplementation->children) {
    if (child->Name() == name) {
      return false;
    }
  }

  //first get a lock on the entry we want to move using the sharedSelf
  //gain access to the entry through the iterator
  if (std::shared_ptr<CEntry> entry = iter->DImplementation->sharedSelf.lock()) {

    //use RemoveChild() to remove entry from the entry's current parent's children vector
    entry->Parent().RemoveChild(entry->Name());

    //rename entry (only after removing it from the current parent's children vector)
    entry->DImplementation->path = std::make_unique<CPath>(CPath(name));

    //add entry as a child of this entry
    DImplementation->children.push_back(entry);

    //re-sort the children of this entry
    std::sort(DImplementation->children.begin(), DImplementation->children.end(), centryCompare);
    return true;
  }
  return false;

}


/**
 * A helper method to split a path into parts
 */
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



/**
 * Add child or path to child using recursion
 */
bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall){

  //return false if this entry is NOT a directory (use data)
  if (DImplementation->data.size() > 0) {
    return false;
  }

  bool addedEntry = false;

  //split the path
  std::vector<std::string> subpaths = splitPath(path);

  if (subpaths.size() > 0) {

    //navigate up the tree if subpath is ".."
    //no need to add a child in this part
    if(subpaths[0] == ".."){

      //can't go up a level from rootParent
      if(!isRoot()) {

        //shorten the path by one
        std::string buffer = "";
        for (int j = 1;j < subpaths.size(); j++) {
          buffer += "/"+subpaths[j];
        }

        //return false if the ".." directive tries to go past rootParent
        if (!Parent().AddChild(buffer, true)) {
          return false;
        }

      } else {
        return false;
      }

    }

    //set the childEntry if the entry already exists
    std::shared_ptr<CEntry> childEntry = NULL;

    for (auto child: DImplementation->children) {
      if(child->DImplementation->path->ToString() == subpaths[0]) {
        childEntry = child;
      }
    }

    //if the childEntry did not exist create it and add it as a child
    if (childEntry == NULL) {

      childEntry = std::make_shared<CEntry>();
      childEntry->DImplementation->path = std::make_unique<CPath>(CPath(subpaths[0]));
      childEntry->DImplementation->sharedSelf = childEntry;
      childEntry->DImplementation->parent = DImplementation->sharedSelf;
      DImplementation->children.push_back(childEntry);
      addedEntry = true;

    } else {
      //the entry already exists so can use that
    }

    if (subpaths.size() > 1) {

      //create new path with remaining subpaths
      std::string buffer = "";
      for(int j = 1; j < subpaths.size(); j++)
      buffer += "/" + subpaths[j];

      //recursively add the children of those remaining subpaths
      bool addedChildEntry = childEntry->AddChild(buffer, true);
      if (addedChildEntry) {
        addedEntry = true;
      }
    }

  }

  //if we added and entry re-sort
  if (addedEntry) {
    std::sort(DImplementation->children.begin(), DImplementation->children.end(), centryCompare);
  }

  return addedEntry;
}

/**
 * Remove a child defined by this name from this entry. No need to use Find here.
 * iterator is a pointer
 */
bool CFileSystemTree::CEntry::RemoveChild(const std::string &path){

  //iterate over the children vector and remove the child with the matching name
  bool removed = false;
  for(auto iter = DImplementation->children.begin(); iter != DImplementation->children.end();) {
    //have to do a double dereference here because iter is ptr to std::shared_ptr<CEntry>
    if ((*iter) -> Name() == path){
      DImplementation->children.erase(iter);
      removed = true;
    } else {
      ++iter;
    }
  }
  return removed;

}


/**
 * Set data by copying elements of the given vector
 * parameter is const here but not in get data
 */
bool CFileSystemTree::CEntry::SetData(const std::vector< char > &data){
  //cannot set the data of a directory, only an entry with no children can have data
  if (DImplementation->children.size() == 0) {
    DImplementation->data.clear();
    for (auto c: data) {
      DImplementation->data.push_back(c);
    }
    return true;
  }
  return false;
}

/**
 * Get data by copying elements to the given vector
 * take paramete and change it for me, filling an empty vector
 */
bool CFileSystemTree::CEntry::GetData(std::vector< char > &data) const{

  //cannot set the data of a directory, only an entry with no children can have data
  if (DImplementation->children.size() == 0) {
    data.clear();
    for(auto c : DImplementation->data) {
      data.push_back(c);
    }
    return true;
  }
  return false;
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
 * Return the parent of this CEntry by locking the weak_ptr
 */
const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
  if (auto parent = DImplementation->parent.lock()) {
    return *parent;
  } else {
    std::cout << "  *************** COULD NOT GET LOCK ON PARENT\n";
    return *this;
  }
}

/**
 * Find the entry using recursion on the children of this entry. Given the parameter is name
 * this may not be what was intended for this method. If the entry is not found then the
 * iter will be backed by a zero length vector.
 */
CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
  std::vector<std::string> subpaths = splitPath(name);

  CFileSystemTree::CEntryIterator iter;
  //iterate over children and recursive call find on children of a matching entry
  for(auto child : DImplementation->children) {
    if (child->Name() == subpaths[0]) {
      if (subpaths.size() > 1) {

        //create the path with this child removed and call Find on this child
        std::string buffer = "";
        for(int j = 1; j < subpaths.size(); j++) {
          buffer += "/"+subpaths[j];
        }
        iter = child->Find(buffer);
      } else {
        //we have a full match so can populated the iterator with this child
        iter.DImplementation->children.push_back(child);
        iter.DImplementation->currentChildIdx = 0;
        return iter;
      }
    }
  }
  return iter;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{

  return CConstEntryIterator();
}

/**
 * Start the iterator and put the first index at 0
 */
CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
  CFileSystemTree::CEntryIterator iter;
  iter.DImplementation->children = DImplementation->children;
  iter.DImplementation->currentChildIdx = 0;
  return iter;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
  return CConstEntryIterator();
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
  return CConstEntryIterator();
}

/**
 * Start the iterator and put the first index at the end of the iterator
 */
CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
  CFileSystemTree::CEntryIterator iter;
  iter.DImplementation->children = DImplementation->children;
  iter.DImplementation->currentChildIdx = (iter.DImplementation->children.size() - 1);
  return iter;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
  return CConstEntryIterator();
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
  return CConstEntryIterator();
}

/**
 * Default Entry Iterator constructor
 */
CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >()){
}

/**
 * Assignment cosntructor for Entry Iterator - assignment using a pointer
 */
CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

/**
 Entry Iterator destructor
 */
CFileSystemTree::CEntryIterator::~CEntryIterator(){
}

/**
 * Set the iterator properties from the given iterator
 */
CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator  &iter){
  DImplementation->children = iter.DImplementation->children;
  DImplementation->currentChildIdx = iter.DImplementation->currentChildIdx;
  return *this;
}

/**
 * Compare two iterators
 */
bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
  if (DImplementation->children.size() == iter.DImplementation->children.size()) {
    //TODO: need to compare the values inside the vector
    return true;
  }
  return false;
}

/**
 * Compare two iterators
 */
bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
  if (DImplementation->children.size() != iter.DImplementation->children.size()) {
    //TODO: need to compare the values inside the vector
    return true;
  }
  return false;
}

/**
 * Increment the iterator. Not used in the tests.
 */
CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
  DImplementation->currentChildIdx++;
  return *this;
}

/**
 * Increment the iterator
 */
CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int value){
  if (value == 0) {
    value = 1;
  }
  DImplementation->currentChildIdx += value;
  return *this;
}

/**
 * Decrement the iterator. Not used in the tests.
 */
CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
  DImplementation->currentChildIdx--;

  return *this;
}

/**
 * Decrement the iterator.
 */
CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int value){
  if (value == 0) {
    value = 1;
  }
  DImplementation->currentChildIdx -= value;
  return *this;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
    // You code here
  std::unique_ptr<CFileSystemTree::CEntry> entryPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *entryPtr;
}

/**
 * Return the CEntry at the current index
 */
CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
  return DImplementation->children.at(DImplementation->currentChildIdx).get();
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()){

}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator::~CConstEntryIterator(){
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter){
 return *this;
}

/**
 * TODO: Not Implemented
 */
bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
  return true;
}

/**
 * TODO: Not Implemented
 */
bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
  return true;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
  return *this;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
  return *this;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
  return *this;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
  return *this;
}

/**
 * TODO: Not Implemented
 */
const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
  std::unique_ptr<CFileSystemTree::CEntry> entryPtr = std::make_unique<CFileSystemTree::CEntry>();
  return *entryPtr;
}

/**
 * TODO: Not Implemented
 */
const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
  CFileSystemTree::CEntry *newEntry = new CFileSystemTree::CEntry();
  return newEntry;
}

/**
 * Default Constructor for the Tree. Need to create a dummy parent for the root CEntry
 */
CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){

  //make shared pointers for both the root and rootParent;
  DImplementation->root       = std::make_shared<CEntry>();
  DImplementation->rootParent = std::make_shared<CEntry>();

  //assign them to the sharedSelf and parent of the root member variable
  //will convert to weak pointers
  DImplementation->root->DImplementation->sharedSelf = DImplementation->root;
  DImplementation->root->DImplementation->parent     = DImplementation->rootParent;

  //set the root parent to be invalid by locking the root first
  if (std::shared_ptr<CEntry> lock = DImplementation->root->DImplementation->parent.lock()) {
    lock->DImplementation->isValid = false;
  } else {
    std::cout << "  *************** ERROR: UNABLE TO SET ISVALID TO FALSE\n";
  }

}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation >()){
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::~CFileSystemTree(){
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
  return *this;
}

/**
 * Return the root CEntry of this tree
 */
CFileSystemTree::CEntry &CFileSystemTree::Root(){
  return *DImplementation->root;
}

/**
 * Return the root CEntry of this tree
 */
const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
  return *DImplementation->root;
}

/**
 * Return the toString of the root
 */
std::string CFileSystemTree::ToString() const{
  return DImplementation->root->ToString();
}

/**
 * Return the toString of the root (call ToString())
 */
CFileSystemTree::operator std::string() const{
  return this->ToString();
}

/**
 * Find the entry defined by the given path
 */
CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path){
  //Call Find on the root entry
  return Root().Find(path);
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const{
  return CConstEntryIterator();
}

/**
 * Return the a empty version of the iterator for comparison purposes
 */
CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
  //return the NOT_FOUND_ITER
  return DImplementation->NOT_FOUND_ITER;
}

/**
 * TODO: Not Implemented
 */
CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
  return CConstEntryIterator();
}
