#!/usr/bin/wish
# Hier stehen alle Variablen und Funktionen, die sonst vom C-Programm
# erstellt werden
 

proc vorher {} {
    global langdat;

    set langdat deu.tcl;

}

proc test {}  {
 
}

proc TC_getvar { name } {
    return "german";
}

proc TC_setvar {  args  } {
}


     
proc open_hwin {}  {   }

proc oeffnen {}  {   }

proc speichern {}  {   }

proc zeineu_HW {}  {   }

proc zeineu_DW {}  {   }

proc zeineu_PW {}  {   }

proc D_to_C {}  {   }

proc S_to_C {}  {   }

proc D_to_tcl {}  {   }

proc S_to_tcl {}  {   }

proc setPref {}  {   }

proc setFlags {}  {   }

proc algo {}  {   }

proc TCset_p {}  {   }

proc TCget_p {}  { return 0  }

proc TCset_prob {}  {   }


#vorher
global env
set env(LISAHOME) "/home/willeniu/cpp/LiSA"
#set env(LISAHOME) "/home/per/cpp/LiSA"
source tclinit.tcl


