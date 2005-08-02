#!/usr/localapp/bin/wish
#############################################################################
# Visual Tcl v1.11p1 Project
#

proc vTclWindow.options {base} {
    global Name
    global options
 
    set options(HTML_VIEWER) [TC_getvar "HTML_VIEWER"]
    set options(LANGUAGE) [TC_getvar "LANGUAGE"]
			      
    if {$base == ""} {
        set base .options
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 420x142+182+491
    wm maxsize $base 665 994
    wm minsize $base 280 1
    wm overrideredirect $base 0
    wm resizable $base 1 0
    wm deiconify $base
    wm title $base $Name(general_options)
    wm transient $base .lisa
    frame $base.cpd19 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.cpd19.01 -relief flat -disabledforeground Black\
        -anchor w  -width 20\
         -text "$Name(Language)" -state disabled
    menubutton $base.cpd19.02 \
         -indicatoron 1 -menu .options.cpd19.02.03 \
        -padx 5 -pady 4 -relief sunken -text $options(LANGUAGE)
    menu $base.cpd19.02.03 \
	-cursor {} \
	-tearoff 0 
    $base.cpd19.02.03 add command \
        -label english -command { 
	    set   options(LANGUAGE) "english"
	    .options.cpd19.02 config -text $options(LANGUAGE)
	}
    $base.cpd19.02.03 add command \
        -label german -command { 
	    set   options(LANGUAGE) "german"
	    .options.cpd19.02 config -text $options(LANGUAGE)
	}
    frame $base.cpd20 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    entry $base.cpd20.02  -textvar options(HTML_VIEWER)
         
    button $base.cpd20.but21 \
        -anchor w  \
        -pady 4 -text $Name(html_viewer) \
        -width "20" -command {
	    set options(HTML_VIEWER) [tk_getOpenFile \
			 -title $Name(Open_HTML_Browser) ] 
	} 
    frame $base.cpd22 \
         -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.cpd22.02 \
	-padx 11 -pady 4 -text $Name(Cancel) \
	-command {destroy .options}
    button $base.cpd22.03 \
	-padx 11 -pady 4 -text $Name(Apply) \
	-command {apply_options; destroy .options}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.cpd19 \
        -in .options -anchor center -expand 0 -fill x -side top 
    pack $base.cpd19.01 \
        -in .options.cpd19 -anchor w -expand 0 -fill none -side left 
    pack $base.cpd19.02 \
        -in .options.cpd19 -anchor center -expand 0 -fill x -side top 
    pack $base.cpd20 \
        -in .options -anchor center -expand 0 -fill x -side top 
    pack $base.cpd20.02 \
        -in .options.cpd20 -anchor center -expand 1 -fill x -ipady 3 \
        -side right 
    pack $base.cpd20.but21 \
        -in .options.cpd20 -anchor center -expand 0  -fill none -side left
#pack propagate  $base.cpd20.but21
    pack $base.cpd22 \
        -in .options -anchor center -expand 1 -fill both -side top 
  #   pack $base.cpd22.01 \
#         -in .options.cpd22 -anchor center -expand 0 -fill none -ipadx 20 \
#         -ipady 4 -padx 20 -pady 20 -side left 
    pack $base.cpd22.03 \
        -in .options.cpd22 -anchor center -expand 0 -fill none -ipadx 20 \
        -ipady 4 -padx 20 -pady 20 -side left 
    pack $base.cpd22.02 \
        -in .options.cpd22 -anchor center -expand 0 -fill none -ipadx 5 \
        -ipady 4 -padx 20 -pady 20 -side right 
}

proc set_options { } {
    global options
    TC_setvar options LANGUAGE $options(LANGUAGE)
    TC_setvar options HTML_VIEWER $options(HTML_VIEWER) 
}

proc apply_options { } {
    global options
    global env
    global Name
    TC_setvar options LANGUAGE $options(LANGUAGE)
    TC_setvar options HTML_VIEWER $options(HTML_VIEWER)
    source [TC_getvar LISAHOME]/tcl/language/$options(LANGUAGE)/messages.tcl
    show_mess_and_choice "$Name(restart)" "$Name(restart_now)" TC_exit
    TC_save_options
#    bind .lisa <Destroy> { }
#    close_all_open_windows
#    destroy .lisa
#    source  [TC_getvar LISAHOME]/tcl/tclinit.tcl
#    Window show .lisa
#    bind .lisa <Destroy> {TC_exit } 
#    TC_new_problem
#    TC_save_options

}
