#include "plugin_netlist_statistics.h"
#include "core/log.h"
#include "netlist/gate.h"
#include "netlist/netlist.h"
#include <sstream>

extern std::shared_ptr<i_base> get_plugin_instance()
{
    return std::make_shared<plugin_netlist_statistics>();
}

std::string plugin_netlist_statistics::get_name() const
{
    return std::string("netlist_statistics");
}

std::string plugin_netlist_statistics::get_version() const
{
    return std::string("0.1");
}

void plugin_netlist_statistics::initialize()
{

}

program_options plugin_netlist_statistics::get_cli_options() const
{
    program_options description;
    description.add("--netlist_statistics", "executes the netlist_statistics");
    return description;
}

bool plugin_netlist_statistics::handle_cli_call(std::shared_ptr<netlist> netlist, program_arguments &args)
{
    UNUSED(args);

    log("netlist_statistics executed");

    log_info(this->get_name(), get_statistics(netlist));
    return true;
}

std::string plugin_netlist_statistics::get_statistics(std::shared_ptr<netlist> netlist)
{
    std::stringstream stream;

    std::map<std::string, u32> type_cnt;
    std::map<u32, std::shared_ptr<gate>> ordered_gates;
    for (const auto& gate : netlist->get_gates())
    {
        type_cnt[gate->get_type()->to_string()]++;
        ordered_gates[gate->get_id()] = gate;
    }

    for (const auto& it : ordered_gates)
        stream << it.second->get_id() << " : " << it.second->get_name() << " (" << it.second->get_type() << ")" << std::endl;

    stream << "##########" << std::endl;
    for (const auto& it : type_cnt)
        stream << "[" << it.first << "] = " << it.second << std::endl;
    stream << "##########" << std::endl;
    return stream.str();
}

