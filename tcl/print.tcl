
proc vTclWindow.printoptions {base} {
    global Name
    global prnt
    if {$base == ""} {
        set base .printoptions
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
         
    wm focusmodel $base passive
    set xPos [expr {([winfo screenwidth .]-497)/2}]
    set yPos [expr {([winfo screenheight .]-364)/2}]
    wm geometry $base 497x364+$xPos+$yPos
#    wm geometry $base 497x364
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "$Name(Printoptions)"
    wm transient $base .lisa

    set prnt(HEIGHT) [TC_getvar mwheight]
    set prnt(WIDTH) [TC_getvar mwwidth]
    frame $base.fra18 \
         -borderwidth 2 -height 75 -relief groove -width 125 
    label $base.fra18.lab19 -anchor w  -borderwidth 1 \
         -text $Name(colormode) -width 20 
    menubutton $base.fra18.men20 \
	-indicatoron 1 -menu .printoptions.fra18.men20.m \
        -padx 5 -pady 4 -relief sunken -text $Name($prnt(COLOR))
    menu $base.fra18.men20.m \
	-cursor {} \
	-tearoff 0 
    $base.fra18.men20.m add command \
        -label $Name(color) -command {prnt_set "COLOR" "color"}
    $base.fra18.men20.m add command \
        -label $Name(grey)  -command {prnt_set "COLOR" "grey"}
    $base.fra18.men20.m add command \
        -label $Name(mono) -command {prnt_set "COLOR" "mono"}
    frame $base.cpd31 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd31.01 \
        -anchor w  -borderwidth 1 \
	-text $Name(height) -width 20 
    entry $base.cpd31.02 \
	-textvar prnt(HEIGHT)
    frame $base.cpd32 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd32.01 \
        -anchor w  -borderwidth 1 \
                 -text $Name(width) -width 20 
    entry $base.cpd32.02 \
      	-textvar prnt(WIDTH)           
    frame $base.cpd33 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd33.01 \
        -anchor w  -borderwidth 1 \
	-text $Name(pageheight) -width 20 
    entry $base.cpd33.02 \
        -textvar prnt(PAGEHEIGHT) 
    frame $base.cpd35 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd35.01 \
        -anchor w  -borderwidth 1 \
	-text $Name(pagewidth) -width 20 
    entry $base.cpd35.02 \
         -textvar prnt(PAGEWIDTH)
    frame $base.cpd36 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd36.01 \
        -anchor w  -borderwidth 1 \
	-text $Name(rotate) -width 20 
    menubutton $base.cpd36.02 \
	-indicatoron 1 -menu .printoptions.cpd36.02.03 \
        -padx 5 -pady 4 -relief sunken -text $Name($prnt(ROTATE))
    menu $base.cpd36.02.03 \
         -cursor {} \
                 -tearoff 0 
    $base.cpd36.02.03 add command \
        -label $Name(rotate_normal) -command {prnt_set "ROTATE" "rotate_normal"} 
    $base.cpd36.02.03 add command \
        -label $Name(rotate_landscape)  -command {prnt_set "ROTATE" "rotate_landscape"} 
    frame $base.cpd40 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd40.01 \
        -anchor w  -borderwidth 1 \
	-text $Name(save_PS) -width 20 
    menubutton $base.cpd40.02 \
	-indicatoron 1 -menu .printoptions.cpd40.02.03 \
        -padx 5 -pady 4 -relief sunken -text $Name($prnt(SAVE_PS))
    menu $base.cpd40.02.03 \
         -cursor {} \
        -tearoff 0 
    $base.cpd40.02.03 add command \
        -label $Name(Yes)  -command {prnt_set "SAVE_PS" "Yes"} 
    $base.cpd40.02.03 add command \
        -label $Name(No)  -command {prnt_set "SAVE_PS" "No"} 
    frame $base.cpd41 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.cpd41.01 \
        -anchor w  -borderwidth 1 \
                 -text $Name(Print_command) -width 20 
    entry $base.cpd41.02 \
	-textvar prnt(PRNT_COMMAND)
    frame $base.cpd42 \
         -borderwidth 2 -height 75 -width 125 
    button $base.cpd42.01 \
	-padx 11 -pady 4 -text OK -command {destroy .printoptions; print_canvas}
    button $base.cpd42.02 \
	-padx 11 -pady 4 -text $Name(Cancel) -command {destroy .printoptions}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra18 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.fra18.lab19 \
        -in .printoptions.fra18 -anchor w -expand 0 -fill none -ipady 4 -side left 
    pack $base.fra18.men20 \
        -in .printoptions.fra18 -anchor center -expand 0 -fill x -side top 
    pack $base.cpd31 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd31.01 \
        -in .printoptions.cpd31 -anchor center -expand 0 -fill none -ipady 3 \
        -side left 
    pack $base.cpd31.02 \
        -in .printoptions.cpd31 -anchor center -expand 0 -fill x -ipady 1 -side top 
    pack $base.cpd32 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd32.01 \
        -in .printoptions.cpd32 -anchor center -expand 0 -fill none -ipady 3 \
        -side left 
    pack $base.cpd32.02 \
        -in .printoptions.cpd32 -anchor center -expand 0 -fill x -ipady 1 -side top 
    pack $base.cpd33 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd33.01 \
        -in .printoptions.cpd33 -anchor center -expand 0 -fill none -side left 
    pack $base.cpd33.02 \
        -in .printoptions.cpd33 -anchor center -expand 0 -fill x -ipady 1 -side top 
    pack $base.cpd35 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd35.01 \
        -in .printoptions.cpd35 -anchor center -expand 0 -fill none -side left 
    pack $base.cpd35.02 \
        -in .printoptions.cpd35 -anchor center -expand 0 -fill x -ipady 1 -side top 
    pack $base.cpd36 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd36.01 \
        -in .printoptions.cpd36 -anchor w -expand 0 -fill none -side left 
    pack $base.cpd36.02 \
        -in .printoptions.cpd36 -anchor center -expand 0 -fill x -side top 
    pack $base.cpd40 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd40.01 \
        -in .printoptions.cpd40 -anchor w -expand 0 -fill none -side left 
    pack $base.cpd40.02 \
        -in .printoptions.cpd40 -anchor center -expand 0 -fill x -side top 
    pack $base.cpd41 \
        -in .printoptions -anchor center -expand 0 -fill x -side top 
    pack $base.cpd41.01 \
        -in .printoptions.cpd41 -anchor center -expand 0 -fill none -side left 
    pack $base.cpd41.02 \
        -in .printoptions.cpd41 -anchor center -expand 0 -fill x -ipady 1 -side top 
    pack $base.cpd42 \
        -in .printoptions -anchor center -expand 1 -fill both -side top 
    pack $base.cpd42.01 \
        -in .printoptions.cpd42 -anchor center -expand 0 -fill none -ipadx 20 \
        -ipady 4 -padx 20 -pady 20 -side left 
    pack $base.cpd42.02 \
        -in .printoptions.cpd42 -anchor center -expand 0 -fill none -ipadx 5 \
        -ipady 4 -padx 20 -pady 20 -side right 

    bind .printoptions <Visibility> {  
	grab set .printoptions;  bind .printoptions <Visibility> { } }
}

proc prnt_set {name value} {
    global prnt;
    global Name;
    if {$name == "COLOR"} {
	set prnt(COLOR) $value
	.printoptions.fra18.men20 config -text $Name($value)
    }
   if {$name == "SAVE_PS"} {
	set prnt(SAVE_PS) $value
	.printoptions.cpd40.02 config -text $Name($value)
    }
    if {$name == "ROTATE"} {
	.printoptions.cpd36.02 config -text $Name($value)
	if {$value=="rotate_landscape"} {set prnt(ROTATE) "rotate_landscape"} else { 
 	set prnt(ROTATE) "rotate_normal" }
    }
}

proc print_canvas { } {
    global Name
    global prnt

    if {$prnt(SAVE_PS)=="Yes"} {
	set filetypes {
	    {{Postscript Files}       {.ps}        }
	    {{All Files}        *             }
	}
	.lisa.frame_for_all_canvas.fra32.cpd34.03 postscript \
	    -file [tk_getSaveFile  -defaultextension .xml \
		       -filetypes  { { {ps} {.ps} } } \
		       -title $Name(Save_File)] \
	    -colormode $prnt(COLOR) -height $prnt(HEIGHT) -width $prnt(WIDTH) \
	    -pageheight $prnt(PAGEHEIGHT) -pagewidth $prnt(PAGEWIDTH)
    } else {
	catch { exec  $prnt(PRNT_COMMAND) << \
		    [ .lisa.frame_for_all_canvas.fra32.cpd34.03 postscript \
			  -colormode  $prnt(COLOR) -height $prnt(HEIGHT) -width $prnt(WIDTH) \
			  -pageheight  $prnt(PAGEHEIGHT) -pagewidth $prnt(PAGEWIDTH) ]
	}
    }
}


