BEGIN{ OFS = "\t";ssuma = 0; ssumr = 0; ssumc = 0; nsuma = 0; nsumr = 0; nsumc = 0; qsuma = 0; qsumr = 0; qsumc = 0}
FNR == 1 { startt = $1" "$2" "$3}
{
    if ($5 == "swrinde") 
    {
        if ($4 == "-"){
            ssumr++;
        }
        else if($4 == "c"){
            ssuma++;
            ssumc++;
        }
        else{
            ssuma++;
        }
    }
    else if($5 == "news.cais.net")
    {
        if ($4 == "-"){
            nsumr++;
        }
        else if($4 == "c"){
            nsuma++;
            nsumc++;
        }
        else{
            nsuma++;
        }
    }
    else if($5 == "?")
    {
        if ($4 == "-"){
            qsumr++;
        }
        else if($4 == "c"){
            qsuma++;
            qsumc++;
        }
        else{
            qsuma++;
        }
    }
}

END{print "", "", "accepted", "rejected", "canceled\nswrinde:", ssuma,"", ssumr,"", ssumc"\nnews.cais.net:", nsuma,"", nsumr,"", nsumc"\n?:","", qsuma,"", qsumr,"", qsumc"\n\nStart Time = " startt, "End Time = "$1" "$2" "$3}
