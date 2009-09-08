#!/usr/bin/wish
#############################################################################
# include files and some basic procedures
#


#################################
# USER DEFINED PROCEDURES
#

global lsa_status
global gwidth

proc Window {args} {
global vTcl
    set cmd [lindex $args 0]
    set name [lindex $args 1]
    set newname [lindex $args 2]
    set rest [lrange $args 3 end]
    if {$name == "" || $cmd == ""} {return}
    if {$newname == ""} {
        set newname $name
    }
    set exists [winfo exists $newname]
    switch $cmd {
        show {
            if {$exists == "1" && $name != "."} {wm deiconify $name; return}
            if {[info procs vTclWindow(pre)$name] != ""} {
                eval "vTclWindow(pre)$name $newname $rest"
            }
            if {[info procs vTclWindow$name] != ""} {
                eval "vTclWindow$name $newname $rest"
            }
            if {[info procs vTclWindow(post)$name] != ""} {
                eval "vTclWindow(post)$name $newname $rest"
            }
        }
        hide    { if $exists {wm withdraw $newname; return} }
        iconify { if $exists {wm iconify $newname; return} }
        destroy { if $exists {destroy $newname; return} }
    }
}

set glob(OLD_LISAHOME) ""
set glob(OLD_LANGUAGE) ""
#initialize variable ptupel_name and ptupel_number:
global ptupel_name;
global ptupel_number;
set ptupel_number(alpha) [TC_getvar ptupel_number alpha]
set ptupel_number(beta) [TC_getvar ptupel_number beta]
set ptupel_number(gamma) [TC_getvar ptupel_number gamma]
set ptupel_number(misc) [TC_getvar ptupel_number misc]
set TUPEL_INDEX [expr $ptupel_number(alpha)+$ptupel_number(beta)+$ptupel_number(gamma)+$ptupel_number(misc) ]
set  ptupel_number(TUPEL_INDEX) $TUPEL_INDEX

for {set i 0} {$i<$TUPEL_INDEX} {incr i 1} {
    set name [TC_getvar ptupel_name $i]
    set ptupel_name($i) $name
    set number [TC_getvar ptupel_number $name ]
    set ptupel_number($name) $number
    set numb_entry_index 0
    set max_entry_index 0
    for {set j 0} {$j<=$number} {incr j 1} {
	set ptupel_name($name,$j) [TC_getvar ptupel_name $i $j]
	if { $ptupel_name($name,$j)!="" } { 
	    incr numb_entry_index 1 
	    set max_entry_index $j
	}
    }
    set ptupel_number($name) $numb_entry_index
    set ptupel_number($name,MAX) $max_entry_index
}

# set some entrys of Name() to empty string
for {set j 0} {$j<=[expr $ptupel_number(alpha)+$ptupel_number(beta)+$ptupel_number(gamma)]} {incr j 1} {
    set myentry $ptupel_name($j)
    set Name($myentry) ""
    for {set i 0} {$i<=$ptupel_number($myentry,MAX)} {incr i 1} {
	set Name($myentry,$ptupel_name($myentry,$i)) ""
    }
}


source [TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/main.tcl
source [TC_getvar LISAHOME]/tcl/validation.tcl
source [TC_getvar LISAHOME]/tcl/main_win.tcl
source [TC_getvar LISAHOME]/tcl/dia_prob.tcl
source [TC_getvar LISAHOME]/tcl/schedule.tcl
source [TC_getvar LISAHOME]/tcl/values.tcl
source [TC_getvar LISAHOME]/tcl/dialog.tcl
source [TC_getvar LISAHOME]/tcl/dia_clas.tcl
source [TC_getvar LISAHOME]/tcl/message.tcl
source [TC_getvar LISAHOME]/tcl/dia_gant.tcl
source [TC_getvar LISAHOME]/tcl/default.tcl
source [TC_getvar LISAHOME]/tcl/print.tcl
source [TC_getvar LISAHOME]/tcl/man_oper.tcl
source [TC_getvar LISAHOME]/tcl/seqlist.tcl
source [TC_getvar LISAHOME]/tcl/gen_algp.tcl
source [TC_getvar LISAHOME]/tcl/mng_ealg.tcl
source [TC_getvar LISAHOME]/tcl/options.tcl


##########################
# create algorithm communication dir
cd [TC_getvar LISACONFIG]
file mkdir proc

############################
# open icon window
#  if {$system(os)=="unix"} {
#      make_lisa_icon "[TC_getvar LISAHOME]/img/llogo_s.gif"
#  }

##########################
# insert additional tcl-files for external algorithms
#read_all_desc_files_xml
#source  [TC_getvar LISAHOME]/tcl/external/ext_alg.tcl
#if { $glob(OLD_LISAHOME) != [TC_getvar LISAHOME] || $glob(OLD_LANGUAGE) != [TC_getvar LANGUAGE] } {
    cd [TC_getvar LISAHOME]/tcl
    file mkdir language
    cd language
    file mkdir [TC_getvar LANGUAGE]
    cd [TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]
    file mkdir external
    cd [TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/external
    #file delete ext_alg.tcl # do not delete this, so that lisa does not rebuild algorithm descriptions everytime
    read_all_desc_files_xml
    source  [TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/external/ext_alg.tcl
#} 
cd [TC_getvar LISAHOME]/data

############################
# open main window

 Window show .
 Window show .lisa
