#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.confid {base} {
    global confid;
    global Name;
    if {$base == ""} {
        set base .confid
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -menu .confid.m56 
    wm focusmodel $base passive
    wm geometry $base 529x503+128+188
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Confidence)
    frame $base.fra70 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
        -padx 11 -pady 4 -text OK -command {TC_confid; destroy .confid }
    frame $base.fra70.fra91 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
        -padx 11 -pady 4 -text CANCEL -command {destroy .confid }
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
    label $base.fra20.fra17.lab20 \
        -borderwidth 1 \
        -text $Name(Fiducial_probability)
    frame $base.fra20.fra17.fra23 \
        -borderwidth 2 -height 10 -width 20 
    label $base.fra20.fra17.lab24 \
        -borderwidth 1 \
        -relief raised -text $Name(alpha)
    entry $base.fra20.fra17.ent25 \
        -textvar confid(alpha)
    frame $base.fra20.fra26 \
        -borderwidth 2 -height 20 -width 10 
    frame $base.fra20.fra29 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    frame $base.fra20.fra29.fra61 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra29.fra62 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra29.fra63 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra29.fra64 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra29.fra64.fra65 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra29.fra64.fra65.01 \
        -borderwidth 1 \
        -text $Name(for:)
    frame $base.fra20.fra29.fra64.fra66 \
        -borderwidth 2 -height 10 -width 10 
    frame $base.fra20.fra29.fra64.fra67 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra29.fra64.fra67.01 \
        -borderwidth 2 -height 20 -width 125 
    radiobutton $base.fra20.fra29.fra64.fra67.01.02 \
        -highlightthickness 0 \
	-value "expv" -variable confid(source)
    label $base.fra20.fra29.fra64.fra67.01.03 \
        -borderwidth 1 \
        -text $Name(Expected_value)
    frame $base.fra20.fra29.fra64.fra67.04 \
        -borderwidth 2 -height 20 -width 10 
    frame $base.fra20.fra29.fra64.fra67.05 \
        -borderwidth 2 -height 20 -width 125 
    radiobutton $base.fra20.fra29.fra64.fra67.05.06 \
        -highlightthickness 0 \
	-value "var" -variable confid(source) 
    label $base.fra20.fra29.fra64.fra67.05.07 \
        -borderwidth 1 \
        -text $Name(Variance)
    frame $base.fra20.fra29.fra81 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra57 \
        -borderwidth 2 -height 20 -width 10 
    frame $base.fra20.fra58 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    frame $base.fra20.fra58.fra68 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra58.fra69 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra58.fra70 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra58.fra71 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra58.fra71.fra72 \
        -borderwidth 2 -height 20 -width 125 
    radiobutton $base.fra20.fra58.fra71.fra72.01 \
        -highlightthickness 0  \
	-value "right" -variable confid(interv)
    label $base.fra20.fra58.fra71.fra72.02 \
        -borderwidth 1 \
        -text $Name(One-sided_right) 
    frame $base.fra20.fra58.fra71.fra73 \
        -borderwidth 2 -height 20 -width 10 
    frame $base.fra20.fra58.fra71.fra74 \
        -borderwidth 2 -height 20 -width 125 
    radiobutton $base.fra20.fra58.fra71.fra74.01 \
        -highlightthickness 0 \
	-value "left" -variable confid(interv)
    label $base.fra20.fra58.fra71.fra74.02 \
        -borderwidth 1 \
        -text $Name(One-sided_left)
    frame $base.fra20.fra58.fra71.fra75 \
        -borderwidth 2 -height 20 -width 10 
    frame $base.fra20.fra58.fra71.fra76 \
        -borderwidth 2 -height 20 -width 125 
    radiobutton $base.fra20.fra58.fra71.fra76.01 \
        -highlightthickness 0\
	-value "twos" -variable confid(interv) 
    label $base.fra20.fra58.fra71.fra76.02 \
        -borderwidth 1 \
        -text $Name(Two-sided) 
    frame $base.fra20.fra58.fra82 \
        -borderwidth 2 -height 20 -width 20 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .confid -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .confid.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .confid.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .confid.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .confid.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .confid.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .confid.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .confid -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .confid -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .confid -anchor center -expand 0 -fill y -side right 
    pack $base.fra20 \
        -in .confid -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.fra17 \
        -in .confid.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra17.lab20 \
        -in .confid.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra17.fra23 \
        -in .confid.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra17.lab24 \
        -in .confid.fra20.fra17 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra17.ent25 \
        -in .confid.fra20.fra17 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra26 \
        -in .confid.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra29 \
        -in .confid.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra29.fra61 \
        -in .confid.fra20.fra29 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra29.fra62 \
        -in .confid.fra20.fra29 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra29.fra63 \
        -in .confid.fra20.fra29 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra20.fra29.fra64 \
        -in .confid.fra20.fra29 -anchor nw -expand 1 -fill x -side top 
    pack $base.fra20.fra29.fra64.fra65 \
        -in .confid.fra20.fra29.fra64 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra29.fra64.fra65.01 \
        -in .confid.fra20.fra29.fra64.fra65 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra29.fra64.fra66 \
        -in .confid.fra20.fra29.fra64 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra29.fra64.fra67 \
        -in .confid.fra20.fra29.fra64 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra29.fra64.fra67.01 \
        -in .confid.fra20.fra29.fra64.fra67 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra29.fra64.fra67.01.02 \
        -in .confid.fra20.fra29.fra64.fra67.01 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra29.fra64.fra67.01.03 \
        -in .confid.fra20.fra29.fra64.fra67.01 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra29.fra64.fra67.04 \
        -in .confid.fra20.fra29.fra64.fra67 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra29.fra64.fra67.05 \
        -in .confid.fra20.fra29.fra64.fra67 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra29.fra64.fra67.05.06 \
        -in .confid.fra20.fra29.fra64.fra67.05 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra29.fra64.fra67.05.07 \
        -in .confid.fra20.fra29.fra64.fra67.05 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra29.fra81 \
        -in .confid.fra20.fra29 -anchor s -expand 0 -fill none -side bottom 
    pack $base.fra20.fra57 \
        -in .confid.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra58 \
        -in .confid.fra20 -anchor nw -expand 0 -fill x -side top 
    pack $base.fra20.fra58.fra68 \
        -in .confid.fra20.fra58 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra58.fra69 \
        -in .confid.fra20.fra58 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra58.fra70 \
        -in .confid.fra20.fra58 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra20.fra58.fra71 \
        -in .confid.fra20.fra58 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra20.fra58.fra71.fra72 \
        -in .confid.fra20.fra58.fra71 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra58.fra71.fra72.01 \
        -in .confid.fra20.fra58.fra71.fra72 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra58.fra71.fra72.02 \
        -in .confid.fra20.fra58.fra71.fra72 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra58.fra71.fra73 \
        -in .confid.fra20.fra58.fra71 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra58.fra71.fra74 \
        -in .confid.fra20.fra58.fra71 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra58.fra71.fra74.01 \
        -in .confid.fra20.fra58.fra71.fra74 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra58.fra71.fra74.02 \
        -in .confid.fra20.fra58.fra71.fra74 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra58.fra71.fra75 \
        -in .confid.fra20.fra58.fra71 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra58.fra71.fra76 \
        -in .confid.fra20.fra58.fra71 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra58.fra71.fra76.01 \
        -in .confid.fra20.fra58.fra71.fra76 -anchor nw -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra58.fra71.fra76.02 \
        -in .confid.fra20.fra58.fra71.fra76 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra20.fra58.fra82 \
        -in .confid.fra20.fra58 -anchor s -expand 0 -fill none -side bottom 
}

# Window show .
# Window show .confid

# main $argc $argv
