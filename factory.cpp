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

void Factory::do_deliveries() {
    int i=0;
    i_=i;
}

void Factory::do_package_passing() {
    int i=0;
    i_=i;
}

void Factory::do_work() {
    int i=0;
    i_=i;
}

ParsedLineData parse_line(std::string line){
    ParsedLineData data;
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);

    while (std::getline(token_stream,token,' ')){
        tokens.push_back(token);
    }
    std::string typ=tokens.front();
    if (typ=="LOADING_RAMP"){
        data.element_type=ElementType::RAMP;
    }
    if (typ=="WORKER"){
        data.element_type=ElementType::WORKER;
    }
    if (typ=="STOREHOUSE"){
        data.element_type=ElementType::STOREHOUSE;
    }
    if (typ=="LINK"){
        data.element_type=ElementType::LINK;
    }
    for(const auto& it:tokens){
        if (it=="LOADING_RAMP" or it=="WORKER" or it=="STOREHOUSE" or it=="LINK"){
            continue;
        }
        std::istringstream subtoken_str(it);
        std::string subtoken;
        std::vector<std::string> subtoken_vec;

        while (std::getline(subtoken_str,subtoken,'=')){
            subtoken_vec.push_back(subtoken);
        }
        data.parameters[subtoken_vec.front()]=subtoken_vec.back();
    }
    //std::ostringstream oss;
    //for (const auto& i:data.parameters){
    //    std::cout<<i.first<<'\n'<<i.second<<'\n';
    //}
    return data;
}

Factory load_factory_structure(std::istream &is) {
    Factory factory;
    std::string line;
    std::vector<int> ramp_ids;
    std::vector<int> delivery_intervals;
    std::vector<int> worker_ids;
    std::vector<int> processing_times;
    std::vector<int> storehouse_ids;
    std::vector<int> node_ids_src;
    std::vector<int> node_ids_dest;
    std::vector<PackageQueueType> queue_types;
    std::vector<NodeTypes> node_types_src;
    std::vector<NodeTypes> node_types_dest;

    while (std::getline(is,line)){
        if(line.empty() or line.front()==';'){
            continue;
        }
        ParsedLineData parsed= parse_line(line);
        if (parsed.element_type==ElementType::STOREHOUSE){
            for(const auto& it :parsed.parameters){
                storehouse_ids.push_back(std::stoi(it.second));
            }
        }
        if (parsed.element_type==ElementType::RAMP){
            for(const auto& it :parsed.parameters){
                if (it.first=="id"){
                    ramp_ids.push_back(std::stoi(it.second));
                }
                if (it.first=="delivery-interval"){
                    delivery_intervals.push_back(std::stoi(it.second));
                }
            }
        }
        if (parsed.element_type==ElementType::WORKER){
            for(const auto& it:parsed.parameters){
                if (it.first=="id"){
                    worker_ids.push_back(std::stoi(it.second));
                }
                if (it.first=="processing-time"){
                    processing_times.push_back(std::stoi(it.second));
                }
                if (it.first=="queue-type"){
                    if(it.second=="FIFO"){
                        queue_types.push_back(PackageQueueType::FIFO);
                    }else{
                        queue_types.push_back(PackageQueueType::LIFO);
                    }
                }
            }
        }
        if (parsed.element_type==ElementType::LINK){
            for(const auto& it :parsed.parameters){
                if (it.first=="src"){
                    std::vector<std::string> tokens;
                    std::string token;
                    std::istringstream token_stream(it.second);
                    while(std::getline(token_stream,token,'-')){
                        tokens.push_back(token);
                    }
                    if (tokens.front()=="ramp"){
                        node_types_src.push_back(NodeTypes::RAMP);
                    }
                    if (tokens.front()=="worker"){
                        node_types_src.push_back(NodeTypes::WORKER);
                    }
                    if (tokens.front()=="store"){
                        node_types_src.push_back(NodeTypes::STORE);
                    }
                    //for (const auto& it:tokens){
                    //    std::cout<<it<<'\n';
                    //}
                    node_ids_src.push_back(std::stoi(tokens.back()));

                }
                if (it.first=="dest"){
                    std::vector<std::string> tokens;
                    std::string token;
                    std::istringstream token_stream(it.second);
                    while(std::getline(token_stream,token,'-')){
                        tokens.push_back(token);
                    }
                    if (tokens.front()=="ramp"){
                        node_types_dest.push_back(NodeTypes::RAMP);
                    }
                    if (tokens.front()=="worker"){
                        node_types_dest.push_back(NodeTypes::WORKER);
                    }
                    if (tokens.front()=="store"){
                        node_types_dest.push_back(NodeTypes::STORE);
                    }
                    //for (const auto& it:tokens){
                    //    std::cout<<it<<'\n';
                    //}
                    node_ids_dest.push_back(std::stoi(tokens.back()));

                }
            }
        }
    }

    // storehouses
    for (const auto& it: storehouse_ids){
        factory.add_storehouse(Storehouse(it));
    }

    //ramps
    for(std::size_t i=0;i<ramp_ids.size();++i)
        factory.add_ramp(Ramp(ramp_ids[i],delivery_intervals[i]));

    //workers
    for(std::size_t i=0;i<worker_ids.size();++i){
        factory.add_worker(Worker(worker_ids[i],processing_times[i],std::make_unique<PackageQueue>(queue_types[i])));
    }

    //links
    for(std::size_t i=0;i<node_ids_src.size();++i){
        if (node_types_src[i]==NodeTypes::RAMP){
            if (node_types_dest[i]==NodeTypes::WORKER){
                Ramp& r = *(factory.find_ramp_by_id(static_cast<ElementID>(node_ids_src[i])));
                Worker& w = *(factory.find_worker_by_id(static_cast<ElementID>(node_ids_dest[i])));
                r.receiver_preferences_.add_receiver(&w);
            }
            if (node_types_dest[i]==NodeTypes::STORE){
                Ramp& r = *(factory.find_ramp_by_id(static_cast<ElementID>(node_ids_src[i])));
                Storehouse& s=*(factory.find_storehouse_by_id(static_cast<ElementID>(node_ids_dest[i])));
                r.receiver_preferences_.add_receiver(&s);
            }
        }
        if (node_types_src[i]==NodeTypes::WORKER){
            if (node_types_dest[i]==NodeTypes::WORKER){
                Worker& w=*(factory.find_worker_by_id(static_cast<ElementID>(node_ids_src[i])));
                Worker& w2=*(factory.find_worker_by_id(static_cast<ElementID>(node_ids_dest[i])));
                w.receiver_preferences_.add_receiver(&w2);
            }
            if (node_types_dest[i]==NodeTypes::STORE){
                Worker& w=*(factory.find_worker_by_id(static_cast<ElementID>(node_types_src[i])));
                Storehouse& s=*(factory.find_storehouse_by_id(static_cast<ElementID>(node_types_dest[i])));
                w.receiver_preferences_.add_receiver(&s);
            }
        }
    }
    return factory;
}

void save_factory_structure(Factory &factory, std::ostream &os) {
    int i=1;
    if(factory.is_consistent()){
        factory.add_ramp(Ramp(i,1));
    }
    os<<"ala";
}
