proc vTclWindow.message {base} {
    global mess;
    if {$base == ""} {
        set base .message
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -cursor xterm 
    wm focusmodel $base passive
    wm geometry $base 400x150
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $mess(title)
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    scrollbar $base.fra22.scr25 \
        -command {.message.fra22.tex26 yview} \
        -orient vert 
    text $base.fra22.tex26 \
        -background #fefcfe \
	-foreground Blue -height 1 -width 1  -wrap word \
        -yscrollcommand {.message.fra22.scr25 set} 
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra23.but24 \
        -padx 11 -pady 4 -text close \
	-command {destroy .message}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra22 \
        -in .message -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.scr25 \
        -in .message.fra22 -anchor center -expand 0 -fill y -side right 
    pack $base.fra22.tex26 \
        -in .message.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra23 \
        -in .message -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.but24 \
        -in .message.fra23 -anchor center -expand 0 -fill none -padx 5 -pady 5 \
        -side top 

}

 
proc showmessage {title text} {
    global mess;
    global Name;
    if {$title == ""} { 
	set title $Name(Message)
    }
    set mess(title) $title
    Window show .message
    .message.fra22.tex26 insert end $text
}

proc vTclWindow.errormessage {base} {
    global errormess;
    if {$base == ""} {
        set base .errormessage
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -cursor xterm 
    wm focusmodel $base passive
    wm geometry $base 400x150
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $errormess(title)

    wm transient $base .lisa
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    scrollbar $base.fra22.scr25 \
        -command {.errormessage.fra22.tex26 yview} \
        -orient vert 
    text $base.fra22.tex26 \
        -background #fefcfe \
	-foreground Blue -height 1 -width 1  -wrap word \
        -yscrollcommand {.errormessage.fra22.scr25 set} 
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra23.but24 \
        -padx 11 -pady 4 -text close \
	-command {destroy .errormessage}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra22 \
        -in .errormessage -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.scr25 \
        -in .errormessage.fra22 -anchor center -expand 0 -fill y -side right 
    pack $base.fra22.tex26 \
        -in .errormessage.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra23 \
        -in .errormessage -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.but24 \
        -in .errormessage.fra23 -anchor center -expand 0 -fill none -padx 5 -pady 5 \
        -side top 
    # to get a modal message window: use the following:
    #    bind .errormessage <Visibility> {  grab set .errormessage;  bind .errormessage <Visibility> { } }
}

 
proc showerrormessage {text} {
    global errormess;
    global Name;

    set errormess(title) $Name(Error)
    Window show .errormessage
    .errormessage.fra22.tex26 insert end $text

}

proc vTclWindow.delaym {base} {
    global mess;
    if {$base == ""} {
        set base .delaym
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -cursor xterm 
    wm focusmodel $base passive
    wm geometry $base 400x150
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
#    wm title $base $mess(title)
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    scrollbar $base.fra22.scr25 \
        -command {.delaym.fra22.tex26 yview} \
        -orient vert 
    text $base.fra22.tex26 \
        -background #fefcfe \
	-foreground Blue -height 1 -width 1  -wrap word \
        -yscrollcommand {.delaym.fra22.scr25 set} 
 #    frame $base.fra23 \
#         -borderwidth 2 -height 75 -relief groove \
#         -width 125 
  #   button $base.fra23.but24 \
#         -padx 11 -pady 4 -text close \
# 	-command {destroy .delaym}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra22 \
        -in .delaym -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.scr25 \
        -in .delaym.fra22 -anchor center -expand 0 -fill y -side right 
    pack $base.fra22.tex26 \
        -in .delaym.fra22 -anchor center -expand 1 -fill both -side top 
  #   pack $base.fra23 \
#         -in .delaym -anchor center -expand 0 -fill x -side top 
 #    pack $base.fra23.but24 \
#         -in .delaym.fra23 -anchor center -expand 0 -fill none -padx 5 -pady 5 \
#         -side top 

    bind $base <Visibility> {
	after 2000 {
	    destroy .delaym
	}
    }

}

proc showdelaym { title text } {
    global Name;
    Window show .delaym
#    set errormess(title) $Name(Error)
    wm title  .delaym $title
    .delaym.fra22.tex26 insert end $text

}

proc show_mess_and_choice { title text action} {
    global Name;
    Window show .mess_and_choice
#    set errormess(title) $Name(Error)
    wm title  .mess_and_choice $title
.mess_and_choice.fra23.but24 config -command $action
    .mess_and_choice.fra22.tex26 insert end $text

}



proc vTclWindow.mess_and_choice {base} {
    global mess;
    global Name;
    if {$base == ""} {
        set base .mess_and_choice
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel \
        -cursor xterm 
    wm focusmodel $base passive
    wm geometry $base 400x150
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base ""
    frame $base.fra22 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    scrollbar $base.fra22.scr25 \
        -command {.mess_and_choice.fra22.tex26 yview} \
        -orient vert 
    text $base.fra22.tex26 \
        -background #fefcfe \
	-foreground Blue -height 1 -width 1  -wrap word \
        -yscrollcommand {.mess_and_choice.fra22.scr25 set} 
    frame $base.fra23 \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button $base.fra23.but24 \
        -padx 11 -pady 4 -text $Name(OK) \
	-command {destroy .mess_and_choice}
   button $base.fra23.but25 \
        -padx 11 -pady 4 -text $Name(Cancel) \
	-command {destroy .mess_and_choice}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra22 \
        -in .mess_and_choice -anchor center -expand 1 -fill both -side top 
    pack $base.fra22.scr25 \
        -in .mess_and_choice.fra22 -anchor center -expand 0 -fill y -side right 
    pack $base.fra22.tex26 \
        -in .mess_and_choice.fra22 -anchor center -expand 1 -fill both -side top 
    pack $base.fra23 \
        -in .mess_and_choice -anchor center -expand 0 -fill x -side top 
    pack $base.fra23.but24 \
        -in .mess_and_choice.fra23 -anchor center -expand 0 -fill none -padx 5 -pady 5 \
        -side left
   pack $base.fra23.but25 \
        -in .mess_and_choice.fra23 -anchor center -expand 0 -fill none -padx 5 -pady 5 \
        -side right

}

 


