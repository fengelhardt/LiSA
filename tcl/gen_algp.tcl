proc {genparawin} {description_file output_file algo_name} {
    global env
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
	    while {$name_desc== "\#" } {
		set line_in_file [gets $description_file_id]
		scan $line_in_file "%s " name_desc 
	    }
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
	    } elseif {$name_desc== "TKfile=" } {
		scan $line_in_file "%s %s" name_desc mytkfile
	    } elseif {$name_desc== "" } {
		puts "no entry: </GENERAL> found in file: $description_file"
	    }

	    if { $mytkfile!="" } {
		file copy -force $env(LISAHOME)/tcl/tksource/$mytkfile  $output_file
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

 	puts $output_file_id "
set ext_algo\(NAME,$algo_name\) \"$myname\"
set ext_algo\(TYPE,$algo_name\) $mytype
proc vTclWindow.$algo_name \{base\} \{

    global $algo_name
    global ext_algo
    global Name

    set $algo_name\(ALGO_CALL\) $mycall
    set $algo_name\(NAME\) \"$myname\"
    set $algo_name\(CODE\) $mycode
    set $algo_name\(HELPFILE\) $myhelp
    if \{\$base == \"\"\} \{
        set base .$algo_name
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

#write foreach entry in <PARAMETERS> the corresponding witget

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
 	while {$name_desc!= "</PARAMETERS>" } {
	    if {$name_desc!= "\#" } {
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
	    
	    set height [expr $height+32]
	    set no_of_entrys [expr $no_of_entrys +1]
	    
	    puts $output_file_id "
    set $algo_name\($no_of_entrys,TYPE) $name_desc
    set $algo_name\($no_of_entrys,NAME_OF_PARAMETER) $name_of_parameter
    frame \$base.f$name_of_parameter \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    label \$base.f$name_of_parameter.01 \
        -anchor w -borderwidth 1 \
        -text $description_of_parameter \
        -width 40
    "
	    if { $default_value!="(" } {
		if { $name_of_parameter=="UPPER_BOUND" } {
		    puts $output_file_id "
    set $algo_name\($name_of_parameter\) \[TC_getvar upper_bound\]
    if \{ \$$algo_name\($name_of_parameter\)==\"schedule_not_valid\"\} \{
    set $algo_name\($name_of_parameter) $default_value
    \}
    "
		} else {
		    puts $output_file_id "
    set $algo_name\($name_of_parameter) $default_value
    "
		}
		puts $output_file_id "
    entry \$base.f$name_of_parameter.02 -textvariable $algo_name\($name_of_parameter)
        "
		if { $name_desc=="double" || $name_desc=="long" } {
		    puts $output_file_id "
	bind \$base.f$name_of_parameter.02 <Return> \{
	set $algo_name\($name_of_parameter) \[float_value \$$algo_name\($name_of_parameter)\]
\}
        bind \$base.f$name_of_parameter.02 <Tab> \{
	set $algo_name\($name_of_parameter) \[float_value \$$algo_name\($name_of_parameter)\]
\}
"         }
	    } else {
		puts $output_file_id "
        menubutton \$base.f$name_of_parameter.men \
                 -indicatoron 1 -menu  \$base.f$name_of_parameter.men.m -padx 5 -pady 4 \
                 -relief sunken -text \"   \"
        menu  \$base.f$name_of_parameter.men.m \
                 -cursor \{\} -tearoff 0
        "
		foreach p_name $default_values {
		    puts $output_file_id "
	\$base.f$name_of_parameter.men.m add command \
	       -label $p_name -command \{ \
                      $algo_name\_set f$name_of_parameter.men $p_name 
                      set  $algo_name\($name_of_parameter) $p_name \}
        \$base.f$name_of_parameter.men  config -text $p_name
        set $algo_name\($name_of_parameter) $p_name
         "
		}
	    }
	    puts $output_file_id "
    pack \$base.f$name_of_parameter \
        -in .$algo_name -anchor center -expand 0 -fill x -side top 
    pack \$base.f$name_of_parameter.01 \
        -in .$algo_name.f$name_of_parameter -anchor center -expand 0 -fill none -side left 
    "
	    if { $default_value!="(" } { 
		puts $output_file_id "
    pack \$base.f$name_of_parameter.02 \
        -in .$algo_name.f$name_of_parameter -anchor center -expand 0 -fill x -ipady 1 -side top 
    "
	    } else {
		puts $output_file_id "
    pack \$base.f$name_of_parameter.men \
        -in .$algo_name.f$name_of_parameter -anchor center -expand 0 -fill x -ipady 1 -side top 
    "
    }
	    }
	    set line_in_file [gets $description_file_id]
	    scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value
	}

	#write end

	puts $output_file_id "
    frame \$base.lastframe \
        -borderwidth 2 -height 75 -relief groove \
        -width 125 
    pack \$base.lastframe \
        -in .$algo_name -anchor center -expand 1 -fill both -side top 
        "
	if { $no_of_entrys>0 } {
	    puts $output_file_id "
    button \$base.lastframe.01 \
	-padx 11 -pady 4 -text \$Name(OK) -command {destroy .$algo_name; TC_startalg  $algo_name} 
    pack \$base.lastframe.01 \
        -in .$algo_name.lastframe -anchor center -expand 0 -fill none -ipadx 20 \
        -ipady 4 -padx 20 -pady 20 -side left 
         "
	    if { $myhelp != "" } {
		puts $output_file_id "
    button \$base.lastframe.help \
	-padx 11 -pady 4 -text \$Name(Help) -command \{ lisa_help $myhelp \} 
    pack \$base.lastframe.help \
        -in .$algo_name.lastframe -anchor center -expand 0 -fill none -ipadx 20 \
        -ipady 4 -padx 20 -pady 20 -side left 
               "
	    }
	    puts $output_file_id "
    button \$base.lastframe.02 \
	-padx 11 -pady 4 -text \$Name(Cancel)  -command {destroy .$algo_name}
    pack \$base.lastframe.02 \
        -in .$algo_name.lastframe -anchor center -expand 0 -fill none -ipadx 5 \
        -ipady 4 -padx 20 -pady 20 -side right 
    bind .$algo_name <Visibility> \{  grab set .$algo_name  \}
        "
	} else {
	    puts $output_file_id "	
	    bind .$algo_name <Visibility> \{ TC_startalg  $algo_name; destroy .$algo_name;  \}    
        "
	}

	# write fixed parameters
	set name_desc ""
	set test [gets $description_file_id line_in_file]
	if {$test>=0} {
	    scan $line_in_file "%s " name_desc
	    while {$name_desc!= "<FIXED>" && $test>=0} {
		set name_desc ""
		set test [gets $description_file_id line_in_file]
		scan $line_in_file "%s " name_desc
	    }
	    if { $test>=0 } {
		set test [gets $description_file_id line_in_file]
		scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value
	 	while {$name_desc!= "</FIXED>" &&  $test>=0 } {
		    if {$name_desc!= "\#" } {
		    set no_of_entrys [expr $no_of_entrys +1]
		    puts $output_file_id "
        set $algo_name\($no_of_entrys,TYPE) $name_desc
        set $algo_name\($no_of_entrys,NAME_OF_PARAMETER) $name_of_parameter
        set $algo_name\($name_of_parameter) $default_value
        "
		    }
		    set test [gets $description_file_id line_in_file]
		    scan $line_in_file "%s %s %s" name_desc name_of_parameter default_value
		} 
		if {$name_desc!= "</FIXED>"} {
		    puts "no entry: </FIXED> found in file: $description_file"
		}
	    }
	}


	puts $output_file_id "	
    wm geometry \$base 450x$height
    set $algo_name\(NO_OF_ENTRYS\) $no_of_entrys
    set ext_algo\(objective\) 0
    set ext_algo\(running\) 1
\}

proc $algo_name\_set \{ menu value \} \{
    global $algo_name
    .$algo_name.\$menu config -text \$value
    \}
"
	close $output_file_id
    } else { puts "file: $description_file not exists" }
}

proc {read_all_desc_files} { } {
    global env
    global lsa_status
    set lsa_status(list_of_external_alg) ""
    set descr_dir "$env(LISAHOME)/data/alg_desc/language/$env(LLANGUAGE)"    
    set source_dir "$env(LISAHOME)/tcl/external"
    file mkdir "$env(LISAHOME)/tcl/external"
    cd $descr_dir
    set filelist ""
    catch {set filelist [glob "*.alg" ] }
    set filename "" 
    set newtime ""
    set newesttime ""
    foreach filename $filelist {
	set newtime [file mtime $filename]
	if { $newesttime<$newtime } { set newesttime $newtime }
	set filename [string trimright $filename ".alg"]
	lappend lsa_status(list_of_external_alg) $filename
    }

    if { [file exists $source_dir/ext_alg.tcl] } {
	if { $newesttime< [file mtime $source_dir/ext_alg.tcl] } {
	    return
	}
    }
    file delete "$source_dir/*.tcl"
    set ext_alg [open "$source_dir/ext_alg.tcl" "w"]
    foreach filename $filelist {
	set filename [string trimright $filename ".alg"]
	genparawin "$descr_dir/$filename.alg" "$source_dir/$filename.tcl" $filename
	puts $ext_alg "source \"$source_dir/$filename.tcl\""
	set ptst_filename "$filename\_ptst"
	
 	genparatestwin "$descr_dir/$filename.alg" "$source_dir/$ptst_filename.tcl" $filename
 	puts $ext_alg "source \"$source_dir/$ptst_filename.tcl\""
    }
    puts $ext_alg "set lsa_status(OLD_LISAHOME) $env(LISAHOME)"
    puts $ext_alg "set lsa_status(OLD_LANGUAGE) $env(LLANGUAGE)"
    close $ext_alg
}


