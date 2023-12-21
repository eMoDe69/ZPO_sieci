#ifndef ZPO_SIECI_FACTORY_HPP
#define ZPO_SIECI_FACTORY_HPP
#include "nodes.hpp"
#include "types.hpp"

template <class Node>

class NodeCollection{
public:
    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;


    void add(Node&& node) {kolekcja_.emplace_back(std::move(node));}
    iterator find_by_id(ElementID id) {auto it=std::find_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem==id);}); if (it==kolekcja_.end()){return it+1;}}
    const_iterator find_by_id(ElementID id) const {{const auto it=std::find_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem==id);}); if (it==kolekcja_.end()){return it+1;}}}
    void remove_by_id(ElementID id) {std::remove_if(kolekcja_.begin(),kolekcja_.end(),[&id](const auto& elem){return (elem==id);});}

    const_iterator begin() const {return kolekcja_.begin();}
    const_iterator cbegin() const {return kolekcja_.cbegin();}
    const_iterator end() const {return kolekcja_.end();}
    const_iterator cend() const {return kolekcja_.cend();}

private:
    container_t kolekcja_;

};

class Factory:public Worker ,public Storehouse,public Ramp,public NodeCollection<class Node> {
    void add_ramp(Ramp&&);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;
    NodeCollection<Ramp>::iterator ramp_cbegin();
    NodeCollection<Ramp>::const_iterator ramp_cend();

    void add_storehouse(Storehouse&&);
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;
    NodeCollection<Storehouse>::iterator storehouse_cbegin();
    NodeCollection<Storehouse>::const_iterator storehouse_cend();

    void add_worker(Worker&&);
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;
    NodeCollection<Worker>::iterator worker_cbegin();
    NodeCollection<Worker>::const_iterator worker_cend();

private:
    void remove_receiver(NodeCollection<Node>& collection,ElementID id);


};
#endif //ZPO_SIECI_FACTORY_HPP
