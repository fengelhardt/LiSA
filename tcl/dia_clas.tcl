#!/usr/local/bin/wish8.0
#############################################################################
# Visual Tcl v1.10 Project
#

#################################
# GLOBAL VARIABLES
#
global widget; 

#################################
# USER DEFINED PROCEDURES
#




proc vTclWindow.textaus {base} {
    global Name
    global system
    
    if {$base == ""} {
        set base .textaus
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
  #   wm geometry $base 531x350+164+202
#     wm maxsize $base 1009 738

    if {$system(screen)=="big"} {
	wm geometry $base  531x350+164+202
	wm maxsize $base 1267 977
    } else {
	wm geometry $base 400x300
    }
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(TEXTAUSGABE)
    wm transient $base .lisa
    frame $base.fra19 \
        -borderwidth 2 -height 59 -relief groove \
        -width 125 
    button $base.fra19.but19 \
        -command {TC_references} \
        -padx 11 -pady 4 -text $Name(References) 
    button $base.fra19.but20 \
        -command {lisa_help classify.htm} \
        -padx 11 -pady 4 -text $Name(Help) 
    button $base.fra19.but18 \
        -command {destroy .textaus} \
        -padx 11 -pady 4 -text $Name(Close) 
    scrollbar $base.scr21 \
        -command {.textaus.fra22.tex23 yview} \
        -orient vert 
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    text $base.fra22.tex23 \
        -wrap word \
        -yscrollcommand {.textaus.scr21 set} 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra19 \
        -in .textaus -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra19.but19 \
        -in .textaus.fra19 -anchor w  -side left \
        -expand 1 -fill both -padx 8 -pady 8 
    pack $base.fra19.but20 \
        -in .textaus.fra19 -anchor center  -side left \
        -expand 1 -fill both -padx 8 -pady 8 
    pack $base.fra19.but18 \
        -in .textaus.fra19 -anchor e  -side right \
        -expand 1 -fill both -padx 8 -pady 8
    pack $base.scr21 \
        -in .textaus -anchor center -expand 0 -fill y -side right 
    pack $base.fra22 \
        -in .textaus -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.tex23 \
        -in .textaus.fra22 -anchor center -expand 1 -fill both -side top 
}
