#ifndef ZPO_SIECI_FACTORY_HPP
#define ZPO_SIECI_FACTORY_HPP
#include "nodes.hpp"
#include "types.hpp"
#include <sstream>

template <class Node>
class NodeCollection{
public:

    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;


    void add(Node&& node) {kolekcja_.emplace_back(std::move(node));}
    iterator find_by_id(ElementID id) {auto it=std::find_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem.get_id()==id);}); if (it==kolekcja_.end()){it+1;}return it;}
    const_iterator find_by_id(ElementID id) const {const auto it=std::find_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem.get_id()==id);}); if (it==kolekcja_.end()){it+1;}return it;}
    void remove_by_id(ElementID id) {std::remove_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem.get_id()==id);});}

    iterator begin() {return kolekcja_.begin();}
    const_iterator cbegin() const {return kolekcja_.cbegin();}
    iterator end() {return kolekcja_.end();}
    const_iterator cend() const {return kolekcja_.cend();}

private:
    container_t kolekcja_;

};

class Factory{
public:
    Factory(int i, int j):i_(i),j_(j){}
    Factory():i_(0),j_(0){}

    void add_ramp(Ramp&& ramp) {ramp_.add(std::move(ramp));}
    void remove_ramp(ElementID id){ remove_receiver(ramp_,id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramp_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const{return ramp_.find_by_id(id);}

    NodeCollection<Ramp>::iterator ramp_begin(){return ramp_.begin();}
    NodeCollection<Ramp>::const_iterator ramp_cbegin(){return ramp_.cbegin();}
    NodeCollection<Ramp>::iterator ramp_end(){return ramp_.end();}
    NodeCollection<Ramp>::const_iterator ramp_cend(){return ramp_.cend();}


    void add_storehouse(Storehouse&& storehouse){storehouse_.add(std::move(storehouse));}
    void remove_storehouse(ElementID id){ remove_receiver(storehouse_,id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return storehouse_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return storehouse_.find_by_id(id);}

    NodeCollection<Storehouse>::iterator storehouse_begin(){return storehouse_.begin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin(){return storehouse_.cbegin();}
    NodeCollection<Storehouse>::iterator storehouse_end(){return storehouse_.end();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend(){return storehouse_.cend();}


    void add_worker(Worker&& worker){worker_.add(std::move(worker));}
    void remove_worker(ElementID id){ remove_receiver(worker_,id);}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return worker_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return worker_.find_by_id(id);}

    NodeCollection<Worker>::iterator worker_begin() {return worker_.begin();}
    NodeCollection<Worker>::const_iterator worker_cbegin(){return worker_.cbegin();}
    NodeCollection<Worker>::iterator worker_end() {return worker_.end();}
    NodeCollection<Worker>::const_iterator worker_cend(){return worker_.cend();}

    bool is_consistent(){return true;}

    void do_deliveries();
    void do_package_passing();
    void do_work();


private:
    void remove_receiver(NodeCollection<Worker>& collection,ElementID id);
    void remove_receiver(NodeCollection<Storehouse>& collection,ElementID id);
    void remove_receiver(NodeCollection<Ramp>& collection,ElementID id);

    NodeCollection<Ramp> ramp_;
    NodeCollection<Storehouse> storehouse_;
    NodeCollection<Worker> worker_;
    int i_;
    int j_;
};
Factory load_factory_structure(std::istream &is);
void save_factory_structure(Factory &factory,std::ostream &os);

enum class ElementType{
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

struct ParsedLineData{
    ElementType element_type;
    std::map<std::string,std::string> parameters;
};

enum class NodeTypes{
    RAMP,
    WORKER,
    STORE
};

#endif //ZPO_SIECI_FACTORY_HPP
