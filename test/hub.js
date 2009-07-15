if ( open ) closeFile();
newFile();
var h1 = createHub(5,0,4);
var h2 = createHub(2,2,8);
var h3 = createHub(8,2,8);
createConnection(h1, h2 , "LAN3" , "LAN8");
createConnection(h1, h3 , "LAN4" , "LAN8");
var net = new Array();
for ( i = 0 ; i < 11 ; i++) {
    if ( i == 5 ) { continue;}
	net[i] = createComputer(i,5,1);
        if ( i < 5 ) createConnection( h2 , net[i] , "LAN" + (i+1) , "eth0" );
        else createConnection( h3 , net[i] , "LAN" + (i-5) , "eth0" );
        setIp( net[i] , "eth0" , "192.168.1." + (i+1) );
	setMask( net[i] , "eth0" , "255.255.255.0");
}
true;

