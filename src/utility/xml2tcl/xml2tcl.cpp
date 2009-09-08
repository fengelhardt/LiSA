/*
 * This program reads a xml-algorithm-description file and generates two tcl scripts from it.
 *
 * The first one handles parameter input and algorithm startup for the LiSA-GUI.
 * The second script handles parameter input and startup of the automated parameter test.
 */

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#include "../../lisa/ptype.hpp"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define LISA_XML_IMPL

#include "../../main/defaults.hpp"
#include "../../xml/LisaXmlFile.hpp"
#include "../../xml/xmldefs.hpp"

#include "../../main/lisapref.hpp"



void writeTcl_AlgWindow(std::string algo_name, 
														const struct Lisa_ExtAlg& Alg, 
														std::string filename)
{
		ofstream out;
		out.open(filename.c_str());
		if(!out.good())
				{
						cerr << "TclWriter error : Bad filename" << endl; return;
				}
		unsigned height = 80;
		out << "# automatically generated script file #DO NOT EDIT#" << endl << endl ;
		out << "# header .... " << endl << endl;
		out << "set ext_algo(NAME," << algo_name << ") \"" << Alg.name << '\"' << endl;
		out << "set ext_algo(TYPE," << algo_name << ") " << Alg.type  << endl;
		out << "proc  vTclWindow." << algo_name << " {base} {" << endl << endl;
		out << "   global " << algo_name << endl;
		out << "   global ext_algo" << endl;
		out << "   global Name" << endl << endl;
		out << "   set " << algo_name << "(ALGO_CALL) " << Alg.call << endl;
		out << "   set " << algo_name << "(NAME) \"" << Alg.name << "\"" <<endl;
		out << "   set " << algo_name << "(CODE) " << Alg.code << endl;
		out << "   set " << algo_name << "(HELPFILE) " << Alg.helpFile << endl;
		out << "   if {$base == \"\"} {" << endl;
		out << "       set base ." << algo_name << endl;
		out << "   }" << endl;
		out << "   if {[winfo exists $base]} {" << endl;
		out << "       wm deiconify $base; return" << endl;
		out << "   }" << endl;
		out << "   toplevel $base -class Toplevel" << endl;
		out << "   wm focusmodel $base passive" << endl;
		out << "   set w [expr [winfo reqwidth $base]+120]" << endl;
		out << "   set h [expr [winfo reqheight $base]+30]" << endl;
		// Neue Code
		out << "   set xPos [expr {([winfo screenwidth .]-$w)/2}]" << endl;
		out << "   set yPos [expr {([winfo screenheight .]-$h)/2}]" << endl;
		out << "   wm geometry $base $w\\x$h+$xPos+$yPos" << endl;
		// Ende
		//out << "   wm geometry $base $w\\x$h" << endl;
		out << "   wm overrideredirect $base 0" << endl;
		out << "   wm resizable $base 1 1" << endl;
		out << "   wm deiconify $base" << endl;
		out << "   wm transient $base .lisa" << endl;
		out << "   wm title $base \"" << Alg.name << '\"' << endl;
		out << "# inserting widgets .... " << endl << endl;
		unsigned entry = 1;
		for(unsigned i = 0; i < Alg.Integer_Controls.size(); i++)
				{
						height += 32;
						out << "   set " << algo_name << "(" << entry << ",TYPE) long" << endl;
						out << "   set " << algo_name << "(" << entry << ",NAME_OF_PARAMETER) " << Alg.Integer_Controls[i].name << endl;
						out << "   frame $base.f" << Alg.Integer_Controls[i].name << " -borderwidth 2 -height 75 -relief groove -width 125" << endl;
						out << "   label $base.f" << Alg.Integer_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" << Alg.Integer_Controls[i].caption << "\" -width 40" << endl;
						if(Alg.Integer_Controls[i].name == "UPPER_BOUND")
								{
										out << "   set " << algo_name << "(UPPER_BOUND) [TC_getvar upper_bound]" << endl;
										out << "   if {$" << algo_name << "(UPPER_BOUND)==\"schedule_not_valid\"} {" << endl;
										out << "      set " << algo_name << "(UPPER_BOUND) " << Alg.Integer_Controls[i].default_value << endl;
										out << "   }" << endl;
										
								}
						else
								out << "   set " << algo_name << "(" << Alg.Integer_Controls[i].name << ") " <<  Alg.Integer_Controls[i].default_value << endl;
						out << "   entry $base.f" <<  Alg.Integer_Controls[i].name << ".02 -textvariable " << algo_name << "(" <<  Alg.Integer_Controls[i].name << ")" << " -validate all -vcmd {validInteger %W %v %V %P " <<  algo_name << "(" << Alg.Integer_Controls[i].name << ")" << " }" << endl;
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".02 <Return> {" << endl;
						out << "     set " <<  algo_name << "(" << Alg.Integer_Controls[i].name << ") [float_value $" << algo_name << "(" <<  Alg.Integer_Controls[i].name << ")]" << endl;
						out << "   }" << endl;
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".02 <Tab> {" << endl;
						out << "     set " <<  algo_name << "(" << Alg.Integer_Controls[i].name << ") [float_value $" << algo_name << "(" <<  Alg.Integer_Controls[i].name << ")]" << endl;
						out << "   }" << endl;
						out << "   pack $base.f" << Alg.Integer_Controls[i].name << " -in ." << algo_name << " -anchor center -expand 0 -fill x -side top" << endl;
						out << "   pack $base.f" << Alg.Integer_Controls[i].name << ".01 -in ." << algo_name << ".f" << Alg.Integer_Controls[i].name << " -anchor center -expand 0 -fill none -side left" << endl;
						out << "   pack $base.f" << Alg.Integer_Controls[i].name << ".02 -in ." << algo_name << ".f" << Alg.Integer_Controls[i].name << " -anchor center -expand 0 -fill x -ipady 1 -side top" << endl;
						entry++;
				}
		for(unsigned i = 0; i < Alg.Real_Controls.size(); i++)
				{
						height += 32;
						out << "   set " << algo_name << "(" << entry << ",TYPE) double" << endl;
						out << "   set " << algo_name << "(" << entry << ",NAME_OF_PARAMETER) " << Alg.Real_Controls[i].name << endl;
						out << "   frame $base.f" << Alg.Real_Controls[i].name << " -borderwidth 2 -height 75 -relief groove -width 125" << endl;
						out << "   label $base.f" << Alg.Real_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" << Alg.Real_Controls[i].caption << "\" -width 40" << endl;
						if(Alg.Real_Controls[i].name == "UPPER_BOUND")
								{
										out << "   set " << algo_name << "(UPPER_BOUND) [TC_getvar upper_bound]" << endl;
										out << "   if {$" << algo_name << "(UPPER_BOUND)==\"schedule_not_valid\"} {" << endl;
										out << "      set " << algo_name << "(UPPER_BOUND) " << Alg.Real_Controls[i].default_value << endl;
										out << "   }" << endl;
								}
						else
								out << "   set " << algo_name << "(" << Alg.Real_Controls[i].name << ") " <<  Alg.Real_Controls[i].default_value << endl;
						out << "   entry $base.f" <<  Alg.Real_Controls[i].name << ".02 -textvariable " << algo_name << "(" <<  Alg.Real_Controls[i].name << ")" << " -validate all -vcmd {validReal %W %v %V %P " <<  algo_name << "(" << Alg.Real_Controls[i].name << ")" << " }" << endl;
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".02 <Return> {" << endl;
						out << "     set " <<  algo_name << "(" << Alg.Real_Controls[i].name << ") [float_value $" << algo_name << "(" <<  Alg.Real_Controls[i].name << ")]" << endl;
						out << "   }" << endl;
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".02 <Tab> {" << endl;
						out << "     set " <<  algo_name << "(" << Alg.Real_Controls[i].name << ") [float_value $" << algo_name << "(" <<  Alg.Real_Controls[i].name << ")]" << endl;
						out << "   }" << endl;
						out << "   pack $base.f" << Alg.Real_Controls[i].name << " -in ." << algo_name << " -anchor center -expand 0 -fill x -side top" << endl;
						out << "   pack $base.f" << Alg.Real_Controls[i].name << ".01 -in ." << algo_name << ".f" << Alg.Real_Controls[i].name << " -anchor center -expand 0 -fill none -side left" << endl;
						out << "   pack $base.f" << Alg.Real_Controls[i].name << ".02 -in ." << algo_name << ".f" << Alg.Real_Controls[i].name << " -anchor center -expand 0 -fill x -ipady 1 -side top" << endl;
						entry++;
				}
		for(unsigned i = 0; i < Alg.Choice_Controls.size(); i++)
				{
						height += 32;
						out << "   set " << algo_name << "(" << entry << ",TYPE) string" << endl;
						out << "   set " << algo_name << "(" << entry << ",NAME_OF_PARAMETER) " << Alg.Choice_Controls[i].name << endl;
						out << "   frame $base.f" << Alg.Choice_Controls[i].name << " -borderwidth 2 -height 75 -relief groove -width 125" << endl;
						out << "   label $base.f" << Alg.Choice_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" << Alg.Choice_Controls[i].caption << "\" -width 40" << endl;
						out << "   menubutton $base.f" << Alg.Choice_Controls[i].name << ".men -indicatoron 1 -menu $base.f" << Alg.Choice_Controls[i].name << ".men.m -padx 5 -pady 4 -relief sunken -text \"   \"" << endl;
						out << "   menu $base.f" << Alg.Choice_Controls[i].name << ".men.m -cursor {} -tearoff 0" << endl;
						for(unsigned j = 0; j < Alg.Choice_Controls[i].items.size(); j++)
								{
										out << "   $base.f" << Alg.Choice_Controls[i].name << ".men.m add command -label " << Alg.Choice_Controls[i].items[j] << " -command { " << algo_name << "_set f" << Alg.Choice_Controls[i].name << ".men " << Alg.Choice_Controls[i].items[j] << endl
														<< "    set " << algo_name << "(" << Alg.Choice_Controls[i].name << ") " << Alg.Choice_Controls[i].items[j] << " }" << endl;
										out << "   $base.f" << Alg.Choice_Controls[i].name << ".men config -text " << Alg.Choice_Controls[i].items[j] << endl;
										out << "   set " << algo_name << "(" << Alg.Choice_Controls[i].name << ") " << Alg.Choice_Controls[i].items[j] << endl;
								}
						out << "   pack $base.f" << Alg.Choice_Controls[i].name << " -in ." << algo_name << " -anchor center -expand 0 -fill x -side top" << endl;
						out << "   pack $base.f" << Alg.Choice_Controls[i].name << ".01 -in ." << algo_name << ".f" << Alg.Choice_Controls[i].name << " -anchor center -expand 0 -fill none -side left" << endl;
						out << "   pack $base.f" << Alg.Choice_Controls[i].name << ".men -in ." << algo_name << ".f" << Alg.Choice_Controls[i].name << " -anchor center -expand 0 -fill x -ipady 1 -side top" << endl;
						entry++;
				}
		out << "# epilog ..... " << endl << endl;
		out << "   frame $base.lastframe -borderwidth 2 -height 75 -relief groove -width 125" << endl;
		out << "   pack $base.lastframe -in ." << algo_name << " -anchor center -expand 1 -fill both -side top" << endl;
		if(entry > 1)
				{
						out << "   button $base.lastframe.01 -padx 11 -pady 4 -text $Name(OK) -command {destroy ." << algo_name << "; TC_startalg " << algo_name << "}" << endl;
						out << "   pack $base.lastframe.01 -in ." << algo_name << ".lastframe -anchor center -expand 0 -fill none -ipadx 20 -ipady 4 -padx 20 -pady 20 -side left" << endl;
						if(!Alg.helpFile.empty())
								{
										out << "   button $base.lastframe.help -padx 11 -pady 4 -text $Name(Help) -command {lisa_help " << Alg.helpFile << "}" << endl;
										out << "   pack $base.lastframe.help -in ." << algo_name << ".lastframe -anchor center -expand 0 -fill none -ipadx 20 -ipady 4 -padx 20 -pady 20 -side left" << endl;
								}
						out << "   button $base.lastframe.02 -padx 11 -pady 4 -text $Name(Cancel) -command {destroy ." << algo_name << "}" << endl;
						out << "   pack $base.lastframe.02 -in ." << algo_name << ".lastframe -anchor center -expand 0 -fill none -ipadx 5 -ipady 4 -padx 20 -pady 20 -side right" << endl;
						out << "   bind ." << algo_name << " <Visibility> { grab set ." << algo_name << " }" << endl;
				}
		else
				out << "   bind ." << algo_name << " <Visibility> { TC_startalg " << algo_name << "; destroy ." << algo_name << "; }" << endl;
		if(Alg.Fixed_Controls.empty())
				out << endl << endl << "# No fixed controls ..." << endl << endl;
		else
				out << endl << endl << "# Fixed controls ..." << endl << endl;
		for(unsigned i = 0; i < Alg.Fixed_Controls.size(); i++)
				{
						out << "   set " << algo_name << "(" << entry << ",TYPE) string" << endl;
						out << "   set " << algo_name << "(" << entry << ",NAME_OF_PARAMETER) " << Alg.Fixed_Controls[i].first << endl;
						out << "   set " << algo_name << "(" << Alg.Fixed_Controls[i].first << ") " << Alg.Fixed_Controls[i].second << endl;
						entry ++;
				}
		out << endl;
		//Neuer Code
		out << "   set xPos [expr {([winfo screenwidth .]-450)/2}]" << endl;
		out << "   set yPos [expr {([winfo screenheight .]-" << height << ")/2}]" << endl;
		out << "   wm geometry $base 450x" << height << "+$xPos+$yPos" << endl;
		//Ende
		//out << "   wm geometry $base 450x" << height << endl;
		out << "   set " << algo_name << "(NO_OF_ENTRYS) " << entry-1 << endl;
		out << "   set ext_algo(objective) 0" << endl;
		out << "   set ext_algo(running) 1" << endl;
		out << "}" << endl << endl << endl;
		out << "proc " << algo_name << "_set {menu value} {" << endl;
		out << "   global " << algo_name << endl;
		out << "   ." << algo_name << ".$menu config -text $value" << endl;
		out << "}" << endl <<  endl;
}



void writeTcl_ParaTest(std::string algo_name, 
														const struct Lisa_ExtAlg& Alg, 
														std::string filename)
{
		unsigned height = 80;
		ofstream out;
		out.open(filename.c_str());
		if(!out.good())
				{
						cerr << "TclWriter error : Bad filename" << endl; return;
				} 
		out << "# automatically generated script file #DO NOT EDIT#" << endl << endl ;
		out << "# header .... " << endl << endl;
		out << "proc vTclWindow." << algo_name << "_ptst {base} {" << endl;
		out << "   global Name" << endl;
		out << "   global " << algo_name << "_ptst" << endl;
		out << "   global ext_algo" << endl;
		out << "   global env" << endl;
		out << "   set " << algo_name << "_ptst(ALGO_CALL) " << Alg.call  << endl;
		out << "   if {$base == \"\"} {" << endl;
		out << "      set base ." << algo_name << "_ptst" << endl;
		out << "   }" << endl;
		out << "   if {[winfo exists $base]} {" << endl;
		out << "      wm deiconify $base; return" << endl;
		out << "   }" << endl;
		out << "   toplevel $base -class Toplevel" << endl;
		out << "   wm focusmodel $base passive" << endl;
		out << "   set w [expr [winfo reqwidth $base]+120]" << endl;
		out << "   set h [expr [winfo reqheight $base]+30]" << endl;
		// Neue Code
		out << "   set xPos [expr {([winfo screenwidth .]-$w)/2}]" << endl;
		out << "   set yPos [expr {([winfo screenheight .]-$h)/2}]" << endl;
		out << "   wm geometry $base $w\\x$h+$xPos+$yPos" << endl;
		// Ende
		//out << "   wm geometry $base $w\\x$h" << endl;
		out << "   wm overrideredirect $base 0" << endl;
		out << "   wm resizable $base 1 1" << endl;
		out << "   wm deiconify $base" << endl;
		out << "   wm transient $base .lisa" << endl;
		out << endl;
		out << "   wm title $base \"" << Alg.name << "\"" << endl;
		out << endl;
	
		out << "#  inserting widgets ....." << endl;
		out << endl;
	
	
		unsigned entry = 1;
		for(unsigned i = 0; i < Alg.Integer_Controls.size(); i++)
				{
						height += 64;
						out << "   set " << algo_name << "_ptst(" << entry << ",TYPE) long" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",NAME_OF_PARAMETER) " << Alg.Integer_Controls[i].name << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_var) \"fixed\"" << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_fa) \"FACTOR\"" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_var)" << endl;
						//end of common stuff
						out << "   frame $base.f" << Alg.Integer_Controls[i].name << " -borderwidth 3 -height 64 -relief groove -width 640" << endl;
						out << "   label $base.f" << Alg.Integer_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" <<  Alg.Integer_Controls[i].caption << "\"" << endl;
						out << "   label $base.f" << Alg.Integer_Controls[i].name << ".02 -borderwidth 1 -text $Name(from)" << endl;
						out << "   label $base.f" << Alg.Integer_Controls[i].name << ".03 -borderwidth 1 -text $Name(to)" << endl;
						out << "   menubutton $base.f" << Alg.Integer_Controls[i].name << ".04 -indicatoron 1 -menu $base.f" << Alg.Integer_Controls[i].name << ".04.m -padx 5 -pady 4 -relief raised -text $Name(FACTOR)" << endl;
						out << "   menu $base.f" << Alg.Integer_Controls[i].name << ".04.m -cursor {} -tearoff 0" << endl;
						out << "   $base.f" << Alg.Integer_Controls[i].name << ".04.m add command -label $Name(FACTOR) -command {nbptst_set." << algo_name << " \"" << Alg.Integer_Controls[i].name << "_FA\" \"FACTOR\"}" << endl;
						out << "   $base.f" << Alg.Integer_Controls[i].name << ".04.m add command -label $Name(ADDEND) -command {nbptst_set." << algo_name << " \"" << Alg.Integer_Controls[i].name << "_FA\" \"ADDEND\"}" << endl;
						//comment skipped
						out << "   set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init) " << Alg.Integer_Controls[i].default_value << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max) " << Alg.Integer_Controls[i].default_value << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr) " << "2" << endl;
						//comment skipped
						out << "   entry $base.f" << Alg.Integer_Controls[i].name << ".05 -textvariable " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init)" << " -validate all -vcmd {validInteger %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init)" << " }" << endl;
						out << "   entry $base.f" << Alg.Integer_Controls[i].name << ".06 -textvariable " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max)" << " -validate all -vcmd {validInteger %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max)" << " }"<< endl;
						out << "   entry $base.f" << Alg.Integer_Controls[i].name << ".07 -textvariable " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr)" << " -validate all -vcmd {validInteger %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr)" << " }"<< endl;
						out << "   menubutton $base.f" << Alg.Integer_Controls[i].name << ".08 -indicatoron 1 -menu $base.f" << Alg.Integer_Controls[i].name << ".08.m -padx 5 -pady 4 -relief raised -text $Name(fixed)" << endl;
						out << "   menu $base.f" << Alg.Integer_Controls[i].name << ".08.m -cursor {} -tearoff 0" << endl;
						out << "   $base.f" << Alg.Integer_Controls[i].name << ".08.m add command -label $Name(variable) -command {nbptst_set." << algo_name << " \"" <<  Alg.Integer_Controls[i].name << "_var\" \"variable\"; set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" <<  Alg.Integer_Controls[i].name << "_var)}" << endl;
						out << "   $base.f" << Alg.Integer_Controls[i].name << ".08.m add command -label $Name(fixed) -command {nbptst_set." << algo_name << " \"" <<  Alg.Integer_Controls[i].name << "_var\" \"fixed\"; set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" <<  Alg.Integer_Controls[i].name << "_var)}" << endl;
						//next part (long and double)
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".05 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init)] }" << endl;
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".05 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_init)] }" << endl;
						
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".06 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max)] }" << endl;
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".06 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_max)] }" << endl;
						
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".07 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr)] }" << endl;
						out << "   bind $base.f" <<  Alg.Integer_Controls[i].name << ".07 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr) [float_value $" << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_incr)] }" << endl;
						//next : all 
						out << endl;
						out << "   pack $base.f"  <<  Alg.Integer_Controls[i].name << " -in ." << algo_name << "_ptst -anchor nw -expand 0 -fill x -side top" << endl;
						out << endl;
						//next : long and double
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".01 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 3 -y 3 -width 279 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".02 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 523 -y 3 -width 59 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".03 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 523 -y 33 -width 59 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".04 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 283 -y 33 -width 119 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".05 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 583 -y 3 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".06 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 583 -y 33 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".07 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 403 -y 33 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Integer_Controls[i].name << ".08 -in ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << " -x 283 -y 3 -width 119 -height 28 -anchor nw -bordermode ignore" << endl;
						out << endl;

						entry++;
				}
		for(unsigned i = 0; i < Alg.Real_Controls.size(); i++)
				{
						height += 64;
						out << "   set " << algo_name << "_ptst(" << entry << ",TYPE) double" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",NAME_OF_PARAMETER) " << Alg.Real_Controls[i].name << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_var) \"fixed\"" << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_fa) \"FACTOR\"" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_var)" << endl;
						//end of common stuff
						out << "   frame $base.f" << Alg.Real_Controls[i].name << " -borderwidth 3 -height 64 -relief groove -width 640" << endl;
						out << "   label $base.f" << Alg.Real_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" <<  Alg.Real_Controls[i].caption << "\"" << endl;
						out << "   label $base.f" << Alg.Real_Controls[i].name << ".02 -borderwidth 1 -text $Name(from)" << endl;
						out << "   label $base.f" << Alg.Real_Controls[i].name << ".03 -borderwidth 1 -text $Name(to)" << endl;
						out << "   menubutton $base.f" << Alg.Real_Controls[i].name << ".04 -indicatoron 1 -menu $base.f" << Alg.Real_Controls[i].name << ".04.m -padx 5 -pady 4 -relief raised -text $Name(FACTOR)" << endl;
						out << "   menu $base.f" << Alg.Real_Controls[i].name << ".04.m -cursor {} -tearoff 0" << endl;
						out << "   $base.f" << Alg.Real_Controls[i].name << ".04.m add command -label $Name(FACTOR) -command {nbptst_set." << algo_name << " \"" << Alg.Real_Controls[i].name << "_FA\" \"FACTOR\"}" << endl;
						out << "   $base.f" << Alg.Real_Controls[i].name << ".04.m add command -label $Name(ADDEND) -command {nbptst_set." << algo_name << " \"" << Alg.Real_Controls[i].name << "_FA\" \"ADDEND\"}" << endl;
						//comment skipped
						out << "   set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init) " << Alg.Real_Controls[i].default_value << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max) " << Alg.Real_Controls[i].default_value << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr) " << "2" << endl;
						//comment skipped
						out << "   entry $base.f" << Alg.Real_Controls[i].name << ".05 -textvariable " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init)" << " -validate all -vcmd {validReal %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init)" << " }" << endl;
						out << "   entry $base.f" << Alg.Real_Controls[i].name << ".06 -textvariable " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max)" << " -validate all -vcmd {validReal %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max)" << " }" << endl;
						out << "   entry $base.f" << Alg.Real_Controls[i].name << ".07 -textvariable " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr)" << " -validate all -vcmd {validReal %W %v %V %P " <<  algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr)" << " }" << endl;
						out << "   menubutton $base.f" << Alg.Real_Controls[i].name << ".08 -indicatoron 1 -menu $base.f" << Alg.Real_Controls[i].name << ".08.m -padx 5 -pady 4 -relief raised -text $Name(fixed)" << endl;
						out << "   menu $base.f" << Alg.Real_Controls[i].name << ".08.m -cursor {} -tearoff 0" << endl;
						out << "   $base.f" << Alg.Real_Controls[i].name << ".08.m add command -label $Name(variable) -command {nbptst_set." << algo_name << " \"" <<  Alg.Real_Controls[i].name << "_var\" \"variable\"; set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" <<  Alg.Real_Controls[i].name << "_var)}" << endl;
						out << "   $base.f" << Alg.Real_Controls[i].name << ".08.m add command -label $Name(fixed) -command {nbptst_set." << algo_name << " \"" <<  Alg.Real_Controls[i].name << "_var\" \"fixed\"; set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" <<  Alg.Real_Controls[i].name << "_var)}" << endl;

						//next part (long and double)
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".05 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init)] }" << endl;
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".05 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_init)] }" << endl;
						
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".06 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max)] }" << endl;
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".06 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_max)] }" << endl;
						
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".07 <Return> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr)] }" << endl;
						out << "   bind $base.f" <<  Alg.Real_Controls[i].name << ".07 <Tab> {" << endl
										<< "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr) [float_value $" << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_incr)] }" << endl;
							//next : all 
						out << endl;
						out << "   pack $base.f"  <<  Alg.Real_Controls[i].name << " -in ." << algo_name << "_ptst -anchor nw -expand 0 -fill x -side top" << endl;
						out << endl;
						//next : long and double
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".01 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 3 -y 3 -width 279 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".02 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 523 -y 3 -width 59 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".03 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 523 -y 33 -width 59 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".04 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 283 -y 33 -width 119 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".05 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 583 -y 3 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".06 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 583 -y 33 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".07 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 403 -y 33 -width 119 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f" << Alg.Real_Controls[i].name << ".08 -in ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << " -x 283 -y 3 -width 119 -height 28 -anchor nw -bordermode ignore" << endl;
						out << endl;

						entry++;
				}
		for(unsigned i = 0; i < Alg.Choice_Controls.size(); i++)
				{
						height += 32;
						out << "   set " << algo_name << "_ptst(" << entry << ",TYPE) string" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",NAME_OF_PARAMETER) " << Alg.Choice_Controls[i].name << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Choice_Controls[i].name << "_var) \"fixed\"" << endl;
						out << "   set " << algo_name << "_ptst(" << Alg.Choice_Controls[i].name << "_fa) \"FACTOR\"" << endl;
						out << "   set " << algo_name << "_ptst(" << entry << ",VAR_OR_FIX) $" << algo_name << "_ptst(" << Alg.Choice_Controls[i].name << "_var)" << endl;
						// end of common stuff
						out << "   frame $base.f" << Alg.Choice_Controls[i].name << " -borderwidth 3 -height 33 -relief groove -width 720" << endl;
						out << "   label $base.f" << Alg.Choice_Controls[i].name << ".01 -anchor w -borderwidth 1 -text \"" << Alg.Choice_Controls[i].caption << "\"" << endl;
						//skipped weird line and two comments
						out << "   menubutton $base.f" << Alg.Choice_Controls[i].name << ".men -indicatoron 1 -menu $base.f" << Alg.Choice_Controls[i].name << ".men.m -padx 5 -pady 4 -relief sunken -text \"   \"" << endl;
						out << "   menu $base.f" << Alg.Choice_Controls[i].name << ".men.m -cursor {} -tearoff 0" << endl;
						for(unsigned j = 0; j < Alg.Choice_Controls[i].items.size(); j++)
								{
										out << "   $base.f" << Alg.Choice_Controls[i].name << ".men.m add command -label " << Alg.Choice_Controls[i].items[j] << " -command { " << algo_name << "_ptst_set f" << Alg.Choice_Controls[i].name << ".men " << Alg.Choice_Controls[i].items[j] << endl
														<< "    set " << algo_name << "_ptst(" << Alg.Choice_Controls[i].name << ") " << Alg.Choice_Controls[i].items[j] << " }" << endl;
										out << "   $base.f" << Alg.Choice_Controls[i].name << ".men config -text " << Alg.Choice_Controls[i].items[j] << endl;
										out << "   set " << algo_name << "_ptst(" << Alg.Choice_Controls[i].name << ") " << Alg.Choice_Controls[i].items[j] << endl;
								}
						//next : all 
						out << endl;
						out << "   pack $base.f"  <<  Alg.Choice_Controls[i].name << " -in ." << algo_name << "_ptst -anchor nw -expand 0 -fill x -side top" << endl;
						out << endl;
						//next : string only
						out << "   place $base.f"  <<  Alg.Choice_Controls[i].name << ".01 -in ." << algo_name << "_ptst.f" << Alg.Choice_Controls[i].name << " -x 3 -y 3 -width 279 -height 27 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.f"  <<  Alg.Choice_Controls[i].name << ".men -in ." << algo_name << "_ptst.f" << Alg.Choice_Controls[i].name << " -x 283 -y 3 -width 419 -height 27 -anchor nw -bordermode ignore" << endl;
						out << endl;
						entry++;
				}
		out << "   set " << algo_name << "_ptst(FILE) \"[TC_getvar "<< Lisa_Pref::LISACONFIG << "]/proc/" << algo_name << "_ptst_" << "tab.lsa" << "\"" << endl;
		out << endl;
		//not only stings ...
		if( !(Alg.Integer_Controls.empty()) && !(Alg.Real_Controls.empty()))
				{
						height += 32;
						out << "   frame $base.fileframe -borderwidth 3 -height 32 -relief groove -width 125" << endl;
						out << "   entry $base.fileframe.01 -textvar " << algo_name << "_ptst(FILE)" << endl;
						out << "   $base.fileframe.01 xview 300" << endl;
						out << "   button $base.fileframe.02 -padx 11 -pady 4 -text $Name(out_file) -command {nbptst_set." << algo_name << " \"FILE\" [tk_getSaveFile -defaultextension .lsa -filetypes $mw(filetypes) -initialfile $" << algo_name << "_ptst(FILE) -initialdir \"[TC_getvar " << Lisa_Pref::LISACONFIG << "]/proc\" -title $Name(out_file)];" << endl
										<< "     ." << algo_name << "_ptst.fileframe.01 xview 300}" << endl;
						out << "   pack $base.fileframe -in ." << algo_name << "_ptst -anchor center -expand 1 -fill both -side top" << endl;
						out << "   place $base.fileframe.02 -x 3 -y 3 -width -279 -height 28 -anchor nw -bordermode ignore" << endl;
						out << "   place $base.fileframe.01 -x 283 -y 3 -width -419 -height 27 -anchor nw -bordermode ignore" << endl;
						out << endl;
				}
		out << endl;
		out << "   frame $base.lastframe -borderwidth 2 -height 75 -relief groove -width 125" << endl;
		out << "   button $base.lastframe.01 -padx 11 -pady 4 -text $Name(START_NB) -command {destroy ." << algo_name << "_ptst; TC_startparam " << algo_name << "}" << endl;
		out << "   button $base.lastframe.02 -padx 11 -pady 4 -text $Name(Cancel) -command {destroy ." << algo_name << "_ptst}" << endl;
		out << "   button $base.lastframe.03 -padx 11 -pady 4 -text $Name(Help) -command { lisa_help [lisa_help_topic HELP_GEN_PTST] }" << endl;
		out << "   pack $base.lastframe -in ." << algo_name << "_ptst -anchor center -expand 1 -fill both -side top" << endl;
		out << "   pack $base.lastframe.01 -in ." << algo_name << "_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 20 -ipady 4 -padx 20 -pady 20 -side left" << endl;
		out << "   pack $base.lastframe.02 -in ." << algo_name << "_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 5 -ipady 4 -padx 20 -pady 20 -side left" << endl;
		out << "   pack $base.lastframe.03 -in ." << algo_name << "_ptst.lastframe -anchor center -expand 0 -fill none -ipadx 5 -ipady 4 -padx 20 -pady 20 -side left" << endl;
		out << endl;
		// Neue Code
		out << "   set xPos [expr {([winfo screenwidth .]-705)/2}]" << endl;
		out << "   set yPos [expr {([winfo screenheight .]-" << height << ")/2}]" << endl;
		out << "   wm geometry $base 705x" << height << "+$xPos+$yPos" << endl;
		// Ende
		//out << "   wm geometry $base 705x" << height << endl;
		out << "   bind ." << algo_name << "_ptst <Visibility> {  }" << endl;
		out << "   set " << algo_name << "_ptst(NO_OF_ENTRYS) " << (entry - 1) << endl;
		out << "   set ext_algo(objective) 0" << endl;
		out << "   set ext_algo(running) 1" << endl;
		out << "}" << endl;
		out << endl;
		out << "proc " << algo_name << "_ptst_set {menu value} {" << endl;
		out << "   global " << algo_name << "_ptst" << endl;
		out << "   ." << algo_name << "_ptst.$menu config -text $value" << endl;
		out << "}" << endl;
		out << endl;
		out << "proc nbptst_set." << algo_name << " {name value} {" << endl;
		out << "   global " << algo_name << "_ptst;" << endl;
		out << "   global Name;" << endl;
		out << "   if {$name == \"FILE\"} {" << endl;
		out << "      if {$value != \"\"} {" << endl;
		out << "         set " << algo_name << "_ptst(FILE) $value" << endl;
		out << "      }" << endl;
		out << "   }" << endl;
		out << endl;
		for(unsigned i = 0; i < Alg.Integer_Controls.size(); i++)
				{
						out << "   if {$name == \"" << Alg.Integer_Controls[i].name << "_FA\"} {" << endl;
						out << "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_fa $value)" << endl;
						out << "      ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << ".04 config -text $Name($value)" << endl;
						out << "   }" << endl;
						out << "   if {$name == \"" << Alg.Integer_Controls[i].name << "_var\"} {" << endl;
						out << "      set " << algo_name << "_ptst(" << Alg.Integer_Controls[i].name << "_var $value)" << endl;
						out << "      ." << algo_name << "_ptst.f" << Alg.Integer_Controls[i].name << ".08 config -text $Name($value)" << endl;
						out << "   }" << endl;
				}
		for(unsigned i = 0; i < Alg.Real_Controls.size(); i++)
				{
						out << "   if {$name == \"" << Alg.Real_Controls[i].name << "_FA\"} {" << endl;
						out << "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_fa $value)" << endl;
						out << "      ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << ".04 config -text $Name($value)" << endl;
						out << "   }" << endl;
						out << "   if {$name == \"" << Alg.Real_Controls[i].name << "_var\"} {" << endl;
						out << "      set " << algo_name << "_ptst(" << Alg.Real_Controls[i].name << "_var $value)" << endl;
						out << "      ." << algo_name << "_ptst.f" << Alg.Real_Controls[i].name << ".08 config -text $Name($value)" << endl;
						out << "   }" << endl;
				}
		out << "}" << endl;
		out << endl;
}

void usage(ostream& o, char* prog)
{
						o << "Gui compiler for lisa algorithms." << endl;
						o << "Usage : " << prog
								<< " [-P] -s [-N name] <infile> <outfile> | -b <file_1> ... <file_n> | -h" << endl;
						o << "   -s <infile> <outfile>   process a single file." << endl;
						o << "   -N name    specify algorithm name (default is call)" << endl;
						o << "   -b <file_1> ... <file_n>   batch processing xml-files" << endl;
						o << "                              extension is replaced by \".tcl\"" << endl;
						o << "   -P   compile for parameter test window instead of algorithm " << endl;
						o << "        control window" << endl;
						o << "   -h   print this help" << endl;
}

int main(int argc, char** argv)
{
		bool alg_win = true;
		int check_opt = 1;
		string alg_name = "";
		if(argc < 3)
				{
						usage(cerr,argv[0]);
						return 1;
				}
		//parse options
		string opt(argv[check_opt]);
		vector<string> infiles;
		vector<string> outfiles;
		int count = 0;
		if(opt == "-P")
				{
						alg_win = false;
						check_opt = 2;
						opt = string(argv[check_opt]);
				}
		if(opt == "-h")
				{
						usage(cout,argv[0]);
						return 0;
				}
		else if(opt == "-s")
				{
						if(string(argv[check_opt+1]) == "-N")
								{
										check_opt++;
										alg_name = string(argv[++check_opt]);
								}
						if(argc < check_opt + 3)
								{
										usage(cerr,argv[0]);
										return 1;
								}
						infiles.push_back(string(argv[check_opt + 1]));
						outfiles.push_back(string(argv[check_opt + 2]));
						count = 1;
				}
		else if(opt == "-b")
				{
						if(argc < check_opt + 2)
								{
										usage(cerr,argv[0]);
										return 1;
								}
						for(int i = check_opt + 1; i < argc ; i++)
								{
										count ++;
										string infile(argv[i]);
										string outfile = infile;
										outfile.erase(outfile.find_last_of("."));
										outfile += ".tcl";
										infiles.push_back(infile);
										outfiles.push_back(outfile);
								}
				}
		else {
				usage(cerr,argv[0]);
				return 1;
		}

		LisaXmlFile::initialize(".");
		
		for(int i = 0; i < count; i++)
				{
						Lisa_ExtAlg Alg;
						if(LisaXmlFile::read_ExtAlg(Alg, infiles[i]))
								{
										if(alg_name == "")
												alg_name = Alg.call;
										if(alg_win)
												writeTcl_AlgWindow(alg_name, Alg , outfiles[i]);
										else
												writeTcl_ParaTest(alg_name, Alg , outfiles[i]);
								}
				}
		return 0;

}
		

