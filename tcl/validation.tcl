#taken from http://wiki.tcl.tk/3991
#modified by marc

proc validInteger {win valid event X textval min} {
    upvar $textval textval1
    
    # Allow valid integers, empty strings, sign without number
    # Reject Octal numbers, but allow a single "0"
    # Which signes are allowed ?
    if {($min <= 0) } {   ;# positive & negative sign
        set pattern {^[+-]?(()|0|([1-9][0-9]*))$}
    } else {                            ;# positive sign
        set pattern {^[+]?(()|0|([1-9][0-9]*))$}
    }
    
    # Weak integer checking: allow empty string, empty sign, reject octals
    set weakCheck [regexp $pattern $X]

    # Strong integer checking with range
    set strongCheck [expr {[string is int -strict $X] && ($X >= $min)}]
    
    switch $event {
        key {
          return $weakCheck
        }
        default {
            if {! $strongCheck} {
              set textval1 $min
              after idle $win config -validate $valid 
            }
            return $strongCheck
        }
    }
}
