#include "factory_netlist_statistics.h"
#include "plugin_netlist_statistics.h"


std::shared_ptr<i_base> factory_netlist_statistics::get_plugin_instance()
{
    return std::dynamic_pointer_cast<i_base>(std::make_shared<plugin_netlist_statistics>());
}

extern i_factory* get_factory()
{
    static factory_netlist_statistics* factory = new factory_netlist_statistics();
    return (i_factory*)factory;
}