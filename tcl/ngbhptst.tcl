proc vTclWindow.ngbh_paratst {base} {
    global Name;
    global nb;
    global neighbor
    global nbmax
    global maxsteps
    global nboldx
    global system
    
    if {$base == ""} {
        set base .ngbh_paratst
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################    # CREATING WIDGETS
    ###################
    set nb(x) 551
    set nb(y) 62
    toplevel $base -class Toplevel 
    wm focusmodel $base passive
    if {$system(screen)=="big"} {
	wm geometry $base 500x554+[expr $nb(x)-5]+[expr $nb(y)-23]
	wm maxsize $base 1265 994
	set w [winfo reqwidth $base]
	set h [winfo reqheight $base]
	wm minsize $base $w $h
    } else {
	wm geometry $base 500x554
    }
    
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Neighborhood_Parameter_Test)
    wm transient $base .lisa

#    set nb(NGBH) "API"
#    set nb(METHOD) "SA"
#    set nb(TYPE) "RAND"
#    set nb(STEPS_init) 5000
#    set nb(STEPS_max) 5000
#    set nb(STEPS_incr) 2
#    set nb(STEPS_FA) "FACTOR"
#    set nb(PROB_init) 3
#    set nb(PROB_max) 12
#    set nb(PROB_incr) 2
#    set nb(PROB_FA) "FACTOR"
#    set nb(MAX_STUCK_init) 30
#    set nb(MAX_STUCK_max) 30
#    set nb(MAX_STUCK_incr) 2
#    set nb(MAX_STUCK_FA) "FACTOR"
#    set nb(TABULENGTH_init) 10
#    set nb(TABULENGTH_max) 100
#    set nb(TABULENGTH_incr) 4
#    set nb(TABULENGTH_FA) "FACTOR"
#    set nb(NUMB_NGHB_init) 10
#    set nb(NUMB_NGHB_max) 100
#    set nb(NUMB_NGHB_incr) 4
#    set nb(NUMB_NGHB_FA) "FACTOR"
#    set nb(ABORT_BOUND) ""
#    set nb(FILE) ""

    frame $base.fra18 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra18.lab19 \
        -borderwidth 1 \
	-relief sunken -text $Name(Fix_Param) 
    frame $base.fra20 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra20.lab21 \
        -borderwidth 1 \
	-relief raised -text $Name(NGBH) 
    menubutton $base.fra20.men22 \
        -indicatoron 1 \
	-menu $base.fra20.men22.m -padx 5 -pady 4 \
        -relief raised -text $Name(API) 
    menu $base.fra20.men22.m \
        -cursor {} \
	-tearoff 0
    $base.fra20.men22.m add command \
        -label $Name(API) \
	-command {nbtst_set "NGBH" "API"}
    $base.fra20.men22.m add command \
        -label $Name(SHIFT) \
	-command {nbtst_set "NGBH" "SHIFT"}
    if {1 != [TC_getvar get_m]} {
    $base.fra20.men22.m add command \
        -label $Name(CR_API) \
	-command {nbtst_set "NGBH" "CR_API"}
    $base.fra20.men22.m add command \
        -label $Name(3_CR) \
        -command {nbtst_set "NGBH" "3_CR"}
    $base.fra20.men22.m add command \
        -label $Name(BL_API) \
	-command {nbtst_set "NGBH" "BL_API"}
    $base.fra20.men22.m add command \
        -label $Name(CR_SHIFT) \
	-command {nbtst_set "NGBH" "CR_SHIFT"}
    $base.fra20.men22.m add command \
        -label $Name(BL_SHIFT) \
	-command {nbtst_set "NGBH" "BL_SHIFT"}}
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra23.lab24 \
        -borderwidth 1 \
	-relief raised -text $Name(METHOD) 
    menubutton $base.fra23.men25 \
	-indicatoron 1 \
	-menu $base.fra23.men25.m -padx 5 -pady 4 \
        -relief raised -text $Name(II) 
    menu $base.fra23.men25.m \
        -cursor {} \
	-tearoff 0
    $base.fra23.men25.m add command \
        -label $Name(SA) \
	-command {nbtst_set "METHOD" "SA";\
	 set nb(x) [winfo rootx .ngbh_paratst];\
	 set nb(y) [winfo rooty .ngbh_paratst];\
	 destroy .ngbh_paratst; Window show .ngbh_paratst }
    $base.fra23.men25.m add command \
        -label $Name(TA) \
	-command {nbtst_set "METHOD" "TA";\
         set nb(x) [winfo rootx .ngbh_paratst];\
	 set nb(y) [winfo rooty .ngbh_paratst];\
	 destroy .ngbh_paratst; Window show .ngbh_paratst }
    $base.fra23.men25.m add command \
	-label $Name(TS) \
	-command {nbtst_set "METHOD" "TS";\
	 set nb(x) [winfo rootx .ngbh_paratst];\
	 set nb(y) [winfo rooty .ngbh_paratst];\
	 destroy .ngbh_paratst; Window show .ngbh_paratst }
    $base.fra23.men25.m add command \
        -label $Name(II) \
	-command {nbtst_set "METHOD" "II";\
	 set nb(x) [winfo rootx .ngbh_paratst];\
	 set nb(y) [winfo rooty .ngbh_paratst];\
	 destroy .ngbh_paratst; Window show .ngbh_paratst }
    frame $base.fra26 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra26.lab27 \
        -borderwidth 1 \
	-relief raised -text $Name(TYPE) 
    menubutton $base.fra26.men28 \
        -indicatoron 1 \
	-menu $base.fra26.men28.m -padx 5 -pady 4 \
        -relief raised -text $Name(RAND) 
    menu $base.fra26.men28.m \
        -cursor {} \
	-tearoff 0
    $base.fra26.men28.m add command \
        -label $Name(ENUM) \
	-command {nbtst_set "TYPE" "ENUM"}
    $base.fra26.men28.m add command \
	-label $Name(RAND) \
	-command {nbtst_set "TYPE" "RAND"}
    frame $base.fra17 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra17.lab18 \
        -borderwidth 1 \
        -relief raised -text $Name(ABORT_BOUND) 
    entry $base.fra17.ent19 \
        -textvar neighbor(ABORT_BOUND)


    frame $base.fra30 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125
    entry $base.fra30.ent32 \
	-textvar nb(FILE) 
    $base.fra30.ent32 xview 200
    button $base.fra30.but31 \
	-padx 11 -pady 4 -text $Name(out_file) \
	-command {nbtst_set "FILE" \
	          [tk_getSaveFile  -defaultextension .lsa \
			           -filetypes $mw(filetypes) \
				   -initialfile $nb(FILE) \
			           -title $Name(out_file)];
                   .ngbh_paratst.fra30.ent32 xview 200}


    frame $base.fra31 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra31.lab32 \
        -borderwidth 1 \
	-relief sunken -text $Name(Var_Param) 
    frame $base.fra33 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra33.lab34 \
        -borderwidth 1 \
	-relief raised -text $Name(STEPS) 
    label $base.fra33.lab35 \
        -borderwidth 1 \
	-relief raised -text $Name(from) 
    label $base.fra33.lab36 \
	-borderwidth 1 \
	-relief raised -text $Name(to) 
    menubutton $base.fra33.men17 \
	-indicatoron 1 \
	-menu $base.fra33.men17.m -padx 5 -pady 4 \
        -relief raised -text $Name(FACTOR) 
    menu $base.fra33.men17.m \
        -cursor {} \
	-tearoff 0
    $base.fra33.men17.m add command \
        -label $Name(FACTOR) \
	-command {nbtst_set "STEPS_FA" "FACTOR"}
    $base.fra33.men17.m add command \
        -label $Name(ADDEND) \
	-command {nbtst_set "STEPS_FA" "ADDEND"}
    entry $base.fra33.ent38 \
	-textvar nb(STEPS_init)
    entry $base.fra33.ent39 \
	-textvar nb(STEPS_max)
    entry $base.fra33.ent40 \
	-textvar nb(STEPS_incr)
#    frame $base.fra44 \
#        -borderwidth 2 -height 75 -relief groove \
#        -width 125 
#    label $base.fra44.lab45 \
#        -borderwidth 1 \
# 	-relief raised -text $Name(ONLY_SA_TA)
if { $neighbor(METHOD)=="SA" || $neighbor(METHOD)=="TA" } {
    frame $base.fra41 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra41.lab42 \
        -borderwidth 1 \
	-relief raised -text $Name(PROB) 
    label $base.fra41.lab43 \
        -borderwidth 1 \
	-relief raised -text $Name(from) 
    label $base.fra41.lab46 \
        -borderwidth 1 \
	-relief raised -text $Name(to) 
    menubutton $base.fra41.men18 \
	-indicatoron 1 \
        -menu $base.fra41.men18.m -padx 5 -pady 4 \
        -relief raised -text $Name(FACTOR) 
    menu $base.fra41.men18.m \
        -cursor {} \
	-tearoff 0
    $base.fra41.men18.m add command \
        -label $Name(FACTOR) \
	-command {nbtst_set "PROB_FA" "FACTOR"}
    $base.fra41.men18.m add command \
        -label $Name(ADDEND) \
	-command {nbtst_set "PROB_FA" "ADDEND"}
    entry $base.fra41.ent48 \
	-textvar nb(PROB_init)
    entry $base.fra41.ent49 \
	-textvar nb(PROB_max)
    entry $base.fra41.ent50 \
	-textvar nb(PROB_incr)
     frame $base.fra51 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra51.lab52 \
        -borderwidth 1 \
	-relief raised -text $Name(MAX_STUCK) 
    label $base.fra51.lab53 \
        -borderwidth 1 \
	-relief raised -text $Name(from) 
    label $base.fra51.lab54 \
        -borderwidth 1 \
	-relief raised -text $Name(to)
    menubutton $base.fra51.men19 \
        -indicatoron 1 \
        -menu $base.fra51.men19.m -padx 5 -pady 4 \
        -relief raised -text $Name(FACTOR) 
    menu $base.fra51.men19.m \
        -cursor {} \
	-tearoff 0
    $base.fra51.men19.m add command \
        -label $Name(FACTOR) \
	-command {nbtst_set "MAX_STUCK_FA" "FACTOR"} 
    $base.fra51.men19.m add command \
        -label $Name(ADDEND) \
	-command {nbtst_set "MAX_STUCK_FA" "ADDEND"} 
    entry $base.fra51.ent56 \
	-textvar nb(MAX_STUCK_init)
    entry $base.fra51.ent57 \
	-textvar nb(MAX_STUCK_max)
    entry $base.fra51.ent58 \
	-textvar nb(MAX_STUCK_incr)
}
#    frame $base.fra59 \
#        -borderwidth 2 -height 75 -relief groove \
#        -width 125 
#    label $base.fra59.lab60 \
#        -borderwidth 1 \
#	-relief raised -text $Name(ONLY_TS) 
if { $neighbor(METHOD)=="TS" } {
    frame $base.fra61 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra61.lab62 \
        -borderwidth 1 \
	-relief raised -text $Name(TABULENGTH) 
    label $base.fra61.lab63 \
        -borderwidth 1 \
	-relief raised -text $Name(from) 
    label $base.fra61.lab64 \
        -borderwidth 1 \
	-relief raised -text $Name(to) 
    menubutton $base.fra61.men20 \
	-indicatoron 1 \
        -menu $base.fra61.men20.m -padx 5 -pady 4 \
        -relief raised -text $Name(FACTOR) 
    menu $base.fra61.men20.m \
        -cursor {} \
	-tearoff 0
    $base.fra61.men20.m add command \
        -label $Name(FACTOR) \
	-command {nbtst_set "TABULENGTH_FA" "FACTOR"}
    $base.fra61.men20.m add command \
        -label $Name(ADDEND) \
	-command {nbtst_set "TABULENGTH_FA" "ADDEND"}
    entry $base.fra61.ent66 \
	-textvar nb(TABULENGTH_init)
    entry $base.fra61.ent67 \
	-textvar nb(TABULENGTH_max)
    entry $base.fra61.ent68 \
	-textvar nb(TABULENGTH_incr)
    frame $base.fra69 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra69.lab70 \
        -borderwidth 1 \
	-relief raised -text $Name(NUMB_NGHB) 
    label $base.fra69.lab71 \
        -borderwidth 1 \
	-relief raised -text $Name(from) 
    label $base.fra69.lab72 \
        -borderwidth 1 \
	-relief raised -text $Name(to) 
    menubutton $base.fra69.men21 \
        -indicatoron 1 \
	-menu $base.fra69.men21.m -padx 5 -pady 4 \
        -relief raised -text $Name(FACTOR) 
    menu $base.fra69.men21.m \
        -cursor {} \
	-tearoff 0
    $base.fra69.men21.m add command \
        -label $Name(FACTOR) \
	-command {nbtst_set "NUMB_NGHB_FA" "FACTOR"}
    $base.fra69.men21.m add command \
        -label $Name(ADDEND) \
	-command {nbtst_set "NUMB_NGHB_FA" "ADDEND"}
    entry $base.fra69.ent74 \
	-textvar nb(NUMB_NGHB_init)
    entry $base.fra69.ent75 \
	-textvar nb(NUMB_NGHB_max)
    entry $base.fra69.ent76 \
	-textvar nb(NUMB_NGHB_incr)
}
    frame $base.fra77 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra77.but78 \
	-padx 11 -pady 4 -text $Name(START_NB) \
	-command {set nb(x) [winfo rootx .ngbh_paratst];\
	          set nb(y) [winfo rooty .ngbh_paratst];\
		  destroy .ngbh_paratst; TC_nb_ptst}
    button $base.fra77.but79 \
	-padx 11 -pady 4 -text $Name(Cancel) \
	-command {set nb(x) [winfo rootx .ngbh_paratst];\
	          set nb(y) [winfo rooty .ngbh_paratst];\
		  destroy .ngbh_paratst}
    button $base.fra77.but80 \
	-padx 11 -pady 4 -text $Name(Help) \
	-command { lisa_help nb_ptest.htm }
    ###################
    # SETTING GEOMETRY
    ###################
    place $base.fra18 \
        -x 0 -y 0 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra18.lab19 \
        -x 0 -y 0 -width 499 -height 28 -anchor nw -bordermode ignore 
    place $base.fra20 \
	-x 0 -y 60 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra20.lab21 \
        -x 0 -y 0 -width 239 -height 28 -anchor nw -bordermode ignore 
    place $base.fra20.men22 \
        -x 240 -y 0 -width 258 -height 28 -anchor nw -bordermode ignore 
    place $base.fra23 \
        -x 0 -y 30 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra23.lab24 \
        -x 0 -y 0 -width 239 -height 28 -anchor nw -bordermode ignore 
    place $base.fra23.men25 \
        -x 240 -y 0 -width 258 -height 28 -anchor nw -bordermode ignore 
    place $base.fra26 \
        -x 0 -y 90 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra26.lab27 \
        -x 0 -y 0 -width 239 -height 28 -anchor nw -bordermode ignore 
    place $base.fra26.men28 \
        -x 240 -y 0 -width 258 -height 28 -anchor nw -bordermode ignore 
    place $base.fra17 \
        -x 0 -y 120 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra17.lab18 \
        -x 0 -y 0 -width 239 -height 28 -anchor nw -bordermode ignore 
    place $base.fra17.ent19 \
        -x 240 -y 0 -width 258 -height 27 -anchor nw -bordermode ignore 

    place $base.fra30 \
        -x 0 -y 150 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra30.but31 \
        -x 0 -y 1 -width 239 -height 28 -anchor nw -bordermode ignore 
    place $base.fra30.ent32 \
        -x 240 -y 0 -width 258 -height 27 -anchor nw -bordermode ignore

    place $base.fra31 \
        -x 0 -y 180 -width 500 -height 30 -anchor nw -bordermode ignore 
    place $base.fra31.lab32 \
        -x 0 -y 0 -width 499 -height 28 -anchor nw -bordermode ignore 
    place $base.fra33 \
        -x 0 -y 210 -width 500 -height 90 -anchor nw -bordermode ignore 
    place $base.fra33.lab34 \
        -x 0 -y 0 -width 239 -height 88 -anchor nw -bordermode ignore 
    place $base.fra33.lab35 \
        -x 240 -y 0 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra33.lab36 \
        -x 240 -y 30 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra33.men17 \
        -x 240 -y 60 -width 119 -height 28 -anchor nw -bordermode ignore
    place $base.fra33.ent38 \
        -x 360 -y 0 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra33.ent39 \
        -x 360 -y 30 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra33.ent40 \
        -x 360 -y 60 -width 138 -height 27 -anchor nw -bordermode ignore 
#    place $base.fra44 \
#        -x 0 -y 300 -width 500 -height 30 -anchor nw -bordermode ignore 
#    place $base.fra44.lab45 \
#        -x 0 -y 0 -width 499 -height 28 -anchor nw -bordermode ignore 
if { $neighbor(METHOD)=="SA" || $neighbor(METHOD)=="TA" } {
    place $base.fra41 \
        -x 0 -y 300 -width 500 -height 90 -anchor nw -bordermode ignore 
    place $base.fra41.lab42 \
        -x 0 -y 0 -width 239 -height 88 -anchor nw -bordermode ignore 
    place $base.fra41.lab43 \
        -x 240 -y 0 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra41.lab46 \
        -x 240 -y 30 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra41.men18 \
        -x 240 -y 60 -width 119 -height 28 -anchor nw -bordermode ignore
    place $base.fra41.ent48 \
        -x 360 -y 0 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra41.ent49 \
        -x 360 -y 30 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra41.ent50 \
        -x 360 -y 60 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra51 \
        -x 0 -y 390 -width 500 -height 90 -anchor nw -bordermode ignore 
    place $base.fra51.lab52 \
        -x 0 -y 0 -width 239 -height 88 -anchor nw -bordermode ignore 
    place $base.fra51.lab53 \
        -x 240 -y 0 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra51.lab54 \
        -x 240 -y 30 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra51.men19 \
        -x 240 -y 60 -width 119 -height 28 -anchor nw -bordermode ignore
    place $base.fra51.ent56 \
        -x 360 -y 0 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra51.ent57 \
        -x 360 -y 30 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra51.ent58 \
        -x 360 -y 60 -width 138 -height 27 -anchor nw -bordermode ignore 
}
#    place $base.fra59 \
#        -x 0 -y 510 -width 500 -height 30 -anchor nw -bordermode ignore 
#    place $base.fra59.lab60 \
#        -x 0 -y 0 -width 499 -height 28 -anchor nw -bordermode ignore 
if { $neighbor(METHOD)=="TS" } {
    place $base.fra61 \
        -x 0 -y 300 -width 500 -height 90 -anchor nw -bordermode ignore 
    place $base.fra61.lab62 \
        -x 0 -y 0 -width 239 -height 88 -anchor nw -bordermode ignore 
    place $base.fra61.lab63 \
        -x 240 -y 0 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra61.lab64 \
        -x 240 -y 30 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra61.men20 \
        -x 240 -y 60 -width 119 -height 28 -anchor nw -bordermode ignore
    place $base.fra61.ent66 \
        -x 360 -y 0 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra61.ent67 \
        -x 360 -y 30 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra61.ent68 \
        -x 360 -y 60 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra69 \
        -x 0 -y 390 -width 500 -height 90 -anchor nw -bordermode ignore 
    place $base.fra69.lab70 \
        -x 0 -y 0 -width 239 -height 88 -anchor nw -bordermode ignore 
    place $base.fra69.lab71 \
        -x 240 -y 0 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra69.lab72 \
        -x 240 -y 30 -width 119 -height 28 -anchor nw -bordermode ignore 
    place $base.fra69.men21 \
        -x 240 -y 60 -width 119 -height 28 -anchor nw -bordermode ignore
    place $base.fra69.ent74 \
        -x 360 -y 0 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra69.ent75 \
        -x 360 -y 30 -width 138 -height 27 -anchor nw -bordermode ignore 
    place $base.fra69.ent76 \
        -x 360 -y 60 -width 138 -height 27 -anchor nw -bordermode ignore 
}
    place $base.fra77 \
        -x 0 -y 478 -width 500 -height 75 -anchor nw -bordermode ignore 
    place $base.fra77.but78 \
        -x 40 -y 20 -anchor nw -bordermode ignore 
    place $base.fra77.but79 \
        -x 185 -y 20 -anchor nw -bordermode ignore 
    place $base.fra77.but80 \
        -x 372 -y 20 -anchor nw -bordermode ignore

    nbtst_set NGBH $neighbor(NGBH)
    nbtst_set METHOD $neighbor(METHOD)
    nbtst_set TYPE $neighbor(TYPE)
    nbtst_set STEPS_FA $nb(STEPS_FA)
    nbtst_set PROB_FA $nb(PROB_FA)
    nbtst_set MAX_STUCK_FA $nb(MAX_STUCK_FA)
    nbtst_set TABULENGTH_FA $nb(TABULENGTH_FA)
    nbtst_set NUMB_NGHB_FA $nb(NUMB_NGHB_FA)
    nbtst_set FILE $nb(FILE)
    set nbmax 0
    set maxsteps 0
    set  nboldx 0
    set nb(xpos) 0
    bind .ngbh_paratst <Visibility> {  
	grab set .ngbh_paratst;  
	bind .ngbh_paratst <Visibility> { } }
#    bind .ngbh_paratst <Destroy> { set mw(fSequence) "0" }
set nb(running) 1
}

proc nbtst_set {name value} {
    global nb;
    global neighbor
    global Name;
    if {$name == "NGBH"} {
	set neighbor(NGBH) $value
	.ngbh_paratst.fra20.men22 config -text $Name($value)
    }
    if {$name == "METHOD"} {
	set neighbor(METHOD) $value
	.ngbh_paratst.fra23.men25 config -text $Name($value)
    }
    if {$name == "TYPE"} {
	set neighbor(TYPE) $value
	.ngbh_paratst.fra26.men28  config -text $Name($value)
    }
    if {$name == "STEPS_FA"} {
	set nb(STEPS_FA) $value
	.ngbh_paratst.fra33.men17  config -text $Name($value)
    }
    if {$name == "FILE"} {
	if {$value != ""} {
	    set nb(FILE) $value
	    # .ngbh_paratst.fra30.ent32 config -textvar nb(FILE)
	}
    }
    if { $neighbor(METHOD)=="SA" || $neighbor(METHOD)=="TA" } { 
       if {$name == "PROB_FA"} {
	   set nb(PROB_FA) $value
	   .ngbh_paratst.fra41.men18  config -text $Name($value)
       }
       if {$name == "MAX_STUCK_FA"} {
	   set nb(MAX_STUCK_FA) $value
	   .ngbh_paratst.fra51.men19  config -text $Name($value)
       }
    }
    if { $neighbor(METHOD)=="TS" } {
	if {$name == "TABULENGTH_FA"} {
	    set nb(TABULENGTH_FA) $value
	    .ngbh_paratst.fra61.men20  config -text $Name($value)
	} 
	if {$name == "NUMB_NGHB_FA"} {
	    set nb(NUMB_NGHB_FA) $value
	    .ngbh_paratst.fra69.men21  config -text $Name($value)
	}
    }
}

proc vTclWindow.nbpt_graph {base} {
    if {$base == ""} {
        set base .nbpt_graph
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    global nbwidth;
    global nbheight;
    global nb;
    global Name;
    global system;
    toplevel $base -class Toplevel
      wm focusmodel $base passive
    if {$system(os)=="unix"} {
	wm geometry $base 410x270+40+40
    } else { 	
	wm geometry $base 410x230+40+40 
#	wm overrideredirect $base 1
    }

#    wm overrideredirect $base 0
    wm resizable $base 0 0
    wm deiconify $base
    wm title $base "$Name(Neighborhood_Parameter_Test)"
    wm transient $base .lisa
    frame $base.fra19 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra19.lab20 \
        -borderwidth 1 \
        -text $Name(Progress) 
    frame $base.fra21 \
        -borderwidth 2 -height 75 -relief sunken \
        -width 125 
    canvas $base.fra21.can22 \
        -borderwidth 2 -height 0 -relief ridge -width 0 
    frame $base.fra24 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra24.lab20 \
        -text "$Name(Objective_value)"
    label $base.fra24.lab21 \
        -text ""
    frame $base.fra25 \
	-borderwidth 2 -height 30 -relief groove \
        -width 125
    label $base.fra25.lab26 \
        -text ""
    label $base.fra25.lab27 \
        -text ""
    label $base.fra25.lab28 \
        -text ""
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra23.but24 \
	-padx 11 -pady 4 -text $Name(Cancel) \
    -command { catch {eval exec kill -2 $nb(pid)}; \
		      set stop(pid) $nb(pid); Window show .alg_stop }
    ###################
    # SETTING GEOMETRY
    ###################

    pack $base.fra19 \
        -in .nbpt_graph -anchor center -expand 0 -fill x -side top 
    pack $base.fra19.lab20 \
        -in .nbpt_graph.fra19 -anchor center -expand 0 -fill x -side top 
    pack $base.fra21 \
        -in .nbpt_graph -anchor center -expand 1 -fill both -side top 
    pack $base.fra21.can22 \
        -in .nbpt_graph.fra21 -anchor center -expand 1 -fill both -side top 
    pack $base.fra24 \
        -in .nbpt_graph -anchor center -expand 0 -fill x -side top 
    pack $base.fra24.lab20 \
        -in .nbpt_graph.fra24 -anchor w -expand 0 -side left 
    pack $base.fra24.lab21 \
        -in .nbpt_graph.fra24 -anchor e -expand 0 -side right
    pack $base.fra25 \
        -in .nbpt_graph -anchor center -expand 0 -fill x -side top 
    pack $base.fra25.lab26 \
        -in .nbpt_graph.fra25 -anchor w -expand 0 -side left 
    pack $base.fra25.lab27 \
        -in .nbpt_graph.fra25 -anchor e -expand 0 -side left
    pack $base.fra25.lab28 \
        -in .nbpt_graph.fra25 -anchor e -expand 0 -side right
    if {$system(os)=="unix"} {
	pack $base.fra23 \
	    -in .nbpt_graph -anchor center -expand 0 -fill x -side top 
	pack $base.fra23.but24 \
	    -in .nbpt_graph.fra23 -anchor center -expand 0 -fill none -side top 
    }
    
    # we know: format= 395x170
    set nbheight 230
    set nbwidth 395
    set nbheight [expr $nbheight - 85]
    set nbwidth [expr $nbwidth - 20]
    
    bind $base <Visibility> { grab set .nbpt_graph; bind .nbpt_graph  <Visibility> { } }
    bind $base <Destroy> {
	bind .nbpt_graph <Destroy> { }
	nbpt_cancel
    }
}

proc nbpt_cancel { } {
    global nb;
    global stop
    if {$nb(running)==1 } {
	catch {eval exec kill -2 $nb(pid)}; 
	set stop(pid) $nb(pid); 
	Window show .alg_stop 
    }
}

# If the external Module prints something or ends
proc nbptevent {fid file} {
    global Name;
    global nbmax;
    global maxsteps;
    global nbwidth;
    global nbheight;
    global nboldx;
    global nb
    
    #    set xpos 0
    set ypos 0
    
    if { [gets $fid line] >=0} {
	set namesteps ""
	set steps 0
	set objective 0
	set best_objective 0
	scan $line "%s %d %*s %d %*s %d" namesteps steps objective best_objective 

	if {$namesteps == "start"} {
	    .nbpt_graph.fra21.can22 delete all
            set nb(xpos) 0
	}
	if {$namesteps == "parameters:"} {
	    .nbpt_graph.fra25.lab26 config -text "steps=$steps"
	    .nbpt_graph.fra25.lab27 config -text "  param1=$objective"
	    .nbpt_graph.fra25.lab28 config -text "param2=$best_objective"
	}
	if {$namesteps == "steps="} {
	   #  if {$steps > $maxsteps} {
# 		set maxsteps $steps
# 		set nbmax [expr $objective*1.07]
# 	    }
	    if {$nb(xpos)==0} { 
		if {$objective>0} {
		    set nbmax $objective
		} else { 
		    set nbmax 100
		}
	    }
	    set fsteps "$steps.0"
	    set nb(xpos) [expr $nb(xpos)+1]
	    if {[winfo exists .nbpt_graph]} {
		.nbpt_graph.fra24.lab20 config -text "$Name(Objective_value)=$objective"
		.nbpt_graph.fra24.lab21 config \
		    -text "($Name(best_Objective)= $best_objective)"
		set objective "$objective.0"
		set ypos [expr 5+$nbheight-($objective / $nbmax)*$nbheight]
		.nbpt_graph.fra21.can22 create line $nb(xpos) $ypos $nb(xpos) \
		    $nbheight -fill Black 
	    }
	} else {
	    if {$namesteps == "ERROR:"} {
		TC_error "In Module nb_iter: $line"
	    }
	} 

    } else {
	if {[winfo exists .nbpt_graph]} { 
	    bind .nbpt_graph <Destroy> { }
	    destroy .nbpt_graph 
	}
	if {[winfo exists .alg_stop]} { destroy .alg_stop}
	catch {
	    TC_open_schedule $file
	    if {$fid != ""} {
		catch {close $fid}
		set nb(running) 0
	    }
	}
    }
}






