proc vTclWindow.man_oper {base} {
    global env;
    global Name;
    global schedule;

    if {$base == ""} {
        set base .man_oper
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    
    TC_save_old_sequence
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 270x420+100+100
    wm overrideredirect $base 0
    wm resizable $base 0 0
    wm deiconify $base
    wm transient $base .lisa
    wm title $base "Operation ([expr $schedule(row)+1],[expr $schedule(column)+1])"
    set  symbol_lleft [ image create bitmap -file "[TC_getvar LISAHOME]/img/lleft.xbm"]
    set  symbol_bbuttom [ image create bitmap -file "[TC_getvar LISAHOME]/img/bbuttom.xbm"]
    set  symbol_ttop [ image create bitmap -file "[TC_getvar LISAHOME]/img/ttop.xbm"]
    set  symbol_rright [ image create bitmap -file "[TC_getvar LISAHOME]/img/rright.xbm"]
    set  symbol_left [ image create bitmap -file "[TC_getvar LISAHOME]/img/left.xbm"]
    set  symbol_buttom [ image create bitmap -file "[TC_getvar LISAHOME]/img/buttom.xbm"]
    set  symbol_top [ image create bitmap -file "[TC_getvar LISAHOME]/img/top.xbm"]
    set  symbol_right [ image create bitmap -file "[TC_getvar LISAHOME]/img/right.xbm"]
    set  symbol_reset [ image create bitmap -file "[TC_getvar LISAHOME]/img/reset.xbm"]
    set  symbol_ar_left [ image create bitmap -file "[TC_getvar LISAHOME]/img/ar_left.xbm"]
    set  symbol_ar_down [ image create bitmap -file "[TC_getvar LISAHOME]/img/ar_down.xbm"]
    set  symbol_ar_up [ image create bitmap -file "[TC_getvar LISAHOME]/img/ar_up.xbm"]
    set  symbol_ar_right [ image create bitmap -file "[TC_getvar LISAHOME]/img/ar_right.xbm"]
    
    button $base.but19 \
        -padx 11 -pady 4 -image $symbol_bbuttom \
	-command {TC_move_operation bbuttom $schedule(row) $schedule(column)}
    button $base.but20 \
        -padx 11 -pady 4 -image $symbol_ttop\
	-command {TC_move_operation ttop $schedule(row) $schedule(column)}
    button $base.but21 \
        -padx 11 -pady 4 -image $symbol_top\
	-command {TC_move_operation top $schedule(row) $schedule(column)}
    button $base.but22 \
        -padx 11 -pady 4 -image $symbol_left\
	-command {TC_move_operation left $schedule(row) $schedule(column)}
    button $base.but23 \
        -padx 11 -pady 4 -image $symbol_right\
	-command {TC_move_operation right $schedule(row) $schedule(column)}
    button $base.but24 \
        -padx 11 -pady 4 -image $symbol_rright\
	-command {TC_move_operation rright $schedule(row) $schedule(column)}
    button $base.but25 \
        -padx 11 -pady 4 -image $symbol_lleft\
	-command {TC_move_operation lleft $schedule(row) $schedule(column)} 
    button $base.but26 \
        -padx 11 -pady 4  -image $symbol_reset\
	-command {TC_move_operation reset $schedule(row) $schedule(column)}
    button $base.but27 \
        -padx 11 -pady 4 -image $symbol_buttom\
	-command {TC_move_operation buttom $schedule(row) $schedule(column)}
    button $base.but30 \
        -padx 11 -pady 4 -text $Name(Edit_Source)\
	-command {TC_move_operation source $schedule(row) $schedule(column)}
    button $base.but31 \
        -padx 11 -pady 4 -text $Name(Edit_Sink)\
	-command {TC_move_operation sink $schedule(row) $schedule(column)}
    label $base.lab28 \
         -borderwidth 1 \
        -text $Name(JO_short) 
    label $base.lab29 \
         -borderwidth 1 \
        -text $Name(MO_short) 
    frame $base.fra39 \
	-borderwidth 2 -height 75 -relief groove \
        -width 125 
    label $base.fra39.lab40 \
	-borderwidth 1 \
	-relief flat -text "" 
    label $base.fra39.lab41 \
	-borderwidth 1 \
	-relief flat -text "" 
    label $base.fra39.lab42 \
	-borderwidth 1 \
	-relief flat -text "" 
    label $base.fra39.lab43 \
	-borderwidth 1 \
	-relief flat -text "" 
    label $base.fra39.lab45 \
	-borderwidth 1 \
	-relief flat -text "" 
    label $base.fra39.lab46 \
	-borderwidth 1 \
	-image $symbol_ar_right
    label $base.fra39.lab47 \
	-borderwidth 1 \
	-image $symbol_ar_up
    label $base.fra39.lab48 \
	-borderwidth 1 \
	-image $symbol_ar_up
    label $base.fra39.lab49 \
	-borderwidth 1 \
	-image $symbol_ar_right
    label $base.fra39.lab50 \
	-borderwidth 1 \
	-relief flat -text $Name(Cij) 
    label $base.fra39.lab51 \
	-borderwidth 1 \
	-relief flat -text $Name(Pij) 
    label $base.fra39.lab52 \
	-borderwidth 1 \
	-text "" 
    button $base.but38 \
        -padx 4 -pady 4 -text $Name(Close) -command { destroy .man_oper;TC_draw_output }
    button $base.but40 \
        -padx 4 -pady 4 -text $Name(Help) -command { lisa_help man_oper.html }
  #   button $base.but39 \
#         -padx 2 -pady 4 -text $Name(Cancel) -command {destroy .man_oper}
    ###################
    # SETTING GEOMETRY
    ###################
    place $base.but19 \
        -x 120 -y 188 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but20 \
        -x 120 -y 47 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but21 \
        -x 120 -y 84 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but22 \
        -x 80 -y 119 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but23 \
        -x 160 -y 119 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but24 \
        -x 198 -y 119 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but25 \
        -x 41 -y 119 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but26 \
        -x 120 -y 120 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but27 \
        -x 120 -y 154 -width 37 -height 31 -anchor nw -bordermode ignore 
    place $base.but30 \
        -x 41 -y 155 -width 73 -height 64 -anchor nw -bordermode ignore 
   place $base.but31 \
	-x 159 -y 47 -width 73 -height 68 -anchor nw -bordermode ignore 
    place $base.lab28 \
        -x 125 -y 16 -width 22 -height 21 -anchor nw -bordermode ignore 
    place $base.lab29 \
        -x 8 -y 124 -width 22 -height 21 -anchor nw -bordermode ignore 
    place $base.fra39 \
        -x 0 -y 235 -width 270 -height 120 -anchor nw -bordermode ignore 
    place $base.fra39.lab40 \
        -x 105 -y 5 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab41 \
        -x 105 -y 45 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab42 \
        -x 105 -y 84 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab43 \
        -x 170 -y 45 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab45 \
        -x 40 -y 45 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab46 \
        -x 100 -y 50 -anchor nw -bordermode ignore 
    place $base.fra39.lab47 \
        -x 135 -y 70 -anchor nw -bordermode ignore 
    place $base.fra39.lab48 \
        -x 135 -y 30 -anchor nw -bordermode ignore 
    place $base.fra39.lab49 \
        -x 165 -y 50 -anchor nw -bordermode ignore 
    place $base.fra39.lab50 \
        -x 180 -y 5 -width 70 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab51 \
        -x 15 -y 84 -width 75 -height 26 -anchor nw -bordermode ignore 
    place $base.fra39.lab52 \
        -x 12 -y 5 -width 49 -height 21 -anchor nw -bordermode ignore 
    place $base.but38 \
        -x 15 -y 380 -width 95 -height 31 -anchor nw -bordermode ignore 
    place $base.but40 \
        -x 165 -y 380 -width 95 -height 31 -anchor nw -bordermode ignore 
  #   place $base.but39 \
#         -x 165 -y 380  -width 97  -height 31 -anchor nw -bordermode ignore 
    
    if { [string match "J*" [TC_getvar "alpha"] ] || [string match "F*" [TC_getvar "alpha"]] || [string match "1*" [TC_getvar "alpha"]] } {
	
	$base.but22 configure -state disabled
	$base.but23 configure -state disabled
	$base.but24 configure -state disabled
	$base.but25 configure -state disabled
	$base.but30 configure -state disabled
	$base.but31 configure -state disabled
    } else {
	bind $base <Left> {TC_move_operation left $schedule(row) $schedule(column)}
	bind $base <Shift-Left> {TC_move_operation lleft $schedule(row) $schedule(column)}
	bind $base <Right> {TC_move_operation right $schedule(row) $schedule(column)}
	bind $base <Shift-Right> {TC_move_operation rright $schedule(row) $schedule(column)}
	bind $base <s> {TC_move_operation source $schedule(row) $schedule(column)}
	bind $base <t> {TC_move_operation sink $schedule(row) $schedule(column)}
    }
    
    if { [string match "1*" [TC_getvar "alpha"]] } {
	bind $base <s> {TC_move_operation source $schedule(row) $schedule(column)}
	bind $base <t> {TC_move_operation sink $schedule(row) $schedule(column)}
    }
    bind $base <Up> {TC_move_operation top $schedule(row) $schedule(column)}
    bind $base <Shift-Up> {TC_move_operation ttop $schedule(row) $schedule(column)}
    bind $base <Down> {TC_move_operation buttom $schedule(row) $schedule(column)}
    bind $base <Shift-Down> {TC_move_operation bbuttom $schedule(row) $schedule(column)}
    #bind $base <Destroy> {TC_draw_output }
    man_oper_update
}

proc man_oper_update {} {
    global schedule
    TC_setvar man_oper $schedule(row) $schedule(column)
    wm title .man_oper "Operation ([expr $schedule(row)+1],[expr $schedule(column)+1])"
    if { [TC_getvar critical $schedule(row) $schedule(column) ] == "0" } {
	.man_oper.fra39.lab41 configure -foreground black 
    } else {
	.man_oper.fra39.lab41 configure -foreground red 
    }
    .man_oper.fra39.lab41 configure -text \
 	"([expr $schedule(row)+1],[expr $schedule(column)+1])"
    set JO_succ [TC_getvar JO_succ $schedule(row) $schedule(column)]
    if {$JO_succ!=0} {
	if { [TC_getvar critical [expr $JO_succ -1] $schedule(column) ] == "0" } {
	    .man_oper.fra39.lab40 configure -foreground black 
	} else {
	    .man_oper.fra39.lab40 configure -foreground red 
	}
	.man_oper.fra39.lab40 configure -text \
	    "($JO_succ,[expr $schedule(column)+1])"
    } else {
	.man_oper.fra39.lab40 configure -text .
    }
    set JO_pred [TC_getvar JO_pred $schedule(row) $schedule(column)]
     if {$JO_pred!=0} {
	 if { [TC_getvar critical [expr $JO_pred -1] $schedule(column) ] == "0" } {
	     .man_oper.fra39.lab42 configure -foreground black 
	 } else {
	     .man_oper.fra39.lab42 configure -foreground red 
	 }
	 .man_oper.fra39.lab42 configure -text \
	     "($JO_pred,[expr $schedule(column)+1])"
     } else {
	 .man_oper.fra39.lab42 configure -text .
     }
     set MO_pred [TC_getvar MO_pred $schedule(row) $schedule(column)]
    if {$MO_pred!=0} {
	if { [TC_getvar critical $schedule(row) [expr $MO_pred -1] ] == "0" } {
	    .man_oper.fra39.lab45 configure -foreground black 
	} else {
	    .man_oper.fra39.lab45 configure -foreground red 
	}
	.man_oper.fra39.lab45 configure -text \
	    "([expr $schedule(row)+1],$MO_pred)"	
    } else {
		.man_oper.fra39.lab45 configure -text .
    }
  set MO_succ [TC_getvar MO_succ $schedule(row) $schedule(column)]
    if {$MO_succ!=0} {
	if { [TC_getvar critical $schedule(row) [expr $MO_succ -1] ] == "0" } {
	    .man_oper.fra39.lab43 configure -foreground black 
	} else {
	    .man_oper.fra39.lab43 configure -foreground red 
	}
	.man_oper.fra39.lab43 configure -text \
	    "([expr $schedule(row)+1],$MO_succ)"
    } else {
	.man_oper.fra39.lab43 configure -text .
    }
    .man_oper.fra39.lab50  configure -text \
	"Cij: [TC_getvar Cij $schedule(row) $schedule(column) ]"
    .man_oper.fra39.lab51 configure -text \
	"pij: [TC_getvar pij $schedule(row) $schedule(column) ]"
    focus .man_oper	
}

