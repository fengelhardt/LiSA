#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 
global gantt;

set  gantt(red) 0
set  gantt(green) 0
set  gantt(blue) 0
set  gantt(brown) 0
set  gantt(yellow) 0
set gantt(orientation) machine

proc vTclWindow.gantt {base} {
    global Name;
    global gantt;
    global system

    if {$base == ""} {
        set base .gantt
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -background #75009500A900 
    wm focusmodel $base passive
    wm geometry $base 601x435
 
    if {$system(screen)=="big"} {
	wm geometry $base 601x435
    } else {
	wm geometry $base 401x415
    }
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "$Name(Gantt_title)"
    wm transient $base .lisa
#    wm focusmodel  $base passive

    frame $base.fra70 \
         -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
         -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
         -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
	-padx 11 -pady 4 -text $Name(OK) \
	-command {destroy .gantt; TC_setvar gantt}
    frame $base.fra70.fra91 \
         -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
         -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
	-padx 11 -pady 4 -text $Name(Cancel) \
	-command {destroy .gantt}
    frame $base.fra17 \
         -borderwidth 2 -height 20 -width 125 
    frame $base.fra18 \
         -borderwidth 2 -height 75 -width 20 
    frame $base.fra19 \
         -borderwidth 2 -height 75 -width 20 
    frame $base.fra20 \
         -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra17 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra17.lab37 \
         -borderwidth 1 \
	-text $Name(Orientation:) 
    frame $base.fra20.fra17.fra39 \
	-borderwidth 2 -height 10 -width 10 
    if {$gantt(orientation)=="machine"} {
	menubutton $base.fra20.fra17.men40 \
	    -indicatoron 1 \
	    -menu .gantt.fra20.fra17.men40.01 -padx 5 -pady 4 -relief raised \
	    -text $Name(Machine_orient) }  else {
		menubutton $base.fra20.fra17.men40 \
		    -indicatoron 1 \
		    -menu .gantt.fra20.fra17.men40.01 -padx 5 -pady 4 -relief raised \
		    -text $Name(Job_orient) }
    
    menu $base.fra20.fra17.men40.01 \
	-cursor {} \
	-tearoff 0 
    $base.fra20.fra17.men40.01 add command \
        -label $Name(Machine_orient) \
	-command {set_machine_orientation; set gantt(orientation) machine}
    $base.fra20.fra17.men40.01 add command \
        -label $Name(Job_orient) \
	-command {set_job_orientation; set gantt(orientation) job}
    frame $base.fra20.fra18 \
         -borderwidth 2 -height 25 -width 15 
    frame $base.fra20.fra19 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra19.lab42 \
         -borderwidth 1 \
	-text $Name(Special_Emphasis:) 
    frame $base.fra20.fra20 \
         -borderwidth 2 -height 75 -width 20 
    frame $base.fra20.fra53 \
         -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra53.fra55 \
         -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra53.fra55.rad60 \
          -value "None" -variable gantt(special)\
	-highlightthickness 0 
    frame $base.fra20.fra53.fra55.fra123 \
         -borderwidth 2 -height 10 -width 6 
    label $base.fra20.fra53.fra55.lab124 \
         -borderwidth 1 \
	-text $Name(None) 
    frame $base.fra20.fra53.fra56 \
         -borderwidth 2 -height 14 -width 10 
    frame $base.fra20.fra53.fra59 \
         -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra53.fra59.rad61 \
          -value "critical_path" -variable gantt(special)\
	-highlightthickness 0 
    frame $base.fra20.fra53.fra59.fra125 \
         -borderwidth 2 -height 10 -width 6 
    label $base.fra20.fra53.fra59.lab126 \
         -borderwidth 1 \
	-text $Name(Critical_path) 
    frame $base.fra20.fra53.fra120 \
         -borderwidth 2 -height 10 -width 125 
    frame $base.fra20.fra54 \
         -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra54.fra57 \
         -borderwidth 2 -height 75 -width 20 
    radiobutton $base.fra20.fra54.fra57.rad116 \
          -value "colors" -variable gantt(special)\
	-highlightthickness 0 
    frame $base.fra20.fra54.fra17 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra54.fra17.lab21 \
         -borderwidth 1 \
	-text $Name(Assign_colors_to_some) 
    frame $base.fra20.fra54.fra17.fra32 \
         -borderwidth 2 -height 10 -width 6 
    label $base.fra20.fra54.fra17.lab33 \
         -borderwidth 1 \
	-text $Name(machines) 
    frame $base.fra20.fra54.fra17.fra34 \
         -borderwidth 2 -height 10 -width 6 
    label $base.fra20.fra54.fra17.lab35 \
         -borderwidth 1 \
	-text " " 
    frame $base.fra20.fra54.fra27 \
         -borderwidth 2 -height 10 -width 10 
    frame $base.fra20.fra54.fra53 \
         -borderwidth 2 -height 75 -width 10 
    frame $base.fra20.fra54.fra54 \
         -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra54.fra54.01 \
         -borderwidth 2 -height 74 -width 273 
    frame $base.fra20.fra54.fra54.01.02 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra54.fra54.01.02.03 \
         -borderwidth 1 \
	-text $Name(red) 
    frame $base.fra20.fra54.fra54.01.05 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra54.fra54.01.05.06 \
         -borderwidth 1 \
	-text $Name(green) 
    frame $base.fra20.fra54.fra54.01.08 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra54.fra54.01.08.09 \
         -borderwidth 1 \
	-text $Name(blue) 
    frame $base.fra20.fra54.fra54.01.011 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra54.fra54.01.011.012 \
         -borderwidth 1 \
	-text $Name(brown) 
    frame $base.fra20.fra54.fra54.01.014 \
         -borderwidth 2 -height 74 -width 125 
    label $base.fra20.fra54.fra54.01.014.015 \
         -borderwidth 1 \
	-text $Name(yellow) 
    frame $base.fra20.fra54.fra54.016 \
         -borderwidth 2 -height 75 -width 20 
    frame $base.fra20.fra54.fra54.017 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    entry $base.fra20.fra54.fra54.017.ent17 \
	-width 8 -textvar gantt(red)
    entry $base.fra20.fra54.fra54.017.ent18 \
         \
         \
         -width 8 -textvar gantt(green)
    entry $base.fra20.fra54.fra54.017.ent19 \
         \
         \
         -width 8 -textvar gantt(blue)
    entry $base.fra20.fra54.fra54.017.ent20 \
         \
         \
         -width 8 -textvar gantt(brown)
    entry $base.fra20.fra54.fra54.017.ent23 \
         \
         \
         -width 8 -textvar gantt(yellow)
    frame $base.fra20.fra54.fra55 \
         -borderwidth 2 -height 75 -width 10 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .gantt -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .gantt.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .gantt.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .gantt.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .gantt.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .gantt.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .gantt.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .gantt -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .gantt -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .gantt -anchor center -expand 0 -fill y -side right 
    pack $base.fra20 \
        -in .gantt -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.fra17 \
        -in .gantt.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra17.lab37 \
        -in .gantt.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra17.fra39 \
        -in .gantt.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra17.men40 \
        -in .gantt.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra18 \
        -in .gantt.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra19 \
        -in .gantt.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra19.lab42 \
        -in .gantt.fra20.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra20 \
        -in .gantt.fra20 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra53 \
        -in .gantt.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra53.fra55 \
        -in .gantt.fra20.fra53 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra53.fra55.rad60 \
        -in .gantt.fra20.fra53.fra55 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra55.fra123 \
        -in .gantt.fra20.fra53.fra55 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra55.lab124 \
        -in .gantt.fra20.fra53.fra55 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra56 \
        -in .gantt.fra20.fra53 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra53.fra59 \
        -in .gantt.fra20.fra53 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra53.fra59.rad61 \
        -in .gantt.fra20.fra53.fra59 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra59.fra125 \
        -in .gantt.fra20.fra53.fra59 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra59.lab126 \
        -in .gantt.fra20.fra53.fra59 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra53.fra120 \
        -in .gantt.fra20.fra53 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra54 \
        -in .gantt.fra20 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra20.fra54.fra57 \
        -in .gantt.fra20.fra54 -anchor center -expand 0 -fill y -side left 
    pack $base.fra20.fra54.fra57.rad116 \
        -in .gantt.fra20.fra54.fra57 -anchor ne -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra17 \
        -in .gantt.fra20.fra54 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra20.fra54.fra17.lab21 \
        -in .gantt.fra20.fra54.fra17 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra17.fra32 \
        -in .gantt.fra20.fra54.fra17 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra17.lab33 \
        -in .gantt.fra20.fra54.fra17 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra17.fra34 \
        -in .gantt.fra20.fra54.fra17 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra17.lab35 \
        -in .gantt.fra20.fra54.fra17 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra27 \
        -in .gantt.fra20.fra54 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra54.fra53 \
        -in .gantt.fra20.fra54 -anchor center -expand 1 -fill x -side left 
    pack $base.fra20.fra54.fra54 \
        -in .gantt.fra20.fra54 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra54.fra54.01 \
        -in .gantt.fra20.fra54.fra54 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra54.01.02 \
        -in .gantt.fra20.fra54.fra54.01 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra20.fra54.fra54.01.02.03 \
        -in .gantt.fra20.fra54.fra54.01.02 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra54.fra54.01.05 \
        -in .gantt.fra20.fra54.fra54.01 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra20.fra54.fra54.01.05.06 \
        -in .gantt.fra20.fra54.fra54.01.05 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra54.fra54.01.08 \
        -in .gantt.fra20.fra54.fra54.01 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra20.fra54.fra54.01.08.09 \
        -in .gantt.fra20.fra54.fra54.01.08 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra54.fra54.01.011 \
        -in .gantt.fra20.fra54.fra54.01 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra20.fra54.fra54.01.011.012 \
        -in .gantt.fra20.fra54.fra54.01.011 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra54.fra54.01.014 \
        -in .gantt.fra20.fra54.fra54.01 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra20.fra54.fra54.01.014.015 \
        -in .gantt.fra20.fra54.fra54.01.014 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra54.fra54.016 \
        -in .gantt.fra20.fra54.fra54 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra54.fra54.017 \
        -in .gantt.fra20.fra54.fra54 -anchor nw -expand 1 -fill none \
        -side left 
    pack $base.fra20.fra54.fra54.017.ent17 \
        -in .gantt.fra20.fra54.fra54.017 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra54.fra54.017.ent18 \
        -in .gantt.fra20.fra54.fra54.017 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra54.fra54.017.ent19 \
        -in .gantt.fra20.fra54.fra54.017 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra54.fra54.017.ent20 \
        -in .gantt.fra20.fra54.fra54.017 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra54.fra54.017.ent23 \
        -in .gantt.fra20.fra54.fra54.017 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra54.fra55 \
        -in .gantt.fra20.fra54 -anchor center -expand 1 -fill x -side left 

    bind $base <Visibility> {  grab set .gantt }
    if {$gantt(orientation)=="machine"} { 
	set_machine_orientation } else {
	    set_job_orientation }
}

proc set_job_orientation { } {
    global Name;
    .gantt.fra20.fra17.men40 config -text $Name(Job_orient)
    .gantt.fra20.fra54.fra17.lab33 config -text $Name(machines)
}
 
proc set_machine_orientation { } {
    global Name;
    .gantt.fra20.fra17.men40 config -text  $Name(Machine_orient)
 .gantt.fra20.fra54.fra17.lab33 config -text $Name(jobs)
}













