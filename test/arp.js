if ( open ) closeFile();
newFile();
print("go");
var net1 = new Array();
var net2 = new Array();
var s1 = createSwitch(4,5,8);
var s2 = createSwitch(8,5,8);
var r1 = createRouter(6,5,4);
createConnection(s1 , r1,"LAN8","LAN3");
createConnection(s2 , r1,"LAN8","LAN4");
for ( i = 0 ; i < 4 ;i++ ) {
    net1[i] = createComputer(2,i+3,1);
    createConnection( net1[i] , s1 ,"eth0","LAN"+(i+1));
    setIp(net1[i] , "eth0" , "192.168.1."+(i+1) );
    setMask(net1[i] , "eth0" , "255.255.255.192" );
    setGateway( net1[i] , "192.168.1.62" );
    net2[i] = createComputer(10,i+3,1);
    createConnection( net2[i] , s2 ,"eth0","LAN"+(i+1));
    setIp(net2[i] , "eth0" , "192.168.1."+(i+65) );
    setMask(net2[i] , "eth0" , "255.255.255.192" );
    setGateway( net2[i] , "192.168.1.126" );
}
setIp( r1 , "LAN3" , "192.168.1.62");
setIp( r1 , "LAN4" , "192.168.1.126");
setMask( r1 , "LAN3" , "255.255.255.192");
setMask( r1 , "LAN4" , "255.255.255.192");
saveScene("test/arp.net");
true;
