if ( open ) closeScene();
newScene();
var net = new Array();
var r = new Array();
var s = new Array();
var n = 1;
var g = 30;
var rg = 157;
for ( i = 0 ; i < 2 ; i++ ) {
    net[i] = new Array();
    net[i+2] = new Array();
    s[i] = addSwitch(2 , i*3 +1);
    s[i+2] = addSwitch(10, i*3+1);
    r[i] = addRouter(4, i*3 + 1);
    r[i+2] = addRouter(8, i*3+1);
    s[i].setSocketsCount(8);
    s[i+2].setSocketsCount(8);
    r[i].setSocketsCount(8);
    r[i+2].setSocketsCount(8);
    addConnection(r[i] , r[i+2] , "LAN3" , "LAN3");
    addConnection(s[i] , r[i] , "LAN8" , "LAN8" );
    addConnection(s[i+2] , r[i+2] , "LAN8" , "LAN8");
    for ( j = 0 ; j < 3 ; j++ ) {
        net[i][j] = addComputer(0,i*3 + j);
        net[i+2][j] = addComputer(12,i*3+j);
        addConnection(net[i][j] , s[i] , "eth0" , "LAN"+(j+1) );
        addConnection(net[i+2][j] , s[i+2] , "eth0" , "LAN"+(j+1) );
        net[i][j].setIp("eth0" , "192.168.1."+(n+j) );
        net[i+2][j].setIp( "eth0" , "192.168.1."+(n+64+j) );
        net[i][j].setMask("eth0" , "255.255.255.224");
        net[i+2][j].setMask( "eth0" , "255.255.255.224");
        net[i][j].setGateway( "192.168.1."+ (g+32*i) );
        net[i+2][j].setGateway( "192.168.1."+ (g+64+32*i) );
    }
    r[i].setIp( "LAN3" , "192.168.1."+rg);
    r[i+2].setIp( "LAN3" , "192.168.1."+(rg+1));
    r[i].setMask( "LAN3" , "255.255.255.224");
    r[i+2].setMask( "LAN3" , "255.255.255.224");
    r[i].setIp( "LAN8" , "192.168.1."+ (g+32*i) );
    r[i+2].setIp( "LAN8" , "192.168.1."+ (g+64+32*i) );
    r[i].setMask( "LAN8" , "255.255.255.224");
    r[i+2].setMask( "LAN8" , "255.255.255.224");
    r[i].setMask( "LAN4" , "255.255.255.224");
    r[i+2].setMask( "LAN4" , "255.255.255.224");
    rg += 32;
    n += 32;
}
addConnection( r[0] , r[1] , "LAN4" , "LAN4");
addConnection( r[2] , r[3] , "LAN4" , "LAN4");
r[0].setIp( "LAN4" ,"192.168.1.221");
r[1].setIp( "LAN4" , "192.168.1.222");
r[2].setIp( "LAN4" , "192.168.1.253");
r[3].setIp( "LAN4" , "192.168.1.254");
for ( i = 0 ; i < 4 ; i++ ) r[i].router = true;
saveScene("big.net");
closeScene();
openScene("big.net");
true;
