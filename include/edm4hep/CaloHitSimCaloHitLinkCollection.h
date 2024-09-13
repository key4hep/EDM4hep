#ifndef EDM4HEP_CALOHITSIMCALOHITLINKCOLLECTION_H
#define EDM4HEP_CALOHITSIMCALOHITLINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/CalorimeterHitCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>

namespace edm4hep {
using CaloHitSimCaloHitLinkCollection = podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::SimCalorimeterHit>;
} // namespace edm4hep

#endif // EDM4HEP_CALOHITSIMCALOHITLINKCOLLECTION_H
