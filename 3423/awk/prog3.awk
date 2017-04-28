BEGIN{ sum = 0; }
/Acct-Session-Time/{sum += $3}
END{ print "Total User Session Time =", sum }
