#include "factory.hpp"

void Factory::remove_receiver(NodeCollection<Ramp> &collection, ElementID id) {
    collection.remove_by_id(id);
};

void Factory::remove_receiver(NodeCollection<Worker> &collection, ElementID id) {
    collection.remove_by_id(id);
};

void Factory::remove_receiver(NodeCollection<Storehouse> &collection, ElementID id) {
    collection.remove_by_id(id);
};

Factory load_factory_structure(std::istream &is) {
    return Factory(1,1);
};

void save_factory_structure(Factory &factory, std::ostream &os) {
    int i=0;
};
