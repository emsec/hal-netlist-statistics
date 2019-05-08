/*
 * MIT License
 *
 * Copyright (c) 2019 Chair for Embedded Security, Ruhr-University Bochum
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __HAL_PLUGIN_MY_FANCY_NEW_PLUGIN_H
#define __HAL_PLUGIN_MY_FANCY_NEW_PLUGIN_H

#include "core/interface_cli.h"
#include "def.h"

class plugin_netlist_statistics : virtual public i_cli
{
public:
    plugin_netlist_statistics() = default;
    
    ~plugin_netlist_statistics() override = default;
    
    /*
     *      interface implementations
     */
    
    /** interface implementation: i_base */
    std::string get_name() override;
    
    /** interface implementation: i_base */
    std::string get_version() override;
    
    /** interface implementation: i_base */
    std::set<interface_type> get_type() override;
    
    /** interface implementation: i_cli */
    program_options get_cli_options() override;
    
    /** interface implementation: i_cli */
    bool handle_cli_call(std::shared_ptr<netlist> n, program_arguments& args) override;
    
    static std::string get_statistics(std::shared_ptr<netlist> n);
};

#endif //__HAL_PLUGIN_MY_FANCY_NEW_PLUGIN_H
