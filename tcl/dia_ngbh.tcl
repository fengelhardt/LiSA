proc vTclWindow.dia_ngbh {base} {
    global Name;
    global nb;
    global nbmax
    global maxsteps
    global nboldx
    global system
    set Name(MAX_STUCK) "Maximale Anzahl der Stillstaende"
    if {$base == ""} {
        set base .dia_ngbh
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel 
    wm focusmodel $base passive
    if {$system(screen)=="big"} {
	wm geometry $base 547x527
	wm maxsize $base 1265 994
	set w [winfo reqwidth $base]
	set h [winfo reqheight $base]
	wm minsize $base $w $h
    } else {
	wm geometry $base 400x425
    }

    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Neighborhood_search)
    wm transient $base .lisa

 #    frame $base.fra18 \
#         -borderwidth 2 -height 75 -width 125 
#     label $base.fra18.lab19 \
#          -borderwidth 1 \
#          -text $Name(Controlparameters) 
    frame $base.fra20 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra20.lab21 \
        -anchor w  -borderwidth 1 \
         -justify left -text $Name(NGBH) -width 30 
    menubutton $base.fra20.men22 \
	-indicatoron 1 -menu .dia_ngbh.fra20.men22.m -padx 5 -pady 4 \
        -relief sunken -text OSP_BL_SHIFT 
    menu $base.fra20.men22.m \
         -cursor {} \
	-tearoff 0 
    $base.fra20.men22.m add command \
        -label $Name(API) \
	-command {nb_set "NGBH" "API"}
    $base.fra20.men22.m add command  \
        -label $Name(SHIFT) \
	-command  {nb_set "NGBH" "SHIFT"}
    if {1 != [TC_getvar get_m]} {
    $base.fra20.men22.m add command  \
        -label $Name(3_CR) \
	-command  {nb_set "NGBH" "3_CR"} 
    $base.fra20.men22.m add command  \
        -label $Name(CR_API) \
	-command  {nb_set "NGBH" "CR_API"}
    $base.fra20.men22.m add command  \
        -label $Name(BL_API) \
	-command  {nb_set "NGBH" "BL_API"}
    $base.fra20.men22.m add command  \
        -label $Name(CR_SHIFT) \
	-command  {nb_set "NGBH" "CR_SHIFT"}
    $base.fra20.men22.m add command  \
        -label $Name(BL_SHIFT) \
	-command  {nb_set "NGBH" "BL_SHIFT"}
  #   $base.fra20.men22.m add command  \
#         -label $Name(CR_TST) \
# 	-command  {nb_set "NGBH" "CR_TST"}  -state disabled}
    frame $base.fra23 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra23.lab24 \
        -anchor w  -borderwidth 1 \
	-text $Name(METHOD) -width 30 
    menubutton $base.fra23.men25 \
	-menu .dia_ngbh.fra23.men25.m -padx 5 -pady 4 \
        -relief sunken -text $Name(SA) -indicatoron 1
    menu $base.fra23.men25.m \
	-cursor {} \
	-tearoff 0 
    $base.fra23.men25.m add command \
        -label $Name(SA) \
	-command  {nb_set "METHOD" "SA"}
    $base.fra23.men25.m add command \
        -label $Name(TA) \
	-command  {nb_set "METHOD" "TA"}
    $base.fra23.men25.m add command \
        -label $Name(TS) \
	-command  {nb_set "METHOD" "TS"}
    $base.fra23.men25.m add command \
        -label $Name(II) \
	-command  {nb_set "METHOD" "II"}
    frame $base.fra26 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra26.lab27 \
        -anchor w  -borderwidth 1 \
	-text $Name(TYPE) -width 30 
    menubutton $base.fra26.men28 \
	-menu .dia_ngbh.fra26.men28.m -padx 5 -pady 4 \
        -relief sunken -text $Name(RAND) -indicatoron 1
    menu $base.fra26.men28.m \
         -cursor {} \
         -tearoff 0 
    $base.fra26.men28.m add command \
        -label $Name(RAND) \
	-command  {nb_set "TYPE" "RAND"}
    $base.fra26.men28.m add command \
        -label $Name(ENUM) \
	-command  {nb_set "TYPE" "ENUM"}
    frame $base.fra30 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra30.lab31 \
        -anchor w  -borderwidth 1 \
	-text $Name(STEPS) -width 30 
    entry $base.fra30.ent32 -textvar nb(STEPS)
#     frame $base.fra33 \
#          -borderwidth 2 -height 75 -relief groove \
#         -width 125 
#     label $base.fra33.lab34 \
#         -anchor w  -borderwidth 1 \
# 	-text $Name(NUMB_STUCKS) -width 30 
#     entry $base.fra33.ent35 -textvar nb(NUMB_STUCKS)
#     frame $base.fra36 \
#          -borderwidth 2 -height 75 -relief groove \
#         -width 125 
#     label $base.fra36.lab37 \
#         -anchor w  -borderwidth 1 \
#          -text $Name(ABORT_BOUND) -width 30 
#     entry $base.fra36.ent38 -textvar nb(ABORT_BOUND)         
    frame $base.fra39 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra39.lab40 \
         -borderwidth 1 \
         -text $Name(ONLY_SA_TA) 
    frame $base.fra41 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra41.lab42 \
        -anchor w  -borderwidth 1 \
         -text $Name(PROB) -width 30 
    entry $base.fra41.ent43 -textvar nb(PROB) 
    frame $base.fra44 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra44.lab45 \
        -anchor w  -borderwidth 1 \
          -text $Name(MAX_STUCK) -width 30 
    entry $base.fra44.ent46 -textvar nb(MAX_STUCK) 
    frame $base.fra47 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra47.lab48 \
         -borderwidth 1 \
         -text $Name(ONLY_TS) 
    frame $base.fra49 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra49.lab50 \
        -anchor w  -borderwidth 1 \
	-text $Name(TABULENGTH) -width 30 
    entry $base.fra49.ent51 -textvar nb(TABULENGTH)
    frame $base.fra52 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra52.lab53 \
        -anchor w  -borderwidth 1 \
         -text $Name(NUMB_NGHB) -width 30 
    entry $base.fra52.ent54 -textvar nb(NUMB_NGHB)        
     frame $base.fra33 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra33.lab34 \
        -anchor w  -borderwidth 1 \
	-text $Name(NUMB_STUCKS) -width 30 
    entry $base.fra33.ent35 -textvar nb(NUMB_STUCKS)
    frame $base.fra36 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra36.lab37 \
        -anchor w  -borderwidth 1 \
         -text $Name(ABORT_BOUND) -width 30 
    entry $base.fra36.ent38 -textvar nb(ABORT_BOUND)    
#     frame $base.fra55 \
#          -borderwidth 2 -height 75 -relief sunken \
#         -width 125 
#     checkbutton $base.fra55.che56 \
#         -text $Name(START_NB_BACK) -variable che56 
    frame $base.fra18 \
         -borderwidth 2 -height 75 -width 125 
     label $base.fra18.lab19 \
          -borderwidth 1 \
          -text $Name(Abort_criteria) 
    frame $base.fra57 \
         -borderwidth 2 -height 75 -width 125 
    button $base.fra57.but59 \
         -borderwidth 4 \
	-padx 11 -pady 4 -text $Name(Cancel) \
	-command {destroy .dia_ngbh}
    button $base.fra57.but58 \
         -borderwidth 4 \
         -padx 11 -pady 4 -text $Name(START_NB) \
	-command {destroy .dia_ngbh; TC_start_nb}
    button $base.fra57.but60 \
         -borderwidth 4 \
         -padx 11 -pady 4 -text $Name(Help) \
	-command {}
    ###################
    # SETTING GEOMETRY
    ###################
  #   pack $base.fra18 \
#         -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
#     pack $base.fra18.lab19 \
#         -in .dia_ngbh.fra18 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra20 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.lab21 \
        -in .dia_ngbh.fra20 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.men22 \
        -in .dia_ngbh.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra23 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.lab24 \
        -in .dia_ngbh.fra23 -anchor center -expand 0 -fill none -side left 
    pack $base.fra23.men25 \
        -in .dia_ngbh.fra23 -anchor center -expand 0 -fill x -side top 
    pack $base.fra26 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.lab27 \
        -in .dia_ngbh.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.men28 \
        -in .dia_ngbh.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra30 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra30.lab31 \
        -in .dia_ngbh.fra30 -anchor center -expand 0 -fill none -side left 
    pack $base.fra30.ent32 \
        -in .dia_ngbh.fra30 -anchor center -expand 0 -fill x -ipady 2 -side top 
#     pack $base.fra33 \
#         -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
#     pack $base.fra33.lab34 \
#         -in .dia_ngbh.fra33 -anchor center -expand 0 -fill none -side left 
#     pack $base.fra33.ent35 \
#         -in .dia_ngbh.fra33 -anchor center -expand 0 -fill x -ipady 2 -side top 
#     pack $base.fra36 \
#         -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
#     pack $base.fra36.lab37 \
#         -in .dia_ngbh.fra36 -anchor center -expand 0 -fill none -side left 
#     pack $base.fra36.ent38 \
#         -in .dia_ngbh.fra36 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra39 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra39.lab40 \
        -in .dia_ngbh.fra39 -anchor center -expand 0 -fill none -ipady 2 \
        -side top 
    pack $base.fra41 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra41.lab42 \
        -in .dia_ngbh.fra41 -anchor center -expand 0 -fill none -side left 
    pack $base.fra41.ent43 \
        -in .dia_ngbh.fra41 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra44 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra44.lab45 \
        -in .dia_ngbh.fra44 -anchor center -expand 0 -fill none -side left 
    pack $base.fra44.ent46 \
        -in .dia_ngbh.fra44 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra47 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra47.lab48 \
        -in .dia_ngbh.fra47 -anchor center -expand 0 -fill none -ipady 2 \
        -side top 
    pack $base.fra49 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra49.lab50 \
        -in .dia_ngbh.fra49 -anchor center -expand 0 -fill none -side left 
    pack $base.fra49.ent51 \
        -in .dia_ngbh.fra49 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra52 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra52.lab53 \
        -in .dia_ngbh.fra52 -anchor center -expand 0 -fill none -side left 
    pack $base.fra52.ent54 \
        -in .dia_ngbh.fra52 -anchor center -expand 0 -fill x -ipady 2 -side top 
#     pack $base.fra55 \
#         -in .dia_ngbh -anchor center -expand 0 -fill x -ipady 2 -side top 
 #    pack $base.fra55.che56 \
#         -in .dia_ngbh.fra55 -anchor center -expand 0 -fill none -side top 
     pack $base.fra18 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
     pack $base.fra18.lab19 \
         -in .dia_ngbh.fra18 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra33 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra33.lab34 \
        -in .dia_ngbh.fra33 -anchor center -expand 0 -fill none -side left 
    pack $base.fra33.ent35 \
        -in .dia_ngbh.fra33 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra36 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side top 
    pack $base.fra36.lab37 \
        -in .dia_ngbh.fra36 -anchor center -expand 0 -fill none -side left 
    pack $base.fra36.ent38 \
        -in .dia_ngbh.fra36 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra57 \
        -in .dia_ngbh -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra57.but58 \
        -in .dia_ngbh.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side left 
    pack $base.fra57.but59 \
        -in .dia_ngbh.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side left
    pack $base.fra57.but60 \
        -in .dia_ngbh.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side right

    nb_set NGBH $nb(NGBH)
    nb_set METHOD $nb(METHOD)
    nb_set TYPE $nb(TYPE)
    set nbmax 0
    set maxsteps 0
    set  nboldx 0
    set nb(xpos) 0
    bind $base <Visibility> {  
	grab set .dia_ngbh;  
	bind .dia_ngbh <Visibility> { } }
#    bind $base <Destroy> { set mw(fSequence) "0" }
set nb(running) 1
}


proc nb_set {name value} {
    global nb;
    global Name;
    if {$name == "NGBH"} {
	set nb(NGBH) $value
	.dia_ngbh.fra20.men22 config -text $Name($value)
    }
    if {$name == "METHOD"} {
	set nb(METHOD) $value
	.dia_ngbh.fra23.men25 config -text $Name($value)
    }
    if {$name == "TYPE"} {
	set nb(TYPE) $value
	.dia_ngbh.fra26.men28  config -text $Name($value)
    }
}

# If the external Module prints something or ends
proc nbevent {fid file} {
    global Name;
    global nbmax;
    global maxsteps;
    global nbwidth;
    global nbheight;
    global nboldx;
    global nb
    
    #    set xpos 0
    set ypos 0
    
    if { [gets $fid line] >=0} {
	set namesteps ""
	set steps 0
	set objective 0
	set best_objective 0
	scan $line "%s %d %*s %d %*s %d" namesteps steps objective best_objective 
	if {$namesteps == "steps="} {
	   #  if {$steps > $maxsteps} {
# 		set maxsteps $steps
# 		set nbmax [expr $objective*1.07]
# 	    }
	    if {$nb(xpos)==0} { 
		if {$objective>0} {
		    set nbmax $objective
		} else { 
		    set nbmax 100
		}
	    }
	    set fsteps "$steps.0"
	    set nb(xpos) [expr $nb(xpos)+1]
	    if {[winfo exists .nb_graph]} {
		.nb_graph.fra24.lab20 config -text "$Name(Objective_value)=$objective"
		.nb_graph.fra24.lab21 config \
		    -text "($Name(best_Objective)= $best_objective)"
		set objective "$objective.0"
		set ypos [expr 5+$nbheight-($objective / $nbmax)*$nbheight]
		.nb_graph.fra21.can22 create line $nb(xpos) $ypos $nb(xpos) \
		    $nbheight -fill Black 
	    }
	} else {
	    if {$namesteps == "ERROR:"} {
		TC_error "In Module nb_iter: $line"  	
	    }
	}
    } else {
	if {[winfo exists .nb_graph]} { 
	    bind .nb_graph <Destroy> { }
	    destroy .nb_graph 
	}
	if {[winfo exists .alg_stop]} { destroy .alg_stop}
	catch {
	    TC_open_schedule $file
	    if {$fid != ""} {
		catch {close $fid}
		set nb(running) 0
	    }
	}
    }
}

proc TC_nbends { } {
    
}
