#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.pie {base} {
    global pie;
    global Name;
    if {$base == ""} {
        set base .pie
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -menu .pie.m56 
    wm focusmodel $base passive
    wm geometry $base 358x435+349+234
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Pie_chart)
    frame $base.fra70 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra70.fra80 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra80.but83 \
        -padx 11 -pady 4 -text OK -command {TC_pie; destroy .pie }
    frame $base.fra70.fra91 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra70.fra92.01 \
        -padx 11 -pady 4 -text CANCEL  -command {destroy .pie }
    frame $base.fra17 \
        -borderwidth 2 -height 20 -width 125 
    frame $base.fra18 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra20 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra20.01 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.01.02 \
        -highlightthickness 0 \
	-value "number"  -variable pie(value)
    label $base.fra20.01.03 \
        -borderwidth 1 \
        -text $Name(Column:)
    frame $base.fra20.01.fra39 \
        -borderwidth 2 -height 10 -width 10 
    entry $base.fra20.01.ent40 \
        -width 4 \
	-textvar pie(column)
    frame $base.fra20.04 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra35 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra35.01 \
        -highlightthickness 0 \
	-value "expv"  -variable pie(value)
    label $base.fra20.fra35.02 \
        -borderwidth 1 \
        -text $Name(Expected_value) 
    frame $base.fra20.fra36 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra38 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra38.01 \
        -highlightthickness 0\
	-value "var"  -variable pie(value) 
    label $base.fra20.fra38.02 \
        -borderwidth 1 \
        -text $Name(Variance) 
    frame $base.fra20.fra41 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra42 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra42.01 \
        -highlightthickness 0 \
	-value "min"  -variable pie(value)
    label $base.fra20.fra42.02 \
        -borderwidth 1 \
        -text $Name(Minimum)
    frame $base.fra20.fra43 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra44 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra44.01 \
        -highlightthickness 0 \
	-value "max"  -variable pie(value)
    label $base.fra20.fra44.02 \
        -borderwidth 1 \
        -text $Name(Maximum)
    frame $base.fra20.fra45 \
        -borderwidth 2 -height 20 -width 20 
    frame $base.fra20.fra46 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra20.fra46.01 \
        -highlightthickness 0 \
	-value "stdev"  -variable pie(value)
    label $base.fra20.fra46.02 \
        -borderwidth 1 \ 
        -text $Name(Standard_deviation)
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra70 \
        -in .pie -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78 \
        -in .pie.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80 \
        -in .pie.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83 \
        -in .pie.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91 \
        -in .pie.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92 \
        -in .pie.fra70 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01 \
        -in .pie.fra70.fra92 -anchor center -expand 0 -fill none \
        -side right 
    pack $base.fra17 \
        -in .pie -anchor center -expand 0 -fill x -side top 
    pack $base.fra18 \
        -in .pie -anchor center -expand 0 -fill y -side left 
    pack $base.fra19 \
        -in .pie -anchor center -expand 0 -fill y -side right 
    pack $base.fra20 \
        -in .pie -anchor center -expand 1 -fill both -side top 
    pack $base.fra20.01 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.01.02 \
        -in .pie.fra20.01 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.01.03 \
        -in .pie.fra20.01 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.01.fra39 \
        -in .pie.fra20.01 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.01.ent40 \
        -in .pie.fra20.01 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.04 \
        -in .pie.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra35 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra35.01 \
        -in .pie.fra20.fra35 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra35.02 \
        -in .pie.fra20.fra35 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra36 \
        -in .pie.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra38 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra38.01 \
        -in .pie.fra20.fra38 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra38.02 \
        -in .pie.fra20.fra38 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra41 \
        -in .pie.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra42 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra42.01 \
        -in .pie.fra20.fra42 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra42.02 \
        -in .pie.fra20.fra42 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra43 \
        -in .pie.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra44 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra44.01 \
        -in .pie.fra20.fra44 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra44.02 \
        -in .pie.fra20.fra44 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra45 \
        -in .pie.fra20 -anchor center -expand 0 -fill none -side top 
    pack $base.fra20.fra46 \
        -in .pie.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.fra46.01 \
        -in .pie.fra20.fra46 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.fra46.02 \
        -in .pie.fra20.fra46 -anchor center -expand 0 -fill none -side left 
}

# Window show .
# Window show .pie

# main $argc $argv
