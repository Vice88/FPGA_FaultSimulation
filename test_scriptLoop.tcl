load_package chip_planner
load_package flow

set COMPONENT_PATH "|FaultInjectionTester|CUT:CUT1|testModLcell:lcCUT|"
set FILEPATH [file dirname [file normalize $argv0]]

# adjust path

project_open -force $FILEPATH/FaultInjectionTester_restored/FaultInjectionTester
# new compilation with your quartus version
#quartus_map
#quartus_fit
execute_module -tool map
execute_module -tool fit
read_netlist 

for {set i 0} {$i < $argc} {incr i 2} {
puts "$i th check"
set nodeName [lindex $argv $i]
set LUTValue [lindex $argv [expr {$i + 1}]]
puts "nodeName: $nodeName"
puts "LUTValue: $LUTValue"

#															adjust component path!
set oldValue [get_node_info -node [get_node_by_name -name $COMPONENT_PATH[set nodeName]] -info "Sum LUT Mask"]
[set_node_info -node [get_node_by_name -name $COMPONENT_PATH[set nodeName]] -info "Sum LUT Mask" $LUTValue]
puts "after changing LutValue to"
puts [get_node_info -node [get_node_by_name -name $COMPONENT_PATH[set nodeName]] -info "Sum LUT Mask"]
puts "check and save success: " 
puts [check_netlist_and_save]

# Ihr Wichser heißt doch beide Daniel...
exec $FILEPATH\\FaultInjectionTest.exe >&@stdout

# undo the changes
[set_node_info -node [get_node_by_name -name $COMPONENT_PATH[set nodeName]] -info "Sum LUT Mask" $oldValue]
puts "re - changing LutValue to"
puts [get_node_info -node [get_node_by_name -name $COMPONENT_PATH[set nodeName]] -info "Sum LUT Mask"]
puts "second check and save success: " 
puts [check_netlist_and_save]
}

# final test, should return no faults
exec $FILEPATH\\FaultInjectionTest.exe >&@stdout


