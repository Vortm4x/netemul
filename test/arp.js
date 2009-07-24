// open - глобальная переменная показывает открыт в данный момент файл или нет.
if ( open ) closeFile();
newFile();
var sw = new Array();
sw[0] = addSwitch(3,3);
sw[0].setSocketsCount(8);
sw[1] = addSwitch(7,3);
sw[1].setSocketsCount(8);
var r = addRouter(5,3);
addConnection(sw[0],r,"LAN8","LAN3");
addConnection(sw[1],r,"LAN8","LAN4");
var net = new Array();
var x = 1;
for ( j = 0 ; j < 2 ; j++ ) {
    net[j] = new Array();
    for ( i = 1 ; i <= 5 ; i++ ) {
        net[j][i-1] = addComputer(x,i);
        addConnection( net[j][i-1] , sw[j] , "eth0" , "LAN"+i );
    }
    x += 8;
}
saveScene("test/arp.net");
true;
