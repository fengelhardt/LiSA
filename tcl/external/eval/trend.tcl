#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.trend {base} {
    global trend;
    global Name;
    if {$base == ""} {
        set base .trend
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -menu .trend.m56 
    wm focusmodel $base passive
    wm geometry $base 621x476+226+125
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Trend_line)
    frame $base.fra70 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
        -padx 11 -pady 4 -text OK -command {TC_trend; destroy .trend }
    frame $base.fra70.fra91 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
        -padx 11 -pady 4 -text CANCEL -command {destroy .trend }
    frame $base.fra17 \
        -borderwidth 2 -height 30 -width 125 
    frame $base.fra18 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra20 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra17 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra17.fra18 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra17.fra18.lab19 \
        -borderwidth 1 \
        -text $Name(Columns:) 
    frame $base.fra20.fra17.fra20 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra17.fra20.rad23 \
        -highlightthickness 0 \
	-value "number" -variable trend(rc)
    entry $base.fra20.fra17.fra20.ent24 \
        -width 10  -textvar trend(columns)
    frame $base.fra20.fra17.fra35 \
        -borderwidth 2 -height 10 -width 20 
    frame $base.fra20.fra17.fra36 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra17.fra36.01 \
        -highlightthickness 0  \
	-value "all" -variable trend(rc)  
    label $base.fra20.fra17.fra36.lab37 \
        -borderwidth 1 \
        -text $Name(all) 
    frame $base.fra20.fra38 \
        -borderwidth 2 -height 30 -width 20 
    frame $base.fra20.fra19 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra20.fra19.lab24 \
        -borderwidth 1 \
        -text $Name(Type) 
    frame $base.fra20.fra21 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra25 \
        -borderwidth 2 -height 10 -width 20 
    frame $base.fra20.fra26 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.fra26.01 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra20.fra26.01.02 \
        -highlightthickness 0 -text $Name(linear) \
        -variable trend(clin) 
    frame $base.fra20.fra26.03 \
        -borderwidth 2 -height 10 -width 20 
    frame $base.fra20.fra26.04 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra20.fra26.04.05 \
        -highlightthickness 0 \
	-text $Name(logarithmic) \
        -variable trend(clog) 
    frame $base.fra20.fra26.06 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra20.fra26.06.07 \
        -highlightthickness 0 -text $Name(potential) \
        -variable trend(cpot) 
    frame $base.fra20.fra26.06.08 \
        -borderwidth 2 -height 20 -width 20 
    label $base.fra20.fra26.06.09 \
        -borderwidth 1 \
        -text $Name(Degree) 
    frame $base.fra20.fra26.06.010 \
        -borderwidth 2 -height 20 -width 20 
    scale $base.fra20.fra26.06.011 \
        -from 2.0 -highlightthickness 0 \
        -orient horiz -to 6.0  -variable trend(degree)
    frame $base.fra20.fra26.06.012 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra26.013 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra20.fra26.013.014 \
        -highlightthickness 0 \
	-text $Name(exponential) \
        -variable trend(cexp) 
    frame $base.fra20.fra26.015 \
        -borderwidth 2 -height 75 -width 125 
    checkbutton $base.fra20.fra26.015.016 \
        -highlightthickness 0 \
        -text $Name(moving_average) -variable trend(cma) 
    frame $base.fra20.fra26.015.017 \
        -borderwidth 2 -height 20 -width 20 
    label $base.fra20.fra26.015.018 \
        -borderwidth 1 \
        -text $Name(Periods) 
    frame $base.fra20.fra26.015.019 \
        -borderwidth 2 -height 20 -width 20 
    scale $base.fra20.fra26.015.020 \
        -from 2.0 -highlightthickness 0 \
        -orient horiz -to 6.0  -variable trend(per)
    frame $base.fra20.fra26.015.021 \
        -borderwidth 2 -height 20 -width 20 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .trend -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .trend.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .trend.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .trend.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .trend.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .trend.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .trend.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .trend -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .trend -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .trend -anchor center -expand 0 -fill y -side right 
    pack $base.fra20 \
        -in .trend -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.fra17 \
        -in .trend.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra17.fra18 \
        -in .trend.fra20.fra17 -anchor nw -expand 0 -fill y -side left 
    pack $base.fra20.fra17.fra18.lab19 \
        -in .trend.fra20.fra17.fra18 -anchor center -expand 0 -fill none \
        -side top 
    pack $base.fra20.fra17.fra20 \
        -in .trend.fra20.fra17 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra17.fra20.rad23 \
        -in .trend.fra20.fra17.fra20 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra17.fra20.ent24 \
        -in .trend.fra20.fra17.fra20 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra17.fra35 \
        -in .trend.fra20.fra17 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra17.fra36 \
        -in .trend.fra20.fra17 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra17.fra36.01 \
        -in .trend.fra20.fra17.fra36 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra17.fra36.lab37 \
        -in .trend.fra20.fra17.fra36 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra38 \
        -in .trend.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra19 \
        -in .trend.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra19.lab24 \
        -in .trend.fra20.fra19 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra21 \
        -in .trend.fra20 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra20.fra25 \
        -in .trend.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra26 \
        -in .trend.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.01 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.01.02 \
        -in .trend.fra20.fra26.01 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.03 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra26.04 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.04.05 \
        -in .trend.fra20.fra26.04 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.06 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.06.07 \
        -in .trend.fra20.fra26.06 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.06.08 \
        -in .trend.fra20.fra26.06 -anchor center -expand 1 -fill x -side left 
    pack $base.fra20.fra26.06.09 \
        -in .trend.fra20.fra26.06 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.06.010 \
        -in .trend.fra20.fra26.06 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.06.011 \
        -in .trend.fra20.fra26.06 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.06.012 \
        -in .trend.fra20.fra26.06 -anchor center -expand 1 -fill x -side left 
    pack $base.fra20.fra26.013 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.013.014 \
        -in .trend.fra20.fra26.013 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.015 \
        -in .trend.fra20.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra26.015.016 \
        -in .trend.fra20.fra26.015 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.015.017 \
        -in .trend.fra20.fra26.015 -anchor center -expand 1 -fill x \
        -side left 
    pack $base.fra20.fra26.015.018 \
        -in .trend.fra20.fra26.015 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.015.019 \
        -in .trend.fra20.fra26.015 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.015.020 \
        -in .trend.fra20.fra26.015 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra20.fra26.015.021 \
        -in .trend.fra20.fra26.015 -anchor center -expand 1 -fill x \
        -side left 
}

# Window show .
# Window show .trend

# main $argc $argv

