
# this function reads all information from the algorithm description files and
# builds tcl-scripts (using the LiSA-program xml2tcl), that handle parameter input and
# algorithm startup
proc {read_all_desc_files_xml} { } {
    global env
    global lsa_status
    set lsa_status(list_of_external_alg) ""
    set descr_dir "[TC_getvar LISAHOME]/data/alg_desc/language/[TC_getvar LANGUAGE]"    
    set source_dir "[TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/external"
				set bin_dir "[TC_getvar LISAHOME]/bin"
    cd $descr_dir
    set filelist ""

    catch {set filelist [glob "*.xml" ] }
    set filename "" 
    set newtime ""
    set newesttime ""

    foreach filename $filelist {
		set newtime [file mtime $filename]
		if { $newesttime<$newtime } { set newesttime $newtime }
		set filename [file rootname $filename]
		lappend lsa_status(list_of_external_alg) $filename
    }
    
    # skip if the xml-files haven't changed
    if { [file exists $source_dir/ext_alg.tcl] } {

		if { $newesttime <= [file mtime $source_dir/ext_alg.tcl] } {
			return
		}
    }
				
    file delete "$source_dir/*.tcl"
    set ext_alg [open "$source_dir/ext_alg.tcl" "w"]
    
	# the following loop takes very much time to evaluate on Vista
    foreach filename $filelist {
                                
		set filename [file rootname $filename]
		catch {exec $bin_dir/xml2tcl -s -N $filename $descr_dir/$filename.xml $source_dir/$filename.tcl}
		puts $ext_alg "source \"\[TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/external/$filename.tcl\""
		set ptst_filename "$filename\_ptst"
		catch {exec $bin_dir/xml2tcl -P -s -N $filename $descr_dir/$filename.xml $source_dir/$ptst_filename.tcl}
		puts $ext_alg "source \"\[TC_getvar LISAHOME]/tcl/language/[TC_getvar LANGUAGE]/external/$ptst_filename.tcl\""
    }
    puts $ext_alg "set lsa_status(OLD_LISAHOME) [TC_getvar LISAHOME]"
    puts $ext_alg "set lsa_status(OLD_LANGUAGE) [TC_getvar LANGUAGE]"
				puts $ext_alg "set glob(OLD_LISAHOME) [TC_getvar LISAHOME]"
    puts $ext_alg "set glob(OLD_LANGUAGE) [TC_getvar LANGUAGE]"
    close $ext_alg
}

