#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11 Project
#

proc vTclWindow.datawin {base} {
    global Name;
    global dat;
    global glob;
    global system;
    if {$base == ""} {
        set base .datawin
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
	wm geometry $base 420x420
    } else {
	wm geometry $base 360x320
    }

    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Parameter)
    #    wm transient $base .lisa
    set dat(config) "0"
    set dat(entry) $Name(nichts_ausgewaehlt)
    set dat(RI) [string match "*r_i*" [TC_getvar "beta"]]
    set dat(WI) [string match "*Wi*" [TC_getvar "gamma"]]
    set dat(DI) [string match "*d_i*" [TC_getvar "beta"]]
    set dat(edit) 0
    set dat(row) -1
    set dat(column) -1
    set dat(valid) 1
    TC_make_MO_Matrix

    if { [string match "*Ui*" [TC_getvar "gamma"]]} {  set dat(DI) "1"}
    if { [string match "*Ti*" [TC_getvar "gamma"]]} {  set dat(DI) "1"}
    if { [string match "*Lmax*" [TC_getvar "gamma"]]} {  set dat(DI) "1"}
    if { [string match "*Irreg*" [TC_getvar "gamma"]]} {  set dat(DI) "1"}
    frame $base.fra19 \
         -borderwidth 2 -height 75 -relief groove -width 125 
    menubutton $base.fra19.men20 \
         -menu .datawin.fra19.men20.m -padx 5 -pady 4 \
	-text $Name(Display) -underline 0
    menu $base.fra19.men20.m \
         -cursor {} -tearoff 0 
    if {$dat(DI)==1} {
	$base.fra19.men20.m add checkbutton \
	    -label $Name(Due_Date) -variable dat(DI) -command { TC_draw_dat } 
    }
    if {$dat(WI)==1} {
    $base.fra19.men20.m add checkbutton \
        -label $Name(WI) -variable dat(WI) -command { TC_draw_dat }
    }
    if {$dat(RI)==1} {
	$base.fra19.men20.m add checkbutton \
	    -label $Name(RI) -variable dat(RI) -command { TC_draw_dat }
    }
    if {$dat(RI)==1 || $dat(WI)==1 || $dat(DI)==1} {
	$base.fra19.men20.m add separator
    }
    if { ![string match "*F*" [TC_getvar "alpha"]]&&"1"!=[TC_getvar "alpha"]} {
	$base.fra19.men20.m add radiobutton \
	    -label $Name(SIJ) -value SIJ -variable dat(Matrix) -command { 
		set dat(row) 0
		set dat(column) 0
		edit_marked_entry  
		.datawin.fra19.but22 configure -state disabled
		TC_draw_dat }
    }
    if { [string match "J*" [TC_getvar "alpha"] ] } {
	$base.fra19.men20.m add radiobutton \
	    -label $Name(Machine_Order) -value MO \
	    -variable dat(Matrix) -command { 
		set dat(row) 0
		set dat(column) 0
		edit_marked_entry 
		.datawin.fra19.but22 configure -state active;
		TC_draw_dat }
    } else {set dat(Matrix) PIJ}
    $base.fra19.men20.m add radiobutton \
        -label $Name(PIJ) -value PIJ \
	-variable dat(Matrix) -command { 
	    set dat(row) 0
	    set dat(column) 0
	    edit_marked_entry 
	    .datawin.fra19.but22 configure -state disabled;
	    TC_draw_dat }
    $base.fra19.men20.m add separator
    $base.fra19.men20.m add command \
        -label $Name(Close_Window) -command {w_ende}
    menubutton $base.fra19.men21 \
        -menu .datawin.fra19.men21.m -padx 5 -pady 4 \
        -text $Name(Actions) -underline 0
    menu $base.fra19.men21.m \
	-cursor {} -tearoff 0 
    $base.fra19.men21.m add command \
        -label $Name(Generate_PT)  -command { generate_PT }
    if { ![string match "*F*" [TC_getvar "alpha"]]&&"1"!=[TC_getvar "alpha"]} {
	$base.fra19.men21.m add command \
	    -label $Name(Generate_SIJ)  -command {Window show .gen_sij; }
    }
      if { [string match "*J*" [TC_getvar "alpha"]]} {
	  $base.fra19.men21.m add command \
	      -label $Name(Generate_MO)  -command { TC_gen_mo;  TC_draw_dat }
      }
    if {$dat(WI)==1} {
	$base.fra19.men21.m add command \
	    -label $Name(GenWI)  -command {set glob(generate) WI; Window show .erzspal; TC_draw_output}
    }
    if {$dat(DI)==1} {
	$base.fra19.men21.m add command \
	    -label $Name(GenDD)  -command {set glob(generate) DD; Window show .erzspal; TC_draw_output}
    }
    if {$dat(RI)==1} {
	$base.fra19.men21.m add command \
	    -label $Name(GenRD)  -command {set glob(generate) RD; Window show .erzspal; TC_draw_output}
    }
    button $base.fra19.but22 -text "$Name(Adopt_MO)" -pady 3 \
	-command adopt_MO -state disabled
    button $base.fra19.but23 -text "$Name(Help)" -pady 3 \
	-command { lisa_help values.html }
    frame $base.fra22 \
         -borderwidth 2 -height 75 -width 125 
    frame $base.fra22.fra25 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra22.fra25.lab26 \
        -borderwidth 1 -text $Name(Entry:) 
    entry $base.fra22.fra25.ent27 \
         -textvar dat(entry)
    frame $base.fra22.fra28 \
         -borderwidth 2 -height 50 -width 125 
    frame $base.fra22.fra28.fra35 \
        -borderwidth 2 -height 40 -width 40 
    frame $base.fra22.fra28.fra36 \
	-borderwidth 2 -height 50 -width 125 
    frame $base.fra22.fra28.fra36.fillfr1 \
	-borderwidth 2 -width 13
    canvas $base.fra22.fra28.fra36.can37 \
	-borderwidth 2 -height 35 -width 374 
    frame $base.fra22.fra32 \
	-borderwidth 2 -height 75 -width 125 
    frame $base.fra22.fra32.fra33 \
	-borderwidth 2 -height 75 -width 50 
    frame $base.fra22.fra32.fra33.fillfr2 \
	-borderwidth 2 -height 12
    canvas $base.fra22.fra32.fra33.can38 \
	-borderwidth 2 -height 262 -width 35 
    frame $base.fra22.fra32.cpd34 \
	-borderwidth 1 -height 30 -width 30 
    scrollbar $base.fra22.fra32.cpd34.01 \
	-borderwidth 1 -command {dw_sch } -orient horiz -width 10 
    scrollbar $base.fra22.fra32.cpd34.02 \
	-borderwidth 1 -command {dw_scv } -orient vert -width 10 
    canvas $base.fra22.fra32.cpd34.03 \
        -background #fefef0 -borderwidth 2 -height 100 -width 100 \
        -xscrollcommand {.datawin.fra22.fra32.cpd34.01 set} \
        -yscrollcommand {.datawin.fra22.fra32.cpd34.02 set} 
    frame $base.fra23 \
	-borderwidth 2 -height 75 -relief groove -width 125 
    label $base.fra23.lab24 \
	-borderwidth 1 -text label
    
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .datawin -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.men20 \
        -in .datawin.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.men21 \
        -in .datawin.fra19 -anchor center -expand 0 -fill none -side left
    pack $base.fra19.but23 \
	    -in .datawin.fra19 -anchor center -expand 0 -fill none -side right 
    if { [string match "J*" [TC_getvar "alpha"] ] } {
	pack $base.fra19.but22 \
	    -in .datawin.fra19 -anchor center -expand 0 -fill none -side right 
    }
    pack $base.fra23 \
        -in .datawin -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra23.lab24 \
        -in .datawin.fra23 -anchor center -expand 0 -fill none -side top 
    pack $base.fra22 \
        -in .datawin -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra25 \
        -in .datawin.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra25.lab26 \
        -in .datawin.fra22.fra25 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.fra25.ent27 \
        -in .datawin.fra22.fra25 -anchor center -expand 0 -fill x -side top 
    bind $base.fra22.fra25.ent27 <Key> {set dat(edit) 1}
    bind $base.fra22.fra25.ent27 <Tab> {
	set dat(config) "1"
	event generate .datawin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar w_identifier $dat(row) [expr $dat(column)+1]]!=""} {
	    set dat(column) [expr $dat(column)+1]
	}
	edit_marked_entry
    }
    bind $base.fra22.fra25.ent27 <Down> {
	set dat(config) "1"
	event generate .datawin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar w_identifier [expr $dat(row)+1] $dat(column)]!=""} {
	    set dat(row) [expr $dat(row)+1]
	}
	edit_marked_entry }
    bind $base.fra22.fra25.ent27 <Up> {
	set dat(config) "1"
	event generate .datawin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar w_identifier [expr $dat(row)-1] $dat(column)]!=""} {
	    set dat(row) [expr $dat(row)-1]
	}
	edit_marked_entry }
    bind $base.fra22.fra25.ent27 <Control-Right> {
	set dat(config) "1"
	event generate .datawin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar w_identifier $dat(row) [expr $dat(column)+1]]!=""} {
	    set dat(column) [expr $dat(column)+1]
	}
	edit_marked_entry
    }
    bind $base.fra22.fra25.ent27 <Control-Left> {
	set dat(config) "1"
	event generate .datawin.fra22.fra25.ent27 <Return> -when now
	if {[TC_getvar w_identifier $dat(row) [expr $dat(column)-1]]!=""} {
	    set dat(column) [expr $dat(column)-1]
	}
	edit_marked_entry
    }
    pack $base.fra22.fra28 \
        -in .datawin.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28.fra35 \
        -in .datawin.fra22.fra28 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.fra28.fra36 \
        -in .datawin.fra22.fra28 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.fra28.fra36.fillfr1 \
	-in .datawin.fra22.fra28.fra36 -anchor e -expand 0 -side right
    pack $base.fra22.fra28.fra36.can37 \
        -in .datawin.fra22.fra28.fra36 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.fra22.fra32 \
        -in .datawin.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.fra32.fra33 \
        -in .datawin.fra22.fra32 -anchor center -expand 0 -fill y -side left 
    pack $base.fra22.fra32.fra33.fillfr2 \
	-in .datawin.fra22.fra32.fra33 -anchor s -expand 0 -side bottom
    pack $base.fra22.fra32.fra33.can38 \
        -in .datawin.fra22.fra32.fra33 -anchor center -expand 1 -fill y \
        -side top 
    pack $base.fra22.fra32.cpd34 \
        -in .datawin.fra22.fra32 -anchor center -expand 1 -fill both -side top 
    grid columnconf $base.fra22.fra32.cpd34 0 -weight 1
    grid rowconf $base.fra22.fra32.cpd34 0 -weight 1
    grid $base.fra22.fra32.cpd34.01 \
        -in .datawin.fra22.fra32.cpd34 -column 0 -row 1 -columnspan 1 \
        -rowspan 1 -sticky ew 
    grid $base.fra22.fra32.cpd34.02 \
        -in .datawin.fra22.fra32.cpd34 -column 1 -row 0 -columnspan 1 \
        -rowspan 1 -sticky ns 
    grid $base.fra22.fra32.cpd34.03 \
        -in .datawin.fra22.fra32.cpd34 -column 0 -row 0 -columnspan 1 \
        -rowspan 1 -sticky nesw 

    bind $base.fra22.fra32.cpd34.03 <Button-1> { editw %x %y ;
	if {  $dat(valid)=="1" } {
	    set dat(config) "0"
	    focus  .datawin.fra22.fra25.ent27  
	    .datawin.fra22.fra25.ent27 selection range 0 end
	}
    }
    bind $base.fra22.fra25.ent27 <Return> {
	if {  $dat(valid)=="1" } {
	    set dat(entry) [positive_value $dat(entry)]
	    w_set_entry
	}
	    if {$dat(config)=="0"} {
		if {[TC_getvar w_identifier $dat(row) [expr $dat(column)+1]]!=""} {
		    set dat(column) [expr $dat(column)+1]
		} else {
		    if {[TC_getvar w_identifier [expr $dat(row)+1] 1]!=""} {
			set dat(column) 0
			set dat(row) [expr $dat(row)+1]
		    }
		}
		edit_marked_entry
	    }
	set dat(config) "0"
    }
    
    if {$dat(Matrix)=="MO"} {.datawin.fra19.but22 configure -state active}
    
    if { [string match "*p_ij=1*" [TC_getvar "beta"]] } {
	TC_genpt 1 1
    } elseif { [string match "*p_ij=p*" [TC_getvar "beta"]] } {
	set myvalue [TC_get_pt 0 0] 
	TC_genpt $myvalue $myvalue
    } 
    
    TC_draw_dat
    #enable some menue entrys:
    TC_new_values
    bind $base <Destroy> { set mw(fParameter) "0" 
	if { $dat(edit)=="1" } { adopt_MO }}
}

proc w_ende {} {
    global mw;
    destroy .datawin
    set mw(fParameter) 0
}

proc dw_scv { args } {
    eval .datawin.fra22.fra32.fra33.can38 yview $args
    eval .datawin.fra22.fra32.cpd34.03 yview $args
}

proc dw_sch { args } {
    eval .datawin.fra22.fra32.cpd34.03 xview $args
    eval .datawin.fra22.fra28.fra36.can37  xview $args
}

proc editw { x y } {
    global dat
    TC_draw_dat
    set dat(row) [TC_getvar w_row [.datawin.fra22.fra32.cpd34.03 canvasy $y] ]
    set dat(column) [TC_getvar w_column [.datawin.fra22.fra32.cpd34.03 canvasx $x ] ]
    edit_marked_entry  
}

proc edit_marked_entry { } { 
    global dat
    set dat(valid) "1"
    set identifier [TC_getvar w_identifier $dat(row) $dat(column)]
    if  { $identifier != ""} {
	    if { $identifier!="DD" && $identifier!="RD" && $identifier!="WI" } {
 		if { [TC_getvar "SIJ" $dat(row) $dat(column)]=="1" || $identifier=="SIJ" } {
		    .datawin.fra22.fra25.lab26 config -text \
			"$identifier ([expr $dat(row)+1],[expr $dat(column)+1])" 
		    set dat(entry) [TC_getvar w_entry $dat(row) $dat(column)]
		} else {
		    .datawin.fra22.fra25.lab26 config -text ""
		    set dat(entry) ""
		    set dat(valid) "0"   
		}
	    } else {
		.datawin.fra22.fra25.lab26 config -text \
		    "$identifier ([expr $dat(row)+1])"
		set dat(entry) [TC_getvar $identifier $dat(row) ]	
	    }
	    TC_mark_value_entry $dat(row) $dat(column)
	    
	    if { $dat(column)>5 } {
		set position [expr [expr $dat(column)- 5.0]/[TC_getvar get_m]]
		.datawin.fra22.fra32.cpd34.03 xview moveto $position
		.datawin.fra22.fra28.fra36.can37 xview moveto $position
	    } else {
		.datawin.fra22.fra32.cpd34.03 xview moveto 0
		.datawin.fra22.fra28.fra36.can37 xview moveto 0
	    }
	    if { $dat(row)>5 } {
		set position [expr [expr $dat(row)- 5.0]/[TC_getvar get_n]]
		.datawin.fra22.fra32.cpd34.03 yview moveto $position
		.datawin.fra22.fra32.fra33.can38  yview moveto $position
	    } else {
		.datawin.fra22.fra32.cpd34.03 yview moveto 0
		.datawin.fra22.fra32.fra33.can38  yview moveto 0
	    }
 .datawin.fra22.fra25.ent27 selection range 0 end
	} else {
	    .datawin.fra22.fra25.lab26 config -text ""
	    set dat(entry) ""
	    set dat(valid) "0"
	}
} 

proc  w_set_entry { } {
    global dat
    set identifier [TC_getvar w_identifier $dat(row) $dat(column)]

    if {  $identifier != "" } {
	if { $identifier!="DD" && $identifier!="RD" && $identifier!="WI" } {
	    if { [TC_getvar "SIJ" $dat(row) $dat(column)]=="1" || $identifier=="SIJ" } {
		if {  [string match "*1*"  [TC_getvar "beta"] ] } {
		    TC_draw_dat_entry $dat(row)  $dat(column) 1
		} else { TC_draw_dat_entry $dat(row)  $dat(column) $dat(entry) }
		
		if { $identifier == "PIJ" } {
		    if {  [string match "*p_ij=1*"  [TC_getvar "beta"] ]  } {
			TC_set_pt $dat(row) $dat(column) 1
		    } elseif  { [string match "*p_ij=p*"  [TC_getvar "beta"] ] } {
			TC_genpt $dat(entry) $dat(entry)
			TC_draw_dat
		    } else { TC_set_pt $dat(row) $dat(column) $dat(entry)}
		}
		if { $identifier == "SIJ" } {
		    TC_setvar "SIJ" $dat(row) $dat(column) $dat(entry)
		    TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar "w_entry"  $dat(row)  $dat(column)]	
		}
		if { $identifier == "MO" } {
		    TC_setvar "MO" $dat(row) $dat(column) $dat(entry)
		    #TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar "w_entry"  $dat(row)]
		}
	    }
	} else {
	    if { $identifier == "RD" } {
		TC_setvar "RD" $dat(row) $dat(entry)
	#	TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar "w_entry"  $dat(row)]
	    }
	    if { $identifier == "DD" } {
		TC_setvar "DD" $dat(row) $dat(entry)
		#TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar "w_entry"  $dat(row)]
	    }
	    if { $identifier == "WI" } {
		TC_setvar "WI" $dat(row) $dat(entry)
		#TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar "w_entry"  $dat(row)]
	    }
	    TC_draw_dat_entry $dat(row)  $dat(column) [TC_getvar $identifier  $dat(row)]
	}
    }
}

proc adopt_MO { } {
    global dat
    set dat(edit) "0"  
    TC_adopt_MO;
    TC_draw_dat
}

proc generate_PT { } {
    global glob
    global dat
    set glob(generate) PT; 
    if { [string match "*p_ij=1*" [TC_getvar "beta"]] } {
	TC_genpt 1 1
	TC_draw_dat
    } elseif { [string match "*p_ij=p*" [TC_getvar "beta"]] } {
 	Window show .gen_const_pij
    } else  {
	Window show .erzpt
    }
}

