#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11 Project
#

proc vTclWindow.seqlist {base} {
    global Name;
    global sl;
    global glob;
    global system
    global ptupel_name;
    global ptupel_number;
    if {$base == ""} {
        set base .seqlist
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -background #A500A500A500 
   # wm focusmodel $base passive
     if {$system(screen)=="big"} {
	 wm geometry $base 529x490
	 wm maxsize $base 1267 977
     } else {
	 wm geometry $base 400x400
     }

    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "List of Sequences"

    set Name(sort) "sort"
    set Name(by_objective) "by_objective"
    set Name(by_property) "by_property"
    set sl(row) -1
    set sl(column) -1
    frame $base.fra19 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    menubutton $base.fra19.men20 \
	-underline 0\
	-menu .seqlist.fra19.men20.m -padx 5 -pady 4 \
        -relief raised -text $Name(Display) 
    menu $base.fra19.men20.m \
	-cursor {} \
	-tearoff 0 
 #    $base.fra19.men20.m add checkbutton \
#         -label "Objective" -variable sl(objective)
#     $base.fra19.men20.m add checkbutton \
#         -label "MaxRank" -variable sl(MaxRank)
#     $base.fra19.men20.m add checkbutton \
#         -label "NoSources" -variable sl(NoSources)
#     $base.fra19.men20.m add checkbutton \
#         -label "NoSinks" -variable sl(NoSinks)
#     $base.fra19.men20.m add separator
    $base.fra19.men20.m add command \
        -label  $Name(Close_Window) -command {schedulel_ende}
    menubutton $base.fra19.men21 \
	-menu .seqlist.fra19.men21.m -padx 5 -pady 4 \
        -relief raised -text $Name(sort) -underline 0
    menu $base.fra19.men21.m \
	-cursor {} \
	-tearoff 0 
  #   $base.fra19.men21.m add command \
# 	-command {TC_sort_sl Number} -label "by number"
#     $base.fra19.men21.m add command \
# 	-command {TC_sort_sl NoSinks} -label "by number of sinks"
#     $base.fra19.men21.m add command \
# 	-command {TC_sort_sl NoSourcess} -label "by number of sources"
    $base.fra19.men21.m add cascade -menu  $base.fra19.men21.m.by_objective \
	-label "$Name(by_objective)"
    set submen [menu  $base.fra19.men21.m.by_objective -tearoff 0]
    set gamma $ptupel_name([expr $ptupel_number(alpha) +$ptupel_number(beta)])
    for {set i 0} {$i<=$ptupel_number($gamma,MAX)} {incr i 1} {
	set myname ""
	catch [set myname $Name($gamma,$ptupel_name($gamma,$i))]
	if { $myname!="" } {
	    $submen add  command -command "TC_sort_sl $i"   -label "$myname"
	}
    }
    $base.fra19.men21.m add cascade -menu  $base.fra19.men21.m.by_property \
	-label "$Name(by_property)"
    set submen [menu  $base.fra19.men21.m.by_property -tearoff 0]
    for {set i 0} {$i<=14} {incr i 1} {
	$submen add  command -command "TC_sort_sl $i"   -label "$i"
    }

   menubutton $base.fra19.men22 \
 	-menu .seqlist.fra19.men22.m -padx 5 -pady 4 \
         -relief raised -text "Exclude" -underline 0
     menu $base.fra19.men22.m \
 	-cursor {} \
 	-tearoff 0 
  $base.fra19.men22.m add command \
 	-command {TC_exclude clear_list } -label "... all but one"
#     $base.fra19.men22.m add command \
\# 	-command {TC_exclude_sl similiar} -label "... similiar"
#     $base.fra19.men22.m add command \
# 	-command {TC_exclude_sl reducible} -label "... reducible"
#    $base.fra19.men22.m add command \
# 	-command {TC_exclude_sl nonoptimal} -label "... not optimal"

    frame $base.fra22 \
	-borderwidth 2 -height 75 -width 125 

    frame $base.fra22.fra28 \
	-borderwidth 2 -height 50 -width 125 
    frame $base.fra22.fra28.fra35 \
        -borderwidth 2 -height 40 -width 40 
    frame $base.fra22.fra28.fra36 \
	-borderwidth 2 -height 50 -width 125 
       frame $base.fra22.fra28.fra36.fillfr1 \
	-borderwidth 2 -width 13
    canvas $base.fra22.fra28.fra36.can37 \
        -borderwidth 2 -height 35 \
        -width 374 
    frame $base.fra22.fra32 \
	-borderwidth 2 -height 75 -width 125 
    frame $base.fra22.fra32.fra33 \
	-borderwidth 2 -height 75 -width 50 
      frame $base.fra22.fra32.fra33.fillfr2 \
	-borderwidth 2 -height 12
    canvas $base.fra22.fra32.fra33.can38 \
        -borderwidth 2 -height 262 \
        -width 35 
    frame $base.fra22.fra32.cpd34 \
         -borderwidth 1 -height 30 -width 30 
    scrollbar $base.fra22.fra32.cpd34.01 \
	-borderwidth 1 \
        -command {schedlw_sch } \
	-orient horiz -width 10 
    scrollbar $base.fra22.fra32.cpd34.02 \
	-borderwidth 1 \
        -command {schedlw_scv } \
	-orient vert -width 10 
    canvas $base.fra22.fra32.cpd34.03 \
        -background white -borderwidth 2 -height 100 -width 100 \
        -xscrollcommand {.seqlist.fra22.fra32.cpd34.01 set} \
        -yscrollcommand {.seqlist.fra22.fra32.cpd34.02 set} 
    frame $base.fra23 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra23.lab24 \
	-borderwidth 1 \
	-text "   " 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .seqlist -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.men20 \
        -in .seqlist.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.men21 \
        -in .seqlist.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.men22 \
        -in .seqlist.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra23 \
        -in .seqlist -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra23.lab24 \
        -in .seqlist.fra23 -anchor center -expand 0 -fill none -side top 
    pack $base.fra22 \
        -in .seqlist -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra28 \
        -in .seqlist.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28.fra35 \
        -in .seqlist.fra22.fra28 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.fra28.fra36 \
        -in .seqlist.fra22.fra28 -anchor center -expand 0 -fill x -side top 
     pack $base.fra22.fra28.fra36.fillfr1 \
	-in .seqlist.fra22.fra28.fra36 -anchor e -expand 0 -side right
    pack $base.fra22.fra28.fra36.can37 \
        -in .seqlist.fra22.fra28.fra36 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra22.fra32 \
        -in .seqlist.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra32.fra33 \
        -in .seqlist.fra22.fra32 -anchor center -expand 0 -fill y -side left 
    pack $base.fra22.fra32.fra33.fillfr2 \
	-in .seqlist.fra22.fra32.fra33 -anchor s -expand 0 -side bottom
    pack $base.fra22.fra32.fra33.can38 \
        -in .seqlist.fra22.fra32.fra33 -anchor center -expand 1 -fill y \
        -side top 
    pack $base.fra22.fra32.cpd34 \
        -in .seqlist.fra22.fra32 -anchor center -expand 1 -fill both -side top 
    grid columnconf $base.fra22.fra32.cpd34 0 -weight 1
    grid rowconf $base.fra22.fra32.cpd34 0 -weight 1
    grid $base.fra22.fra32.cpd34.01 \
        -in .seqlist.fra22.fra32.cpd34 -column 0 -row 1 -columnspan 1 \
        -rowspan 1 -sticky ew 
    grid $base.fra22.fra32.cpd34.02 \
        -in .seqlist.fra22.fra32.cpd34 -column 1 -row 0 -columnspan 1 \
        -rowspan 1 -sticky ns 
    grid $base.fra22.fra32.cpd34.03 \
        -in .seqlist.fra22.fra32.cpd34 -column 0 -row 0 -columnspan 1 \
        -rowspan 1 -sticky nesw 
    bind $base.fra22.fra32.cpd34.03 <Button-1> { 
	    editsl %x %y 
    }

    # set some menue entrys active
    bind $base <Destroy> { set mw(fSequencelist) "0" 
    }
    bind $base <Visibility> {
	bind .seqlist  <Visibility> { }
	    TC_draw_schedule_list
	}

} 

proc schedulel_ende {} {
    destroy .seqlist
}

proc schedlw_scv { args } {
    eval .seqlist.fra22.fra32.fra33.can38 yview $args
    eval .seqlist.fra22.fra32.cpd34.03 yview $args
}

proc schedlw_sch { args } {
    eval .seqlist.fra22.fra32.cpd34.03 xview $args
    eval .seqlist.fra22.fra28.fra36.can37  xview $args
}

proc editsl { x y } {
    global sl
    set sl(row) [TC_getvar schedulelist_row \
		     [.seqlist.fra22.fra32.cpd34.03 canvasy $y] ]
    
    TC_choose_sequence $sl(row)
}









