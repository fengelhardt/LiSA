set ext_algo(NAME,nb_iter) $Name(Neighborhood_Search)
set ext_algo(TYPE,neighbor) iterative

proc vTclWindow.neighbor {base} {

    global Name;
    global neighbor
    global ext_algo

    set neighbor(ALGO_CALL) nb_iter
    set neighbor(NAME) "Neighborhood Search (neighbor(NAME), obsolet?)"
    set neighbor(CODE) external
    set neighbor(HELPFILE) german_u/neighbor.html

    global system
    if {$base == ""} {
        set base .neighbor
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel 
    wm focusmodel $base passive
    if {$system(screen)=="big"} {
	wm geometry $base 547x527
	wm maxsize $base 1265 994
	set w [winfo reqwidth $base]
	set h [winfo reqheight $base]
	wm minsize $base $w $h
    } else {
	wm geometry $base 400x425
    }
    
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Neighborhood_search)
    wm transient $base .lisa

    frame $base.fra20 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra20.lab21 \
        -anchor w  -borderwidth 1 \
         -justify left -text $Name(NGBH) -width 30 
    menubutton $base.fra20.men22 \
	-indicatoron 1 -menu .neighbor.fra20.men22.m -padx 5 -pady 4 \
        -relief sunken -text OSP_BL_SHIFT 
    menu $base.fra20.men22.m \
         -cursor {} \
	-tearoff 0 
    $base.fra20.men22.m add command \
        -label $Name(API) \
	-command {nb_set "NGBH" "API"}
    $base.fra20.men22.m add command  \
        -label $Name(SHIFT) \
	-command  {nb_set "NGBH" "SHIFT"}
    if {1 != [TC_getvar get_m]} {
    $base.fra20.men22.m add command  \
        -label $Name(3_CR) \
	-command  {nb_set "NGBH" "3_CR"} 
    $base.fra20.men22.m add command  \
        -label $Name(CR_API) \
	-command  {nb_set "NGBH" "CR_API"}
    $base.fra20.men22.m add command  \
        -label $Name(BL_API) \
	-command  {nb_set "NGBH" "BL_API"}
    $base.fra20.men22.m add command  \
        -label $Name(CR_SHIFT) \
	-command  {nb_set "NGBH" "CR_SHIFT"}
    $base.fra20.men22.m add command  \
        -label $Name(BL_SHIFT) \
	-command  {nb_set "NGBH" "BL_SHIFT"}
    }
    frame $base.fra23 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra23.lab24 \
        -anchor w  -borderwidth 1 \
	-text $Name(METHOD) -width 30 
    menubutton $base.fra23.men25 \
	-menu .neighbor.fra23.men25.m -padx 5 -pady 4 \
        -relief sunken -text $Name(SA) -indicatoron 1
    menu $base.fra23.men25.m \
	-cursor {} \
	-tearoff 0 
    $base.fra23.men25.m add command \
        -label $Name(SA) \
	-command  {nb_set "METHOD" "SA"}
    $base.fra23.men25.m add command \
        -label $Name(TA) \
	-command  {nb_set "METHOD" "TA"}
    $base.fra23.men25.m add command \
        -label $Name(TS) \
	-command  {nb_set "METHOD" "TS"}
    $base.fra23.men25.m add command \
        -label $Name(II) \
	-command  {nb_set "METHOD" "II"}
    frame $base.fra26 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra26.lab27 \
        -anchor w  -borderwidth 1 \
	-text $Name(TYPE) -width 30 
    menubutton $base.fra26.men28 \
	-menu .neighbor.fra26.men28.m -padx 5 -pady 4 \
        -relief sunken -text $Name(RAND) -indicatoron 1
    menu $base.fra26.men28.m \
         -cursor {} \
         -tearoff 0 
    $base.fra26.men28.m add command \
        -label $Name(RAND) \
	-command  {nb_set "TYPE" "RAND"}
    $base.fra26.men28.m add command \
        -label $Name(ENUM) \
	-command  {nb_set "TYPE" "ENUM"}
    frame $base.fra30 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra30.lab31 \
        -anchor w  -borderwidth 1 \
	-text $Name(STEPS) -width 30 
    set neighbor(1,TYPE) long
    set neighbor(1,NAME_OF_PARAMETER) STEPS
    entry $base.fra30.ent32 -textvar neighbor(STEPS)
    frame $base.fra39 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra39.lab40 \
         -borderwidth 1 \
         -text $Name(ONLY_SA_TA) 
    frame $base.fra41 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra41.lab42 \
        -anchor w  -borderwidth 1 \
         -text $Name(PROB) -width 30 
    set neighbor(2,TYPE) long
    set neighbor(2,NAME_OF_PARAMETER) PROB
    entry $base.fra41.ent43 -textvar neighbor(PROB) 
    frame $base.fra44 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra44.lab45 \
        -anchor w  -borderwidth 1 \
          -text $Name(MAX_STUCK) -width 30
    set neighbor(3,TYPE) long
    set neighbor(3,NAME_OF_PARAMETER) MAX_STUCK	
    entry $base.fra44.ent46 -textvar neighbor(MAX_STUCK) 
    frame $base.fra47 \
         -borderwidth 2 -height 75 -width 125 
    label $base.fra47.lab48 \
         -borderwidth 1 \
         -text $Name(ONLY_TS) 
    frame $base.fra49 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra49.lab50 \
        -anchor w  -borderwidth 1 \
	-text $Name(TABULENGTH) -width 30 
    set neighbor(4,TYPE) long
    set neighbor(4,NAME_OF_PARAMETER) TABULENGTH
    entry $base.fra49.ent51 -textvar neighbor(TABULENGTH)
    frame $base.fra52 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra52.lab53 \
        -anchor w  -borderwidth 1 \
         -text $Name(NUMB_NGHB) -width 30 
    set neighbor(5,TYPE) long
    set neighbor(5,NAME_OF_PARAMETER) NUMB_NGHB
    entry $base.fra52.ent54 -textvar neighbor(NUMB_NGHB)        
    frame $base.fra33 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra33.lab34 \
        -anchor w  -borderwidth 1 \
	-text $Name(NUMB_STUCKS) -width 30
    set neighbor(6,TYPE) long
    set neighbor(6,NAME_OF_PARAMETER) NUMB_STUCKS
    entry $base.fra33.ent35 -textvar neighbor(NUMB_STUCKS)
    frame $base.fra36 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra36.lab37 \
        -anchor w  -borderwidth 1 \
         -text $Name(ABORT_BOUND) -width 30
    set neighbor(7,TYPE) double
    set neighbor(7,NAME_OF_PARAMETER) ABORT_BOUND
    entry $base.fra36.ent38 -textvar neighbor(ABORT_BOUND)    
    frame $base.fra18 \
         -borderwidth 2 -height 75 -width 125 
     label $base.fra18.lab19 \
          -borderwidth 1 \
          -text $Name(Abort_criteria) 
    frame $base.fra57 \
         -borderwidth 2 -height 75 -width 125 
    button $base.fra57.but59 \
         -borderwidth 4 \
	-padx 11 -pady 4 -text $Name(Cancel) \
	-command {destroy .neighbor}
    button $base.fra57.but58 \
         -borderwidth 4 \
         -padx 11 -pady 4 -text $Name(START_NB) \
	-command {destroy .neighbor; TC_startalg neighbor }
    button $base.fra57.but60 \
         -borderwidth 4 \
         -padx 11 -pady 4 -text $Name(Help) \
	-command { lisa_help neighbor.htm }
    ###################
    # SETTING GEOMETRY
    ###################
    
    pack $base.fra20 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra20.lab21 \
        -in .neighbor.fra20 -anchor center -expand 0 -fill none -side left 
    pack $base.fra20.men22 \
        -in .neighbor.fra20 -anchor center -expand 0 -fill x -side top 
    pack $base.fra23 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.lab24 \
        -in .neighbor.fra23 -anchor center -expand 0 -fill none -side left 
    pack $base.fra23.men25 \
        -in .neighbor.fra23 -anchor center -expand 0 -fill x -side top 
    pack $base.fra26 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.lab27 \
        -in .neighbor.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.men28 \
        -in .neighbor.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra30 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra30.lab31 \
        -in .neighbor.fra30 -anchor center -expand 0 -fill none -side left 
    pack $base.fra30.ent32 \
        -in .neighbor.fra30 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra39 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra39.lab40 \
        -in .neighbor.fra39 -anchor center -expand 0 -fill none -ipady 2 \
        -side top 
    pack $base.fra41 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra41.lab42 \
        -in .neighbor.fra41 -anchor center -expand 0 -fill none -side left 
    pack $base.fra41.ent43 \
        -in .neighbor.fra41 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra44 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra44.lab45 \
        -in .neighbor.fra44 -anchor center -expand 0 -fill none -side left 
    pack $base.fra44.ent46 \
        -in .neighbor.fra44 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra47 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra47.lab48 \
        -in .neighbor.fra47 -anchor center -expand 0 -fill none -ipady 2 \
        -side top 
    pack $base.fra49 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra49.lab50 \
        -in .neighbor.fra49 -anchor center -expand 0 -fill none -side left 
    pack $base.fra49.ent51 \
        -in .neighbor.fra49 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra52 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra52.lab53 \
        -in .neighbor.fra52 -anchor center -expand 0 -fill none -side left 
    pack $base.fra52.ent54 \
        -in .neighbor.fra52 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra18 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra18.lab19 \
         -in .neighbor.fra18 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra33 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra33.lab34 \
        -in .neighbor.fra33 -anchor center -expand 0 -fill none -side left 
    pack $base.fra33.ent35 \
        -in .neighbor.fra33 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra36 \
        -in .neighbor -anchor center -expand 0 -fill x -side top 
    pack $base.fra36.lab37 \
        -in .neighbor.fra36 -anchor center -expand 0 -fill none -side left 
    pack $base.fra36.ent38 \
        -in .neighbor.fra36 -anchor center -expand 0 -fill x -ipady 2 -side top 
    pack $base.fra57 \
        -in .neighbor -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra57.but58 \
        -in .neighbor.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side left 
    pack $base.fra57.but59 \
        -in .neighbor.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side left
    pack $base.fra57.but60 \
        -in .neighbor.fra57 -anchor center -expand 0 -fill none -ipadx 10 \
        -ipady 2 -padx 30 -pady 10 -side right
  
    set neighbor(8,TYPE) string
    set neighbor(8,NAME_OF_PARAMETER) NGBH
    set neighbor(9,TYPE) string
    set neighbor(9,NAME_OF_PARAMETER) METHOD
    set neighbor(10,TYPE) string
    set neighbor(10,NAME_OF_PARAMETER) TYPE
  
    nb_set NGBH $neighbor(NGBH)
    nb_set METHOD $neighbor(METHOD)
    nb_set TYPE $neighbor(TYPE)

     wm geometry $base 590x527
    set neighbor(NO_OF_ENTRYS) 10
      set ext_algo(objective) 0
    set ext_algo(running) 1

    bind $base <Visibility> {  
	grab set .neighbor;  
	bind .neighbor <Visibility> { } }
    
    
}


proc nb_set {name value} {
    global neighbor;
    global Name;
    if {$name == "NGBH"} {
	set neighbor(NGBH) $value
	.neighbor.fra20.men22 config -text $Name($value)
    }
    if {$name == "METHOD"} {
	set neighbor(METHOD) $value
	.neighbor.fra23.men25 config -text $Name($value)
    }
    if {$name == "TYPE"} {
	set neighbor(TYPE) $value
	.neighbor.fra26.men28  config -text $Name($value)
    }
}










