BEGIN{OFS = "\t"; lg = 0; la = 0; l = 0; ld = 0; lt = 0; rung = 0; runa = 0; run = 0; rund = 0;runt = 0; rgrg = 0; rgra = 0; rgr = 0; rgrd = 0; rgrt = 0;rgg = 0; rga = 0; rg = 0; rgd = 0; rgt = 0}

FNR == 1{startt = $1" "$2" "$3}

{
    if($7 == "group"){
        if(match($6, "lonestar")){
            if(match($8, "2413.d")){
                ld += $9;
            }
            else if(match($8, "2413")){
                l += $9;
            }
        }
        else if(match($6, "runner")){
            if(match($8, "2413.d")){
                rund += $9;
            }
            else if(match($8, "2413")){
                run += $9;
            }
        }
        else if(match($6, "ringer")){
            if(match($8, "2413.d")){
                rgrd += $9;
            }
            else if(match($8, "2413")){
                rgr += $9;
            }
        }
        else if(match($6, "ring")){
            if(match($8, "2413.d")){
                rgd += $9;
            }
            else if(match($8, "2413")){
                rg += $9;
            }
        }
    }


    else if($7 == "exit"){
        if(match($6, "lonestar")){
            la += $9;
            lg += $11;
        }
        else if(match($6, "runner")){
            runa += $9;
            rung += $11;
        }
        else if(match($6, "ringer")){
            rgra += $9;
            rgrg += $11;
        }
        else if(match($6, "ring")){
            rga += $9;
            rgg += $11;
        }
    }


    else if($7 == "times"){
        if(match($6, "lonestar")){
            lt += $13;
        }
        else if(match($6, "runner")){
            runt += $13;
        }
        else if(match($6, "ringer")){
            rgrt += $13;
        }
        else if(match($6, "ring")){
            rgt += $13;
        }
    }


}

END{
    print "", "", "lonestar", "runner","", "ringer","", "rings\n";
    print "Articles:", la,"", runa,"", rgra,"", rga;
    print "Groups:","", lg,"", rung,"", rgrg,"", rgg;
    print "Cs2413:","", l,"", run,"", rgr,"", rg;
    print "Cs2413.d:", ld,"", rund,"", rgrd,"", rgd;
    print "User Time:", lt,"", runt,"", rgrt,"", rgt;
    print "\nStart Time = " startt, "End Time = " $1" "$2" "$3;
}
