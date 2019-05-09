# Netlist Statistics Plugin

In this repository, you find the example plugin which we also explained in [the corresponding Wiki article](https://github.com/emsec/hal/wiki/How-to-create-your-first-HAL-Cpp-plugin). Here we briefly explain how to build and use it.

Here you have two options. Either you have a clone of the [HAL repository](https://github.com/emsec/hal) and build the plugin from within the plugin directory, or you have HAL installed on your system and build and link against it.

## Building the Plugin in Source

To use the main HAL build scripts, you first need a clone of HAL and clone the plugin into the plugins folder:

```
git clone https://github.com/emsec/hal.git
cd hal/plugins
git clone https://github.com/emsec/hal-netlist-statistics.git
```

Then you can rerun cmake in your build directory and build the plugin:

```
cd ../build/
cmake .. -G Ninja
ninja netlist_statistics
```

## Building the Plugin with an installed version of HAL

You can also use a precompiled version of HAL to build the plugin. Here you need to clone the plugin and have the current version of HAL installed on your system. On Ubuntu you can for example use our [ppa:sebastian-wallat/hal](ppa:sebastian-wallat/hal).

```
git clone https://github.com/emsec/hal-netlist-statistics.git
mkdir -p hal-netlist-statistics/build
cd hal-netlist-statistics/build
cmake .. -G Ninja
ninja
cd ..
```

As we currently not use the System libraries of pybind11 and spdlog, we included these sources also here in the plugin.

The plugin can also be installed. Here you should set the `CMAKE_INSTALL_PREFIX` during cmake run:

```
cd build
rm -rf *
cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=/usr/
sudo ninja install
```

## Using the plugin

After compile and optionally install the plugin is used from the command line via the `--netlist_statistics` parameter. You can use the example file `examples/fsm.v` file in the main repository to test the plugin:

```
hal -i examples/fsm.v --netlist_statistics
```

It produces following output:

```
...
[core] [info] executing 'netlist_statistics' with
[core] [info]   '--netlist_statistics':
[netlist_statistics] [info] netlist_statistics executed
[netlist_statistics] [info] 1 : CLK_BUF_BUF_inst_inst (BUF
2 : CLK_BUF_inst_inst (BUF
3 : FSM_sequential_STATE_REG_0_i_2_inst (LUT6
4 : FSM_sequential_STATE_REG_0_i_3_inst (LUT6
5 : FSM_sequential_STATE_REG_1_i_2_inst (LUT6
6 : FSM_sequential_STATE_REG_1_i_3_inst (LUT6
7 : FSM_sequential_STATE_REG_reg_0_inst (FFR
8 : FSM_sequential_STATE_REG_reg_0_i_1_inst (MUX
9 : FSM_sequential_STATE_REG_reg_1_inst (FFR
10 : GND_inst (GND
11 : FSM_sequential_STATE_REG_reg_1_i_1_inst (MUX
12 : INPUT_BUF_0_inst_inst (BUF
13 : INPUT_BUF_1_inst_inst (BUF
14 : INPUT_BUF_2_inst_inst (BUF
15 : INPUT_BUF_3_inst_inst (BUF
16 : INPUT_BUF_4_inst_inst (BUF
17 : OUTPUT_BUF_0_inst_inst (BUF
18 : OUTPUT_BUF_0_inst_i_1_inst (LUT1
19 : OUTPUT_BUF_1_inst_inst (BUF
20 : OUTPUT_BUF_1_inst_i_1_inst (LUT2
21 : VCC_inst (VCC
##########
[BUF] = 9
[FFR] = 2
[GND] = 1
[LUT1] = 1
[LUT2] = 1
[LUT6] = 4
[MUX] = 2
[VCC] = 1

[netlist.persistent] [info] serialized netlist in 0.00 seconds
[core] [info] unloaded all 1 plugins
[core] [info] closed 'examples/fsm.v'
```

## Using the plugin from the python shell

Alternatively you can use the python interface:

```
$ hal --python
[09.05.2019 09:38:50] [core] [info] Starting hal_python.
Python 3.7.3 (default, Mar 27 2019, 09:23:15)
[Clang 10.0.1 (clang-1001.0.46.3)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> n = netlist_factory.load_netlist("examples/fsm.v", "verilog", "EXAMPLE_LIB")
[09.05.2019 09:38:51] [netlist] [info] loaded gate library 'EXAMPLE_GATE_LIBRARY' in 0.00 seconds.
[09.05.2019 09:38:51] [hdl_parser] [info] parsed 'examples/fsm.v' in 0.00 seconds.
>>> from hal_plugins.libnetlist_statistics import netlist_statistics
>>> print(netlist_statistics.get_statistics(n))
1 : CLK_BUF_BUF_inst_inst (BUF
2 : CLK_BUF_inst_inst (BUF
3 : FSM_sequential_STATE_REG_0_i_2_inst (LUT6
4 : FSM_sequential_STATE_REG_0_i_3_inst (LUT6
5 : FSM_sequential_STATE_REG_1_i_2_inst (LUT6
6 : FSM_sequential_STATE_REG_1_i_3_inst (LUT6
7 : FSM_sequential_STATE_REG_reg_0_inst (FFR
8 : FSM_sequential_STATE_REG_reg_0_i_1_inst (MUX
9 : FSM_sequential_STATE_REG_reg_1_inst (FFR
10 : GND_inst (GND
11 : FSM_sequential_STATE_REG_reg_1_i_1_inst (MUX
12 : INPUT_BUF_0_inst_inst (BUF
13 : INPUT_BUF_1_inst_inst (BUF
14 : INPUT_BUF_2_inst_inst (BUF
15 : INPUT_BUF_3_inst_inst (BUF
16 : INPUT_BUF_4_inst_inst (BUF
17 : OUTPUT_BUF_0_inst_inst (BUF
18 : OUTPUT_BUF_0_inst_i_1_inst (LUT1
19 : OUTPUT_BUF_1_inst_inst (BUF
20 : OUTPUT_BUF_1_inst_i_1_inst (LUT2
21 : VCC_inst (VCC
##########
[BUF] = 9
[FFR] = 2
[GND] = 1
[LUT1] = 1
[LUT2] = 1
[LUT6] = 4
[MUX] = 2
[VCC] = 1

>>>
```