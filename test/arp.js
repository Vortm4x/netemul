// open - глобальная переменная показывает открыт в данный момент файл или нет.
if ( open ) closeFile();
newFile();
stop();
var sw = new Array();
sw[0] = addSwitch(3,3);
sw[0].setSocketsCount(8);
sw[1] = addSwitch(7,3);
sw[1].setSocketsCount(8);
var r = addRouter(5,3);
r.router = true;
addConnection(sw[0],r,"LAN8","LAN3");
addConnection(sw[1],r,"LAN8","LAN4");
var net = new Array();
var x = 1 , g = 126;
for ( j = 0 ; j < 2 ; j++ ) {
    net[j] = new Array();
    for ( i = 1 ; i <= 5 ; i++ ) {
        net[j][i-1] = addComputer(x,i);
        addConnection( net[j][i-1] , sw[j] , "eth0" , "LAN"+i );
	net[j][i-1].setIp("eth0","192.168.1."+( i + j*128) );
	net[j][i-1].setMask("eth0", "255.255.255.128");
	net[j][i-1].setGateway("192.168.1."+g);
    }
    r.setMask("LAN"+(j+3) , "255.255.255.128");
    r.setIp("LAN"+(j+3) , "192.168.1." + g );
    x += 8;
    g += 128;
}
net[0][0].sendMessage("192.168.1.133",50,0);
emulateTime();
play();
result = ( net[0][0].sendPacketCount("eth0") == net[1][4].receivePacketCount("eth0") );
saveScene("test/arp.net");
result;
