#ifndef ROOTWRITER_H
#define ROOTWRITER_H

#include "podio/CollectionBase.h"
#include "EventStore2.h"

#include <string>
#include <vector>
#include <iostream>

// ROOT specific includes
#include "TTree.h"

// forward declarations
class TFile;

namespace podio {
  class Registry;

  class ROOTWriter {

  public:
    ROOTWriter(const std::string& filename, EventStore* store);
    ~ROOTWriter();

    void registerForWrite(const std::string& name);
    void writeEvent();
    void finish();
    inline TTree* getEventsTree() {return m_datatree;};

  private:
    // members
    std::string m_filename;
    EventStore* m_store;
    TFile* m_file;
    TTree* m_datatree;
    TTree* m_metadatatree;
    TTree* m_runMDtree;
    TTree* m_evtMDtree;
    TTree* m_colMDtree;
    GenericParameters* m_evtMD ;
    std::vector<CollectionBase*> m_storedCollections;

  };

} //namespace
#endif
