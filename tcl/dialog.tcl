#################################
# VTCL GENERATED GUI PROCEDURES
#

proc vTclWindow.erzpt {base} {
    global pt
    global Name
    global system

    if {$base == ""} {
        set base .erzpt
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
	set xPos [expr {([winfo screenwidth .]-352)/2}]
	set yPos [expr {([winfo screenheight .]-209)/2}]
	wm geometry $base 352x209+$xPos+$yPos
	#wm geometry $base 352x209
	wm maxsize $base 1265 994
    } else {
	set xPos [expr {([winfo screenwidth .]-220)/2}]
	set yPos [expr {([winfo screenheight .]-150)/2}]
	wm geometry $base 220x150+$xPos+$yPos
	#wm geometry $base 220x150
    }
   set w [winfo reqwidth $base]
    set h [winfo reqheight $base]
    wm minsize $base $w $h
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Generate)
    wm transient $base .datawin
    frame $base.fra19 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra19.lab25 \
         -borderwidth 1 \
        -text $Name(Minimum) -width 16 
    entry $base.fra19.ent32 \
         -textvariable pt(min)
    frame $base.fra20 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra20.lab26 \
        -borderwidth 1 \
        -text $Name(Maximum) -width 16
    entry $base.fra20.ent33 \
         -textvariable pt(max)
    frame $base.fra21 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra21.lab27 \
        -borderwidth 1 \
        -text $Name(Step) -width 16 
    entry $base.fra21.ent34 -textvariable pt(step)
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra22.lab28 \
        -borderwidth 1 \
        -text $Name(Time_Seed) -width 16
    set pt(seed1) [TC_getvar time_seed] 
    entry $base.fra22.ent35 -textvariable pt(seed1) 
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125
    label $base.fra23.lab29 \
        -borderwidth 1 \
	-text $Name(Mach_Seed) -width 16 
	   set pt(seed2) [TC_getvar mach_seed]
    entry $base.fra23.ent36 -textvariable pt(seed2) 
    frame $base.fra24 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra24.but30 \
        -padx 11 -pady 4 -width 8 \
	-command {TC_genpt $pt(min) $pt(max) $pt(step) \
		      $pt(seed1) $pt(seed2); destroy .erzpt; TC_draw_dat } \
	-text $Name(OK) 
    button $base.fra24.but31 \
         -padx 11 -pady 4 \
	-command {destroy .erzpt} -text $Name(Cancel) 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .erzpt -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab25 \
        -in .erzpt.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.ent32 \
        -in .erzpt.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20 \
        -in .erzpt -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.lab26 \
        -in .erzpt.fra20 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.ent33 \
        -in .erzpt.fra20 -anchor center -expand 0 -fill x -side top 
  #   pack $base.fra21 \
#         -in .erzpt -anchor center -expand 0 -fill x -side top 
#     pack $base.fra21.lab27 \
#         -in .erzpt.fra21 -anchor center -expand 0 -fill none -side left 
#     pack $base.fra21.ent34 \
#         -in .erzpt.fra21 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22 \
        -in .erzpt -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.lab28 \
        -in .erzpt.fra22 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.ent35 \
        -in .erzpt.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra23 \
        -in .erzpt -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.lab29 \
        -in .erzpt.fra23 -anchor center -expand 0 -fill none -side left 
    pack $base.fra23.ent36 \
        -in .erzpt.fra23 -anchor center -expand 0 -fill x -side top 
    pack $base.fra24 \
        -in .erzpt -anchor center -expand 1 -fill both -side top 
    pack $base.fra24.but30 \
        -in .erzpt.fra24 -anchor center -expand 0 -fill none -side left 
    pack $base.fra24.but31 \
        -in .erzpt.fra24 -anchor center -expand 0 -fill none -side right 
}

proc vTclWindow.gen_sij {base} {
    global sij
    global Name
    global system

    if {$base == ""} {
        set base .gen_sij
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
	set xPos [expr {([winfo screenwidth .]-352)/2}]
	set yPos [expr {([winfo screenheight .]-109)/2}]
	wm geometry $base 352x109+$xPos+$yPos
	#wm geometry $base 352x109
    } else {
	set xPos [expr {([winfo screenwidth .]-220)/2}]
	set yPos [expr {([winfo screenheight .]-100)/2}]
	wm geometry $base 220x100+$xPos+$yPos
	#wm geometry $base 220x100
    }

    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Generate)
    wm transient $base .datawin
    frame $base.fra19 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra19.lab25 \
         -borderwidth 1 \
        -text $Name(Probability) -width 28 
    entry $base.fra19.ent32 \
         -textvariable sij(prob)
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra22.lab28 \
        -borderwidth 1 \
        -text $Name(Time_Seed) -width 28
    set sij(seed1) [TC_getvar time_seed] 
    entry $base.fra22.ent35 -textvariable sij(seed1) 
    frame $base.fra24 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra24.but30 \
	-padx 11 -pady 4 -width 8 \
	-command {TC_gen_sij $sij(prob) $sij(seed1);
	    destroy .gen_sij; TC_draw_dat } \
	-text $Name(OK) 
    button $base.fra24.but31 \
         -padx 11 -pady 4 \
	-command {destroy .gen_sij} -text $Name(Cancel) 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .gen_sij -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab25 \
        -in .gen_sij.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.ent32 \
        -in .gen_sij.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22 \
        -in .gen_sij -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.lab28 \
        -in .gen_sij.fra22 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.ent35 \
        -in .gen_sij.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra24 \
        -in .gen_sij -anchor center -expand 1 -fill both -side top 
    pack $base.fra24.but30 \
        -in .gen_sij.fra24 -anchor center -expand 0 -fill none -side left 
    pack $base.fra24.but31 \
        -in .gen_sij.fra24 -anchor center -expand 0 -fill none -side right 
}
proc vTclWindow.gen_const_pij {base} {
    global pij
    global Name
    global system

    if {$base == ""} {
        set base .gen_const_pij
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
	set xPos [expr {([winfo screenwidth .]-352)/2}]
	set yPos [expr {([winfo screenheight .]-109)/2}]
	wm geometry $base 352x109+$xPos+$yPos
	#wm geometry $base 352x109
    } else {
	set xPos [expr {([winfo screenwidth .]-220)/2}]
	set yPos [expr {([winfo screenheight .]-100)/2}]
	wm geometry $base 220x100+$xPos+$yPos
	#wm geometry $base 220x100
    }

    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Generate)
    wm transient $base .datawin
    frame $base.fra19 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra19.lab25 \
         -borderwidth 1 \
        -text $Name(const_pt) -width 28 
    entry $base.fra19.ent32 \
	-textvariable pij(value)
    frame $base.fra24 \
	-borderwidth 2 -height 75 -relief groove \
	-width 125 
    button $base.fra24.but30 \
	-padx 11 -pady 4 -width 8 \
	-command {TC_genpt $pij(value) $pij(value);
	    destroy .gen_const_pij; TC_draw_dat } \
	-text $Name(OK) 
    button $base.fra24.but31 \
         -padx 11 -pady 4 \
	-command {destroy .gen_const_pij} -text $Name(Cancel) 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .gen_const_pij -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab25 \
        -in .gen_const_pij.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.ent32 \
        -in .gen_const_pij.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra24 \
	-in .gen_const_pij -anchor center -expand 1 -fill both -side top 
    pack $base.fra24.but30 \
        -in .gen_const_pij.fra24 -anchor center -expand 0 -fill none -side left 
    pack $base.fra24.but31 \
        -in .gen_const_pij.fra24 -anchor center -expand 0 -fill none -side right 
}

proc vTclWindow.erzspal {base} {
    global spal
    global glob
    global Name
    global system
    
    set spal(min) 1
    set spal(max) 99
    set spal(step) 1

    if {$base == ""} {
        set base .erzspal
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
	set xPos [expr {([winfo screenwidth .]-352)/2}]
	set yPos [expr {([winfo screenheight .]-209)/2}]
	wm geometry $base 352x209+$xPos+$yPos
	#wm geometry $base 352x209
	wm maxsize $base 1267 977
    } else {
	set xPos [expr {([winfo screenwidth .]-250)/2}]
	set yPos [expr {([winfo screenheight .]-150)/2}]
	wm geometry $base 250x150+$xPos+$yPos
	#wm geometry $base 250x150
    }

    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Generate)
    frame $base.fra19 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra19.lab25 \
	-borderwidth 1 \
        -text $Name(Minimum) -width 16 
    entry $base.fra19.ent32 \
	-textvariable spal(min)
    frame $base.fra20 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra20.lab26 \
        -borderwidth 1 \
        -text $Name(Maximum) -width 16
    entry $base.fra20.ent33 \
	-textvariable spal(max)
    frame $base.fra21 \
	-borderwidth 2 -height 75 -relief groove \
	-width 125 
    label $base.fra21.lab27 \
        -borderwidth 1 \
        -text $Name(Step) -width 16 
    entry $base.fra21.ent34 -textvariable spal(step)
	frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra22.lab28 \
        -borderwidth 1 \
        -text $Name(Time_Seed) -width 16
    set spal(seed1) [TC_getvar time_seed] 
    entry $base.fra22.ent35 -textvariable spal(seed1) 
    frame $base.fra24 \
	-borderwidth 2 -height 75 -relief groove \
	-width 125 
    button $base.fra24.but30 \
	-padx 11 -pady 4 -width 8 \
	-command {TC_gen_column $spal(min) $spal(max) $spal(step) \
		      $spal(seed1) $glob(generate) ; destroy .erzspal; TC_draw_dat ;} \
	-text $Name(OK) 
    button $base.fra24.but31 \
	-padx 11 -pady 4 \
	-command {destroy .erzspal} -text $Name(Cancel) 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .erzspal -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab25 \
        -in .erzspal.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra19.ent32 \
        -in .erzspal.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20 \
        -in .erzspal -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.lab26 \
        -in .erzspal.fra20 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.ent33 \
        -in .erzspal.fra20 -anchor center -expand 0 -fill x -side top 
  #   pack $base.fra21 \
#         -in .erzspal -anchor center -expand 0 -fill x -side top 
#     pack $base.fra21.lab27 \
#         -in .erzspal.fra21 -anchor center -expand 0 -fill none -side left 
#     pack $base.fra21.ent34 \
#         -in .erzspal.fra21 -anchor center -expand 0 -fill x -side top 
    pack $base.fra22 \
        -in .erzspal -anchor center -expand 0 -fill x -side top 
    pack $base.fra22.lab28 \
        -in .erzspal.fra22 -anchor center -expand 0 -fill none -side left 
    pack $base.fra22.ent35 \
        -in .erzspal.fra22 -anchor center -expand 0 -fill x -side top 
    pack $base.fra24 \
        -in .erzspal -anchor center -expand 1 -fill both -side top 
    pack $base.fra24.but30 \
        -in .erzspal.fra24 -anchor center -expand 0 -fill none -side left 
    pack $base.fra24.but31 \
        -in .erzspal.fra24 -anchor center -expand 0 -fill none -side right 
}





