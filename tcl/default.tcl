proc set_default_var { } {
    global prnt
    global gantt
    global system
    global dat
    global glob
    global pt
    global env
    global sij
    global schedule

    set pt(min) 1
    set pt(max) 99
    set pt(step) 1
    # big or small
    set system(screen) "big"
    # win or unix
    set system(os) "unix"

    set prnt(COLOR) "color"
    set prnt(ROTATE) "rotate_normal"
    set prnt(SAVE_PS) "No"
    set prnt(PAGEHEIGHT) 1300
    set prnt(PAGEWIDTH) 600
    set prnt(PRNT_COMMAND) "lpr"

    set gantt(special) "None"
    set dat(Matrix) "PIJ"

    set glob(machines) "0"
    set glob(jobs) "0"
    set sij(prob) "100"

    set schedule(Matrix) "LR"
}

set_default_var
