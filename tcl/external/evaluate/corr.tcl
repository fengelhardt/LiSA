#############################################################################
# Visual Tcl v1.11p1 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.corr {base} {
    global corr;
    global Name;
    if {$base == ""} {
        set base .corr
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -menu .corr.m56 
    wm focusmodel $base passive
    wm geometry $base 381x379+442+200
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Correlation)
    frame $base.fra70 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
        -padx 11 -pady 4 -text OK -command {TC_corr; destroy .corr }
    frame $base.fra70.fra91 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
        -padx 11 -pady 4 -text CANCEL -command {destroy .corr }
    frame $base.fra17 \
        -borderwidth 2 -height 20 -width 125 
    frame $base.fra18 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra20 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra19 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra19.rad28 \
        -highlightthickness 0 -text $Name(Covariance) \
	-value "cov"  -variable corr(type)
    frame $base.fra20.fra19.fra29 \
        -borderwidth 2 -height 15 -width 20 
    radiobutton $base.fra20.fra19.rad30 \
        -highlightthickness 0 \
        -text $Name(Correlation_coefficient) \
	-value "corrc"  -variable corr(type)
    frame $base.fra20.fra20 \
        -borderwidth 2 -height 30 -width 20 
    frame $base.fra20.fra22 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra22.fra53 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra22.fra53.fra67 \
        -borderwidth 2 -height 3 -width 15 
    frame $base.fra20.fra22.fra53.fra68 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra22.fra53.fra68.01 \
        -borderwidth 1 \
        -text $Name(for)
    frame $base.fra20.fra22.fra53.fra68.02 \
        -borderwidth 2 -height 10 -width 10 
    frame $base.fra20.fra22.fra53.fra69 \
        -borderwidth 2 -height 57 -width 20 
    frame $base.fra20.fra22.fra53.fra70 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra22.fra53.fra70.01 \
        -borderwidth 1 \
        -text $Name(with)
    frame $base.fra20.fra22.fra53.fra70.02 \
        -borderwidth 2 -height 10 -width 10 
    frame $base.fra20.fra22.fra54 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra22.fra54.fra57 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra22.fra54.fra57.01 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra22.fra54.fra57.01.02 \
        -highlightthickness 0 \
	-text $Name(all_columns1) -value "all"  -variable corr(rc1)
    frame $base.fra20.fra22.fra54.fra57.03 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra22.fra54.fra57.03.04 \
        -highlightthickness 0 \
	-text $Name(Columns:) -value "number"  -variable corr(rc1)
    entry $base.fra20.fra22.fra54.fra57.03.05 \
        -width 0 -textvar corr(columns1)
    frame $base.fra20.fra22.fra54.fra59 \
        -borderwidth 2 -height 20 -width 125 
    frame $base.fra20.fra22.fra54.fra60 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra22.fra54.fra60.01 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra22.fra54.fra60.01.02 \
        -highlightthickness 0 \
	-text $Name(all_columns2) -value "all"  -variable corr(rc2)
    frame $base.fra20.fra22.fra54.fra60.03 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra22.fra54.fra60.03.04 \
        -highlightthickness 0 \
	-text $Name(Columns:) -value "number"  -variable corr(rc2)
    entry $base.fra20.fra22.fra54.fra60.03.05 \
        -width 0 -textvar corr(columns2)
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .corr -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .corr.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .corr.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .corr.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .corr.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .corr.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .corr.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .corr -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .corr -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .corr -anchor center -expand 0 -fill y -side right 
    pack $base.fra20 \
        -in .corr -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.fra19 \
        -in .corr.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra19.rad28 \
        -in .corr.fra20.fra19 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra20.fra19.fra29 \
        -in .corr.fra20.fra19 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra19.rad30 \
        -in .corr.fra20.fra19 -anchor nw -expand 0 -fill none -side top 
    pack $base.fra20.fra20 \
        -in .corr.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra22 \
        -in .corr.fra20 -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.fra22.fra53 \
        -in .corr.fra20.fra22 -anchor center -expand 0 -fill y -side left 
    pack $base.fra20.fra22.fra53.fra67 \
        -in .corr.fra20.fra22.fra53 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra53.fra68 \
        -in .corr.fra20.fra22.fra53 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra53.fra68.01 \
        -in .corr.fra20.fra22.fra53.fra68 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra53.fra68.02 \
        -in .corr.fra20.fra22.fra53.fra68 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra53.fra69 \
        -in .corr.fra20.fra22.fra53 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra53.fra70 \
        -in .corr.fra20.fra22.fra53 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra53.fra70.01 \
        -in .corr.fra20.fra22.fra53.fra70 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra53.fra70.02 \
        -in .corr.fra20.fra22.fra53.fra70 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra54 \
        -in .corr.fra20.fra22 -anchor center -expand 1 -fill both -side left 
    pack $base.fra20.fra22.fra54.fra57 \
        -in .corr.fra20.fra22.fra54 -anchor nw -expand 0 -fill x -side top 
    pack $base.fra20.fra22.fra54.fra57.01 \
        -in .corr.fra20.fra22.fra54.fra57 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra54.fra57.01.02 \
        -in .corr.fra20.fra22.fra54.fra57.01 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra54.fra57.03 \
        -in .corr.fra20.fra22.fra54.fra57 -anchor center -expand 1 -fill x \
        -side left 
    pack $base.fra20.fra22.fra54.fra57.03.04 \
        -in .corr.fra20.fra22.fra54.fra57.03 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra54.fra57.03.05 \
        -in .corr.fra20.fra22.fra54.fra57.03 -anchor center -expand 1 \
        -fill x -side left 
    pack $base.fra20.fra22.fra54.fra59 \
        -in .corr.fra20.fra22.fra54 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra54.fra60 \
        -in .corr.fra20.fra22.fra54 -anchor nw -expand 1 -fill x -side top 
    pack $base.fra20.fra22.fra54.fra60.01 \
        -in .corr.fra20.fra22.fra54.fra60 -anchor nw -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra22.fra54.fra60.01.02 \
        -in .corr.fra20.fra22.fra54.fra60.01 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra54.fra60.03 \
        -in .corr.fra20.fra22.fra54.fra60 -anchor center -expand 1 -fill x \
        -side left 
    pack $base.fra20.fra22.fra54.fra60.03.04 \
        -in .corr.fra20.fra22.fra54.fra60.03 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra20.fra22.fra54.fra60.03.05 \
        -in .corr.fra20.fra22.fra54.fra60.03 -anchor center -expand 1 \
        -fill x -side left 
}

#Window show .
#Window show .corr

#main $argc $argv



