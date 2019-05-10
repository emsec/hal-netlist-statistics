#include "core/log.h"
#include "core/utils.h"
#include "netlist/gate.h"
#include "netlist/net.h"
#include "netlist/netlist.h"
#include "pybind11/operators.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"

#include "plugin_netlist_statistics.h"

namespace py = pybind11;

#ifdef PYBIND11_MODULE
PYBIND11_MODULE(libnetlist_statistics, m)
{
    m.doc() = "hal netlist_statistics python bindings";
#else
    PYBIND11_PLUGIN(libnetlist_statistics)
{
    py::module m("libnetlist_statistics", "hal netlist_statistics python bindings");
#endif    // ifdef PYBIND11_MODULE

    py::implicitly_convertible<std::string, hal::path>();
//    py::class_<i_cli, std::shared_ptr<i_cli>, Pyi_cli>(m, "i_cli").def("exec", &i_cli::get_cli_options).def("handle_cli_call", &i_cli::handle_cli_call, py::arg("netlist"), py::arg("args"));
    py::class_<plugin_netlist_statistics, std::shared_ptr<plugin_netlist_statistics>>(m, "netlist_statistics")
        .def(py::init<>())
        .def_property_readonly("name", &plugin_netlist_statistics::get_name)
        .def_property_readonly("version", &plugin_netlist_statistics::get_version)
        .def("get_cli_options", &plugin_netlist_statistics::get_cli_options)
        .def("handle_cli_call", &plugin_netlist_statistics::handle_cli_call)
        .def_static("get_statistics", &plugin_netlist_statistics::get_statistics, py::arg("netlist"));

#ifndef PYBIND11_MODULE
    return m.ptr();
#endif    // PYBIND11_MODULE
}
