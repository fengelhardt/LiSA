#!/usr/bin/wish
#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# VTCL GENERATED GUI PROCEDURES
#

proc vTclWindow.dprob {base} {
    global Name;
    global sw;
    global glob;
    global tupel;
    global flags;
    global mw;
    global system;
  # internal Variables corresponding global_.h:
    global ptupel_name;
    global ptupel_number;

    close_all_open_windows

    tupelinit
    set glob(machines) [TC_getvar get_m]
    set glob(jobs) [TC_getvar get_n]
    dpinit  
    if {$base == ""} {
        set base .dprob
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel
    wm focusmodel $base passive
    if {$system(screen)=="big"} {
	wm geometry $base 641x126
	wm maxsize $base 1009 738
	set w 550
	set h 178
	wm geometry $base $w\x$h
	wm maxsize $base 1009 738
    } else {
	wm geometry $base 440x132
    }
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Problem)

    wm transient $base .lisa
    frame $base.topframe 

    # alpha entry in Problemtupel
    frame $base.topframe.fra59 \
        -borderwidth 2 -height 30 -relief groove -width 125 
    menubutton $base.topframe.fra59.men60 \
        -indicatoron 1 -menu .dprob.topframe.fra59.men60.m -padx 4 -pady 3 \
        -relief raised 
    menu $base.topframe.fra59.men60.m \
        -cursor {} -tearoff 0 
    menubutton $base.topframe.fra59.men61 \
        -indicatoron 1 -menu .dprob.topframe.fra59.men61.m -padx 4 -pady 3 \
        -relief raised 
    menu $base.topframe.fra59.men61.m \
        -cursor {} -tearoff 0 
    for  { set j 0 } \
  	{$j<[expr $ptupel_number(alpha)] } \
  	{incr j 1} {
	    if { $j==1 || $j==2} { $base.topframe.fra59.men61.m add separator	}
  	    set alpha_entry $ptupel_name($j)
  	    if { $ptupel_number($alpha_entry)=="0" } {
		 # alpha entrys as 0 1 variable
		set myname ""
		catch [set myname $Name($alpha_entry) ]
		if { $myname!="" } {
		    $base.topframe.fra59.men61.m add checkbutton \
			-label $myname  -variable tupel($alpha_entry) \
			-command " set_one_tupelentry $j; tupelinit "
		}
	    } else {
		# alpha entrys with more than two values
	# 	if { $j>0 } {
# 		    $base.topframe.fra59.men61.m add separator
# 		}
		for {set i 0} {$i<=$ptupel_number($alpha_entry,MAX)} {incr i 1} {
		    set myname ""
		    catch [set myname $Name($alpha_entry,$ptupel_name($alpha_entry,$i))]
		    if { $myname!="" } {
			$base.topframe.fra59.men61.m add radiobutton \
			    -label $myname \
			    -state active -value $i \
			    -variable tupel($alpha_entry) \
			    -command "set_one_tupelentry $j; tupelinit "
		    }
		}
	    }
	}
    label $base.topframe.fra59.lab62 \
        -borderwidth 1 -text $Name(Alpha)  -width 20 -anchor w
    label $base.topframe.fra59.ent63 -text "" -relief sunken
    frame $base.topframe.fra64 \
        -borderwidth 2 -height 29 -relief groove -width 125 

    # beta entry in Problemtupel
    menubutton $base.topframe.fra64.men65 \
        -indicatoron 1 -menu .dprob.topframe.fra64.men65.m -padx 4 -pady 3 \
        -relief raised 
    menu $base.topframe.fra64.men65.m \
        -cursor {} -tearoff 0 
    for  { set j $ptupel_number(alpha) } \
  	{$j<[expr $ptupel_number(alpha)+$ptupel_number(beta)] } \
  	{incr j 1} {
  	    set beta_entry $ptupel_name($j)
  	    if { $ptupel_number($beta_entry)=="0" } {
		 # beta entrys as 0 1 variable
		set myname ""
		catch [set myname $Name($beta_entry) ]
		if { $myname!="" } {
		    $base.topframe.fra64.men65.m add checkbutton \
			-label $myname  -variable tupel($beta_entry) \
			-command "set_one_tupelentry $j; tupelinit"
		}
	    } else {
    # beta entrys with more than two values
		$base.topframe.fra64.men65.m add  cascade -menu \
		    $base.topframe.fra64.men65.m.cas$j \
		    	-label $Name($beta_entry)
		set submen [menu $base.topframe.fra64.men65.m.cas$j -tearoff 0]
		for {set i 0} {$i<=$ptupel_number($beta_entry,MAX)} {incr i 1} {
		    set myname ""
		    catch [set myname $Name($beta_entry,$ptupel_name($beta_entry,$i))]
		    if { $myname!="" } {
			$base.topframe.fra64.men65.m.cas$j add radiobutton \
			    -label $myname \
			    -state active -value $i \
			    -variable tupel($beta_entry) \
			    -command "set_one_tupelentry $j; tupelinit "
		    }
		}
	    }
	}
    label $base.topframe.fra64.lab67 \
        -borderwidth 1 -text $Name(Beta)  -width 20 -anchor w
    label $base.topframe.fra64.ent68 -text "" -relief sunken

    # gamma entry of problemtupel
    frame $base.topframe.fra69 \
        -borderwidth 2 -height 75 -relief groove -width 125 
    menubutton $base.topframe.fra69.men70 \
        -indicatoron 1 -menu .dprob.topframe.fra69.men70.m -padx 4 -pady 3 \
        -relief raised 
    menu $base.topframe.fra69.men70.m \
        -cursor {} -tearoff 0 
    set gamma $ptupel_name([expr $ptupel_number(alpha) +$ptupel_number(beta)])
    for {set i 0} {$i<=$ptupel_number($gamma,MAX)} {incr i 1} {
	set myname ""
	catch [set myname $Name($gamma,$ptupel_name($gamma,$i))]
	if { $myname!="" } {
	    $base.topframe.fra69.men70.m add radiobutton \
		-label $myname \
		-state active -value $i \
		-variable tupel($gamma) \
 	    	-command " set_one_tupelentry [expr $ptupel_number(alpha) +$ptupel_number(beta)] ; tupelinit"
	}
    }
    label $base.topframe.fra69.lab71 \
        -borderwidth 1 -text $Name(Objective)  -width 20 -anchor w 
    label $base.topframe.fra69.ent72 -text "" -relief sunken
    label $base.topframe.lab79 \
        -borderwidth 1 -text ""  -width 20 -anchor w 

    label $base.topframe.lab73 \
        -borderwidth 1 -text "$Name(Anzmasch)   " 
    entry $base.topframe.ent73 -width 6 -textvar glob(machines)
    label $base.topframe.lab74 \
        -borderwidth 1 -text "$Name(Anzauftr)" 
    entry $base.topframe.ent74 -width 6 -textvar glob(jobs)
    frame $base.fra75 \
        -borderwidth 2 
    button $base.fra75.but74 \
        -command {destroy .dprob} -padx 9 -pady 3 -text $Name(Cancel)
    button $base.fra75.but76 \
	-padx 9 -pady 3 -text $Name(OK) \
	-command {dpend }
    button $base.fra75.but77 \
        -padx 9 -pady 3 -text $Name(Help) -command { lisa_help problem.htm }

    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.topframe \
	-in .dprob  -expand 0 -fill x  -side top
    pack $base.topframe.fra59 \
	-in .dprob.topframe -anchor center -expand 0 -fill x -side top 
    pack $base.topframe.fra59.men61 \
	-in .dprob.topframe.fra59 -anchor center -expand 0 -fill none -side right 
    pack $base.topframe.fra59.lab62 \
	-in .dprob.topframe.fra59 -anchor center -expand 0 -fill none -side left 
    pack $base.topframe.fra59.ent63 \
	-in .dprob.topframe.fra59 -anchor center -expand 1 -fill x -side top 
    pack $base.topframe.fra64 \
	-in .dprob.topframe -anchor center -expand 0 -fill x -side top 
    pack $base.topframe.fra64.men65 \
	-in .dprob.topframe.fra64 -anchor center -expand 0 -fill none -side right 
    pack $base.topframe.fra64.lab67 \
	-in .dprob.topframe.fra64 -anchor center -expand 0 -fill none -side left 
    pack $base.topframe.fra64.ent68 \
	-in .dprob.topframe.fra64 -anchor center -expand 1 -fill x -side top 
    pack $base.topframe.fra69 \
	-in .dprob.topframe -anchor center -expand 0 -fill x -side top 
    pack $base.topframe.fra69.men70 \
	-in .dprob.topframe.fra69 -anchor center -expand 0 -fill none -side right 
    pack $base.topframe.fra69.lab71 \
	-in .dprob.topframe.fra69 -anchor center -expand 0 -fill none -side left 
    pack $base.topframe.fra69.ent72 \
	-in .dprob.topframe.fra69 -anchor center -expand 1 -fill x -side top 
    pack $base.topframe.lab79 \
	-in .dprob.topframe -anchor center -expand 0 -fill none -side left 
    pack $base.topframe.ent73 \
	-in .dprob.topframe -anchor center -expand 0 -fill none -side left 
    bind   $base.topframe.ent73 <Return> \
	{dpset }
    pack $base.topframe.lab73 \
	-in .dprob.topframe -anchor center -expand 0 -fill x -side left 
    pack $base.topframe.ent74 \
	-in .dprob.topframe -anchor center -expand 0 -fill none -side left 
    bind   $base.topframe.ent74 <Return> \
	{dpset }
    pack $base.topframe.lab74 \
	-in .dprob.topframe -anchor center -expand 0 -fill x -side left 
    pack $base.fra75 \
	-in .dprob -anchor s -expand 1 -fill both -side bottom 
    pack $base.fra75.but76 \
	-in .dprob.fra75 -anchor w -expand 0 -fill none -padx 3 -pady 3 \
	-side left 
    bind $base.fra75.but76 <Return> \
	{ dpend}
  pack $base.fra75.but77 \
	-in .dprob.fra75 -anchor w -expand 0 -fill none -padx 3 -pady 3 \
	-side left 
    pack $base.fra75.but74 \
	-in .dprob.fra75 -anchor e -expand 0 -fill none -padx 3 -pady 3 \
	-side right 
    bind $base.fra75.but74 <Return> \
	{destroy .dprob}
    bind .dprob <Destroy> { set mw(fProblem) "0" }
    bind .dprob <Visibility> {  grab set .dprob; }
    #dpset to initialize text entrys
    dpset 
}

# read tuple entrys from C++
proc tupelinit { } {
    global tupel;
    global ptupel_name;
    global ptupel_number;
    set TUPEL_INDEX [expr $ptupel_number(alpha)+$ptupel_number(beta)+$ptupel_number(gamma)+$ptupel_number(misc) ]
    for {set i 0} {$i<=$TUPEL_INDEX} {incr i 1} {
	set name $ptupel_name($i)
	set tupel($name) [TC_getvar entry_in_ptupel $i]
    } 
}

#set names of alpha, beta and gamma corresponding the C++ main program
proc dpinit { } {
    global Name;
    global tupel;
    if {[winfo exists .dprob]} {
	if { $tupel(M_ENV)=="1"} { 
	    set glob(machines) 1
	    set tupel(M_NUMBER) 0
	    catch {
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_FIX) -state disabled
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_ARB) -state disabled
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_VAL) -state disabled
	    }
	}  else {
	    catch {
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_FIX) -state active
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_ARB) -state active
		.dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_VAL) -state active
	    }	
	}
	.dprob.topframe.fra59.ent63 config -text [TC_getvar alpha]
	.dprob.topframe.fra64.ent68 config -text [TC_getvar beta]
	.dprob.topframe.fra69.ent72 config -text [TC_getvar gamma]
    }
    set entry [TC_getvar problemtype]
    if {$entry != "no valid Problem"} {
	.lisa.fra40.ent43 config -text [TC_getvar problemtype]
    } else {
	.lisa.fra40.ent43 config -text $Name(No_Problem_defined)
    }
}

# set one_tupelentry
proc set_one_tupelentry { index } {
    global tupel
    global ptupel_name
    # nix
    TC_set_Tupel $index $tupel($ptupel_name($index));
    dpset;
}

# set tupelentrys as C++ Variables
proc dpset { } {
    global glob;
    global Name;
    global tupel;
    global ptupel_name;
    global ptupel_number;

     set glob(gamma) dpw(e_gamma) 
     if { $tupel(M_ENV) == "1"}  {
	 set glob(machines) 1
 	 set tupel(M_NUMBER) 0
	 catch {
	     .dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_FIX) -state disabled
	     .dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_ARB) -state disabled
	     .dprob.topframe.fra59.men61.m entryconfigure  $Name(M_NUMBER,M_VAL) -state disabled
	 }
     } else {
 	if { $glob(machines)==1 } { set glob(machines) 2 }
     }
   set TUPEL_INDEX [expr $ptupel_number(alpha)+$ptupel_number(beta)+$ptupel_number(gamma) ]
    for {set i $TUPEL_INDEX } {$i<=[expr $TUPEL_INDEX+1]} {incr i 1} {
	if {$ptupel_name($i) == "M_NO" } { TC_set_Tupel $i $glob(machines) }
	if {$ptupel_name($i) == "N_NO" } { TC_set_Tupel $i $glob(jobs) }
    }    
    TC_set_prob
    dpinit
}

# close .dprob window
proc dpend { } {
    global flags;
    global mw;
    dpset
    destroy .dprob 
    set mw(fProblem) 0;
    TC_new_problem
}

# read names and numbers as defined in the C++ file: global.hpp
# this function is called in tclinit.tcl
proc read_tupelname { } {
    global ptupel_name;
    global ptupel_number;
    set ptupel_number(alpha) [TC_getvar ptupel_number alpha]
    set ptupel_number(beta) [TC_getvar ptupel_number beta]
    set ptupel_number(gamma) [TC_getvar ptupel_number gamma]
    set ptupel_number(misc) [TC_getvar ptupel_number misc]
    set TUPEL_INDEX [expr $ptupel_number(alpha)+$ptupel_number(beta)+$ptupel_number(gamma)+$ptupel_number(misc) ]
    set  ptupel_number(TUPEL_INDEX) $TUPEL_INDEX
    for {set i 0} {$i<=$TUPEL_INDEX} {incr i 1} {
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
}






