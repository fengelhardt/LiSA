proc set_default_var { } {
    global nb;
    global neighbor;
    global bb;
    global prnt;
    global gantt;
    global system;
    global dat;
    global glob;
    global pt;
    global env
    global sij

    set pt(min) 1
    set pt(max) 99
    set pt(step) 1
    # big or small
    set system(screen) "big"
    # win or unix
    set system(os) "unix"

    set neighbor(x) 351
    set neighbor(y) 62
    set neighbor(NGBH) "API"
    set neighbor(METHOD) "SA"
    set neighbor(TYPE) "RAND"
    set neighbor(STEPS) 10000
    set neighbor(NUMB_STUCKS) "1000000"
    set neighbor(ABORT_BOUND) ""
    set neighbor(PROB) 87
    set neighbor(MAX_STUCK) 3000
    set neighbor(TABULENGTH) 7
    set neighbor(NUMB_NGHB) 50
    set nb(STEPS_init) 5000
    set nb(STEPS_max) 5000
    set nb(STEPS_incr) 2
    set nb(STEPS_FA) "FACTOR"
    set nb(PROB_init) 3
    set nb(PROB_max) 12
    set nb(PROB_incr) 2
    set nb(PROB_FA) "FACTOR"
    set nb(MAX_STUCK_init) 30
    set nb(MAX_STUCK_max) 30
    set nb(MAX_STUCK_incr) 2
    set nb(MAX_STUCK_FA) "FACTOR"
    set nb(TABULENGTH_init) 10
    set nb(TABULENGTH_max) 100
    set nb(TABULENGTH_incr) 4
    set nb(TABULENGTH_FA) "FACTOR"
    set nb(NUMB_NGHB_init) 10
    set nb(NUMB_NGHB_max) 100
    set nb(NUMB_NGHB_incr) 4
    set nb(NUMB_NGHB_FA) "FACTOR"
    set nb(FILE) "$env(LISACONFIG)/proc/ptst_out.lsa"

    set bb(lower_bound) ""
    set bb(bounding) "NORMAL"
    set bb(ins_order) "LPT"
    set bb(upper_bound) ""
    set bb(no_of_solutions) "1"

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
}

set_default_var
