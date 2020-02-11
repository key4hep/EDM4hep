#include "edm4hepRDF.h"
#include "Math/Vector4D.h"

namespace edm4hep {

std::vector<float> pt (std::vector<MCParticleData> const& in){
  std::vector<float> result;
  for (size_t i = 0; i < in.size(); ++i) {
    result.push_back(std::sqrt(in[i].momentum.x * in[i].momentum.x + in[i].momentum.y * in[i].momentum.y));
  }
  return result;
}

std::vector<float> eta(std::vector<MCParticleData> const& in){
  std::vector<float> result;
  ROOT::Math::PxPyPzMVector lv;
  for (size_t i = 0; i < in.size(); ++i) {
    lv.SetCoordinates(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    result.push_back(lv.Eta());
  }
  return result;
}

std::vector<float> cos_theta(std::vector<MCParticleData> const& in){
  std::vector<float> result;
  ROOT::Math::PxPyPzMVector lv;
  for (size_t i = 0; i < in.size(); ++i) {
    lv.SetCoordinates(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    result.push_back(cos(lv.Theta()));
  }
  return result;
}

std::vector<double> r (std::vector<SimTrackerHitData> const& in) {
  std::vector<double> result;
  for (size_t i = 0; i < in.size(); ++i) {
    result.push_back(std::sqrt(in[i].position.x*in[i].position.x + in[i].position.y*in[i].position.y));
  }
  return result; 
}

}
