###########################################################################
# This is kind of a compiler which reads an .alg file and constructs the 
# tcl source code for the parameter test window
###########################################################################

proc {genparatestwin} {description_file output_file algo_name} {
    global env
    #puts "genparatestwin $description_file $output_file $algo_name"
    if { [file exists $description_file]=="1" } {
	set description_file_id [open $description_file r]

	set line_in_file [gets $description_file_id]
	set name_desc ""
	set myname ""
	set no_of_entrys "0"
	set myname ""
	set mytype ""
	set mycall ""
	set mycode ""
	set myhelp ""
	set mytkfile ""
	scan $line_in_file "%s " name_desc 	
	while {$name_desc== "\#" } {
	    set line_in_file [gets $description_file_id]
	    scan $line_in_file "%s " name_desc 
	}
	while {$name_desc!= "<GENERAL>" } {
	    set line_in_file [gets $description_file_id]
	    scan $line_in_file "%s " name_desc 
	}
	set line_in_file [gets $description_file_id]
	scan $line_in_file "%s " name_desc
	while {$name_desc!= "</GENERAL>" } {
	    if {$name_desc== "Name=" } {
		## read names consisting of more than one word:
                regsub  "Name=" $line_in_file "" myname
                set myname [string trim $myname]
                ## name read
	    } elseif {$name_desc== "Type=" } {
		scan $line_in_file "%s %s" name_desc mytype
	    } elseif {$name_desc== "Call=" } {
		scan $line_in_file "%s %s" name_desc mycall
	    } elseif {$name_desc== "Code=" } {
		scan $line_in_file "%s %s" name_desc mycode		
	    } elseif {$name_desc== "Help=" } {
		scan $line_in_file "%s %s" name_desc myhelp
	    } elseif {$name_desc== "TKfile_test=" } {
		scan $line_in_file "%s %s" name_desc mytkfile
	    } 

	    if { $mytkfile!="" } {
		file copy -force [TC_getvar LISAHOME]/tcl/tksource/$mytkfile  $output_file
		return
	    }
	    set line_in_file [gets $description_file_id]
	    set name_desc ""
	    scan $line_in_file "%s " name_desc  
	}
	if { $myname=="" } {
	  	puts "no entry: myname= found in file: $description_file"
	}
  	if { $mytype=="" } {
	  	puts "no entry: mytype= found in file: $description_file"
	}
  	if { $mycall=="" } {
	  	puts "no entry: mycall= found in file: $description_file"
	}
	if { $mycode=="" } {
	  	puts "no entry: mycode= found in file: $description_file"
	}
	if { $myhelp=="" } {
	    puts "no entry: myhelp= found in file: $description_file"
	}

#	scan $line_in_file "%s %s" name_desc algo_call

	#write first part of file
	
	if { [file exists output_file]=="1" } { file delete output_file } 
	set output_file_id [open $output_file w]


seek $description_file_id 0
 	puts $output_file_id "

proc vTclWindow.$algo_name\_ptst \{base\} \{
    global Name
    global $algo_name\_ptst
    global ext_algo
    global env
    set $algo_name\_ptst\(ALGO_CALL\) $mycall
    if \{\$base == \"\"\} \{
        set base .$algo_name\_ptst
    \}
    if \{\[winfo exists \$base\]\} \{
        wm deiconify \$base; return
    \}
    toplevel \$base -class Toplevel  
    wm focusmodel \$base passive
	set w \[expr \[winfo reqwidth \$base\]+120\]
	set h \[expr \[winfo reqheight \$base\]+30\]
	wm geometry \$base \$w\\x\$h
    wm overrideredirect \$base 0
    wm resizable \$base 1 1
    wm deiconify \$base
    wm transient \$base .lisa
"

#write second part of file
	puts $output_file_id "
 wm title \$base \"$myname\"
"

#write for each entry in <PARAMETERS> the corresponding witget

set height 80
set description_of_parameter ""
set myfirst ""
set mylast ""
        if {[gets $description_file_id line_in_file]<0} {
	    puts "no entry: <PARAMETERS> found in file: $description_file"
	}
	set name_desc ""
	set myname ""
	scan $line_in_file "%s " name_desc
	while {$name_desc!= "<PARAMETERS>" } {
	    if {[gets $description_file_id line_in_file]<0} {
		puts "no entry: <PARAMETERS> found in file: $description_file"
	    }
	    scan $line_in_file "%s " name_desc
	}
	set line_in_file [gets $description_file_id]
	scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value 

	#set description_of_parameter [string trim $description_of_parameter \"]
	#puts "desc_para: $description_of_parameter"
	#puts "first: $myfirst   last: $mylast"
	set only_string "yes"
	while {$name_desc!= "</PARAMETERS>" } {
	    set myfirst [string first \" $line_in_file  ]
	    set mylast [string last \" $line_in_file  ]
	    set description_of_parameter [string range $line_in_file $myfirst $mylast]
	    if { $default_value=="(" } {
		set myfirst [string first "(" $line_in_file  ]
		set mylast [string last ")" $line_in_file  ]
		set default_values [string range $line_in_file \
					[expr $myfirst +1 ] [expr $mylast -1 ] ]
	    }
	    # now insert the corresponding widget in window

	    set no_of_entrys [expr $no_of_entrys +1]
	    
	    puts $output_file_id "
	    set $algo_name\_ptst\($no_of_entrys,TYPE) $name_desc
	    set $algo_name\_ptst\($no_of_entrys,NAME_OF_PARAMETER) $name_of_parameter
	    set $algo_name\_ptst\($name_of_parameter\_var) \"fixed\"
	    set $algo_name\_ptst\($name_of_parameter\_fa) \"FACTOR\"
	    set $algo_name\_ptst\($no_of_entrys,VAR_OR_FIX) \$$algo_name\_ptst\($name_of_parameter\_var)
	    "
	    if { $name_desc=="double" || $name_desc=="long" } {
		set height [expr $height+64]
		set only_string "no"
		puts $output_file_id "
		frame \$base.f$name_of_parameter \
			-borderwidth 3 -height 64 -relief groove \
			-width 640 
		label \$base.f$name_of_parameter.01 \
			-anchor w -borderwidth 1 \
			-text $description_of_parameter
		label \$base.f$name_of_parameter.02 \
		        -borderwidth 1 -text \$Name(from) 
		label \$base.f$name_of_parameter.03 \
			-borderwidth 1 -text \$Name(to)
		menubutton \$base.f$name_of_parameter.04 \
			-indicatoron 1 \
			-menu \$base.f$name_of_parameter.04.m -padx 5 -pady 4 \
			-relief raised -text \$Name(FACTOR) 
		menu \$base.f$name_of_parameter.04.m \
			-cursor {} \
			-tearoff 0
		\$base.f$name_of_parameter.04.m add command \
			-label \$Name(FACTOR) \
			-command {nbptst_set.$algo_name \"$name_of_parameter\_FA\" \"FACTOR\"}
		\$base.f$name_of_parameter.04.m add command \
			-label \$Name(ADDEND) \
			-command {nbptst_set.$algo_name \"$name_of_parameter\_FA\" \"ADDEND\"}
#		nbptst_set.$algo_name $name_of_parameter\_FA \$$algo_name\_ptst\($name_of_parameter\_fa)
		set $algo_name\_ptst\($name_of_parameter\_init) $default_value
		set $algo_name\_ptst\($name_of_parameter\_max) $default_value
		set $algo_name\_ptst\($name_of_parameter\_incr) 2
#		puts \$$algo_name\_ptst\($name_of_parameter\_init)
		entry \$base.f$name_of_parameter.05 \
			-textvariable $algo_name\_ptst\($name_of_parameter\_init)
		entry \$base.f$name_of_parameter.06 \
			-textvariable $algo_name\_ptst\($name_of_parameter\_max)
		entry \$base.f$name_of_parameter.07 \
			-textvariable $algo_name\_ptst\($name_of_parameter\_incr)
		menubutton \$base.f$name_of_parameter.08 \
			-indicatoron 1 \
			-menu \$base.f$name_of_parameter.08.m -padx 5 -pady 4 \
			-relief raised -text \$Name(fixed) 
		menu \$base.f$name_of_parameter.08.m \
			-cursor {} \
			-tearoff 0
		\$base.f$name_of_parameter.08.m add command \
			-label \$Name(variable) \
			-command {nbptst_set.$algo_name \"$name_of_parameter\_var\" \"variable\"; set $algo_name\_ptst\($no_of_entrys,VAR_OR_FIX) \$$algo_name\_ptst\($name_of_parameter\_var)}
		\$base.f$name_of_parameter.08.m add command \
			-label \$Name(fixed) \
			-command {nbptst_set.$algo_name \"$name_of_parameter\_var\" \"fixed\"; set $algo_name\_ptst\($no_of_entrys,VAR_OR_FIX) \$$algo_name\_ptst\($name_of_parameter\_var)}
		"
	    }
	    if { $name_desc=="string" } {
		set height [expr $height+32]
		puts $output_file_id "
		frame \$base.f$name_of_parameter \
			-borderwidth 3 -height 33 -relief groove \
			-width 720 
		label \$base.f$name_of_parameter.01 \
			-anchor w -borderwidth 1 \
			-text $description_of_parameter
		set $algo_name\_ptst\($name_of_parameter) $default_value
#		entry \$base.f$name_of_parameter.05 \
#			-textvariable $algo_name\_ptst\($name_of_parameter)
		menubutton \$base.f$name_of_parameter.men \
			-indicatoron 1 \
			-menu  \$base.f$name_of_parameter.men.m \
			-padx 5 -pady 4 \
			-relief sunken -text \"   \"
		menu  \$base.f$name_of_parameter.men.m \
			-cursor \{\} -tearoff 0
		"
		foreach p_name $default_values {
		    puts $output_file_id "
		    \$base.f$name_of_parameter.men.m add command \
			    -label $p_name -command \{ \
			    $algo_name\_ptst\_set f$name_of_parameter.men $p_name 
		    set  $algo_name\_ptst\($name_of_parameter) $p_name \}
		    \$base.f$name_of_parameter.men  config -text $p_name
		    set $algo_name\_ptst\($name_of_parameter) $p_name
		"
		}
	    }
	    if { $name_desc=="double" || $name_desc=="long" } {
		puts $output_file_id "
		bind \$base.f$name_of_parameter.05 <Return> \{
		    set $algo_name\_ptst\($name_of_parameter\_init) \[float_value \$$algo_name\_ptst\($name_of_parameter\_init)\]
		\}
		bind \$base.f$name_of_parameter.05 <Tab> \{
		    set $algo_name\_ptst\($name_of_parameter\_init) \[float_value \$$algo_name\_ptst\($name_of_parameter\_init)\]
		\}

		bind \$base.f$name_of_parameter.06 <Return> \{
		    set $algo_name\_ptst\($name_of_parameter\_max) \[float_value \$$algo_name\_ptst\($name_of_parameter\_max)\]
		\}
		bind \$base.f$name_of_parameter.06 <Tab> \{
		    set $algo_name\_ptst\($name_of_parameter\_max) \[float_value \$$algo_name\_ptst\($name_of_parameter\_max)\]
		\}

		bind \$base.f$name_of_parameter.07 <Return> \{
		    set $algo_name\_ptst\($name_of_parameter\_incr) \[float_value \$$algo_name\_ptst\($name_of_parameter\_incr)\]
		\}
		bind \$base.f$name_of_parameter.07 <Tab> \{
		    set $algo_name\_ptst\($name_of_parameter\_incr) \[float_value \$$algo_name\_ptst\($name_of_parameter\_incr)\]
		\}
		" 
	    }
	    puts $output_file_id "
	    pack \$base.f$name_of_parameter \
		    -in .$algo_name\_ptst -anchor nw \
		    -expand 0 -fill x -side top
	    "
	    if { $name_desc=="double" || $name_desc=="long" } {
		puts $output_file_id "
		place \$base.f$name_of_parameter.01 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 3 -y 3 -width 279 -height 28 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.02 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 523 -y 3 -width 59 -height 28 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.03 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 523 -y 33 -width 59 -height 28 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.04 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 283 -y 33 -width 119 -height 28 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.05 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 583 -y 3 -width 119 -height 27 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.06 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 583 -y 33 -width 119 -height 27 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.07 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 403 -y 33 -width 119 -height 27 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.08 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 283 -y 3 -width 119 -height 28 -anchor nw \
			-bordermode ignore
		"
	    }

##################################################################
# For each string parameter, place a field containing its name and 
# a second field contaning the selection list: 

	    if { $name_desc=="string" } {
		puts $output_file_id "
		place \$base.f$name_of_parameter.01 \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 3 -y 3 -width 279 -height 27 -anchor nw \
			-bordermode ignore
		place \$base.f$name_of_parameter.men \
			-in .$algo_name\_ptst.f$name_of_parameter \
			-x 283 -y 3 -width 419 -height 27 -anchor nw \
			-bordermode ignore
		"
	    }
	    set line_in_file [gets $description_file_id]
	    scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value

	 #   set description_of_parameter [string trim $description_of_parameter \"]
	}

	#write end
	#set height [expr $height+32]
	puts $output_file_id "
    set $algo_name\_ptst\(FILE) \"[TC_getvar LISACONFIG]\/proc\/ptst\_out.lsa\"
"
if { $only_string=="no" } { 
    set height [expr $height+32]
    puts $output_file_id "

    frame \$base.fileframe \
	-borderwidth 3 -height 32 -relief groove \
        -width 125
    entry \$base.fileframe.01 \
	-textvar $algo_name\_ptst\(FILE)
    \$base.fileframe.01 xview 300
    button \$base.fileframe.02 \
	-padx 11 -pady 4 -text \$Name(out_file) \
	-command \{nbptst_set.$algo_name \"FILE\" \
	          \[tk_getSaveFile  -defaultextension .lsa \
			           -filetypes \$mw(filetypes) \
				   -initialfile \$$algo_name\_ptst\(FILE) \
				   -initialdir \"[TC_getvar LISACONFIG]\/proc\" \
			           -title \$Name(out_file)\];
                   .$algo_name\_ptst.fileframe.01 xview 300\}
    pack \$base.fileframe \
	-in .$algo_name\_ptst -anchor center -expand 1 -fill both -side top
    place \$base.fileframe.02 \
        -x 3 -y 3 -width 279 -height 28 -anchor nw -bordermode ignore 
    place \$base.fileframe.01 \
        -x 283 -y 3 -width 419 -height 27 -anchor nw -bordermode ignore
    "
	       }
    puts $output_file_id "
    frame \$base.lastframe \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    button \$base.lastframe.01 \
	-padx 11 -pady 4 -text \$Name(START_NB) -command {destroy .$algo_name\_ptst; TC_startparam  $algo_name} 
    button \$base.lastframe.02 \
	-padx 11 -pady 4 -text \$Name(Cancel)  -command {destroy .$algo_name\_ptst}
    button \$base.lastframe.03 \
	-padx 11 -pady 4 -text \$Name(Help)  -command { lisa_help utility/gen_ptst.html }
    pack \$base.lastframe \
        -in .$algo_name\_ptst -anchor center -expand 1 -fill both -side top 
    pack \$base.lastframe.01 \
        -in .$algo_name\_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 20 \
        -ipady 4 -padx 20 -pady 20 -side left 
    pack \$base.lastframe.02 \
        -in .$algo_name\_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 5 \
        -ipady 4 -padx 20 -pady 20 -side left
    pack \$base.lastframe.03 \
        -in .$algo_name\_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 5 \
        -ipady 4 -padx 20 -pady 20 -side right

    wm geometry \$base 705x$height
    bind .$algo_name\_ptst <Visibility> \{  \}
    set $algo_name\_ptst\(NO_OF_ENTRYS) $no_of_entrys
    set ext_algo(objective\) 0
    set ext_algo(running\) 1
\}

proc $algo_name\_ptst\_set \{ menu value \} \{
    global $algo_name\_ptst
    .$algo_name\_ptst.\$menu config -text \$value
    \}


\#Window show .
\#Window show .$algo_name\_ptst

\#main \$argc \$argv "


seek $description_file_id 0
set line_in_file [gets $description_file_id]
scan $line_in_file "%s " name_desc
 	while {$name_desc!= "<PARAMETERS>" } {
	    set line_in_file [gets $description_file_id]
	    scan $line_in_file "%s " name_desc
	}

puts $output_file_id "
proc nbptst_set.$algo_name \{name value\} \{
    global $algo_name\_ptst\;
    global Name;

    if \{\$name \=\= \"FILE\"\} \{
	if \{\$value \!\= \"\"\} \{
	    set $algo_name\_ptst\(FILE) \$value
	\}
    \}
    "
    set line_in_file [gets $description_file_id]
    scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value
    while {$name_desc!= "</PARAMETERS>" } {
	if { $name_desc=="double" || $name_desc=="long" } {
	    puts $output_file_id "
	    if \{\$name \=\= \"$name_of_parameter\_FA\"\} \{
		set $algo_name\_ptst\($name_of_parameter\_fa\) \$value
		.$algo_name\_ptst.f$name_of_parameter.04  config -text \$Name\(\$value\)
	    \}
	    if \{\$name \=\= \"$name_of_parameter\_var\"\} \{
		set $algo_name\_ptst\($name_of_parameter\_var\) \$value
		.$algo_name\_ptst.f$name_of_parameter.08  config -text \$Name\(\$value\)
	    \} "
	}
        set line_in_file [gets $description_file_id]
	scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value
    }
puts $output_file_id "
\} "

	
	close $output_file_id
    } else { puts "file: $description_file not exists" }
}









