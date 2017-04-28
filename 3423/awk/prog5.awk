BEGIN{
    insum = 0
    outsum = 0
}
/Acct-Input-Packets/{ insum+=$3 }
/Acct-Output-Packets/{ outsum+=$3 }

END{
    print "Total Input Bandwidth Usage  =",insum,"packets"
    print "Total Output Bandwidth Usage =",outsum,"packets"
}
