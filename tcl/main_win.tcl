#!/usr/bin/wish
#############################################################################
# 
# Attempt to write the main window without too much unnecessary code
#
######## Generate Icon: ####################

proc make_lisa_icon { iconfile } {
    if {$iconfile==""} { set iconfile "$env(LISAHOME)/img/llogo_s.gif" }

    set ac(xpm.ApaControl) [image create photo -file $iconfile]
    
    set icon ".__Icon"
    toplevel $icon
    label $icon.pm -image $ac(xpm.ApaControl); pack $icon.pm
}

######################################################################
######## create root of the window hierarchy #########################
######################################################################

proc vTclWindow. {base} {
    if {$base == ""} {
        set base .
    }
    wm withdraw $base
    wm title $base "LiSA"
}

#######################################################################
######## create the LiSA main window ##################################
#######################################################################

proc vTclWindow.lisa {base} {
    global Name;
    global lsa_status;
    global mw;
    global tupel;
    global dat;
    global schedule;
    global flags;
    global open;
    global env;
    global system;

    set mw(xpos1) 1
    set mw(ypos1) 1
    set flags(mwin) 1
    if {$base == ""} {
        set base .lisa
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }

# set some defaults in the main window variable:
    set mw(zoom) 0
    set mw(mwout) {Gantt Diagram}    
# you have to use $Name() here!
    set mw(filetypes) {
	{{LiSA Files}       {.lsa}        }  
	{{All Files}        *             }
    }
# define appearance of the LiSA main window:
    toplevel $base -class Toplevel
    if {$system(screen)=="big"} {
	wm geometry $base 735x620+107+70
    } else { 
	wm geometry $base 620x500+50+10
    }
    wm maxsize $base 1267 977

    set w [winfo reqwidth $base]
    set h [winfo reqheight $base]
    wm minsize $base $w $h
    wm overrideredirect $base 0
    wm deiconify $base
    wm title $base $Name(Titel)
    
# mmenf = main menu frame
    frame $base.mmenf \
	-borderwidth 2 -height 30 -relief groove  -width 489 
# define menu file
    menubutton $base.mmenf.fileb \
	-menu .lisa.mmenf.fileb.m -padx 4 -pady 3 -text $Name(File) -underline 0
    menu $base.mmenf.fileb.m -cursor {} -tearoff 0 
    $base.mmenf.fileb.m add command \
	-command { TC_new;
                   set mw(fProblem) 1;   
	           mw_no_problem;
	           showpro}\
        -label "$Name(New)" 
    $base.mmenf.fileb.m add separator
    $base.mmenf.fileb.m add command \
	-command {TC_open [tk_getOpenFile -defaultextension .lsa \
			       -filetypes $mw(filetypes) \
			       -title $Name(Read_File) ] } \
	-label "$Name(Open)" 

    $base.mmenf.fileb.m add command \
        -command {TC_save [tk_getSaveFile  -defaultextension .lsa \
			       -filetypes $mw(filetypes) \
			       -title $Name(Save_File)]} \
	-label "$Name(Save_as)" 
    $base.mmenf.fileb.m add command \
	-label "$Name(Print)" \
	-command { Window show .printoptions}
    $base.mmenf.fileb.m add separator
    $base.mmenf.fileb.m add command \
        -command lisa_exit -label "$Name(Exit)" -underline 0

# define menu edit
    menubutton $base.mmenf.men27 \
        -menu .lisa.mmenf.men27.m -padx 4 -pady 3 -text $Name(Edit) \
        -underline 0
    menu $base.mmenf.men27.m \
        -cursor {} -tearoff 0 

    $base.mmenf.men27.m add command \
        -command {showpro} -label $Name(Problem)

    $base.mmenf.men27.m add checkbutton \
        -command {showdat} -label $Name(Parameter) -variable mw(fParameter)

    $base.mmenf.men27.m add checkbutton \
	-command {showseq} -label $Name(Solution) -variable mw(fSequence)

    $base.mmenf.men27.m add checkbutton \
	-command {showseqlist} -label $Name(List_of_Sequences) \
        -variable mw(fSequencelist)

# define menu algorithm
    menubutton $base.mmenf.men30 \
        -menu .lisa.mmenf.men30.m -padx 4 -pady 3 -text $Name(Algorithm) \
        -underline 0 
    menu $base.mmenf.men30.m \
        -cursor {} -tearoff 0 
    $base.mmenf.men30.m add cascade -menu  $base.mmenf.men30.m.exakt_alg \
	-label "$Name(exact_algs)" -state disabled
    set submen [menu  $base.mmenf.men30.m.exakt_alg -tearoff 0]
    $base.mmenf.men30.m add cascade -menu  $base.mmenf.men30.m.heur_alg \
	-label "$Name(heuristic_algs)"  -state disabled
    set submen [menu  $base.mmenf.men30.m.heur_alg -tearoff 0]

#define menu display
    menubutton $base.mmenf.men31 \
        -menu .lisa.mmenf.men31.m -padx 4 -pady 3 -text $Name(Display) \
        -underline 0
    menu $base.mmenf.men31.m \
        -cursor {} -tearoff 0 
    $base.mmenf.men31.m add radiobutton -command TC_draw_output \
        -label "$Name(Parameter)" -value {Parameter} -variable mw(mwout)
    $base.mmenf.men31.m add separator
    $base.mmenf.men31.m add radiobutton -command TC_draw_output \
        -label "$Name(Sequence)" -value {Sequence} -variable mw(mwout)
    $base.mmenf.men31.m add radiobutton -command TC_draw_output \
        -label $Name(Sequence_Graph) -value {Sequence Graph} \
        -variable mw(mwout)  
    $base.mmenf.men31.m add separator
    $base.mmenf.men31.m add radiobutton -command TC_draw_output \
        -label $Name(Schedule) -value Schedule -variable mw(mwout)
    $base.mmenf.men31.m add radiobutton -command TC_draw_output \
        -label $Name(Gantt_Diagram) -value {Gantt Diagram} -variable mw(mwout)
    
#define menu extras
    menubutton $base.mmenf.menex \
        -menu .lisa.mmenf.menex.m -padx 4 -pady 3 -text "$Name(Extras)" \
        -underline 0
    menu $base.mmenf.menex.m \
        -cursor {} -tearoff 0 
    $base.mmenf.menex.m add command \
        -label $Name(Classification) -command { TC_classify}
    $base.mmenf.menex.m add separator
    $base.mmenf.menex.m add cascade -menu  $base.mmenf.menex.m.alg_ptst \
	-label "$Name(general_parameter_test)"
    set submen [menu  $base.mmenf.menex.m.alg_ptst -tearoff 0] 
    $base.mmenf.menex.m add separator
    $base.mmenf.menex.m add checkbutton  -state disabled\
        -label $Name(Zoommode) -variable mw(zoom) \
        -command { config_zoom_button }

# define menu options
    menubutton $base.mmenf.men32 \
        -menu .lisa.mmenf.men32.m -padx 4 -pady 3 -text "$Name(Options)"\
        -underline 0
    menu $base.mmenf.men32.m \
        -cursor {} -tearoff 0 
    $base.mmenf.men32.m add command \
        -label "$Name(Gantt_Diagram)" -command {Window show .gantt}
   $base.mmenf.men32.m add command \
        -label "$Name(general_options)" -command {Window show .options}

# define menu help
    menubutton $base.mmenf.men34 \
        -menu .lisa.mmenf.men34.m -padx 4 -pady 3 -text "$Name(Help)" \
        -underline 0
    menu $base.mmenf.men34.m \
        -cursor {} -tearoff 0 
   $base.mmenf.men34.m add command \
        -label "$Name(Documentation)" -command lisa_docu
    $base.mmenf.men34.m add command \
	-label "$Name(WWW_Page)" -command lisa_page
    $base.mmenf.men34.m add command \
        -label "$Name(About)" -command lisa_about
    
# configure the lower status bar (lstb):
    frame $base.lstb \
        -borderwidth 2 -height 20 -relief ridge \
	-width 125 
    label $base.lstb.lab39 \
        -borderwidth 1 -text "  "
    label $base.lstb.lab40 \
        -borderwidth 1 -width 30 -text " "
    
# configure the problem type status line 
    frame $base.fra40 -borderwidth 2 -height 32 -width 125 
    label $base.fra40.ent43 -text "" -relief sunken -background white \
	-foreground black -anchor w

# define main canvas, scales and roll bars:
# fra28=upper frame, fra35=zoom button frame, fra36=upper scale frame
# can37=upper scale 
    frame $base.frame_for_all_canvas \
        -borderwidth 2 -height 75 -width 130 
    #upper region:
    frame $base.frame_for_all_canvas.fra28 \
        -borderwidth 2  -height 50 -width 125 
    frame $base.frame_for_all_canvas.fra28.fra35 \
        -borderwidth 2 -height 40 -width 46 
    set  zoomsymbol [image create bitmap -file "$env(LISAHOME)/img/lens.xbm"]
    button $base.frame_for_all_canvas.fra28.fra35.cb \
	-command {mw_zoom}  -height 30 -width 30 -relief raised \
	-image $zoomsymbol -padx 60 -state disabled
    frame $base.frame_for_all_canvas.fra28.fra36 \
	-borderwidth 2 -height 50 -width 123 
    frame $base.frame_for_all_canvas.fra28.fra36.fillfr1 \
	-borderwidth 2 -width 13 
    canvas $base.frame_for_all_canvas.fra28.fra36.can37 \
	-borderwidth 2 -height 35 -width 372 
    #lower frame: 
    frame $base.frame_for_all_canvas.fra32 \
	-borderwidth 0 -height 75 -width 125 
    #left region:
    frame $base.frame_for_all_canvas.fra32.fra33 \
	-borderwidth 2 -height 75 -width 50  
    frame $base.frame_for_all_canvas.fra32.fra33.fillfr2 \
	-borderwidth 2 -height 12
    canvas $base.frame_for_all_canvas.fra32.fra33.can38 \
	-borderwidth 2 -height 262 -width 35 
    #main drawing region with scrollbars:                                     
    frame $base.frame_for_all_canvas.fra32.cpd34 \
	-borderwidth 1 -height 30 -width 30 
    scrollbar $base.frame_for_all_canvas.fra32.cpd34.01 \
	-borderwidth 1 -command {lw_sch } -orient horiz -width 10 
    scrollbar $base.frame_for_all_canvas.fra32.cpd34.02 \
	-borderwidth 1 -command {lw_scv } -orient vert -width 10 
    canvas $base.frame_for_all_canvas.fra32.cpd34.03 \
        -background white -borderwidth 2 -height 100 -width 100 \
        -xscrollcommand {.lisa.frame_for_all_canvas.fra32.cpd34.01 set} \
        -yscrollcommand {.lisa.frame_for_all_canvas.fra32.cpd34.02 set} 
    bind $base.frame_for_all_canvas.fra32.cpd34.03 <ButtonPress-1> {
	if {$mw(zoom)=="1"} {
	    .lisa.frame_for_all_canvas.fra32.cpd34.03 delete   rect
	    set mw(xpos1) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x]
	    set mw(ypos1) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y]
	    mw_add_motion
	}
    }

   bind $base.frame_for_all_canvas.fra32.cpd34.03 <Button-2> {
       if {[TC_getvar VALID_SCHEDULE]==1} {
	   if {$mw(mwout)=={Gantt Diagram} } {
	       set schedule(row) [TC_getvar job_from_gantt \
		       [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
		       [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
	       set schedule(column) [TC_getvar machine_from_gantt \
		       [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
		       [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
	       if { $schedule(row)==-1 || $schedule(column)==-1 } {
		   TC_error "No such Operation exists"
	       } else {
		   
	       }
	   } 
       }
   }

   bind $base.frame_for_all_canvas.fra32.cpd34.03 <Button-3> { 
       if {[TC_getvar VALID_SCHEDULE]==1} {
	   if {$mw(mwout)=={Gantt Diagram} } {
	       set schedule(row) [TC_getvar job_from_gantt \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
		set schedule(column) [TC_getvar machine_from_gantt \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
		if { $schedule(row)==-1 || $schedule(column)==-1 } {
		    TC_error "No such Operation exists"
		} else {
		    #		TC_set_pt $schedule(row) $schedule(column) [expr [TC_get_pt  $schedule(row) $schedule(column)]+1]
		}
	    }
	}
    }
    

    # manipulate position of an operation in job- or machine order
    # the operation is selected by double click in gantt-chart
    bind $base.frame_for_all_canvas.fra32.cpd34.03 <Double-Button-1> { 
	if {[TC_getvar VALID_SCHEDULE]==1} {
	    if {$mw(mwout)=={Gantt Diagram} } {
		set oldrow 1
		set oldcolumn 1
		catch {set oldrow $schedule(row)}
		catch {set oldcolumn $schedule(column)}
		set schedule(row) [TC_getvar job_from_gantt \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
		set schedule(column) [TC_getvar machine_from_gantt \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x] \
			[.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y] ]
		if { $schedule(row)==-1 || $schedule(column)==-1 } {
		    set schedule(row) $oldrow;
		    set schedule(column) $oldcolumn;
		    TC_error "No such Operation exists"
		}
		
		if {[winfo exists .man_oper]} {
		    man_oper_update} else {
			Window show .man_oper } 
		    } 
		}
	    }
    
####### now put all the defined elements into the main window: ############

    pack $base.mmenf -in .lisa -fill x -side top 
    pack $base.mmenf.fileb -in .lisa.mmenf -side left 
    pack $base.mmenf.men27 -in .lisa.mmenf -side left 
    pack $base.mmenf.men30 -in .lisa.mmenf -side left 
    pack $base.mmenf.men31 -in .lisa.mmenf -side left
    pack $base.mmenf.menex -in .lisa.mmenf -side left 
    pack $base.mmenf.men32 -in .lisa.mmenf -side left 
    pack $base.mmenf.men34 -in .lisa.mmenf -side right 

    pack $base.lstb -in .lisa -fill x -side bottom 
    pack $base.lstb.lab40 -in .lisa.lstb -side right 
    pack $base.lstb.lab39 -in .lisa.lstb -fill x -side left 

    pack $base.fra40 -in .lisa -fill x -side top 
    pack $base.fra40.ent43 \
        -in .lisa.fra40 -expand 1 -fill x -side right 
    
    pack $base.frame_for_all_canvas -in .lisa  -expand 1 -fill both  
    pack $base.frame_for_all_canvas.fra28 \
        -in .lisa.frame_for_all_canvas -fill x -side top 
    pack $base.frame_for_all_canvas.fra28.fra35 \
        -in .lisa.frame_for_all_canvas.fra28  -side left
    pack $base.frame_for_all_canvas.fra28.fra35.cb \
	-in .lisa.frame_for_all_canvas.fra28.fra35 -side left 
    pack $base.frame_for_all_canvas.fra28.fra36 \
        -in .lisa.frame_for_all_canvas.fra28 -fill x -side top
    pack $base.frame_for_all_canvas.fra28.fra36.fillfr1 \
	-in .lisa.frame_for_all_canvas.fra28.fra36 -anchor e -side right
    pack $base.frame_for_all_canvas.fra28.fra36.can37 \
        -in .lisa.frame_for_all_canvas.fra28.fra36 -fill x -side top 
    pack $base.frame_for_all_canvas.fra32 \
        -in .lisa.frame_for_all_canvas -expand 1 -fill both -side top 
    pack $base.frame_for_all_canvas.fra32.fra33 \
        -in .lisa.frame_for_all_canvas.fra32 -fill y -side left
    pack $base.frame_for_all_canvas.fra32.fra33.fillfr2 \
	-in .lisa.frame_for_all_canvas.fra32.fra33 -anchor s -side bottom
    pack $base.frame_for_all_canvas.fra32.fra33.can38 \
        -in .lisa.frame_for_all_canvas.fra32.fra33 -expand 1 -fill y \
        -side top 
    pack $base.frame_for_all_canvas.fra32.cpd34 \
        -in .lisa.frame_for_all_canvas.fra32 -expand 1 -fill both -side top 
    grid columnconf $base.frame_for_all_canvas.fra32.cpd34 0 -weight 1
    grid rowconf $base.frame_for_all_canvas.fra32.cpd34 0 -weight 1
    grid $base.frame_for_all_canvas.fra32.cpd34.01 \
        -in .lisa.frame_for_all_canvas.fra32.cpd34 -column 0 -row 1 \
        -columnspan 1 \
        -rowspan 1 -sticky ew 
    grid $base.frame_for_all_canvas.fra32.cpd34.02 \
        -in .lisa.frame_for_all_canvas.fra32.cpd34 -column 1 -row 0 \
        -columnspan 1 \
        -rowspan 1 -sticky ns 
    grid $base.frame_for_all_canvas.fra32.cpd34.03 \
        -in .lisa.frame_for_all_canvas.fra32.cpd34 -column 0 -row 0 \
        -columnspan 1 -rowspan 1 -sticky nesw 

    # initialise problem tupel
    tupelinit
    dpinit   
    mw_no_problem
    mw_set_label ""
    set mw(oldheight) 1
    set mw(oldwidth) 1
    # draw output, if size of window changed
    bind .lisa <Configure> {
	if { [expr [winfo height .lisa] -$mw(oldheight)]> 4 \
		 || [expr [winfo height .lisa] -$mw(oldheight)]< -4 \
		 || [expr [winfo width .lisa] -$mw(oldwidth)]> 4 \
		 || [expr [winfo width .lisa] -$mw(oldwidth)]< -4 } {
	    set mw(oldheight) [winfo height .lisa]
	    set mw(oldwidth) [winfo width .lisa]
	    if {$mw(zoom)==0}  TC_draw_output
	}
    }
    
    # open logo:
    if {$system(screen)=="big"} {
	set  mw(LiSAsymbol) [ image create  photo -file "$env(LISAHOME)/img/llogo_xl.gif"] } else {
	    set  mw(LiSAsymbol) [ image create  photo -file "$env(LISAHOME)/img/llogo_l.gif"] }
    
    mw_add_logo

    bind $base <Destroy> {TC_exit }
    # make windows .datawin .schedulewin and .seqlist into icons, if .lisa is an icon  
    bind $base <Unmap> {
	if {[winfo exists .datawin]} { wm iconify .datawin }
	if {[winfo exists .schedulewin]} { wm iconify .schedulewin }
	if {[winfo exists .seqlist]} { wm iconify .seqlist}
    }
    # make icons .datawin .schedulewin and .seqlist into windows, if .lisa is a window  
    bind $base <Map> {
	if {[winfo exists .datawin]} { wm deiconify .datawin }
	if {[winfo exists .schedulewin]} { wm deiconify .schedulewin }
	if {[winfo exists .seqlist]} { wm deiconify .seqlist}
    }
    disable_zoom
}

# procedure for scrolling two canvas at the same time
proc lw_scv { args } {
    eval .lisa.frame_for_all_canvas.fra32.fra33.can38 yview $args
    eval .lisa.frame_for_all_canvas.fra32.cpd34.03 yview $args
}

# procedure for scrolling two canvas at the same time
proc lw_sch { args } {
    eval .lisa.frame_for_all_canvas.fra32.cpd34.03 xview $args
    eval .lisa.frame_for_all_canvas.fra28.fra36.can37  xview $args
}

# opens window to display the solution
proc showseq { } { 
    global mw;
    if ($mw(fSequence)!="0") { 
 	Window show .schedulewin 
    } else { 
	if {[winfo exists .schedulewin]} {destroy .schedulewin}
    }    
}

# opens window to display a list of solutions
proc showseqlist { } { 
    global mw;
    if ($mw(fSequencelist)!="0") { 
 	Window show .seqlist 
    } else { 
	if {[winfo exists .seqlist]} {destroy .seqlist}
    }    
}

# opens window to display the parameter window
proc showdat { } { 
    global mw;
    if ($mw(fParameter)!="0") { 
 	Window show .datawin 
    } else { 
	if {[winfo exists .datawin]} {destroy .datawin }
    }    
}

# opens window to display the dialog to edit the probletype
proc showpro { } { 
    Window show .dprob
}

# new number of machines or jobs
proc new_mn { } {
    global mw;
    global dat;

    if ($mw(fParameter)!="0") { 
	TC_draw_dat 
    }
}

# machine or job number is 0
proc mw_no_mn { } {
    global Name;
       .lisa.mmenf.men27.m entryconfigure $Name(Parameter) -state disabled  
    mw_no_values
}

# no problem defined
proc mw_no_problem { } {
    global Name;
    global mw;
    .lisa.mmenf.fileb.m entryconfigure $Name(Save_as) -state disabled
    .lisa.mmenf.men27.m entryconfigure $Name(Solution) -state disabled
    .lisa.mmenf.men27.m entryconfigure $Name(List_of_Sequences) -state disabled
    .lisa.mmenf.menex.m  entryconfigure  $Name(Classification) -state disabled
    .lisa.mmenf.men27.m entryconfigure $Name(Parameter) -state disabled
    .lisa.mmenf.menex.m  entryconfigure $Name(general_parameter_test) -state disabled
    if {[winfo exists .datawin]} { destroy .datawin }
    mw_no_values
}

# no values defined
proc mw_no_values { } {
    global Name;
    global mw;
    .lisa.mmenf.fileb.m entryconfigure "$Name(Print)" -state disabled
    .lisa.mmenf.men27.m entryconfigure $Name(Solution) -state disabled  
    .lisa.mmenf.menex.m  entryconfigure $Name(general_parameter_test) -state disabled
    .lisa.mmenf.men31.m  entryconfigure $Name(Parameter) -state disabled
    set mw(fParameter) "0"
    if {[winfo exists .schedulewin]} { destroy .schedulewin }
    if {[winfo exists .seqlist]} { destroy .seqlist }
    #no values => no schedule:
    mw_no_schedule
}

# no solution defined
proc  mw_no_schedule { } {
    global Name;
    global mw;
    global ext_algo
    set test "0"
    .lisa.mmenf.men31.m  entryconfigure  $Name(Sequence_Graph) -state disabled
    .lisa.mmenf.men31.m entryconfigure $Name(Schedule)  -state disabled
    .lisa.mmenf.men31.m  entryconfigure $Name(Gantt_Diagram)  -state disabled
    .lisa.mmenf.men31.m  entryconfigure $Name(Sequence) -state disabled
    .lisa.mmenf.men32.m entryconfigure  $Name(Gantt_Diagram)  -state disabled
    catch { if {  $ext_algo(LIST_OF_HEURISTIC_ALGOS)!=""} { set test "1" } }
    if { $test=="1" } {
	foreach algo $ext_algo(LIST_OF_HEURISTIC_ALGOS) {
	    if { $ext_algo(TYPE,$algo)=="iterative" } {
		.lisa.mmenf.men30.m.heur_alg entryconfigure $ext_algo(NAME,$algo) -state disabled
		.lisa.mmenf.menex.m.alg_ptst  entryconfigure $ext_algo(NAME,$algo) -state disabled
	    }
	}
    }
    set mw(fSequence) "0"
    set mw(fSequencelist) "0"
    if {[winfo exists .man_oper]} { destroy .man_oper}
    disable_zoom
}

# problemtype is correctly defined 
proc problem_exist { } {
    global lsa_status
    global Name
    global env
    global ext_algo

    .lisa.mmenf.men30.m.exakt_alg delete 0 last
    .lisa.mmenf.men30.m.heur_alg delete 0 last
    .lisa.mmenf.menex.m.alg_ptst delete 0 last
    set test 0
    set ext_algo(LIST_OF_EXACT_ALGOS) ""
    set ext_algo(LIST_OF_HEURISTIC_ALGOS) ""
    foreach algname $lsa_status(list_of_external_alg) {
	if { [TC_problem_reduction \
		  exact \
		  "$env(LISAHOME)/data/alg_desc/language/$env(LLANGUAGE)/$algname.alg"]==1 } {
	    .lisa.mmenf.men30.m.exakt_alg add command -command "Window show .$algname" \
		-label $ext_algo(NAME,$algname) -state disabled
	    .lisa.mmenf.menex.m.alg_ptst add command -command "Window show .$algname\_ptst" \
		-label $ext_algo(NAME,$algname) -state disabled
	    lappend ext_algo(LIST_OF_EXACT_ALGOS) $algname
	    set test 1
	}
    }
    if { $test==1 } {
 	.lisa.mmenf.men30.m entryconfigure $Name(exact_algs)   -state active
	.lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)   -state active
     } else {
 	.lisa.mmenf.men30.m entryconfigure $Name(exact_algs)  -state disabled
	.lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)   -state disabled
     }
     set test 0
    foreach algname $lsa_status(list_of_external_alg) {
	if { [TC_problem_reduction \
		  heuristic \
		  "$env(LISAHOME)/data/alg_desc/language/$env(LLANGUAGE)/$algname.alg"]==1 } {
	    .lisa.mmenf.men30.m.heur_alg add command -command "Window show .$algname" \
		-label $ext_algo(NAME,$algname) -state disabled
	     .lisa.mmenf.menex.m.alg_ptst add command -command "Window show .$algname\_ptst" \
		-label $ext_algo(NAME,$algname) -state disabled
	    lappend ext_algo(LIST_OF_HEURISTIC_ALGOS) $algname
	    set test 1
	} 
    }
     if { $test==1 } {
 	.lisa.mmenf.men30.m entryconfigure $Name(heuristic_algs) -state active
	.lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)   -state active
     } else {
 	.lisa.mmenf.men30.m entryconfigure $Name(heuristic_algs) -state disabled
     }

    .lisa.mmenf.fileb.m entryconfigure $Name(Save_as) -state active
    .lisa.mmenf.menex.m  entryconfigure  $Name(Classification) -state active
    .lisa.mmenf.men27.m entryconfigure $Name(Parameter) -state active
    .lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)  \
	-state active
}

# values are correctly defined 
proc values_exist { } {
    global Name;
    global ext_algo;

    .lisa.mmenf.fileb.m entryconfigure "$Name(Print)" -state active
   .lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)  \
	-state active
    foreach algo $ext_algo(LIST_OF_EXACT_ALGOS) {
	if { $ext_algo(TYPE,$algo)=="constructive" } {
	    .lisa.mmenf.men30.m.exakt_alg entryconfigure $ext_algo(NAME,$algo) -state active
	    .lisa.mmenf.menex.m.alg_ptst entryconfigure $ext_algo(NAME,$algo) -state active
	}
    }
    foreach algo $ext_algo(LIST_OF_HEURISTIC_ALGOS) {
	if { $ext_algo(TYPE,$algo)=="constructive" } {
	    .lisa.mmenf.men30.m.heur_alg entryconfigure $ext_algo(NAME,$algo) -state active
	    .lisa.mmenf.menex.m.alg_ptst  entryconfigure $ext_algo(NAME,$algo) -state active 
	}
    }

    set M_ENV [TC_getvar "alpha"]
    if {  [string match "*O*" $M_ENV ] ||
	  [string match "*1*" $M_ENV ] ||
	  [string match "*J*" $M_ENV ] ||
	  [string match "*F*" $M_ENV ] } {
	set BETA [TC_getvar "beta"]
	      if {  [string match "*tree*" $BETA]  ||
	      [string match "*sp_graph*" $BETA] ||
	      [string match "*chains*" $BETA] ||
	      [string match "*prec*" $BETA] ||
	      [string match "*d_i*" $BETA] ||
	      [string match "*batch*" $BETA] ||
	      [string match "*pmtn*" $BETA] ||
	      [string match "*no-wait*" $BETA] } { } else {
		  .lisa.mmenf.men27.m entryconfigure $Name(Solution) -state active
		#   .lisa.mmenf.men30.m entryconfigure  $Name(B_and_B) -state active
		 #  .lisa.mmenf.men30.m entryconfigure  $Name(Priority_Lists) -state active
		  .lisa.mmenf.men31.m  entryconfigure $Name(Parameter) -state active
		 #  .lisa.mmenf.men30.m.prior  entryconfigure "RANDOM" -state active
		  if {  [string match "*O*" $M_ENV ] ||
			[string match "*1*" $M_ENV ] } {
		    #   .lisa.mmenf.men30.m.prior  entryconfigure "SPT" -state active
# 		      .lisa.mmenf.men30.m.prior  entryconfigure "WSPT" -state active
# 		      .lisa.mmenf.men30.m.prior  entryconfigure "EDD" -state active
		  }
	      }
    } 
}

# a valid solution exits
proc  schedule_exist { } {
    global Name;
    global mw
    global ext_algo;
    foreach algo $ext_algo(LIST_OF_EXACT_ALGOS) {
	.lisa.mmenf.men30.m.exakt_alg entryconfigure $ext_algo(NAME,$algo) -state active
#	.lisa.mmenf.men30.m.exakt_alg  entryconfigure $ext_algo(NAME,bb) -state active
    }
    foreach algo $ext_algo(LIST_OF_HEURISTIC_ALGOS) {
	.lisa.mmenf.men30.m.heur_alg entryconfigure $ext_algo(NAME,$algo) -state active
  .lisa.mmenf.menex.m.alg_ptst  entryconfigure $ext_algo(NAME,$algo) -state active 
    }

    if { [TC_getvar no_solutions]>1 } {
	.lisa.mmenf.men27.m entryconfigure $Name(List_of_Sequences) -state active
    }
 #    .lisa.mmenf.men30.m entryconfigure $Name(Neighborhood_Search) -state active
 #    .lisa.mmenf.menex.m entryconfigure $Name(Ngbh-Test)  -state active
    .lisa.mmenf.menex.m entryconfigure $Name(general_parameter_test)  \
	-state active
    .lisa.mmenf.men31.m  entryconfigure  $Name(Sequence_Graph) -state active
    .lisa.mmenf.men31.m entryconfigure $Name(Schedule)  -state active
    .lisa.mmenf.men31.m  entryconfigure $Name(Gantt_Diagram)  -state active
    .lisa.mmenf.men32.m entryconfigure  $Name(Gantt_Diagram)  -state active
    .lisa.mmenf.men31.m  entryconfigure $Name(Sequence) -state active
}

#proc mw_remove_unofficial_entrys { } {
#.lisa.mmenf.fileb.m  entryconfigure  $Name(New) -remove
#}

# set the left label at the bottom of the main window to text
proc mw_set_label { text } {
 .lisa.lstb.lab39 config -text $text 
}

# set the right label at the bottom of the main window to text
proc mw_set_objective { text } {
 .lisa.lstb.lab40 config -text $text 
}

# activate zoom
proc mw_zoom { } {
    global mw;
    if {$mw(zoom)==0} {
	set mw(zoom) 1 
    } else {
	set mw(zoom) 0
    }
config_zoom_button
 if {$mw(zoom)==0}  TC_draw_output
}

# set appearance of the zoom button corresponding the zoom flag
proc config_zoom_button { } {
    global lsa_status
    global mw
    if {$mw(zoom)==1} {
	.lisa.frame_for_all_canvas.fra28.fra35.cb config -background #e02020 \
	    -activebackground  #ff4040 
	.lisa.frame_for_all_canvas.fra28.fra35.cb config -relief sunken
	TC_setvar zoom 1
    } else {
	.lisa.frame_for_all_canvas.fra28.fra35.cb config -background #a0a0a0 \
	-activebackground  #c0c0c0
	.lisa.frame_for_all_canvas.fra28.fra35.cb config -relief raised
	TC_setvar zoom 0 
    }
}

# add lisa symbol
proc mw_add_logo { } {
    global mw
    
    .lisa.frame_for_all_canvas.fra32.cpd34.03 create image 0 0 -anchor nw -image $mw(LiSAsymbol)
}

# draw rectangles to set the zoom area
proc  mw_add_motion { } {
    global mw
    set mw(motion) "0"
    bind .lisa.frame_for_all_canvas.fra32.cpd34.03 <Motion> {
	set mw(motion) "1"
	.lisa.frame_for_all_canvas.fra32.cpd34.03 delete   rect
	set mw(xpos2) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x]
	set mw(ypos2) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y]	       
	.lisa.frame_for_all_canvas.fra32.cpd34.03 \
	    create rectangle $mw(xpos1) $mw(ypos1) \
	    $mw(xpos2) $mw(ypos2) -tags rect -width 3
    }
    bind .lisa.frame_for_all_canvas.fra32.cpd34.03 <ButtonRelease> {
	if { $mw(motion)=="1" } {
	    set mw(xpos2) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasx %x]
	    set mw(ypos2) [.lisa.frame_for_all_canvas.fra32.cpd34.03 canvasy %y]
	    TC_setvar zoom_area $mw(xpos1) $mw(ypos1) $mw(xpos2) $mw(ypos2) 
	}
	bind .lisa.frame_for_all_canvas.fra32.cpd34.03 <Motion> ""
	bind .lisa.frame_for_all_canvas.fra32.cpd34.03 <ButtonRelease> { }
    }
}  

# exits the main program
proc  lisa_exit { } {
    global Name
    set flag 1
    set flag [tk_dialog .dialog $Name(Exit) $Name(really_exit) {} 1 $Name(Yes) $Name(No) ]
    if {$flag=="0"} {
	TC_exit
    }     
}

# activate zoom button
proc activate_zoom { } {
    global Name
    global mw
   	.lisa.mmenf.menex.m  entryconfigure $Name(Zoommode) -state active
	.lisa.frame_for_all_canvas.fra28.fra35.cb configure -state active 
	if {$mw(active_zoom)=="1"}  {
	    set mw(active_zoom) "0"
	    set mw(zoom) 0
	    config_zoom_button
	}
}

# deactivate zoom button
proc disable_zoom { } {
    global Name
    global mw
    set mw(active_zoom) "1"
    set mw(zoom) 0
    config_zoom_button
    .lisa.mmenf.menex.m  entryconfigure $Name(Zoommode) -state disabled
    .lisa.frame_for_all_canvas.fra28.fra35.cb configure -state disabled 
}

# return value as float number 
proc float_value { value } {
    set temp_value 0
    scan $value "%f" temp_value
    if {[string match "*.0" $temp_value]} {  scan $value "%d" temp_value}
    return $temp_value
}

# return value as positive number and write an error, if value is negative 
proc positive_value { value } {
    set temp_value 0
    scan $value "%f" temp_value
    if {[string match "*.0" $temp_value]} {  scan $value "%d" temp_value}
    if { $temp_value<0 } {
	TC_error "positive value expected"
	set temp_value [expr - $temp_value]
    }
    return $temp_value
}

proc lisa_about { } {

 TC_stop_extalg
    global env
    global Name
    Window show .write_text
    wm title .write_text $Name(About)
    .write_text.mmenf.tex23  delete 1.0 end
    set myfile [open  $env(LISAHOME)/doc/lisa/language/$env(LLANGUAGE)/about.txt "r"]
    .write_text.mmenf.tex23  insert end [read $myfile]
    .write_text.mmenf.tex23 configure -state disabled
    .write_text.mmenf.tex23 configure -takefocus 0
}

proc lisa_text { mytitle mytext } {
    global env
    Window show .write_text
    wm title .write_text $mytitle
    .write_text.mmenf.tex23  delete 1.0 end
    .write_text.mmenf.tex23  insert end $mytext
    .write_text.mmenf.tex23 configure -state disabled
    .write_text.mmenf.tex23 configure -takefocus 0
}

proc vTclWindow.write_text {base} {
    global Name
    global system
    if {$base == ""} {
        set base .write_text
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -background #81BC95007BDF 
    wm focusmodel $base passive

    if {$system(screen)=="big"} {
	wm geometry $base 530x400+0+0
    } else {
	wm geometry $base 300x250
    }
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
#    wm title $base $Name(About)
    wm transient $base .lisa
    frame $base.fra19 \
        -borderwidth 2 -height 59 -relief groove \
        -width 125 
    button $base.fra19.but18 \
        -command {destroy .write_text} \
        -padx 11 -pady 4 -text $Name(Close)
    scrollbar $base.scr21 \
        -command {.write_text.mmenf.tex23 yview} \
        -orient vert 
    frame $base.mmenf \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    text $base.mmenf.tex23 \
        -wrap word \
        -yscrollcommand {.write_text.scr21 set} 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .write_text   -fill x -side bottom 
    pack $base.fra19.but18 \
         -in .write_text.fra19   -side bottom
    pack $base.scr21 \
        -in .write_text   -fill y -side right 
    pack $base.mmenf \
        -in .write_text  -expand 1 -fill both -side top 
    pack $base.mmenf.tex23 \
        -in .write_text.mmenf  -expand 1 -fill both -side top 
}

proc lisa_docu { } {
    lisa_help main.htm
}

proc lisa_page { } {
    global env
    exec [TC_getvar "HTML_VIEWER"] http://lisa.math.uni-magdeburg.de &
}

proc lisa_help { helpfile } {
    global env
    global Name
    global system
    set html_viewer [TC_getvar "HTML_VIEWER"]
    showdelaym "$Name(Wait...)" "$html_viewer $Name(started)"
    set lang $env(LLANGUAGE)
    if {$env(LLANGUAGE)=="german_u"} { set lang "german" }
    if {$system(os)=="win" } { 
        set start_node  "file:///" 
    } else {
        set start_node ""
    }
	exec $html_viewer $start_node$env(LISAHOME)/doc/language/$lang/$helpfile &
}

proc close_all_open_windows { } {
    if {[winfo exists .datawin]} { destroy .datawin }
    if {[winfo exists .textaus]} { destroy .textaus } 
    if {[winfo exists .schedulewin]} { destroy .schedulewin } 
    if {[winfo exists .write_text]} { destroy .write_text }
    if {[winfo exists .man_oper]} { destroy .man_oper }
    if {[winfo exists .seqlist]} { destroy .seqlist }
}


proc set_width { } {
global lsa_status
set lsa_status(width) [winfo width .lisa]
}

proc set_height { } {
global lsa_status
set lsa_status(height) [winfo height .lisa]
}
