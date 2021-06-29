#ifndef ud_h
#define ud_h

#include "edm4hep/UserExtCollection.h"

#include <vector>
#include <map>

// helper class to encode/decode the User Defined Data
// an example with x/y/z (float) and i (int)
struct ud {

    // for writing
    ud() {}

    ud& reg(const std::string& n, int t, int e) {
        ns.push_back(n);
        ts.push_back(t);
        es.push_back(e);

        ++tc[t];
        return *this;
    }

    // empty impl, need template specialization
    template<class T>
    ud& put(const std::string&, T) {
        return *this;
    }

    template<>
    ud& put(const std::string& name, int v) {
        mi[name] = v;
        return *this;
    }

    template<>
    ud& put(const std::string& name, float v) {
        mf[name] = v;
        return *this;
    }

    // for simplicity, return the value via argument
    template<class T>
    ud& get(const std::string&, T&) {
        return *this;
    }
    
    template<>
    ud& get(const std::string& name, int& v) {
        v = mi[name];
        return *this;
    }

    template<>
    ud& get(const std::string& name, float& v) {
        v = mf[name];
        return *this;
    }
    
    ud& to(edm4hep::UserExt ue) {
        for (int i = 0; i < ns.size(); ++i) {
            const std::string& k = ns[i];
            int ti = ts[i];
            int ei = es[i];

            if (ti == 0) { // int
                ue.addToValI(mi[k]);
            } else if (ti == 1) { // float
                ue.addToValF(mf[k]);
            } else {
                // todo: throw an error
            }

                
        }

        return *this;
    }

    ud& from(edm4hep::ConstUserExt ue, int ith) {

        for (int i = 0; i < ns.size(); ++i) {
            const std::string& k = ns[i];
            int ti = ts[i];
            int ei = es[i]; // the local idx in the element
            ei = ei + ith*tc[ti]; // the global idx in the array

            if (ti == 0) { // int
                mi[k] = ue.getValI(ei);
            } else if (ti == 1) { // float
                mf[k] = ue.getValF(ei);
            } else {
                // todo: throw an error
            }

        }
        
        return *this;
    }

    // internal idx
    std::vector<std::string> ns; // names
    std::vector<int> ts; // type idx
    std::vector<int> es; // elem idx in 'type' array

    // counter for each type
    std::map<int, int> tc; // type counter

    // holder
    std::map<std::string, int> mi;
    std::map<std::string, float> mf;
};

#endif
