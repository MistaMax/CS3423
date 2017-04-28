BEGIN{ 
    OFS = "\t"
    rate1 = 0
    rate2 = 0
    rate3 = 0
    rate4 = 0
    rate5 = 0
}
/Ascend-Data-Rate/{
    if($3 <= 14400) {
        rate1++
    }
    else if($3 <= 19200){
        rate2++
    }
    else if($3 <= 28800){
        rate3++
    }
    else if($3 <= 33600){
        rate4++
    }
    else if($3 > 33600){
        rate5++
    }
}
END{
    print "0-14400","",rate1
    print "14401-19200",rate2
    print "19201-28800",rate3
    print "28801-33600",rate4
    print "above 33600",rate5

}
