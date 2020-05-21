
// ROOT specifc includes
#include "TFile.h"
#include "TTree.h"

// podio specific includes
#include "podio/CollectionBase.h"
#include "podio/EventStore.h"

// override with new prototype
#include "ROOTWriter.h"

namespace podio {

  ROOTWriter::ROOTWriter(const std::string& filename, EventStore* store) :
    m_filename(filename),
    m_store(store),
    m_file(new TFile(filename.c_str(),"RECREATE","data file")),
    m_datatree(new TTree("events","Events tree")),
    m_metadatatree(new TTree("metadata", "Metadata tree"))
  {}

  ROOTWriter::~ROOTWriter(){
    delete m_file;
  }

  void ROOTWriter::writeEvent(){
    for (auto& coll : m_storedCollections){
      coll->prepareForWrite();
    }
    m_datatree->Fill();
  }

  void ROOTWriter::finish(){
    // now we want to save the metadata
    m_metadatatree->Branch("CollectionIDs", m_store->getCollectionIDTable());
    m_metadatatree->Fill();
    m_file->Write();
    m_file->Close();
  }

 void ROOTWriter::registerForWrite(const std::string& name){
    const podio::CollectionBase* tmp_coll(nullptr);
    m_store->get(name, tmp_coll);

    podio::CollectionBase* coll = const_cast<CollectionBase*>(tmp_coll);
    std::string className( coll->getValueTypeName() ) ;
    std::string collClassName = "vector<"+className+"Data>";

    if(coll==nullptr) {
      std::cerr<<"no such collection to write, throw exception."<<std::endl;
    }
    else {
      m_datatree->Branch(name.c_str(),  collClassName.c_str(), coll->getBufferAddress());
      auto colls = coll->referenceCollections();
      if (colls != nullptr){
      int i = 0;
      for(auto& c : (*colls)){
        m_datatree->Branch((name+"#"+std::to_string(i)).c_str(),c);
        ++i;
      }
      }
// ---- vector members
      auto vminfo = coll->vectorMembers();
      if (vminfo != nullptr){
      	int i = 0;
      	for(auto& c : (*vminfo)){
      	  std::string typeName = "vector<"+c.first+">" ;
      	  void* add = c.second ;
      	  m_datatree->Branch((name+"_"+std::to_string(i)).c_str(),
      			     typeName.c_str(),
      			     add);
      	  ++i;
      	}
      }
      m_storedCollections.emplace_back(coll);
    }
 }

} // namespace
