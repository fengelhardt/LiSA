#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11 Project
#

proc vTclWindow.schedulewin {base} {
    global Name;
    global schedule;
    global glob;
    global system
    if {$base == ""} {
        set base .schedulewin
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
	 wm geometry $base 420x420
     } else {
	 wm geometry $base 360x320
     }

    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Solution)
 
    set edit_flag 0
    set schedule(edit) 0
    set schedule(entry)  $Name(nichts_ausgewaehlt)
    set  schedule(Matrix) LR
    set schedule(semiactive)  [TC_getvar semiactive_flag]
    set schedule(config) "0"
    set schedule(row) -1
    set schedule(column) -1
    frame $base.fra19 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    menubutton $base.fra19.men20 \
	-underline 0\
	-menu .schedulewin.fra19.men20.m -padx 5 -pady 4 \
        -text $Name(Display) 
    menu $base.fra19.men20.m \
	-cursor {} \
	-tearoff 0 
  #   $base.fra19.men20.m add radiobutton \
#         -label $Name(Machine_Order) -value MO -variable schedule(Matrix) -command { TC_draw_schedule }  -state disabled
#     $base.fra19.men20.m add radiobutton \
#         -label $Name(Job_Order) -value JO \
# 	-variable schedule(Matrix) -command { TC_draw_schedule } -state disabled
    $base.fra19.men20.m add radiobutton \
        -label $Name(Latin_Rectangle) -value LR \
	-variable schedule(Matrix) -command { 
	    set schedule(row) 0
	    set schedule(column) 0
	    edit_schedule_entry
	    LR_is_displayed
	    TC_draw_schedule }
    $base.fra19.men20.m add radiobutton \
        -label $Name(Schedule) -value SCHEDULE \
	-variable schedule(Matrix) -command {
	    set schedule(row) 0
	    set schedule(column) 0
	    edit_schedule_entry
	    SCHEDULE_is_displayed
	    TC_draw_schedule }
    $base.fra19.men20.m add separator
    $base.fra19.men20.m add command \
        -label  $Name(Close_Window) -command {schedule_ende}
    menubutton $base.fra19.men21 \
	-menu .schedulewin.fra19.men21.m -padx 5 -pady 4 \
        -text $Name(Actions) -underline 0
    menu $base.fra19.men21.m \
	-cursor {} \
	-tearoff 0 
    $base.fra19.men21.m add checkbutton \
        -label $Name(Semiactive_mark) -variable schedule(semiactive) \
	-command { change_semiactive }
    $base.fra19.men21.m add separator
    $base.fra19.men21.m add command -command {TC_prior RAND} \
        -label $Name(Generate_Sequence)
  #   $base.fra19.men21.m add command \
#         -label $Name(Irreducible_Test)  -state disabled
    $base.fra19.men21.m add command -command { TC_rev_seq } \
        -label $Name(Revers) 
  $base.fra19.men21.m add command -command { TC_update_seq } \
        -label $Name(Update_Sequence) 
 #  $base.fra19.men21.m add command -command { 
#        if {[winfo exists .man_oper]} {
# 	   man_oper_update} else {
# 	       Window show .man_oper } } \
#         -label "modify"
    button $base.fra19.but22 -text "$Name(Adopt)" -pady 3 -command adopt_seq
    button $base.fra19.but23 -text "$Name(Help)" -pady 3 \
	-command { lisa_help schedule.htm }
    frame $base.fra22 \
	-borderwidth 2 -width 125 
    frame $base.fra22.fra25 \
	-borderwidth 2 -height 75 -width 125 
    label $base.fra22.fra25.lab26 \
        -borderwidth 1 -text  $Name(Entry:) 
    entry $base.fra22.fra25.ent27 \
	-textvar schedule(entry)
    bind $base.fra22.fra25.ent27 <Key> {set schedule(edit) 1}
    bind $base.fra22.fra25.ent27 <Tab> {
	set schedule(config) "1"
	event generate .schedulewin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar schedule_identifier $schedule(row) [expr $schedule(column)+1]]!=""} {
	    set schedule(column) [expr $schedule(column)+1]
	}
	edit_schedule_entry
    }
    bind $base.fra22.fra25.ent27 <Down> {
	set schedule(config) "1"
	event generate .schedulewin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar schedule_identifier [expr $schedule(row)+1] $schedule(column)]!=""} {
	    set schedule(row) [expr $schedule(row)+1]
	}
	edit_schedule_entry }
    bind $base.fra22.fra25.ent27 <Up> {
	set schedule(config) "1"
	event generate .schedulewin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar schedule_identifier [expr $schedule(row)-1] $schedule(column)]!=""} {
	    set schedule(row) [expr $schedule(row)-1]
	}
	edit_schedule_entry }
    bind $base.fra22.fra25.ent27 <Control-Right> {
	set schedule(config) "1"
	event generate .schedulewin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar schedule_identifier $schedule(row) [expr $schedule(column)+1]]!=""} {
	    set schedule(column) [expr $schedule(column)+1]
	}
	edit_schedule_entry
    }
    bind $base.fra22.fra25.ent27 <Control-Left> {
 	set schedule(config) "1"
	event generate .schedulewin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar schedule_identifier $schedule(row) [expr $schedule(column)-1]]!=""} {
	    set schedule(column) [expr $schedule(column)-1]
	}
	edit_schedule_entry
    }
    bind $base.fra22.fra25.ent27 <Return> {
	if {$edit_flag!=0} {
	    set schedule(entry) [positive_value $schedule(entry)]
	    schedule_set_entry
	    if {$schedule(config)=="0"} {
		if {[TC_getvar schedule_identifier $schedule(row) [expr $schedule(column)+1]]!=""} {
		    set schedule(column) [expr $schedule(column)+1]
		} else {
		    if {[TC_getvar schedule_identifier [expr $schedule(row)+1] 1]!=""} {
			set schedule(column) 0
			set schedule(row) [expr $schedule(row)+1]
		    }
		}
		edit_schedule_entry
	    }  
	}
	set schedule(config) "0"
    }
    frame $base.fra22.fra28 \
	-borderwidth 2 -height 50 -width 125 
    frame $base.fra22.fra28.fra35 \
        -borderwidth 2 -height 49 -width 44
    frame $base.fra22.fra28.fra36 \
	-borderwidth 2 -height 50
    frame $base.fra22.fra28.fra36.fillfr1 \
	-borderwidth 2 -width 13
    canvas $base.fra22.fra28.fra36.can37 \
        -borderwidth 2 -height 35
    frame $base.fra22.fra32 \
	-borderwidth 2 -height 75 -width 125 
    frame $base.fra22.fra32.fra33 \
	-borderwidth 2 
    frame $base.fra22.fra32.fra33.fillfr2 \
	-borderwidth 2 -height 12
    canvas $base.fra22.fra32.fra33.can38 \
        -borderwidth 2 \
        -width 35 
    frame $base.fra22.fra32.cpd34 \
         -borderwidth 1 -height 30 -width 30 
    scrollbar $base.fra22.fra32.cpd34.01 \
	-borderwidth 1 \
        -command {schedw_sch } \
	-orient horiz -width 10 
    scrollbar $base.fra22.fra32.cpd34.02 \
	-borderwidth 1 \
        -command {schedw_scv } \
	-orient vert -width 10 
    canvas $base.fra22.fra32.cpd34.03 \
        -background white -borderwidth 2 \
        -xscrollcommand {.schedulewin.fra22.fra32.cpd34.01 set} \
        -yscrollcommand {.schedulewin.fra22.fra32.cpd34.02 set}
    frame $base.fra23 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra23.lab24 \
	-borderwidth 1 \
	-text label 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .schedulewin -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.men20 \
        -in .schedulewin.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.men21 \
        -in .schedulewin.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.but23 \
 	-in .schedulewin.fra19 -anchor center -expand 0 -fill none -side right 
     pack $base.fra19.but22 \
 	-in .schedulewin.fra19 -anchor center -expand 0 -fill none -side right 
    pack $base.fra23 \
        -in .schedulewin -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra23.lab24 \
        -in .schedulewin.fra23 -anchor center -expand 0 -fill none -side bottom
    pack $base.fra22 \
        -in .schedulewin -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra25 \
        -in .schedulewin.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra25.lab26 \
        -in .schedulewin.fra22.fra25 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.fra25.ent27 \
        -in .schedulewin.fra22.fra25 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28 \
        -in .schedulewin.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28.fra35 \
        -in .schedulewin.fra22.fra28 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.fra28.fra36 \
        -in .schedulewin.fra22.fra28 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28.fra36.fillfr1 \
	-in .schedulewin.fra22.fra28.fra36 -anchor e -expand 0 -side right
    pack $base.fra22.fra28.fra36.can37 \
        -in .schedulewin.fra22.fra28.fra36 -anchor center -expand 0 -fill x \
        -side top
    pack $base.fra22.fra32 \
        -in .schedulewin.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra32.fra33 \
        -in .schedulewin.fra22.fra32 -anchor center -expand 0 -fill y -side left
    pack $base.fra22.fra32.fra33.fillfr2 \
	-in .schedulewin.fra22.fra32.fra33 -anchor s -expand 0 -side bottom
    pack $base.fra22.fra32.fra33.can38 \
        -in .schedulewin.fra22.fra32.fra33 -anchor center -expand 1 -fill y \
        -side top 
    pack $base.fra22.fra32.cpd34 \
        -in .schedulewin.fra22.fra32 -anchor center -expand 1 -fill both -side top 
    grid columnconf $base.fra22.fra32.cpd34 0 -weight 1
    grid rowconf $base.fra22.fra32.cpd34 0 -weight 1
    grid $base.fra22.fra32.cpd34.01 \
        -in .schedulewin.fra22.fra32.cpd34 -column 0 -row 1 -columnspan 1 \
        -rowspan 1 -sticky ew 
    grid $base.fra22.fra32.cpd34.02 \
        -in .schedulewin.fra22.fra32.cpd34 -column 1 -row 0 -columnspan 1 \
        -rowspan 1 -sticky ns 
    grid $base.fra22.fra32.cpd34.03 \
        -in .schedulewin.fra22.fra32.cpd34 -column 0 -row 0 -columnspan 1 \
        -rowspan 1 -sticky nesw 
  #   pack $base.fra23 \
#         -in .schedulewin -anchor center -expand 0 -fill x -side bottom 
#     pack $base.fra23.lab24 \
#         -in .schedulewin.fra23 -anchor center -expand 0 -fill none -side bottom
    bind $base.fra22.fra32.cpd34.03 <Button-1> { 
	if { ($schedule(Matrix)=="LR" && $schedule(semiactive)=="1" ) || ($schedule(Matrix)=="SCHEDULE" && $schedule(semiactive)=="0" ) } {
	    set edit_flag 1
	    edits %x %y 
	    set dat(config) "0"
	    focus  .schedulewin.fra22.fra25.ent27
	    .schedulewin.fra22.fra25.ent27 selection range 0 end
	} else {
	    set schedule(entry) $Name(Edit_not_possible)
	    set edit_flag 0
	}
    }
  bind $base.fra22.fra32.cpd34.03 <Double-Button-1> { 
      if {$schedule(Matrix)=="LR" && $schedule(semiactive)=="1" } {
	    edits %x %y 
	   #  set dat(config) "0"
# 	    focus  .schedulewin.fra22.fra25.ent27
# 	    .schedulewin.fra22.fra25.ent27 selection range 0 end
	  if {[winfo exists .man_oper]} {
	      man_oper_update} else {
		  Window show .man_oper } 
	} else {
	    set schedule(entry) $Name(modify_not_possible)
	}
    }


#    TC_draw_schedule
    # set some menue entrys active
    # schedule_exist 
    bind $base <Destroy> { set mw(fSequence) "0" 
	if { $schedule(edit)=="1" } { adopt_seq }
    }
    bind $base <Visibility> {
	bind .schedulewin  <Visibility> { }
	if {$schedule(semiactive)=="1"} {
	    # the following because mark menu entrys must be disabled
	    is_not_semiactive
	    set schedule(Matrix) "LR"
	    is_semiactive
	    TC_draw_schedule
	} else {
	    set schedule(Matrix) "SCHEDULE"
	    is_not_semiactive  
	}
    }
}

proc schedule_ende {} {
    global hw;
    destroy .schedulewin
    set hw(fSequence) 0
}

proc schedw_scv { args } {
    eval .schedulewin.fra22.fra32.fra33.can38 yview $args
    eval .schedulewin.fra22.fra32.cpd34.03 yview $args
}

proc schedw_sch { args } {
    eval .schedulewin.fra22.fra32.cpd34.03 xview $args
    eval .schedulewin.fra22.fra28.fra36.can37  xview $args
}

proc edits { x y } {
    global schedule
    TC_draw_schedule
    set schedule(row) [TC_getvar schedule_row \
			     [.schedulewin.fra22.fra32.cpd34.03 canvasy $y] ]
    set schedule(column) [TC_getvar schedule_column \
			      [.schedulewin.fra22.fra32.cpd34.03 canvasx $x ] ]
    edit_schedule_entry
}

proc edit_schedule_entry { } {
    global schedule
if { $schedule(Matrix)!="SCHEDULE" || [TC_getvar VALID_SCHEDULE]==1 } {
	set bezeichner [TC_getvar schedule_identifier $schedule(row) $schedule(column)]
	if  { $bezeichner != ""} {
	    .schedulewin.fra22.fra25.lab26 config -text \
		"$bezeichner ([expr $schedule(row)+1],[expr $schedule(column)+1])" 
	    set schedule(entry) [TC_getvar schedule_entry $schedule(row) $schedule(column)]
	    TC_mark_schedule_entry $schedule(row) $schedule(column)
	    if { $schedule(column)>5 } {
		set position [expr [expr $schedule(column)- 5.0]/[TC_getvar get_m]]
		.schedulewin.fra22.fra32.cpd34.03 xview moveto $position
		.schedulewin.fra22.fra28.fra36.can37 xview moveto $position
	    } else {
		.schedulewin.fra22.fra32.cpd34.03 xview moveto 0
		.schedulewin.fra22.fra28.fra36.can37 xview moveto 0
	    }
	    if { $schedule(row)>5 } {
		set position [expr [expr $schedule(row)- 5.0]/[TC_getvar get_n]]
		.schedulewin.fra22.fra32.cpd34.03 yview moveto $position
		.schedulewin.fra22.fra32.fra33.can38  yview moveto $position
	    } else {
		.schedulewin.fra22.fra32.cpd34.03 yview moveto 0
		.schedulewin.fra22.fra32.fra33.can38  yview moveto 0
	    }	
	} else {
	    .schedulewin.fra22.fra25.lab26 config -text ""
	    set schedule(entry) ""
	}
	.schedulewin.fra22.fra25.ent27 selection range 0 end
    }
}

proc  schedule_set_entry { } {
    global schedule
    if { $schedule(Matrix) == "MO" } {
	#TC_setvar "MO" $schedule(row) $schedule(column) $schedule(entry)
    }
    if { $schedule(Matrix) == "JO" } {
	#TC_setvar "JO" $schedule(row) $schedule(column) $schedule(entry)
    }
    
    if {$schedule(Matrix) == "LR"} {
	TC_setvar "LR" $schedule(row) $schedule(column) $schedule(entry)
    }
    if {$schedule(Matrix) == "SCHEDULE"} {
	TC_setvar "Schedule" $schedule(row) $schedule(column) $schedule(entry)
    }
    if {  $schedule(Matrix) != "" } {
	TC_draw_schedule_entry $schedule(row) $schedule(column) $schedule(entry)
    }
}

proc adopt_seq { } {
    global schedule
    set schedule(edit) 0
    if {$schedule(semiactive)=="1"} {
	TC_update_seq; 
	#TC_draw_schedule
	#TC_draw_output
    } else {
	TC_update_cij
	#TC_draw_schedule
	#TC_draw_output
    }
}

proc change_semiactive { } {
    global schedule
    global Name
    TC_setvar semiactive_flag $schedule(semiactive) 
    if { $schedule(semiactive)=="0"} {	
	is_not_semiactive
    } else {
	is_semiactive
    }
}

proc is_semiactive { } {
    global Name
    global schedule
    set schedule(Matrix) "LR"
  #   .schedulewin.fra19.men20.m  entryconfigure \
# 	$Name(Latin_Rectangle) -state active
#     .schedulewin.fra19.men20.m  entryconfigure \
# 	$Name(Schedule) -state disabled
    LR_is_displayed
    TC_draw_schedule
}

proc is_not_semiactive { } {
    global Name
    global schedule
    # puts "is not semiactive"
 #    .schedulewin.fra19.men20.m  entryconfigure \
# 	$Name(Latin_Rectangle) -state disabled
#     .schedulewin.fra19.men20.m  entryconfigure \
# 	$Name(Schedule) -state active
    set schedule(Matrix) SCHEDULE
    SCHEDULE_is_displayed
}

proc LR_is_displayed { } {
    global Name
    global schedule
    .schedulewin.fra19.men20.m  entryconfigure \
	$Name(Latin_Rectangle) -state active
    .schedulewin.fra19.men21.m  entryconfigure \
	$Name(Generate_Sequence) -state active
    if { [TC_getvar M_ENV]!="F" && [TC_getvar M_ENV]!="J" } {
	.schedulewin.fra19.men21.m  entryconfigure \
	    $Name(Revers) -state active
    }
    .schedulewin.fra19.men21.m  entryconfigure \
	$Name(Update_Sequence) -state active   
    .schedulewin.fra19.but22 configure -state active
  if { $schedule(semiactive)=="0"} {	
      .schedulewin.fra19.men21.m  entryconfigure \
	  $Name(Generate_Sequence) -state disabled
      .schedulewin.fra19.men21.m  entryconfigure \
	  $Name(Revers) -state disabled
      .schedulewin.fra19.men21.m  entryconfigure \
	  $Name(Update_Sequence) -state disabled
      .schedulewin.fra19.but22 configure -state disabled
  }

}

proc SCHEDULE_is_displayed { } {
    global Name
    .schedulewin.fra19.men21.m  entryconfigure \
	$Name(Generate_Sequence) -state disabled
    .schedulewin.fra19.men21.m  entryconfigure \
	$Name(Revers) -state disabled
    .schedulewin.fra19.men21.m  entryconfigure \
	$Name(Update_Sequence) -state disabled
    .schedulewin.fra19.but22 configure -state disabled
    #TC_draw_schedule
}













