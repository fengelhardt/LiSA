#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.characte {base} {
    global characte;
    global Name;
    if {$base == ""} {
        set base .characte
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -menu .characte.m56 
    wm focusmodel $base passive
    wm geometry $base 382x388+319+229
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Characteristica)
    frame $base.fra70 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
        -padx 11 -pady 4 -text OK  -command {TC_characte; destroy .characte }
    frame $base.fra70.fra91 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
        -padx 11 -pady 4 -text CANCEL  -command { destroy .characte }
    frame $base.fra17 \
        -borderwidth 2 -height 20 -width 125 
    frame $base.fra18 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra29 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra29.fra30 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra29.fra30.04 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra29.fra30.04.05 \
        -highlightthickness 0 \
	-text $Name(Expected_value) \
        -variable characte(cexpv) 
    frame $base.fra29.fra30.06 \
        -borderwidth 2 -height 6 -width 20 
    frame $base.fra29.fra30.07 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra29.fra30.07.08 \
        -highlightthickness 0 -text $Name(Minimum) \
        -variable characte(cmin) 
    frame $base.fra29.fra30.09 \
        -borderwidth 2 -height 6 -width 20 
    frame $base.fra29.fra30.010 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra29.fra30.010.011 \
        -highlightthickness 0 -text $Name(Maximum) \
        -variable characte(cmax) 
    frame $base.fra29.fra30.012 \
        -borderwidth 2 -height 6 -width 20 
    frame $base.fra29.fra30.013 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra29.fra30.013.014 \
        -highlightthickness 0 -text $Name(Variance) \
        -variable characte(cvar) 
    frame $base.fra29.fra30.015 \
        -borderwidth 2 -height 6 -width 20 
    frame $base.fra29.fra30.016 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra29.fra30.016.017 \
        -highlightthickness 0 \
        -text $Name(Standard_deviation) -variable characte(cstdev) 
    frame $base.fra29.fra31 \
        -borderwidth 2 -height 30 -width 125 
    frame $base.fra29.fra33 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra29.fra33.lab36 \
        -borderwidth 1 \
        -text $Name(for)
    frame $base.fra29.fra47 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra29.fra47.rad48 \
        -highlightthickness 0 -text $Name(all)\
	-value "all" -variable characte(rc)
    frame $base.fra29.fra50 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra29.fra50.01 \
        -highlightthickness 0 -text $Name(Columns:) \
	-value "number" -variable characte(rc)
    entry $base.fra29.fra50.02 \
        -textvar characte(columns)
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .characte -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .characte.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .characte.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .characte.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .characte.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .characte.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .characte.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .characte -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .characte -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .characte -anchor center -expand 0 -fill y -side right 
    pack $base.fra29 \
        -in .characte -anchor center -expand 0 -fill both -side top 
    pack $base.fra29.fra30 \
        -in .characte.fra29 -anchor nw -expand 1 -fill x -side top 
    pack $base.fra29.fra30.04 \
        -in .characte.fra29.fra30 -anchor nw -expand 0 -fill x -side top 
    pack $base.fra29.fra30.04.05 \
        -in .characte.fra29.fra30.04 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra29.fra30.06 \
        -in .characte.fra29.fra30 -anchor center -expand 0 -fill none -side top 
    pack $base.fra29.fra30.07 \
        -in .characte.fra29.fra30 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra30.07.08 \
        -in .characte.fra29.fra30.07 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra29.fra30.09 \
        -in .characte.fra29.fra30 -anchor center -expand 0 -fill none -side top 
    pack $base.fra29.fra30.010 \
        -in .characte.fra29.fra30 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra30.010.011 \
        -in .characte.fra29.fra30.010 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra29.fra30.012 \
        -in .characte.fra29.fra30 -anchor center -expand 0 -fill none -side top 
    pack $base.fra29.fra30.013 \
        -in .characte.fra29.fra30 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra30.013.014 \
        -in .characte.fra29.fra30.013 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra29.fra30.015 \
        -in .characte.fra29.fra30 -anchor center -expand 0 -fill none -side top 
    pack $base.fra29.fra30.016 \
        -in .characte.fra29.fra30 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra30.016.017 \
        -in .characte.fra29.fra30.016 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra29.fra31 \
        -in .characte.fra29 -anchor nw -expand 0 -fill x -side top 
    pack $base.fra29.fra33 \
        -in .characte.fra29 -anchor center -expand 0 -fill y -side left 
    pack $base.fra29.fra33.lab36 \
        -in .characte.fra29.fra33 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra29.fra47 \
        -in .characte.fra29 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra47.rad48 \
        -in .characte.fra29.fra47 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra29.fra50 \
        -in .characte.fra29 -anchor nw -expand 0 -fill x -side top 
    pack $base.fra29.fra50.01 \
        -in .characte.fra29.fra50 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra29.fra50.02 \
        -in .characte.fra29.fra50 -anchor center -expand 0 -fill x -side bottom 
}

# Window show .
# Window show .characte

# main $argc $argv








