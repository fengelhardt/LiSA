#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global widget; 

source $env(LISAHOME)/tcl/external/evaluate/german.tcl
source $env(LISAHOME)/tcl/external/evaluate/trend.tcl
source $env(LISAHOME)/tcl/external/evaluate/pie.tcl
source $env(LISAHOME)/tcl/external/evaluate/corr.tcl
source $env(LISAHOME)/tcl/external/evaluate/characte.tcl
source $env(LISAHOME)/tcl/external/evaluate/confid.tcl
source $env(LISAHOME)/tcl/external/evaluate/export.tcl


global export
global characte
global confid
global corr
global pie
global trend


# Vorinitialisierung
 
set export(doc) ps
set export(filename) evaluate
set export(dec_point) .
set export(ctab) 0
set characte(rc) all
set characte(columns) 1
set characte(cexpv) 0
set characte(cmin) 0
set characte(cmax) 0
set characte(cvar) 0 
set characte(cstdev) 0
set confid(alpha) 0.05
set confid(source) expv
set confid(interv) twos
set corr(rc1) all
set corr(rc2) all
set corr(columns1) 1
set corr(columns2) 2
set corr(type) corrc
set pie(column) 1
set pie(value) number
set trend(clin) 0
set trend(clog) 0
set trend(cpot) 0
set trend(cexp) 0
set trend(cma) 0
set trend(rc) all
set trend(columns) 1
set trend(degree) 2
set trend(per) 2





proc Window {args} {
    global vTcl

    set cmd [lindex $args 0]
    set name [lindex $args 1]
    set newname [lindex $args 2]
    set rest [lrange $args 3 end]
    if {$name == "" || $cmd == ""} {return}
    if {$newname == ""} {
        set newname $name
    }
    set exists [winfo exists $newname]
    switch $cmd {
        show {
            if {$exists == "1" && $name != "."} {wm deiconify $name; return}
            if {[info procs vTclWindow(pre)$name] != ""} {
                eval "vTclWindow(pre)$name $newname $rest"
            }
            if {[info procs vTclWindow$name] != ""} {
                eval "vTclWindow$name $newname $rest"
            }
            if {[info procs vTclWindow(post)$name] != ""} {
                eval "vTclWindow(post)$name $newname $rest"
            }
        }
        hide    { if $exists {wm withdraw $newname; return} }
        iconify { if $exists {wm iconify $newname; return} }
        destroy { if $exists {destroy $newname; return} }
    }
}

#################################
# VTCL GENERATED GUI PROCEDURES
#

proc vTclWindow. {base} {
    if {$base == ""} {
        set base .
    }
    ###################
    # CREATING WIDGETS
    ###################
    wm focusmodel $base passive
    wm geometry $base 200x200+0+0
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm withdraw $base
    wm title $base "vt.tcl"
    ###################
    # SETTING GEOMETRY
    ###################
}

proc vTclWindow.menu {base} {

    global menu;
    global Name;
    if {$base == ""} {
        set base .menu
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 583x512+383+194
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Titel)

    frame $base.fra26 -borderwidth 2 -height 75 -relief groove \
        -width 125 
    frame $base.fra26.fra27 -borderwidth 2 -height 75 -width 125 
    menubutton $base.fra26.fra27.men31 \
        -menu .menu.fra26.fra27.men31.m -padx 5 \
        -pady 4 -text $Name(File)
    menu $base.fra26.fra27.men31.m -cursor {} \
        -tearoff 0 
    $base.fra26.fra27.men31.m add command -command {Window show .export}\
        -label $Name(Export)
    $base.fra26.fra27.men31.m add separator
    $base.fra26.fra27.men31.m add command -command {exit}\
        -label $Name(End)
    frame $base.fra26.fra29 -borderwidth 2 -height 75 -width 125 
    menubutton $base.fra26.fra29.men33 \
        -menu .menu.fra26.fra29.men33.m -padx 5 \
        -pady 4 -text $Name(Statistics)
    menu $base.fra26.fra29.men33.m \
        -cursor {} -tearoff 0 
    $base.fra26.fra29.men33.m add command -command {Window show .characte}\
        -label $Name(Characteristica)
    $base.fra26.fra29.men33.m add command -command {Window show .confid}\
        -label $Name(Confidence) -state disabled
    $base.fra26.fra29.men33.m add command -command {Window show .corr}\
        -label $Name(Correlation)
    frame $base.fra26.fra30 \
        -borderwidth 2 -height 75 -width 125 
    menubutton $base.fra26.fra30.men34 \
        -menu .menu.fra26.fra30.men34.m -padx 5 \
        -pady 4 -text $Name(Graphics) 
    menu $base.fra26.fra30.men34.m \
        -cursor {} -tearoff 0 
    $base.fra26.fra30.men34.m add command -command {TC_Table}\
        -label $Name(Table)
    $base.fra26.fra30.men34.m add separator
    $base.fra26.fra30.men34.m add command -command {TC_PointXY}\
        -label $Name(Point_XY)
    $base.fra26.fra30.men34.m add cascade \
        -label $Name(Bar_chart) -menu .menu.fra26.fra30.men34.m.men18
    $base.fra26.fra30.men34.m add command -command {Window show .pie}\
        -label $Name(Pie_chart) -state disabled
    $base.fra26.fra30.men34.m add separator
    $base.fra26.fra30.men34.m add command -command {Window show .trend}\
        -label $Name(Trend_line) -state disabled
    menu $base.fra26.fra30.men34.m.men18 \
        -tearoff 0 
$base.fra26.fra30.men34.m.men18 add command -command {TC_Bar2}\
        -label $Name(2_dimensional)
    $base.fra26.fra30.men34.m.men18 add command -command {TC_Bar3}\
        -label $Name(3_dimensional) -state disabled
    frame $base.frame_for_all_canvas \
	-borderwidth 2 -height 75 -width 125 
    frame $base.frame_for_all_canvas.fra28 \
	-borderwidth 2 -height 50 -width 125 
    frame $base.frame_for_all_canvas.fra28.fra35 \
        -borderwidth 2 -height 40 -width 44 
    frame $base.frame_for_all_canvas.fra28.fra36 \
	-borderwidth 2 -height 50 -width 125 
    frame $base.frame_for_all_canvas.fra28.fra36.fillfr1 \
	-borderwidth 2 -width 13
    canvas $base.frame_for_all_canvas.fra28.fra36.can37 \
	-borderwidth 2 -height 35 -width 374

    frame $base.frame_for_all_canvas.fra32 \
	-borderwidth 2 -height 75 -width 125 
    frame $base.frame_for_all_canvas.fra32.fra33 \
	-borderwidth 2 -height 75 -width 50 
    frame $base.frame_for_all_canvas.fra32.fra33.fillfr2 \
	-borderwidth 2 -height 12
    canvas $base.frame_for_all_canvas.fra32.fra33.can38 \
	-borderwidth 2 -height 262 -width 35 
    frame $base.frame_for_all_canvas.fra32.cpd34 \
	-borderwidth 1 -height 30 -width 30 
    scrollbar $base.frame_for_all_canvas.fra32.cpd34.01 \
	-borderwidth 1 -command {lw_sch } -orient horiz -width 10 
    scrollbar $base.frame_for_all_canvas.fra32.cpd34.02 \
	-borderwidth 1 -command {lw_scv } -orient vert -width 10 
    set hw(canv) [canvas $base.frame_for_all_canvas.fra32.cpd34.03 \
	-background #fefef0 -borderwidth 2 -height 100 -width 100 \
	-xscrollcommand {.menu.frame_for_all_canvas.fra32.cpd34.01 set} \
	-yscrollcommand {.menu.frame_for_all_canvas.fra32.cpd34.02 set} ]

    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra26 \
        -in .menu -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.fra27 \
        -in .menu.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.fra27.men31 \
        -in .menu.fra26.fra27 -anchor center -expand 0 -fill none -side top 
    pack $base.fra26.fra29 \
        -in .menu.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.fra29.men33 \
        -in .menu.fra26.fra29 -anchor center -expand 0 -fill none -side top 
    pack $base.fra26.fra30 \
        -in .menu.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.fra30.men34 \
        -in .menu.fra26.fra30 -anchor center -expand 0 -fill none -side top 
   pack $base.frame_for_all_canvas \
        -in .menu -anchor center -expand 1 -fill both -side top 
    pack $base.frame_for_all_canvas.fra28 \
        -in .menu.frame_for_all_canvas -anchor center -expand 0 -fill x -side top 
    pack $base.frame_for_all_canvas.fra28.fra35 \
        -in .menu.frame_for_all_canvas.fra28 -anchor center -expand 0 -fill none -side left
    pack $base.frame_for_all_canvas.fra28.fra36 \
        -in .menu.frame_for_all_canvas.fra28 -anchor center -expand 0 -fill x -side top
    pack $base.frame_for_all_canvas.fra28.fra36.fillfr1 \
	-in .menu.frame_for_all_canvas.fra28.fra36 -anchor e -expand 0 -side right
    pack $base.frame_for_all_canvas.fra28.fra36.can37 \
        -in .menu.frame_for_all_canvas.fra28.fra36 -anchor center -expand 0 -fill x \
        -side top 
    pack $base.frame_for_all_canvas.fra32 \
        -in .menu.frame_for_all_canvas -anchor center -expand 1 -fill both -side top 
    pack $base.frame_for_all_canvas.fra32.fra33 \
        -in .menu.frame_for_all_canvas.fra32 -anchor center -expand 0 -fill y -side left
    pack $base.frame_for_all_canvas.fra32.fra33.fillfr2 \
	-in .menu.frame_for_all_canvas.fra32.fra33 -anchor s -expand 0 -side bottom
    pack $base.frame_for_all_canvas.fra32.fra33.can38 \
        -in .menu.frame_for_all_canvas.fra32.fra33 -anchor center -expand 1 -fill y \
        -side top 
    pack $base.frame_for_all_canvas.fra32.cpd34 \
        -in .menu.frame_for_all_canvas.fra32 -anchor center -expand 1 -fill both -side top 
    grid columnconf $base.frame_for_all_canvas.fra32.cpd34 0 -weight 1
    grid rowconf $base.frame_for_all_canvas.fra32.cpd34 0 -weight 1
    grid $base.frame_for_all_canvas.fra32.cpd34.01 \
        -in .menu.frame_for_all_canvas.fra32.cpd34 -column 0 -row 1 -columnspan 1 \
        -rowspan 1 -sticky ew 
    grid $base.frame_for_all_canvas.fra32.cpd34.02 \
        -in .menu.frame_for_all_canvas.fra32.cpd34 -column 1 -row 0 -columnspan 1 \
        -rowspan 1 -sticky ns 
    grid $base.frame_for_all_canvas.fra32.cpd34.03 \
        -in .menu.frame_for_all_canvas.fra32.cpd34 -column 0 -row 0 -columnspan 1 \
        -rowspan 1 -sticky nesw 


    $hw(canv) config -scrollregion { 0 0 400 400 }


}

proc lw_scv { args } {
    eval .menu.frame_for_all_canvas.fra32.fra33.can38 yview $args
    eval .menu.frame_for_all_canvas.fra32.cpd34.03 yview $args
}

proc lw_sch { args } {
    eval .menu.frame_for_all_canvas.fra32.cpd34.03 xview $args
    eval .menu.frame_for_all_canvas.fra28.fra36.can37  xview $args
}

Window show .
Window show .menu
TC_Table

#main $argc $argv











