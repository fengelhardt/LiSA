# LiSA setup program
# Copyright Lars Dornheim 2002
# E-Mail: eldeh@web.de
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

set argc 0
set argv {}

if {![info exists vTcl(sourcing)]} {
    switch $tcl_platform(platform) {
	windows {
	}
	default {
	    option add *Scrollbar.width 10
	}
   }
    
}

#############################################################################
# Visual Tcl v1.51 Project
#

#################################
# VTCL LIBRARY PROCEDURES
#

if {![info exists vTcl(sourcing)]} {
proc Window {args} {
    global vTcl
    set cmd     [lindex $args 0]
    set name    [lindex $args 1]
    set newname [lindex $args 2]
    set rest    [lrange $args 3 end]
    if {$name == "" || $cmd == ""} { return }
    if {$newname == ""} { set newname $name }
    if {$name == "."} { wm withdraw $name; return }
    set exists [winfo exists $newname]
    switch $cmd {
        show {
            if {$exists} {
                wm deiconify $newname
            } elseif {[info procs vTclWindow$name] != ""} {
                eval "vTclWindow$name $newname $rest"
            }
            if {[wm state $newname] == "normal"} {
                vTcl:FireEvent $newname <<Show>>
            }
        }
        hide    {
            if {$exists} {
                wm withdraw $newname
                vTcl:FireEvent $newname <<Hide>>
                return}
        }
        iconify { if $exists {wm iconify $newname; return} }
        destroy { if $exists {destroy $newname; return} }
    }
}
}

if {![info exists vTcl(sourcing)]} {
proc {vTcl:DefineAlias} {target alias widgetProc top_or_alias cmdalias} {
    global widget

   set widget($alias) $target
   set widget(rev,$target) $alias

   if {$cmdalias} {
       interp alias {} $alias {} $widgetProc $target
   }

  if {$top_or_alias != ""} {
       set widget($top_or_alias,$alias) $target

       if {$cmdalias} {
           interp alias {} $top_or_alias.$alias {} $widgetProc $target
       }
   }
}

proc {vTcl:DoCmdOption} {target cmd} {
    ## menus are considered toplevel windows
    set parent $target
    while {[winfo class $parent] == "Menu"} {
        set parent [winfo parent $parent]
    }

    regsub -all {\%widget} $cmd $target cmd
    regsub -all {\%top} $cmd [winfo toplevel $parent] cmd

    uplevel #0 [list eval $cmd]
}

proc {vTcl:FireEvent} {target event} {
    foreach bindtag [bindtags $target] {
        set tag_events [bind $bindtag]
        set stop_processing 0
        foreach tag_event $tag_events {
            if {$tag_event == $event} {
                set bind_code [bind $bindtag $tag_event]
                regsub -all %W $bind_code $target bind_code
                set result [catch {uplevel #0 $bind_code} errortext]
                if {$result == 3} {
                    # break exception, stop processing
                    set stop_processing 1
                } elseif {$result != 0} {
                    bgerror $errortext
                }
                break
            }
        }
        if {$stop_processing} {break}
    }
}

proc {vTcl:Toplevel:WidgetProc} {w args} {
    if {[llength $args] == 0} {
        return -code error "wrong # args: should be \"$w option ?arg arg ...?\""
    }

    ## The first argument is a switch, they must be doing a configure.
    if {[string index $args 0] == "-"} {
        set command configure

        ## There's only one argument, must be a cget.
        if {[llength $args] == 1} {
            set command cget
        }
    } else {
        set command [lindex $args 0]
        set args [lrange $args 1 end]
    }

    switch -- $command {
        "hide" -
        "Hide" {
            Window hide $w
        }

        "show" -
        "Show" {
            Window show $w
        }

        "ShowModal" {
            Window show $w
            grab $w
            tkwait window $w
            grab release $w
        }

        default {
            eval $w $command $args
        }
    }
}

proc {vTcl:WidgetProc} {w args} {
    if {[llength $args] == 0} {
        return -code error "wrong # args: should be \"$w option ?arg arg ...?\""
    }

    ## The first argument is a switch, they must be doing a configure.
    if {[string index $args 0] == "-"} {
        set command configure

        ## There's only one argument, must be a cget.
        if {[llength $args] == 1} {
            set command cget
        }
    } else {
        set command [lindex $args 0]
        set args [lrange $args 1 end]
    }

    eval $w $command $args
}

proc {vTcl:toplevel} {args} {
    uplevel #0 eval toplevel $args
    set target [lindex $args 0]
    namespace eval ::$target {}
}
}

if {[info exists vTcl(sourcing)]} {
proc vTcl:project:info {} {
    namespace eval ::widgets::.top64 {
        array set save {}
    }
    namespace eval ::widgets::.top64.mes65 {
        array set save {-padx 1 -pady 1 -relief 1 -textvariable 1 -width 1}
    }
    namespace eval ::widgets::.top64.mes66 {
        array set save {-justify 1 -padx 1 -pady 1 -textvariable 1 -width 1}
    }
    namespace eval ::widgets::.top64.fra67 {
        array set save {-borderwidth 1 -height 1 -width 1}
    }
    namespace eval ::widgets::.top64.fra67.but68 {
        array set save {-command 1 -text 1}
    }
    namespace eval ::widgets::.top64.fra67.but70 {
        array set save {-command 1 -text 1}
    }
    namespace eval ::widgets::.top33 {
        array set save {}
    }
    namespace eval ::widgets::.top33.mes35 {
        array set save {-justify 1 -padx 1 -pady 1 -relief 1 -textvariable 1 -width 1}
    }
    namespace eval ::widgets::.top33.but36 {
        array set save {-command 1 -pady 1 -text 1}
    }
    namespace eval ::widgets_bindings {
        set tagslist {}
    }
}
}
#################################
# USER DEFINED PROCEDURES
#
###########################################################
## Procedure:  doInstall

proc {doInstall} {} {
     global tcl_platform
     switch $tcl_platform(platform) {
          unix {
               doUNIXInstall
               showModalMessage "Installation finished successfully.\n\nYou can start LiSA with \"lisa\"."
               exit
          }
          windows {
               doWinInstall
               #showModalMessage "Installation finished successfully.\n\nYou can start LiSA with \"lisa.bat\"."
               exit
          }
          default {
               showModalMessage "The platform $tcl_platform(platform) is not supported."
               exit
          }
     }
}
###########################################################
## Procedure:  doUNIXInstall

proc {doUNIXInstall} {} {
     global installDir
     set startScriptName $installDir/lisa
     set startScript [open $startScriptName w]
     puts $startScript "#!/bin/sh


# LiSA start shell script
# Copyright Lars Dornheim 1998, 1999, 2002
# E-Mail: eldeh@web.de
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html


# setting the LiSA home path

LISAHOME=\"$installDir\"
export LISAHOME


# checking the LiSA configuration

USERHOME=\${HOME:-\"/tmp\"}
if test ! -w \"\${USERHOME}\"; then
     USERHOME=\"/tmp\"
     fi
LISACONFIG=\"\${USERHOME}/.lisa\"
if test ! -d \"\${LISACONFIG}\"; then
     rm -fr \"\${LISACONFIG}\"
     mkdir \"\${LISACONFIG}\"
     fi
if test ! -f \"\${LISACONFIG}/default.xml\"; then
     rm -fr \"\${LISACONFIG}/default.xml\"
     cp \"\${LISAHOME}/data/default.xml\" \"\${LISACONFIG}\"
     fi


# setting the LiSA config path

export LISACONFIG


# starting LiSA

cd \"\${LISAHOME}/bin\"
exec \"\${LISAHOME}/bin/main\" \"\${LISACONFIG}/default.xml\""
     close $startScript
     file attributes $startScriptName -permissions 0755
}
###########################################################
## Procedure:  doWinInstall

proc {doWinInstall} {} {
     global installDir
     regsub -all / $installDir \\ installDirDOSStyle
     set startScriptName $installDir/lisa.bat
     set startScript [open $startScriptName w]
     puts $startScript "@echo off


rem LiSA start shell script
rem Copyright Lars Dornheim 1998, 1999, 2002
rem E-Mail: eldeh@web.de
rem WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html


rem setting the LiSA home path

set LISAHOME=$installDir


rem setting the LiSA config path

set LISACONFIG=%LISAHOME%/data


rem starting LiSA

cd \"$installDirDOSStyle\\bin\"
call \"$installDirDOSStyle\\bin\\main.exe\" \"%LISACONFIG%/default.xml\""
     close $startScript
}
###########################################################
## Procedure:  showModalMessage

proc {showModalMessage} {text} {
     global dialogText
     set dialogText $text
     vTcl:Toplevel:WidgetProc .top33 ShowModal
}
###########################################################
## Procedure:  init
###########################################################
## Procedure:  main

proc {main} {argc argv} {
#dialogs should be removed only for windows installation
global tcl_platform
switch $tcl_platform(platform) {
	windows {
		doInstall
	}
	default {
		wm protocol .top64 WM_DELETE_WINDOW {exit}
		tkwait window .top64
	}
}
}

proc init {argc argv} {
     set version "3.0pre1"
     global lisaVersionText
     set lisaVersionText "LiSA $version"
     global installDir
     set installDir [pwd]
     global installText
     set installText "Would you like to install LiSA in $installDir ?"
}

init $argc $argv

#################################
# VTCL GENERATED GUI PROCEDURES
#

proc vTclWindow. {base {container 0}} {
    if {$base == ""} {
        set base .
    }
    ###################
    # CREATING WIDGETS
    ###################
    if {!$container} {
    wm focusmodel $base passive
    wm geometry $base 1x1+0+0; update
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm withdraw $base
    wm title $base "vtcl.tcl"
    bindtags $base "$base Vtcl.tcl all"
    vTcl:FireEvent $base <<Create>>
    }
    ###################
    # SETTING GEOMETRY
    ###################

    vTcl:FireEvent $base <<Ready>>
}

proc vTclWindow.top33 {base {container 0}} {
    if {$base == ""} {
        set base .top33
    }
    if {[winfo exists $base] && (!$container)} {
        wm deiconify $base; return
    }

    global widget
    vTcl:DefineAlias "$base" "Toplevel2" vTcl:Toplevel:WidgetProc "" 1
    vTcl:DefineAlias "$base.but36" "Button3" vTcl:WidgetProc "Toplevel2" 1
    vTcl:DefineAlias "$base.mes35" "Message4" vTcl:WidgetProc "Toplevel2" 1

    ###################
    # CREATING WIDGETS
    ###################
    if {!$container} {
    vTcl:toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 350x150; update
    wm minsize $base 350 150
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "Confirmation"
    vTcl:FireEvent $base <<Create>>
    }
    message $base.mes35 \
        -justify center -padx 5 -pady 2 -relief groove \
        -textvariable dialogText -width 300 
    button $base.but36 \
        -command {destroy .top33} -pady 1m -text OK
    bind $base.but36 <Return> {destroy .top33}
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.mes35 \
        -in $base -anchor center -expand 0 -fill x -ipadx 10 -ipady 10 \
        -padx 10 -pady 10 -side top 
    pack $base.but36 \
        -in $base -anchor center -expand 0 -fill none -pady 10 -side bottom 

    vTcl:FireEvent $base <<Ready>>
}

proc vTclWindow.top64 {base {container 0}} {
    if {$base == ""} {
        set base .top64
    }
    if {[winfo exists $base] && (!$container)} {
        wm deiconify $base; return
    }

    global widget
    vTcl:DefineAlias "$base" "Toplevel1" vTcl:Toplevel:WidgetProc "" 1
    vTcl:DefineAlias "$base.fra67" "Frame1" vTcl:WidgetProc "Toplevel1" 1
    vTcl:DefineAlias "$base.fra67.but68" "Button1" vTcl:WidgetProc "Toplevel1" 1
    vTcl:DefineAlias "$base.fra67.but70" "Button2" vTcl:WidgetProc "Toplevel1" 1
    vTcl:DefineAlias "$base.mes65" "Message1" vTcl:WidgetProc "Toplevel1" 1
    vTcl:DefineAlias "$base.mes66" "Message2" vTcl:WidgetProc "Toplevel1" 1

    ###################
    # CREATING WIDGETS
    ###################
    if {!$container} {
    vTcl:toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 300x150; update
    wm minsize $base 300 150
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "LiSA Installation"
    vTcl:FireEvent $base <<Create>>
    }
    message $base.mes65 \
        -padx 5 -pady 2 -relief groove -textvariable lisaVersionText -width 127
    message $base.mes66 \
        -justify center -padx 5 -pady 2 -textvariable installText -width 250
    frame $base.fra67 \
        -borderwidth 2 -height 75 -width 176 
    button $base.fra67.but68 \
        -command doInstall -text OK 
    bind $base.fra67.but68 <Return> doInstall
    button $base.fra67.but70 \
        -command exit -text Cancel 
    bind $base.fra67.but70 <Return> exit
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.mes65 \
        -in $base -anchor center -expand 0 -fill x -ipadx 10 -ipady 10 \
        -padx 10 -pady 10 -side top 
    pack $base.mes66 \
        -in $base -anchor center -expand 0 -fill x -ipadx 10 -side top 
    pack $base.fra67 \
        -in $base -anchor center -expand 0 -fill x -padx 10 -pady 10 \
        -side bottom 
    pack $base.fra67.but68 \
        -in $base.fra67 -anchor center -expand 0 -fill none -side left 
    pack $base.fra67.but70 \
        -in $base.fra67 -anchor center -expand 0 -fill none -side right 

    vTcl:FireEvent $base <<Ready>>
}


#dialogs should be removed only for windows installation
global tcl_platform
switch $tcl_platform(platform) {
	windows {
	}
	default {
		Window show .
		Window show .top64
	}
}

main $argc $argv
