#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11 Project
#

proc vTclWindow.ext_alg_graph {base} {
    if {$base == ""} {
        set base .ext_alg_graph
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    global lsa_status
    global ext_algo
    global Name
    global stop
    global system
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel 
    wm focusmodel $base passive

    wm geometry $base 410x240+40+40
    # This must be changed to obey PROGRESS_INDICATOR_STEPS !!!
    wm resizable $base 0 0
    wm deiconify $base
    wm title $base "External Algorithm"
    wm transient $base .lisa
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra19.lab20 \
        -borderwidth 1 \
        -text $Name(Progress) 
    frame $base.fra21 \
        -borderwidth 2 -height 75 -relief sunken \
        -width 125 
    canvas $base.fra21.can22 \
        -background white -borderwidth 2 -height 0 -relief ridge -width 0 
    frame $base.fra25 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra25.lab20 \
        -text ""
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra23.but24 \
        -padx 11 -pady 4 -text $Name(Cancel) \
	-command {exec kill -2 $lsa_status(pid); \
		      set stop(pid) $lsa_status(pid); Window show .alg_stop }
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .ext_alg_graph -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab20 \
        -in .ext_alg_graph.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra21 \
        -in .ext_alg_graph -anchor center -expand 1 -fill both -side top 
    pack $base.fra21.can22 \
        -in .ext_alg_graph.fra21 -anchor center -expand 1 -fill both -side top 
    pack $base.fra25 \
        -in .ext_alg_graph -anchor center -expand 0 -fill x -side top 
    pack $base.fra25.lab20 \
        -in .ext_alg_graph.fra25 -anchor w -expand 0 -side left 
  	pack $base.fra23 \
	    -in .ext_alg_graph -anchor center -expand 0 -fill x -side top 
	pack $base.fra23.but24 \
	    -in .ext_alg_graph.fra23 -anchor center -expand 0 -fill none -side top 


    # we know: format= 410x290
    set ext_algo(height) 230
    set ext_algo(width) [TC_getvar PROGRESS_INDICATOR_STEPS]
    set ext_algo(height) [expr $ext_algo(height) - 85]
    set ext_algo(xpos) 0
    bind $base <Visibility> {  grab set .ext_alg_graph }
    bind $base <Destroy> {
	bind .ext_alg_graph <Destroy> { }
	ext_alg_cancel
    }

}

proc ext_alg_cancel { } {
    global ext_algo;
    global stop
    global lsa_status
    if {$ext_algo(running)==1 } {
	catch {exec ./kill -2 $lsa_status(pid)}; 
	set stop(pid) $lsa_status(pid); 
	Window show .alg_stop 
    }
}

# The following procedure is called if the external algorithm prints something to cout

proc algo_event {fid file name_of_algo} {
    global ext_algo;
    global lsa_status;
    set entryname ""
    set ext_algo(objective) 0
    
    if { [gets $fid line] >=0} {
	if {[winfo exists .ext_alg_graph]} {
	    .ext_alg_graph.fra25.lab20 config -text $line
	    scan $line "%*s  %*s %s %d" entryname ext_algo(objective)
	    if {$entryname!="objective="} { 
		scan $line "%s %d" entryname ext_algo(objective)
	    }
	    if {$entryname=="PID="} {
		# puts "pid=$ext_algo(objective)"
		set lsa_status(pid)  $ext_algo(objective)
	    }
	    if {$entryname=="objective="} {
		set ext_algo(objective) "$ext_algo(objective).0"
		if {$ext_algo(xpos)==0} { 
		    if {$ext_algo(objective)>0} {
			set ext_algo(max) $ext_algo(objective)
		    } else { 
			set ext_algo(max) 100
		    }
		}
		set ext_algo(xpos) [expr $ext_algo(xpos)+1]
		if {$ext_algo(xpos)>$ext_algo(width)} {
		    set ext_algo(xpos) 1
		    .ext_alg_graph.fra21.can22  delete all}
		set ypos [expr 5+$ext_algo(height)-($ext_algo(objective) / $ext_algo(max))*$ext_algo(height)]
		.ext_alg_graph.fra21.can22 create line $ext_algo(xpos) $ypos $ext_algo(xpos) \
		    $ext_algo(height) -fill Black
	    } else {  
		if {$entryname=="ERROR:" || $entryname=="WARNING:" } {
		    TC_error "In External-Module: $line"} 
	    }
	}
    }  else { if {$lsa_status(algo_call)=="gen_ptst"} {
	showdelaym "Parameter test finshed" "Test finished. Check output file manually" 
    }
	if {[winfo exists .alg_stop]} { destroy .alg_stop}; 
	catch {close $fid}
	if {[winfo exists .ext_alg_graph]} {
	    bind .ext_alg_graph <Destroy> { }
	    destroy .ext_alg_graph
	}
	TC_open_schedule $file
    }
}


proc vTclWindow.alg_stop {base} {
    global Name
    global nb
    global stop
    if {$base == ""} {
        set base .alg_stop
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -background #D5A5D5A5F000 -cursor xterm 
    wm focusmodel $base passive
    wm geometry $base 441x180
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "Alg_stop"
    wm transient $base .lisa
    frame $base.fra25 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    message $base.fra25.mes30 \
        -aspect 500 -background white \
        -padx 6 -pady 3 -text $Name(Alg_stop_message)
    frame $base.fra26 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra26.but27 \
        -padx 11 -pady 4 -text $Name(Kill_algo) \
	-command {catch {exec kill -9 $stop(pid)}
		      if {[winfo exists .ext_alg_graph]} { destroy .ext_alg_graph};
		      catch {destroy .alg_stop} }
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra25 \
        -in .alg_stop -anchor center -expand 1 -fill both -side top 
    pack $base.fra25.mes30 \
        -in .alg_stop.fra25 -anchor center -expand 1 -fill both -side top 
    pack $base.fra26 \
        -in .alg_stop -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.but27 \
        -in .alg_stop.fra26 -anchor center -expand 0 -fill none -side top 
   bind $base <Visibility> {  grab set .alg_stop }
}

