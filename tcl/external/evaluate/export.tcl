#############################################################################
# Visual Tcl v1.11p1 Project
#

#################################
# GLOBAL VARIABLES
#
global selectedButton; 
global widget; 

proc vTclWindow.export {base} {
    global export;
    global Name;
    set export(bild)  $Name(Table)
    if {$base == ""} {
        set base .export
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel 
    wm focusmodel $base passive
    wm geometry $base 485x406+301+450
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base $Name(Export_to)
    frame $base.fra21 \
        -borderwidth 2 -height 85 -width 150 
    frame $base.fra21.01 \
        -borderwidth 2 -height 55 -width 50 
    frame $base.fra21.02 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra21.02.03 \
        -padx 11 -pady 4 -text OK -command {TC_export; destroy .export }
    frame $base.fra21.04 \
        -borderwidth 2 -height 20 -width 25 
    frame $base.fra21.05 \
        -borderwidth 2 -height 75 -width 125 
    button $base.fra21.05.06 \
        -padx 11 -pady 4 -text CANCEL -command {destroy .export }
    frame $base.fra17 \
        -borderwidth 2 -height 20 -width 125 
    frame $base.fra25 \
        -borderwidth 2 -height 75 -width 20 
    frame $base.fra26 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra26.fra19 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra26.fra19.01 \
        -highlightthickness 0 \
	-value "excel" -variable export(doc) \
        -text $Name(Excel_document)
    frame $base.fra26.fra19.fra28 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra26.fra19.fra28.fra29 \
        -borderwidth 2 -height 20 -width 27 
    frame $base.fra26.fra19.fra28.fra31 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra26.fra19.fra28.fra31.fra32 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra26.fra19.fra28.fra31.fra32.fra40 \
        -borderwidth 2 -height 2 -width 125 
    label $base.fra26.fra19.fra28.fra31.fra32.lab41 \
        -borderwidth 1 \
        -text $Name(Decimal_point)
    frame $base.fra26.fra19.fra28.fra31.fra32.fra42 \
        -borderwidth 2 -height 12 -width 20 
    label $base.fra26.fra19.fra28.fra31.fra32.lab43 \
        -borderwidth 1 \
        -text $Name(Separator)
    frame $base.fra26.fra19.fra28.fra31.fra33 \
        -borderwidth 2 -height 75 -width 125 
    entry $base.fra26.fra19.fra28.fra31.fra33.02 \
        -textvariable export(dec_point) -width 4 
    frame $base.fra26.fra19.fra28.fra31.fra33.fra38 \
        -borderwidth 2 -height 9 -width 20 
    entry $base.fra26.fra19.fra28.fra31.fra33.ent39 \
        -textvariable export(separator) -width 4 
    frame $base.fra26.fra19.fra28.fra17 \
        -borderwidth 2 -height 75 -width 125 
    frame $base.fra26.fra19.fra28.fra17.fra18 \
        -borderwidth 2 -height 38 -width 30 
    label $base.fra26.fra19.fra28.fra17.lab19 \
        -borderwidth 1 \
        -text $Name(plus)
    checkbutton $base.fra26.fra19.fra28.fra17.che20 \
        -highlightthickness 0 -text $Name(Tabulator) \
        -variable export(ctab) 
    frame $base.fra26.fra20 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra26.fra20.01 \
        -highlightthickness 0 \
	-value "spss" -variable export(doc) \
        -text $Name(SPSS_document) -state disabled
    frame $base.fra26.fra22 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra26.fra22.01 \
        -highlightthickness 0\
        -value "latex" -variable export(doc)\
        -text $Name(Latex_document) 
    frame $base.fra26.fra23 \
        -borderwidth 2 -height 75 -width 125 
    radiobutton $base.fra26.fra23.01 \
        -value "ps" -variable export(doc)\
        -highlightthickness 0 -text $Name(Postscript) 
    frame $base.fra26.fra17 \
        -borderwidth 2 -height 35 -width 125 
    frame $base.fra26.fra21 \
        -borderwidth 2 -height 75 -width 125 
    label $base.fra26.fra21.lab23 \
        -borderwidth 1 \
        -text $Name(Filename:)
    entry $base.fra26.fra21.ent24 \
        -textvariable export(filename) -width 18 
    frame $base.fra30 \
        -borderwidth 2 -height 75 -width 20 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.fra21 \
        -in .export -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra21.01 \
        -in .export.fra21 -anchor center -expand 0 -fill none -side left 
    pack $base.fra21.02 \
        -in .export.fra21 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra21.02.03 \
        -in .export.fra21.02 -anchor center -expand 0 -fill none -side left 
    pack $base.fra21.04 \
        -in .export.fra21 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra21.05 \
        -in .export.fra21 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra21.05.06 \
        -in .export.fra21.05 -anchor center -expand 0 -fill none -side right 
    pack $base.fra17 \
        -in .export -anchor center -expand 0 -fill x -side top 
    pack $base.fra25 \
        -in .export -anchor n -expand 0 -fill y -side left 
    pack $base.fra26 \
        -in .export -anchor n -expand 1 -fill x -side left 
    pack $base.fra26.fra19 \
        -in .export.fra26 -anchor n -expand 1 -fill x -side top 
    pack $base.fra26.fra19.01 \
        -in .export.fra26.fra19 -anchor w -expand 0 -fill none -side top
    pack $base.fra26.fra19.fra28 \
        -in .export.fra26.fra19 -anchor center -expand 0 -fill none -side left
    pack $base.fra26.fra19.fra28.fra29 \
        -in .export.fra26.fra19.fra28 -anchor center -expand 0 -fill none \
        -side left 
    pack $base.fra26.fra19.fra28.fra31 \
        -in .export.fra26.fra19.fra28 -anchor center -expand 0 -fill none \
        -side left
    pack $base.fra26.fra19.fra28.fra31.fra32 \
        -in .export.fra26.fra19.fra28.fra31 -anchor n -expand 0 -fill none \
        -side left 
    pack $base.fra26.fra19.fra28.fra31.fra32.fra40 \
        -in .export.fra26.fra19.fra28.fra31.fra32 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra32.lab41 \
        -in .export.fra26.fra19.fra28.fra31.fra32 -anchor e -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra32.fra42 \
        -in .export.fra26.fra19.fra28.fra31.fra32 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra32.lab43 \
        -in .export.fra26.fra19.fra28.fra31.fra32 -anchor e -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra33 \
        -in .export.fra26.fra19.fra28.fra31 -anchor n -expand 0 -fill none \
        -side left 
    pack $base.fra26.fra19.fra28.fra31.fra33.02 \
        -in .export.fra26.fra19.fra28.fra31.fra33 -anchor w -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra33.fra38 \
        -in .export.fra26.fra19.fra28.fra31.fra33 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra31.fra33.ent39 \
        -in .export.fra26.fra19.fra28.fra31.fra33 -anchor w -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra17 \
        -in .export.fra26.fra19.fra28 -anchor n -expand 0 -fill none \
        -side left 
    pack $base.fra26.fra19.fra28.fra17.fra18 \
        -in .export.fra26.fra19.fra28.fra17 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra19.fra28.fra17.lab19 \
        -in .export.fra26.fra19.fra28.fra17 -anchor center -expand 0 \
        -fill none -side left 
    pack $base.fra26.fra19.fra28.fra17.che20 \
        -in .export.fra26.fra19.fra28.fra17 -anchor center -expand 0 \
        -fill none -side top 
    pack $base.fra26.fra20 \
        -in .export.fra26 -anchor n -expand 1 -fill x -side top 
    pack $base.fra26.fra20.01 \
        -in .export.fra26.fra20 -anchor nw -expand 0 -fill x -side left 
    pack $base.fra26.fra22 \
        -in .export.fra26 -anchor n -expand 1 -fill x -side top 
    pack $base.fra26.fra22.01 \
        -in .export.fra26.fra22 -anchor nw -expand 0 -fill x -side left 
    pack $base.fra26.fra23 \
        -in .export.fra26 -anchor n -expand 1 -fill x -side top 
    pack $base.fra26.fra23.01 \
        -in .export.fra26.fra23 -anchor nw -expand 0 -fill x -side left 
    pack $base.fra26.fra17 \
        -in .export.fra26 -anchor center -expand 0 -fill none -side top 
    pack $base.fra26.fra21 \
        -in .export.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.fra21.lab23 \
        -in .export.fra26.fra21 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.fra21.ent24 \
        -in .export.fra26.fra21 -anchor center -expand 0 -fill none -side left 
    pack $base.fra30 \
        -in .export -anchor center -expand 1 -fill y -side top 
}

proc vTclWindow.top19 {base} {
    if {$base == ""} {
        set base .top19
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel
    wm focusmodel $base passive
    wm geometry $base 200x200+552+362
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm title $base "New Toplevel 3"
    ###################
    # SETTING GEOMETRY
    ###################
}

proc vTclWindow.top20 {base} {
    if {$base == ""} {
        set base .top20
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel 
    wm focusmodel $base passive
    wm geometry $base 200x200+297+264
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm title $base "New Toplevel 4"
    ###################
    # SETTING GEOMETRY
    ###################
}

proc vTclWindow.top25 {base} {
    if {$base == ""} {
        set base .top25
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    ###################
    # CREATING WIDGETS
    ###################
    toplevel $base -class Toplevel  -menu .top25.m56 
    wm focusmodel $base passive
    wm geometry $base 554x253+407+683
    wm maxsize $base 1265 994
    wm minsize $base 1 1
    wm overrideredirect $base 0
    wm resizable $base 1 1
    wm deiconify $base
    wm title $base "Neu"
    frame $base.01  -borderwidth 2 -height 75 -width 125 
    label $base.01.02  -borderwidth 1  -relief raised -text Zeilen -width 10 
    entry $base.01.03  
    frame $base.fra26  -borderwidth 2 -height 75 -width 125 
    label $base.fra26.01  -borderwidth 1  -relief raised -text Spalten -width 10 
    entry $base.fra26.02  
    frame $base.fra27  -borderwidth 2 -height 75 -width 125 
    label $base.fra27.lab28  -borderwidth 1  -relief raised  -text {Vorinitialisierter Wert} 
    frame $base.fra27.fra35  -borderwidth 2 -height 75 -relief groove  -width 125 
    radiobutton $base.fra27.fra35.rad36  
    entry $base.fra27.fra35.ent37  
    radiobutton $base.fra27.rad52  -anchor w -text zufallsgeneriert 
    frame $base.fra27.fra62  -height 75 -relief groove -width 125 
    label $base.fra27.fra62.01  -borderwidth 0  -text bis 
    entry $base.fra27.fra62.02  -width 2 
    frame $base.fra27.fra63  -borderwidth 2 -height 20 -width 34 
    frame $base.fra27.fra64  -height 75 -relief groove -width 125 
    label $base.fra27.fra64.01  -borderwidth 1  -text von 
    entry $base.fra27.fra64.02  -width 2 
    frame $base.fra70  -borderwidth 2 -height 85 -width 150 
    frame $base.fra70.fra78  -borderwidth 2 -height 75 -width 50 
    frame $base.fra70.fra80  -borderwidth 2 -height 75 -relief groove  -width 125 
    button $base.fra70.fra80.but83  -padx 11 -pady 4 -text OK 
    frame $base.fra70.fra91  -borderwidth 2 -height 20 -width 25 
    frame $base.fra70.fra92  -borderwidth 2 -height 75 -relief groove  -width 125 
    button $base.fra70.fra92.01  -padx 11 -pady 4 -text CANCEL 
    ###################
    # SETTING GEOMETRY
    ###################
    pack $base.01  -in .top25 -anchor center -expand 0 -fill x -side top 
    pack $base.01.02  -in .top25.01 -anchor center -expand 0 -fill none -side left 
    pack $base.01.03  -in .top25.01 -anchor center -expand 0 -fill x -side top 
    pack $base.fra26  -in .top25 -anchor center -expand 0 -fill x -side top 
    pack $base.fra26.01  -in .top25.fra26 -anchor center -expand 0 -fill none -side left 
    pack $base.fra26.02  -in .top25.fra26 -anchor center -expand 0 -fill x -side top 
    pack $base.fra27  -in .top25 -anchor center -expand 0 -fill x -side top 
    pack $base.fra27.lab28  -in .top25.fra27 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra27.fra35  -in .top25.fra27 -anchor center -expand 0 -fill x -side top 
    pack $base.fra27.fra35.rad36  -in .top25.fra27.fra35 -anchor nw -expand 0 -fill none -side left 
    pack $base.fra27.fra35.ent37  -in .top25.fra27.fra35 -anchor center -expand 0 -fill x -side top 
    pack $base.fra27.rad52  -in .top25.fra27 -anchor nw -expand 1 -fill x -side top 
    pack $base.fra27.fra62  -in .top25.fra27 -anchor ne -expand 1 -fill x -side right 
    pack $base.fra27.fra62.01  -in .top25.fra27.fra62 -anchor center -expand 0 -fill none -side left 
    pack $base.fra27.fra62.02  -in .top25.fra27.fra62 -anchor center -expand 1 -fill x -side right 
    pack $base.fra27.fra63  -in .top25.fra27 -anchor center -expand 0 -fill none -side left 
    pack $base.fra27.fra64  -in .top25.fra27 -anchor nw -expand 1 -fill x -side left 
    pack $base.fra27.fra64.01  -in .top25.fra27.fra64 -anchor center -expand 0 -fill none -side left 
    pack $base.fra27.fra64.02  -in .top25.fra27.fra64 -anchor center -expand 1 -fill x -side left 
    pack $base.fra70  -in .top25 -anchor center -expand 0 -fill x -side bottom 
    pack $base.fra70.fra78  -in .top25.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80  -in .top25.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra80.but83  -in .top25.fra70.fra80 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra91  -in .top25.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92  -in .top25.fra70 -anchor center -expand 0 -fill none -side left 
    pack $base.fra70.fra92.01  -in .top25.fra70.fra92 -anchor center -expand 0 -fill none  -side right
}









